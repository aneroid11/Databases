#include "database.h"
#include "sha256.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlResult>
#include <QDate>
#include <QDebug>

Database::Database()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("task3");
    db.setUserName("root");
    db.setPassword("msql_pss_rt");
    db.setPort(3306);
    bool ok = db.open();

    if (!ok)
    {
        throw QString("cannot connect to db!");
    }
}

Database::~Database()
{
    if (db.isOpen())
    {
        db.close();
    }
}

void Database::prepareExec(QSqlQuery& q, QString queryStr)
{
    if (!q.prepare(queryStr))
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
    if (!q.exec())
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
}

void Database::prepareExecWithBinding(QSqlQuery &q, QString qStr, const QList<QVariant> &params)
{
    if (!q.prepare(qStr))
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }

    for (int i = 0; i < params.size(); i++)
    {
        q.bindValue(i, params[i]);
    }

    if (!q.exec())
    {
        QSqlError err = q.lastError();
        qDebug() << err.databaseText() << "\n";
        throw QString(err.databaseText());
    }
}

Report Database::extractReportFromQuery(const QSqlQuery &q)
{
    Report newReport;
    newReport.id = q.value(0).toInt();
    newReport.title = q.value(1).toString();
    qDebug() << newReport.title;
    newReport.contents = q.value(2).toString();
    newReport.authorId = q.value(3).toInt();
    newReport.reportType = q.value(4).toString();
    newReport.objectId = q.value(5).toInt();
    return newReport;
}

Artist Database::extractArtistFromQuery(const QSqlQuery &q)
{
    Artist a;
    a.id = q.value(0).toInt();
    a.email = q.value(1).toString();
    a.passwordHash = q.value(2).toString();
    a.nickname = q.value(3).toString();
    a.dateOfBirth = q.value(4).toString();
    a.gender = q.value(5).toString();
    a.premiumSubscriptionId = q.value(6).toInt();
    a.cardDetailsId = q.value(7).toInt();
    return a;
}

TrackInfo Database::extractTrackInfoFromQuery(const QSqlQuery &q)
{
    TrackInfo info;
    info.id = q.value(0).toInt();
    info.timestamp = q.value(1).toString();
    info.title = q.value(2).toString();
    info.lengthSeconds = q.value(3).toInt();
    info.artistNickname = q.value(4).toString();
    return info;
}

DataRow Database::extractDataRowFromQuery(const QSqlQuery &q)
{
    DataRow ret;
    const int numColumns = q.record().count();

    for (int i = 0; i < numColumns; i++)
    {
        ret.data[q.record().fieldName(i)] = q.value(i);
    }

    return ret;
}

void Database::signUpArtist(QString email,
                            QString passwordHash,
                            QString nickname,
                            QString dateOfBirth,
                            QString gender)
{
    QSqlQuery q;
    //prepareExec(q, queryStr);
    prepareExecWithBinding(q,
                           "call RegisterArtist(:email, :pwdHash, :nickname, :dateOfBirth, :gender);",
                           { email, passwordHash, nickname, dateOfBirth, gender });
}

void Database::signInUser(QString email, QString password)
{
    /*QString queryStr = QString("select id, role from Users where email = '%1' and password_hash = '%2';")
            .arg(email)
            .arg(sha256hash(password));*/

    //qDebug() << queryStr;

    QSqlQuery q;
    //prepareExec(q, queryStr);
    prepareExecWithBinding(q,
                           "select id, role from Users where email = :email and password_hash = :pwdHash;",
                           { email, sha256hash(password) });

    if (!q.next())
    {
        throw QString("No user with such email and/or password!");
    }
    else
    {
        qDebug() << q.value(0).toInt() << "\n";
    }
    const int userId = q.value(0).toInt();
    const QString userRole = q.value(1).toString();
    currUserId = userId;
    currUserRole = userRole;

    prepareExec(q, QString("set @curr_session_user_id = %1;").arg(userId));
}

void Database::signOffCurrUser()
{
    currUserId = -1;
    currUserRole = QString();

    QString qStr = "set @curr_session_user_id = NULL;";
    QSqlQuery q;
    prepareExec(q, qStr);

    qDebug() << currUserId << " is currUserId";
    qDebug() << currUserRole << " is currUserRole";
}

