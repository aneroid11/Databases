#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sha256.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QInputDialog>

enum {
    STARTING_PAGE_ID = 0,
    SIGNIN_PAGE_ID = 1,
    SIGNUP_PAGE_ID = 2,
    ARTIST_ACC_PAGE_ID = 3,
    MODERATOR_ACC_PAGE_ID = 4,
    ADMIN_ACC_PAGE_ID = 5,
    ARTIST_ACC_DETAILS_PAGE_ID = 6,
    ALL_TRACKS_PAGE_ID = 7,
    MY_TRACKS_PAGE_ID = 8,
    MY_TRACK_EDIT_PAGE_ID = 9,
    PLAYLISTS_PAGE_ID = 10,
    CREATE_ADMIN_PAGE_ID = 11,
    REPORTS_PAGE_ID = 12,
    ARTISTS_PAGE_ID = 13,
    COMMENTS_PAGE_ID = 14
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    db = new Database();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
}

bool MainWindow::areYouSure()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure?", "Do you really want to do that?",
                                  QMessageBox::Yes|QMessageBox::No);
    return reply == QMessageBox::Yes;
}

void MainWindow::showMsg(QString msg)
{

    QMessageBox msgBox;
    msgBox.setWindowTitle("Message");
    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::on_starting_signInButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNIN_PAGE_ID);
}

void MainWindow::on_starting_signUpButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(SIGNUP_PAGE_ID);
}

