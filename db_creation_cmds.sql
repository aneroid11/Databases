-- Создать базу данных:
create database task3;
use task3;

-- Создание таблиц:

-- Пользователи (Users):
create table Users (
    id int not null auto_increment,
    email varchar(255) not null unique,
    password_hash varchar(255) not null,
    role enum('artist', 'admin', 'moderator') not null,

    constraint valid_email check(email REGEXP '(\w|\.)+@[a-z]+\.[a-z]+'),

    primary key(id)
);

Артисты (Artists):
create table Artists (
    id int not null auto_increment,
    nickname varchar(100) not null unique,
    date_of_birth date,
    gender enum('male', 'female', 'other') not null,
    premium_subscription_id int unique,     -- one to one relation
    card_details_id int unique,             -- the same here

    check(date_of_birth between '1900-01-01' and sysdate()),

    primary key(id),
    foreign key(id) references Users(id) on update cascade on delete cascade,
    foreign key(premium_subscription_id) references PremiumSubscriptions(id) on update cascade on delete set null,
    foreign key(card_details_id) references CardDetails(id) on update cascade on delete set null
);

Данные банковских карт (CardDetails):
create table CardDetails (
    id int not null auto_increment,
    first_name varchar(60) not null,
    last_name varchar(80) not null,
    card_number varchar(16) not null unique,
    expiration date not null,
    
    check (card_number regexp '[0-9]{16}'),
    check (expiration > sysdate()),

    primary key(id)
);

Премиум-подписки:
create table PremiumSubscriptions (
    id int not null auto_increment,
    start_datetime datetime not null,
    end_datetime datetime not null,
    active boolean not null,
    id_tariff int not null,
    
    check (start_datetime < end_datetime and cast(start_datetime as date) > '1900-01-01'),   -- yyyy-mm-dd
    check (end_datetime > sysdate()),

    primary key(id),
    foreign key(id_tariff) references Tariffs(id) on update cascade on delete restrict
);

Тарифы:
create table Tariffs (
    id int not null auto_increment,
    monthly_payment_dollars float not null,

    check (monthly_payment_dollars > 0.0),

    primary key(id)
);

Платежи:
create table Payments (
    id int not null auto_increment,
    subscription_id int not null,
    timestamp datetime not null,
    status enum('failed', 'successful') not null,
    sum float not null,
    transaction_id varchar(100) not null unique,

    check (sum > 0.0),
    check (timestamp <= sysdate()),

    primary key(id),
    foreign key(subscription_id) references PremiumSubscriptions(id) on update cascade on delete cascade
);

Жалобы:
create table Reports (
    id int not null auto_increment,
    title varchar(50) not null,
    contents varchar(200),
    author_id int not null,
    report_type enum('Tracks', 'Artists') not null,
    object_id int not null,
    
    -- check for self-report

    primary key(id),
    foreign key(author_id) references Artists(id) on update cascade on delete cascade
);


Лайки:
create table Likes (
    id int not null auto_increment,
    artist_id int,
    track_id int not null,
    
    -- check for self-like

    primary key(id),
    foreign key(artist_id) references Artists(id) on update cascade on delete set null,
    foreign key(track_id) references Tracks(id) on update cascade on delete cascade
);

Теги:
create table Tags (
    id int not null auto_increment,
    name varchar(45) not null unique,

    primary key(id)
);

Треки:
create table Tracks (
    id int not null auto_increment,
    timestamp datetime not null,
    title varchar(100) not null, -- index for title
    length_seconds int not null,
    artist_id int not null,

    check (length_seconds > 0),
    check (timestamp <= sysdate()),

    primary key(id),
    foreign key(artist_id) references Artists(id) on update cascade on delete cascade,
    index (title)
);

Связь между тегами и треками:
create table TagsToTracks (
    id int not null auto_increment,
    id_tag int not null,
    id_track int not null,

    unique(id_tag, id_track),

    primary key(id),
    foreign key(id_tag) references Tags(id) on update cascade on delete cascade,
    foreign key(id_track) references Tracks(id) on update cascade on delete cascade
);

-- Комментарии:
create table Comments (
    id int not null auto_increment,
    timestamp datetime not null,
    contents varchar(500) not null,
    artist_id int,
    track_id int not null,

    check (timestamp <= sysdate()),

    primary key(id),
    foreign key(artist_id) references Artists(id) on delete set null on update cascade,
    foreign key(track_id) references Tracks(id) on delete cascade on update cascade
);

-- Плейлисты:
create table Playlists (
    id int not null auto_increment,
    title varchar(100) not null,
    artist_id int not null,

    -- unique(title, artist_id), -- soundcloud allows for repeating playlist names
    
    primary key(id),
    foreign key(artist_id) references Artists(id) on delete cascade on update cascade
);

Связь тегов с плейлистами:
create table TagsToPlaylists (
    id int not null auto_increment,
    id_tag int not null,
    id_playlist int not null,

    unique(id_tag, id_playlist),

    primary key(id),
    foreign key(id_tag) references Tags(id) on delete cascade on update cascade,
    foreign key(id_playlist) references Playlists(id) on delete cascade on update cascade
);

Связь плейлистов с треками:
create table TracksToPlaylists (
    id int not null auto_increment,
    track_id int not null,
    playlist_id int not null,

    unique (track_id, playlist_id),

    primary key(id),
    foreign key(track_id) references Tracks(id) on delete cascade on update cascade,
    foreign key(playlist_id) references Playlists(id) on delete cascade on update cascade
);

-- Альбомы:
create table Albums (
    id int not null auto_increment,
    release_date date not null,

    check (release_date <= sysdate()),

    primary key(id),
    foreign key(id) references Playlists(id) on update cascade on delete cascade
);

-- Журнал действий пользователей:
create table Actions (
    id int not null auto_increment,
    timestamp datetime not null,
    user_id int,     -- если юзер удаляется, то его действия всё равно сохраняются
    object_type enum('Users', 'Artists', 'CardDetails', 'Likes', 'Comments', 'Reports', 'PremiumSubscriptions', 'Payments', 'Tags', 'Tracks', 'Playlists', 'Albums', 'TagsToTracks', 'TagsToPlaylists', 'TracksToPlaylists') not null,
    object_id int,   -- если объект удаляется, то действия над ним всё равно остаются
    action_type enum('create', 'update', 'delete') not null,

    check (timestamp <= sysdate()),

    primary key(id),
    -- foreign key(user_id) references Users(id) on update cascade on delete set null
);