QString Database::getCurrArtistNickname()
{
    if (currUserId < 0 || currUserRole != "artist")
    {
        return QString();
    }

    QString qStr = QString("select nickname from Artists where id = %1").arg(currUserId);
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toString();
}

QString Database::getCurrUserEmail()
{
    if (currUserId < 0) { return QString(); }

    QString qStr = QString("select email from Users where id = %1").arg(currUserId);
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toString();
}

int Database::numAdmins()
{
    QString qStr = QString("select count(id) from Users where role = 'admin';");
    QSqlQuery q;
    prepareExec(q, qStr);
    q.next();
    return q.value(0).toInt();
}

void Database::deleteAccount(int id)
{
    if (currUserRole == "artist")
    {
        QString qStr = QString("call DeleteArtistAccount(%1);").arg(id);
        QSqlQuery q;
        prepareExec(q, qStr);
    }
    else if (currUserRole == "admin")
    {
        if (numAdmins() < 2)
        {
            throw QString("Cannot have no admins!");
        }

        QString qStr = QString("delete from Users where id = %1;").arg(id);
        QSqlQuery q;
        prepareExec(q, qStr);
    }

    signOffCurrUser();
}

void Database::createAdmin(QString email, QString password)
{
    /*QString qStr = QString("insert into Users (email, password_hash, role) values ('%1', '%2', 'admin');")
            .arg(email)
            .arg(sha256hash(password));*/
    QString qStr = "insert into Users (email, password_hash, role) values (:email, :pwdHash, 'admin');";
    QSqlQuery q;
    prepareExecWithBinding(q, qStr, { email, sha256hash(password) });
}

QList<Report> Database::getReportsByArtist(const int artistId)
{
    QString s = QString("select * from Reports where author_id = %1;").arg(artistId);
    QSqlQuery q;
    prepareExec(q, s);
    QList<Report> ret;

    while (q.next())
    {
        ret.push_back(extractReportFromQuery(q));
    }

    return ret;
}

QList<Report> Database::getAllReports()
{
    QString s = QString("select * from Reports;");
    QSqlQuery q;
    prepareExec(q, s);
    QList<Report> ret;

    while (q.next())
    {
        ret.push_back(extractReportFromQuery(q));
    }

    return ret;
}

Report Database::getReport(int id)
{
    QString s = QString("select * from Reports where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
    if (!q.next())
    {
        throw QString("No report with such id!");
    }

    return extractReportFromQuery(q);
}

QList<Artist> Database::getAllArtists()
{
    QString s = QString("select * from ArtistsInfo;");
    QSqlQuery q;
    prepareExec(q, s);

    QList<Artist> ret;

    while (q.next())
    {
        ret.push_back(extractArtistFromQuery(q));
    }

    return ret;
}

Artist Database::getArtistInfo(const int id)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "select * from ArtistsInfo where id = :id", { id });

    if (!q.next()) { throw QString("No artist with id = %1!").arg(id); }

    return extractArtistFromQuery(q);
}

QList<TrackInfo> Database::getTracksInfo(const int artistId)
{
    QSqlQuery q;
    prepareExecWithBinding(q,
                           "select * from TracksInfo where nickname = (select nickname from Artists where id = :artistId);",
                           QList<QVariant> { artistId });
    QList<TrackInfo> ret;

    while (q.next())
    {
        ret.push_back(extractTrackInfoFromQuery(q));
    }

    return ret;
}

TrackInfo Database::getTrackInfo(const int id)
{
    QSqlQuery q;
    prepareExec(q, QString("select * from TracksInfo where id = %1").arg(id));

    if (q.next())
    {
        return extractTrackInfoFromQuery(q);
    }

    throw QString("No track with such id!");
}

QList<TrackInfo> Database::getTracksFromPlaylistInfo(int playlistId)
{
    QSqlQuery q;
    prepareExec(q, QString("select TracksInfo.id, timestamp, title, length_seconds, nickname "
                           "from TracksInfo "
                           "inner join TracksToPlaylists "
                           "on TracksInfo.id = TracksToPlaylists.track_id "
                           "where playlist_id = %1;").arg(playlistId));
    QList<TrackInfo> ret;

    while (q.next())
    {
        ret.push_back(extractTrackInfoFromQuery(q));
    }

    return ret;
}