void MainWindow::on_signUp_signUpButton_clicked()
{
    const QString email = ui->signUp_emailLineEdit->text();
    if (email.isEmpty())
    {
        showMsg("Email line is empty!");
        return;
    }
    const QString pwd = ui->signUp_passwordLineEdit->text();
    if (pwd.isEmpty())
    {
        showMsg("Password line is empty!");
        return;
    }
    const QString nickname = ui->signUp_nicknameLineEdit->text();
    if (nickname.isEmpty())
    {
        showMsg("Nickname line is empty!");
        return;
    }

    try
    {
        db->signUpArtist(email, sha256hash(pwd), nickname,
                         ui->signUp_birthDateEdit->date().toString("yyyy-MM-dd"),
                         ui->signUp_genderComboBox->currentText());
    }
    catch (QString msg)
    {
        showMsg(msg);
        return;
    }

    /*try
    {

    }
    catch (const std::exception& exc)
    {
        showMsg(QString("Error: ") + exc.what());
        return;
    }*/

    showMsg("Signed up successfully!");

    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_signIn_signInButton_clicked()
{
    const QString email = ui->signIn_emailLineEdit->text();
    if (email.isEmpty())
    {
        showMsg("Email line is empty!");
        return;
    }
    const QString pwd = ui->signIn_passwordLineEdit->text();
    if (pwd.isEmpty())
    {
        showMsg("Password line is empty!");
        return;
    }

    try
    {
        db->signInUser(email, pwd);
    }
    catch (QString msg)
    {
        showMsg(msg);
        return;
    }

    //showMsg("Signed in successfully!");

    const QString role = db->getCurrUserRole();
    int newIndex;

    if (role == "artist") { newIndex = ARTIST_ACC_PAGE_ID; }
    else if (role == "admin") { newIndex = ADMIN_ACC_PAGE_ID; }
    else
    {
        showMsg(QString("What is this role: %1?").arg(role));
        return;
    }

    ui->stackedWidget->setCurrentIndex(newIndex);
}

void MainWindow::artistPageInit()
{
    const QString nickname = db->getCurrArtistNickname();
    ui->artistAcc_nicknameLabel->setText(nickname);
}

void MainWindow::adminPageInit()
{
    const QString email = db->getCurrUserEmail();
    ui->adminAcc_emailLabel->setText(email);
}

void MainWindow::fillReportsList(QListWidget *listWidget, const QList<Report> &reports)
{
    listWidget->clear();

    for (const Report& rp : reports)
    {
        QString infoStr = QString("%1 - '%2'").arg(rp.id).arg(rp.title);
        listWidget->addItem(infoStr);
    }
}

void MainWindow::fillReportsList()
{
    fillReportsList(ui->reports_reportsList, db->getAllReports());
}

void MainWindow::fillTracksList(QListWidget *listWidget, const QList<TrackInfo> &tracks)
{
    listWidget->clear();

    for (const TrackInfo& t : tracks)
    {
        QString info = QString("%1 - %2 - %3 (%4 sec), %5")
                .arg(t.id)
                .arg(t.artistNickname)
                .arg(t.title)
                .arg(t.lengthSeconds)
                .arg(t.timestamp);

        listWidget->addItem(info);
    }
}

void MainWindow::fillTracksList()
{
    fillTracksList(ui->allTracks_tracksListWidget, db->getAllTracksInfo());
}

void MainWindow::fillArtistsList()
{
    ui->artists_artistsList->clear();

    QList<Artist> artists = db->getAllArtists();

    for (const Artist& a : artists)
    {
        QString info = QString("%1 - %2").arg(a.id).arg(a.nickname);
        ui->artists_artistsList->addItem(info);
    }
}

void MainWindow::fillCommentsList(QListWidget *listWidget, const QList<DataRow> &comments)
{
    listWidget->clear();

    for (const DataRow& c : comments)
    {
        QString info = QString("%1 - artist id: %2, track id: %3, %4, contents: '%5'")
                .arg(c.data["id"].toInt())
                .arg(c.data["artist_id"].toInt())
                .arg(c.data["track_id"].toInt())
                .arg(c.data["timestamp"].toString())
                .arg(c.data["contents"].toString());
        listWidget->addItem(info);
    }
}

void MainWindow::fillPlaylistsList(QListWidget *listWidget, const QList<DataRow> &playlists)
{
    listWidget->clear();

    for (const DataRow& p : playlists)
    {
        const QString releaseDate = p.data["release_date"].toString();
        QString info;
        if (releaseDate.isNull() || releaseDate.isEmpty())
        {
            // this is a playlist
            info = QString("%1 - '%2', by %3")
                    .arg(p.data["id"].toInt())
                    .arg(p.data["title"].toString())
                    .arg(p.data["nickname"].toString());
        }
        else
        {
            // this is an album
            info = QString("%1 - '%2', by %3 (album, %4)")
                    .arg(p.data["id"].toInt())
                    .arg(p.data["title"].toString())
                    .arg(p.data["nickname"].toString())
                    .arg(p.data["release_date"].toDate().toString("yyyy-MM-dd"));
        }
        listWidget->addItem(info);
    }
}

void MainWindow::reportsPageInit(const int artistId)
{
    if (artistId < 0)
    {
        ui->reports_titleLabel->setText("All reports:");
        fillReportsList();
    }
    else
    {
        ui->reports_titleLabel->setText("Reports by:");
        ui->reports_artistIdLabel->setText(QString::number(artistId));
        fillReportsList(ui->reports_reportsList, db->getReportsByArtist(artistId));
    }
}

void MainWindow::artistsPageInit()
{
    const bool admin = db->getCurrUserRole() == "admin";
    ui->artists_deleteButton->setDisabled(!admin);

    fillArtistsList();
}

void MainWindow::allTracksPageInit()
{
    const QString role = db->getCurrUserRole();

    const bool admRole = role == "admin";
    ui->allTracks_likeButton->setDisabled(admRole);
    ui->allTracks_commentButton->setDisabled(admRole);
    ui->allTracks_reportButton->setDisabled(admRole);
    ui->allTracks_addToPlaylist->setDisabled(admRole);
    ui->allTracks_filterByTag->setDisabled(admRole);

    ui->allTracks_deleteButton->setDisabled(!admRole);

    fillTracksList();
}

void MainWindow::artistAccDetailsPageInit(const int artistId)
{
    const QString role = db->getCurrUserRole();
    const bool disableEdit = (role == "admin") || (artistId != db->getCurrUserId());

    ui->artistAccDetails_saveChangesButton->setDisabled(disableEdit);
    ui->artistAccDetails_birthDateEdit->setDisabled(disableEdit);
    ui->artistAccDetails_genderComboBox->setDisabled(disableEdit);
    ui->artistAccDetails_emailLineEdit->setDisabled(disableEdit);
    ui->artistAccDetails_passwordLineEdit->setDisabled(disableEdit);
    ui->artistAccDetails_nicknameLineEdit->setDisabled(disableEdit);
    ui->artistAccDetails_playlistsButton->setDisabled((role == "admin"));

    ui->artistAccDetails_reportsButton->setDisabled((role != "admin") && (artistId != db->getCurrUserId()));
    ui->artistAccDetails_commentsButton->setDisabled((role != "admin") && (artistId != db->getCurrUserId()));

    // fill the data
    Artist info = db->getArtistInfo(artistId);
    ui->artistAccDetails_idLabel->setText(QString::number(info.id));
    ui->artistAccDetails_nicknameLineEdit->setText(info.nickname);
    //ui->artistAccDetails_passwordLineEdit->setText(info.passwordHash);
    ui->artistAccDetails_emailLineEdit->setText(info.email);
    qDebug() << info.dateOfBirth;

    ui->artistAccDetails_birthDateEdit->show();
    if (info.dateOfBirth.isNull() || info.dateOfBirth.isEmpty())
    {
        ui->artistAccDetails_birthDateEdit->hide();
    }
    else
    {
        ui->artistAccDetails_birthDateEdit->setDate(QDate::fromString(info.dateOfBirth, "yyyy-MM-dd"));
    }

    ui->artistAccDetails_genderComboBox->setCurrentText(info.gender);
}

void MainWindow::myTracksPageInit(const int artistId)
{
    const bool admin = db->getCurrUserRole() == "admin";

    ui->myTracks_uploadTrackButton->setDisabled(admin);
    ui->myTracks_addToPlaylistButton->setDisabled(admin);
    ui->myTracks_detailsButton->setDisabled(admin);

    ui->myTracks_artistId->setText(QString::number(artistId));

    const bool disableBtns = !admin && artistId != db->getCurrUserId();
    ui->myTracks_deleteButton->setDisabled(disableBtns);
    ui->myTracks_detailsButton->setDisabled(disableBtns);
    ui->myTracks_uploadTrackButton->setDisabled(disableBtns);

    fillTracksList(ui->myTracks_tracksListWidget, db->getTracksInfo(artistId));
}

void MainWindow::commentsPageInit(const int artistId)
{
    ui->comments_detailsButton->hide();
    ui->comments_idTextLabel->hide();
    ui->comments_idLabel->hide();

    ui->comments_idLabel->setText(QString::number(artistId));

    fillCommentsList(ui->comments_commentsList, db->getCommentsBy(artistId));
}

void MainWindow::playlistsPageInit(const int artistId)
{
    /*const QString role = db->getCurrUserRole();
    const bool admin = role == "admin";*/

    //ui->playlists_deleteButton->setDisabled(admin);
    ui->playlists_deleteButton->setDisabled(artistId < 0);

    if (artistId < 0)
    {
        ui->playlists_titleLabel->setText("All playlists:");
        fillPlaylistsList(ui->playlists_playlistsList, db->getAllPlaylists());
    }
    else
    {
        ui->playlists_titleLabel->setText("Playlists by:");
        ui->playlists_artistIdLabel->setText(QString::number(artistId));
        fillPlaylistsList(ui->playlists_playlistsList, db->getPlaylistsBy(artistId));
    }
}

void MainWindow::on_stackedWidget_currentChanged(int index)
{
    qDebug() << "on_stackedWidget_currentChanged()\n";

    switch (index)
    {
    case ARTIST_ACC_PAGE_ID:
        artistPageInit();
        break;
    case ADMIN_ACC_PAGE_ID:
        adminPageInit();
        break;
    case ARTISTS_PAGE_ID:
        artistsPageInit();
        break;
    case ALL_TRACKS_PAGE_ID:
        allTracksPageInit();
    }
}

void MainWindow::on_artistAcc_signOffButton_clicked()
{
    db->signOffCurrUser();
    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::deleteCurrAcc()
{
    if (areYouSure()) {
        try
        {
            db->deleteCurrAccount();
        }
        catch (QString msg)
        {
            showMsg(msg);
            return;
        }

        ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);

        showMsg("Your account was deleted");
    }
}

void MainWindow::on_artistAcc_deleteAccountButton_clicked()
{
    deleteCurrAcc();
}

void MainWindow::on_artistAcc_accountButton_clicked()
{
    artistAccDetailsPageInit(db->getCurrUserId());
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
}

void MainWindow::on_artistAccDetails_cancelButton_clicked()
{
    if (db->getCurrUserRole() == "artist")
    {
        const int artistId = ui->artistAccDetails_idLabel->text().toInt();

        if (artistId == db->getCurrUserId())
        {
            ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(ARTISTS_PAGE_ID);
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(ARTISTS_PAGE_ID);
    }
}

void MainWindow::on_artistAccDetails_saveChangesButton_clicked()
{
    if (areYouSure())
    {
        //ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
        // get data from the page and save it.

        const int id = ui->artistAccDetails_idLabel->text().toInt();
        const QString email = ui->artistAccDetails_emailLineEdit->text();
        const QString nickname = ui->artistAccDetails_nicknameLineEdit->text();
        const QString password = ui->artistAccDetails_passwordLineEdit->text();
        const QString dateOfBirth = ui->artistAccDetails_birthDateEdit->date().toString("yyyy-MM-dd");
        const QString gender = ui->artistAccDetails_genderComboBox->currentText();

        try
        {
            db->updateArtist(id, email, nickname, password, dateOfBirth, gender);
        }
        catch (QString msg)
        {
            showMsg(msg);
            return;
        }

        showMsg("Changes saved.");
    }
}

void MainWindow::on_artistAcc_allTracksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ALL_TRACKS_PAGE_ID);
}

