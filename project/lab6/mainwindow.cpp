#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sha256.h"

#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QList>

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
    MY_PLAYLISTS_PAGE_ID = 10,
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

void MainWindow::fillReportsList()
{
    ui->reports_reportsList->clear();

    QList<Report> reports = db->getAllReports();

    for (const Report& rp : reports)
    {
        QString infoStr = QString("%1 - '%2'").arg(rp.id).arg(rp.title);
        ui->reports_reportsList->addItem(infoStr);
    }
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

void MainWindow::reportsPageInit()
{
    fillReportsList();
}

void MainWindow::artistsPageInit()
{
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
    const bool admRole = role == "admin";

    ui->artistAccDetails_saveChangesButton->setDisabled(admRole);
    ui->artistAccDetails_birthDateEdit->setDisabled(admRole);
    ui->artistAccDetails_genderComboBox->setDisabled(admRole);
    ui->artistAccDetails_emailLineEdit->setDisabled(admRole);
    ui->artistAccDetails_passwordLineEdit->setDisabled(admRole);
    ui->artistAccDetails_nicknameLineEdit->setDisabled(admRole);

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

    fillTracksList(ui->myTracks_tracksListWidget, db->getTracksInfo(artistId));
}

void MainWindow::commentsPageInit(const int artistId)
{
    //fillCommen
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
    case REPORTS_PAGE_ID:
        reportsPageInit();
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
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
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
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
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
        ui->stackedWidget->setCurrentIndex(ARTIST_ACC_PAGE_ID);
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
    ui->stackedWidget->setCurrentIndex(REPORTS_PAGE_ID);
}

void MainWindow::on_reports_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
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
    const QString currReport = ui->reports_reportsList->currentItem()->text();
    const int rpId = extractIdFromBeginning(currReport);

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

    fillReportsList();

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
    ui->stackedWidget->setCurrentIndex(ADMIN_ACC_PAGE_ID);
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