QList<TrackInfo> Database::searchTracksByTitle(QString title)
{
    title = QString("%") + title + "%";

    QSqlQuery q;
    prepareExecWithBinding(q, "select * from TracksInfo where LOWER(title) like LOWER(:title)", {title});
    QList<TrackInfo> ret;
    while (q.next())
    {
        qDebug() << "extract";
        ret.push_back(extractTrackInfoFromQuery(q));
    }

    qDebug() << ret.size();
    return ret;
}

QList<TrackInfo> Database::searchTracksByTitleAndTags(QString title, QStringList tags)
{
    title = QString("%") + title + "%";
    const int numTags = tags.size();

    QString qStr = "with t1 as "
                   "("
                   "select id_tag, id_track from TagsToTracks where ";

    for (int i = 0; i < numTags; i++)
    {
        qStr += "id_tag = (select id from Tags where name = :tag" + QString::number(i);
        qStr += ")";

        if (i < numTags - 1)
        {
            qStr += " or ";
        }
    }
    qStr += ") ";
    qStr += "select TracksInfo.* from t1 "
            "inner join TracksInfo on TracksInfo.id = t1.id_track "
            "group by id_track "
            "having count(id_track) = :numTags and LOWER(TracksInfo.title) like LOWER(:title);";

    // :tag1, :tag2, :tag3, ...
    // :numTags, :title
    QList<QVariant> params;
    for (auto tag : tags)
    {
        params.push_back(tag);
    }
    params.push_back(numTags);
    params.push_back(title);

    qDebug() << qStr;

    QSqlQuery q;
    prepareExecWithBinding(q, qStr, params);
    QList<TrackInfo> ret;
    while (q.next())
    {
        ret.push_back(extractTrackInfoFromQuery(q));
    }

    return ret;
}

int Database::numLikesOnTrack(const int trackId)
{
    QSqlQuery q;
    prepareExec(q, QString("select count(id) num_of_likes from Likes where track_id = %1;").arg(trackId));
    q.next();

    qDebug() << q.size();
    const int numLikes = q.value(0).toInt();
    qDebug() << "likes on" << trackId << ":" << numLikes;

    return numLikes;
}

QList<DataRow> Database::getCommentsOnTrack(const int trackId)
{
    //"select Comments.contents, Artists.nickname from Comments inner join Artists on Comments.artist_id = Artists.id where track_id = 5;"
    QSqlQuery q;
    prepareExec(q, QString("select Comments.contents, Artists.nickname "
                           "from Comments "
                           "inner join Artists on Comments.artist_id = Artists.id "
                           "where track_id = %1;").arg(trackId));
    QList<DataRow> comments;

    while (q.next())
    {
        comments.push_back(extractDataRowFromQuery(q));
    }

    return comments;
}

QList<TrackInfo> Database::getAllTracksInfo()
{
    QSqlQuery q;
    prepareExec(q, "select * from TracksInfo;");
    QList<TrackInfo> ret;

    while (q.next())
    {
        ret.push_back(extractTrackInfoFromQuery(q));
    }

    return ret;
}

void Database::deleteReport(int id)
{
    QString s = QString("delete from Reports where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}

void Database::createTrack(int artistId, QString title, int length)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "call UploadTrack(:title, :length, :artistId);", { title, length, artistId });
}

void Database::deleteTrack(int id)
{
    QString s = QString("delete from Tracks where id = %1;").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}

QList<DataRow> Database::getCommentsBy(const int artistId)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "select * from Comments where artist_id = :artistId order by timestamp desc;", { artistId });
    QList<DataRow> ret;

    while (q.next())
    {
        DataRow row = extractDataRowFromQuery(q);

        for (QString key : row.data.keys())
        {
            qDebug() << key << ": " << row.data[key];
        }

        ret.push_back(row);
    }

    return ret;
}

void Database::deleteRecordFromTable(QString tableName, const int id)
{
    QString s = QString("delete from %1 where id = %2;").arg(tableName).arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}

QList<DataRow> Database::getAllPlaylists()
{
    QString s =
            QString("select Playlists.id, Playlists.title, Playlists.artist_id, Albums.release_date, Artists.nickname "
                    "from Playlists left join Albums on Playlists.id = Albums.id "
                    "left join Artists on artist_id = Artists.id;");
    QSqlQuery q;
    prepareExec(q, s);
    QList<DataRow> ret;

    while (q.next())
    {
        ret.push_back(extractDataRowFromQuery(q));
    }

    return ret;
}