void MainWindow::on_allTracks_exitButton_clicked()
{
    if (db->getCurrUserRole() == "admin")
    {
        ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
    }
}

void MainWindow::on_myTracks_backButton_clicked()
{
    if (db->getCurrUserRole() == "admin")
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
    }
    else
    {
        const int artistId = ui->myTracks_artistId->text().toInt();

        if (artistId == db->getCurrUserId())
        {
            ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
        }
    }
}

void MainWindow::on_myTracks_listenButton_clicked()
{
    int trackId;
    if ((trackId = getCurrentItemId(ui->myTracks_tracksListWidget)) >= 0)
    {
        showMsg("...wonderful music plays...");
    }
}

void MainWindow::on_myTracks_deleteButton_clicked()
{
    if (areYouSure())
    {
        int id;
        if ((id = getCurrentItemId(ui->myTracks_tracksListWidget)) >= 0)
        {
            db->deleteTrack(id);
            fillTracksList(ui->myTracks_tracksListWidget, db->getTracksInfo(id));

            showMsg("The track was deleted!");
        }
    }
}

void MainWindow::on_myTracks_detailsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(MY_TRACK_EDIT_PAGE_ID);
}

void MainWindow::on_adminAcc_signOffButton_clicked()
{
    db->signOffCurrUser();
    ui->stackedWidget->setCurrentIndex(STARTING_PAGE_ID);
}

void MainWindow::on_adminAcc_deleteMyAccButton_clicked()
{
    deleteCurrAcc();
}

void MainWindow::on_adminAcc_createAdminButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(CREATE_ADMIN_PAGE_ID);
}

void MainWindow::on_createAdmin_cancelButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
}

void MainWindow::on_createAdmin_createButton_clicked()
{
    const QString email = ui->createAdmin_emailLineEdit->text();
    if (email.isEmpty())
    {
        showMsg("Email is empty!");
        return;
    }
    const QString password = ui->createAdmin_passwordLineEdit->text();
    if (password.isEmpty())
    {
        showMsg("Password is empty!");
        return;
    }

    try
    {
        db->createAdmin(email, password);
    }
    catch (QString msg)
    {
        showMsg(msg);
        return;
    }

    ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
    showMsg("New admin created!");
}

void MainWindow::on_adminAcc_reportsButton_clicked()
{
    reportsPageInit();
    ui->stackedWidget->setCurrentIndex(REPORTS_PAGE_ID);
}

void MainWindow::on_reports_backButton_clicked()
{
    if (ui->reports_titleLabel->text() == "All reports:")
    {
        ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
    }
}

int MainWindow::extractIdFromBeginning(QString str)
{
    QTextStream stream(&str);
    int id;
    stream >> id;
    return id;
}