QList<DataRow> Database::getPlaylistsBy(const int artistId)
{
    QString s =
            QString("select Playlists.id, Playlists.title, Playlists.artist_id, Albums.release_date, Artists.nickname "
                    "from Playlists "
                    "left join Albums on Playlists.id = Albums.id "
                    "left join Artists on artist_id = Artists.id "
                    "where artist_id = %1;").arg(artistId);
    QSqlQuery q;
    prepareExec(q, s);
    QList<DataRow> ret;

    while (q.next())
    {
        ret.push_back(extractDataRowFromQuery(q));
    }

    return ret;
}

DataRow Database::getPlaylist(const int playlistId)
{
    QSqlQuery q;
    prepareExec(q, QString("select Playlists.id, Playlists.title, Playlists.artist_id, Albums.release_date, Artists.nickname "
                           "from Playlists left join Albums on Playlists.id = Albums.id "
                           "left join Artists on artist_id = Artists.id "
                           "where Playlists.id = %1;").arg(playlistId));

    q.next();
    return extractDataRowFromQuery(q);
}

int Database::idPlaylistOwner(const int playlistId)
{
    QSqlQuery q;
    prepareExec(q, QString("select artist_id from Playlists where id = %1").arg(playlistId));
    q.next();
    return q.value(0).toInt();
}

void Database::deletePlaylist(const int id)
{
    QString s = QString("call DeletePlaylist(%1);").arg(id);
    QSqlQuery q;
    prepareExec(q, s);
}

void Database::addTrackToPlaylist(const int playlistId, const int currTrackId)
{
    QSqlQuery q;
    prepareExec(q, QString("insert into TracksToPlaylists "
                           "(track_id, playlist_id) values (%2, %1);").arg(playlistId).arg(currTrackId));
}

void Database::deleteTrackFromPlaylist(const int trackId, const int playlistId)
{
    QSqlQuery q;
    prepareExec(q, QString("delete from TracksToPlaylists "
                           "where track_id = %1 and playlist_id = %2;").arg(trackId).arg(playlistId));
}

void Database::updatePlaylistTitle(const int playlistId, const QString newTitle)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "update Playlists set title = :newTitle where id = :playlistId;", { newTitle, playlistId });
}

void Database::updateAlbumReleaseDate(const int playlistId, const QString newDate)
{
    QSqlQuery q;
    prepareExec(q, QString("update Albums set release_date = '%1' where id = %2;").arg(newDate).arg(playlistId));
}

void Database::updateArtist(int id, QString email, QString nickname, QString password, QString dateOfBirth, QString gender)
{
    Artist a = getArtistInfo(id);
    QString passwordHash;

    if (email.isNull() || email.isEmpty()) { email = a.email; }
    if (nickname.isNull() || nickname.isEmpty()) { nickname = a.nickname; }
    if (password.isNull() || password.isEmpty()) { passwordHash = a.passwordHash; }
    else                                         { passwordHash = sha256hash(password); }
    if (dateOfBirth.isNull() || dateOfBirth.isEmpty()) { dateOfBirth = a.dateOfBirth; }
    if (gender.isNull() || gender.isEmpty()) { gender = a.gender; }

    QSqlQuery q;
    prepareExecWithBinding(q,
                           "update Artists set nickname = :nickname, "
                           "date_of_birth = :dateOfBirth, gender = :gender "
                           "where id = :id;",
                           QList<QVariant>{ nickname, dateOfBirth, gender, id });
    prepareExecWithBinding(q,
                           "update Users set email = :email, password_hash = :passwordHash "
                           "where id = :id;",
                           QList<QVariant>{ email, passwordHash, id });
}

void Database::updateTrackTitle(int trackId, QString title)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "update Tracks set title = :title where id = :trackId", { title, trackId });
}

QStringList Database::getTrackTags(const int trackId)
{
    return getTagsFor("Tracks", trackId);
    /*QString s = QString("select name from Tags "
                        "inner join TagsToTracks "
                        "on TagsToTracks.id_tag = Tags.id "
                        "where id_track = %1;").arg(trackId);
    QSqlQuery q;
    prepareExec(q, s);
    QList<DataRow> rows;

    while (q.next())
    {
        rows.push_back(extractDataRowFromQuery(q));
    }

    QStringList ret;
    for (const DataRow& r : rows)
    {
        ret.push_back(r.data["name"].toString());
    }

    return ret;*/
}