void MainWindow::on_reports_detailsButton_clicked()
{
    int rpId;
    if ((rpId = getCurrentItemId(ui->reports_reportsList)) >= 0)
    {
        Report rep = db->getReport(rpId);
        QString details =
                QString("Report details:\n\n"
                        "id = %1\n"
                        "type: %2\n"
                        "title: '%3'\n"
                        "contents: '%4'\n"
                        "author id: %5\n"
                        "reported id: %6")
                .arg(rep.id)
                .arg(rep.reportType)
                .arg(rep.title)
                .arg(rep.contents)
                .arg(rep.authorId)
                .arg(rep.objectId);

        showMsg(details);
    }
}

void MainWindow::on_reports_deleteButton_clicked()
{
    QListWidgetItem* item = ui->reports_reportsList->currentItem();
    if (item == nullptr)
    {
        showMsg("You didn't select any report!");
        return;
    }

    const QString currReport = item->text();

    const int rpId = extractIdFromBeginning(currReport);
    db->deleteReport(rpId);

    if (ui->reports_titleLabel->text() == "All reports:")
    {
        fillReportsList();
    }
    else
    {
        fillReportsList(ui->reports_reportsList, db->getReportsByArtist(ui->reports_artistIdLabel->text().toInt()));
    }

    showMsg("The report was deleted!");
}

void MainWindow::on_adminAcc_artistsButton_clicked()
{
    // show the list of artists
    ui->stackedWidget->setCurrentIndex(ARTISTS_PAGE_ID);
}

void MainWindow::on_adminAcc_tracksButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ALL_TRACKS_PAGE_ID);
}

void MainWindow::on_allTracks_deleteButton_clicked()
{
    if (areYouSure())
    {
        QListWidgetItem* item = ui->allTracks_tracksListWidget->currentItem();
        if (!item)
        {
            showMsg("You didn't select anything!");
            return;
        }

        const QString itemStr = item->text();
        const int trackId = extractIdFromBeginning(itemStr);

        db->deleteTrack(trackId);

        showMsg("The track was deleted");
        fillTracksList();
    }
}

void MainWindow::on_artists_backButton_clicked()
{
    if (db->getCurrUserRole() == "admin")
    {
        ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
    }
}

void MainWindow::on_artists_detailsButton_clicked()
{
    QListWidgetItem* item = ui->artists_artistsList->currentItem();
    if (!item)
    {
        showMsg("You didn't select anything!");
        return;
    }

    int id;
    if ((id = getCurrentItemId(ui->artists_artistsList)) >= 0)
    {
        artistAccDetailsPageInit(id);
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
    }
}