QStringList Database::getTagsFor(QString table, int id)
{
    QString idStr = table == "Tracks" ? "id_track" : "id_playlist";

    QString s = QString("select name from Tags "
                        "inner join TagsTo%1 "
                        "on TagsTo%1.id_tag = Tags.id "
                        "where %2 = %3;").arg(table).arg(idStr).arg(id);
    QSqlQuery q;
    prepareExec(q, s);
    QList<DataRow> rows;

    while (q.next())
    {
        rows.push_back(extractDataRowFromQuery(q));
    }

    QStringList ret;
    for (const DataRow& r : rows)
    {
        ret.push_back(r.data["name"].toString());
    }

    return ret;
}

QStringList Database::getAllTags()
{
    QSqlQuery q;
    prepareExec(q, "select name from Tags;");

    QList<DataRow> rows;
    while (q.next())
    {
        rows.push_back(extractDataRowFromQuery(q));
    }

    QStringList ret;
    for (const DataRow& r : rows)
    {
        ret.push_back(r.data["name"].toString());
    }

    return ret;
}

void Database::attachTagToTrack(const int trackId, const QString newTag)
{
    QSqlQuery q;

    try
    {
        prepareExecWithBinding(q, QString("insert into Tags (name) values (:newTag);"), {newTag});
    }
    // ?
    catch (QString) {}

    try
    {
        prepareExecWithBinding(q, QString("insert into TagsToTracks (id_tag, id_track) "
                                          "select id, :trackId from Tags where name = :newTag;"), {trackId, newTag});
    }
    // ?
    catch (QString) {}
}

void Database::deattachTagFromTrack(const int trackId, const QString tag)
{
    QSqlQuery q;
    prepareExec(q, QString("delete from TagsToTracks where id_tag = "
                           "(select id from Tags where name = '%1') and id_track = %2;").arg(tag).arg(trackId));
}

void Database::attachTagToPlaylist(const int playlistId, const QString newTag)
{
    QSqlQuery q;

    try
    {
        prepareExecWithBinding(q, QString("insert into Tags (name) values (:newTag);"), {newTag});
    }
    // ?
    catch (QString) {}

    try
    {
        prepareExecWithBinding(q, QString("insert into TagsToPlaylists (id_tag, id_playlist) "
                                          "select id, :trackId from Tags where name = :newTag;"), {playlistId, newTag});
    }
    // ?
    catch (QString) {}
}

void Database::deattachTagFromPlaylist(const int playlistId, const QString tag)
{
    QSqlQuery q;
    prepareExec(q, QString("delete from TagsToPlaylists where id_tag = "
                           "(select id from Tags where name = '%1') and id_playlist = %2;").arg(tag).arg(playlistId));
}

void Database::createPlaylist(const int artistId, const QString title)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "insert into Playlists (title, artist_id) "
                              "values (:title, :artistId);", { title, artistId });
}

void Database::createAlbum(const int artistId, const QString title, const QDate releaseDate)
{
    createPlaylist(artistId, title);
    QSqlQuery q;
    prepareExecWithBinding(q, "insert into Albums values (last_insert_id(), :releaseDate)", { releaseDate });
}

void Database::addLike(const int artistId, const int trackId)
{
    QSqlQuery q;
    prepareExec(q, QString("insert into Likes (artist_id, track_id) values (%1, %2)").arg(artistId).arg(trackId));
}

void Database::addComment(const int artistId, const int trackId, const QString contents)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "call CommentTrack(:contents, :artistId, :trackId);", { contents, artistId, trackId });
}

void Database::reportTrack(int artistId, int trackId, QString title, QString contents)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "call ReportTrack(:title, :contents, :artistId, :trackId)", {title, contents, artistId, trackId});
}

void Database::reportArtist(int artistId, int reportedArtistId, QString title, QString contents)
{
    QSqlQuery q;
    prepareExecWithBinding(q, "call ReportArtist(:title, :contents, :artistId, :reportedId)", {title, contents, artistId, reportedArtistId});
}