void MainWindow::on_artists_deleteButton_clicked()
{
    if (areYouSure())
    {
        QListWidgetItem* item = ui->artists_artistsList->currentItem();
        if (!item)
        {
            showMsg("You didn't select anything!");
            return;
        }

        const QString itemStr = item->text();
        const int artistId = extractIdFromBeginning(itemStr);

        db->deleteAccount(artistId);

        showMsg("Artist was deleted");
        fillArtistsList();
    }
}

int MainWindow::getCurrentItemId(QListWidget *list)
{
    QListWidgetItem* item = list->currentItem();
    if (!item)
    {
        showMsg("You didn't select anything!");
        return -1;
    }

    const QString itemStr = item->text();
    const int id = extractIdFromBeginning(itemStr);
    return id;
}

void MainWindow::on_artistAccDetails_tracksButton_clicked()
{
    myTracksPageInit(ui->artistAccDetails_idLabel->text().toInt());
    ui->stackedWidget->setCurrentIndex(MY_TRACKS_PAGE_ID);
}

void MainWindow::on_artistAccDetails_commentsButton_clicked()
{
    commentsPageInit(ui->artistAccDetails_idLabel->text().toInt());
    ui->stackedWidget->setCurrentIndex(COMMENTS_PAGE_ID);
}

void MainWindow::on_comments_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
}

void MainWindow::on_comments_deleteButton_clicked()
{
    if (areYouSure())
    {
        int id;
        if ((id = getCurrentItemId(ui->comments_commentsList)) >= 0)
        {
            db->deleteRecordFromTable("Comments", id);
            showMsg("The comment was deleted");
            fillCommentsList(ui->comments_commentsList, db->getCommentsBy(ui->comments_idLabel->text().toInt()));
        }
    }
}

void MainWindow::on_artistAccDetails_reportsButton_clicked()
{
    reportsPageInit(ui->artistAccDetails_idLabel->text().toInt());
    ui->stackedWidget->setCurrentIndex(REPORTS_PAGE_ID);
    //showMsg("reports");
}

void MainWindow::on_artistAccDetails_playlistsButton_clicked()
{
    playlistsPageInit(ui->artistAccDetails_idLabel->text().toInt());
    ui->stackedWidget->setCurrentIndex(PLAYLISTS_PAGE_ID);
}

void MainWindow::on_playlists_backButton_clicked()
{
    // TODO: add check for title
    if (ui->playlists_titleLabel->text() == "All playlists:")
    {
        ui->stackedWidget->setCurrentIndex(ARTISTS_PAGE_ID);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_DETAILS_PAGE_ID);
    }
}

void MainWindow::on_playlists_deleteButton_clicked()
{
    if (areYouSure())
    {
        int id;
        if ((id = getCurrentItemId(ui->playlists_playlistsList)) >= 0)
        {
            db->deletePlaylist(id);

            if (ui->playlists_titleLabel->text() == "All playlists:")
            {
                fillPlaylistsList(ui->playlists_playlistsList, db->getAllPlaylists());
            }
            else
            {
                fillPlaylistsList(ui->playlists_playlistsList, db->getPlaylistsBy(ui->playlists_artistIdLabel->text().toInt()));
            }

            showMsg("The playlist was deleted!");
        }
    }
}

void MainWindow::on_artistAcc_artistsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ARTISTS_PAGE_ID);
}

void MainWindow::on_myTracks_uploadTrackButton_clicked()
{
    bool ok = false;
    QString title;
    int length;
    while (!ok)
    {
        title = QInputDialog::getText(this, "Title", "Enter the track title: ", QLineEdit::Normal, QString(), &ok);
    }

    ok = false;
    while (!ok)
    {
        length = QInputDialog::getInt(this, "Length", "Enter the track length (seconds)", 1, 1, 1000, 1, &ok);
    }

    const int artistId = ui->myTracks_artistId->text().toInt();

    try
    {
        db->createTrack(artistId, title, length);
    }
    catch (QString err)
    {
        showMsg(err);
        return;
    }

    fillTracksList(ui->myTracks_tracksListWidget, db->getTracksInfo(artistId));
}
