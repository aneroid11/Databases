--ЗАПРОСЫ:

--Процедура для логирования:

delimiter //

create procedure Log (user_id int, object_type enum('Users', 'Artists', 'CardDetails', 'Likes', 'Comments', 'Reports', 'PremiumSubscriptions', 'Payments', 'Tags', 'Tracks', 'Playlists', 'Albums', 'TagsToTracks', 'TagsToPlaylists', 'TracksToPlaylists'), object_id int, action_type enum('create', 'update', 'delete'))
begin
    insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (CURRENT_TIMESTAMP(), user_id, object_type, object_id, action_type);
end//


-- Регистрация артиста:

id int not null auto_increment,
email varchar(255) not null unique,
password_hash varchar(255) not null,
nickname varchar(100) not null unique,
date_of_birth date,
gender enum('male', 'female', 'other') not null,

create procedure RegisterArtist(email varchar(255), password_hash varchar(255), nickname varchar(100), date_of_birth date, gender enum('male', 'female', 'other'))
begin
    insert into Users (email, password_hash, role) values (email, password_hash, 'artist');
    
    if row_count() > 0 then
        -- previous insert was successful
        insert into Artists values (last_insert_id(), nickname, date_of_birth, gender, NULL, NULL);
    end if;
end//


--Создать вьюшку для информации об артисте:

create view ArtistsInfo as 
select Users.id, Users.email, Users.password_hash, Artists.nickname, Artists.date_of_birth, Artists.gender, Artists.premium_subscription_id, Artists.card_details_id 
from Users 
right join Artists on Users.id = Artists.id;


--Просмотреть всю информацию об артисте:

select * from ArtistInfo where id = <artist id>;
-- insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (<current timestamp>, <watching artist id>, 'Artists', <watched artist id>, 'read');

--Поиск артиста по никнейму:
select * from Artists where LOWER(nickname) like LOWER("%<search>%");
select * from ArtistsInfo where LOWER(nickname) like LOWER("%<search>%");


-- Изменить информацию об артисте:
update Artists set nickname = <nickname> where id = <artist id>
-- insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (<current timestamp>, <artist id>, 'Artists', <artist id>, 'edit');


-- Удалить аккаунт артиста:
create procedure DeleteArtistAccount(id int)
begin
    delete from Artists where Artists.id = id;
    
    -- delete all orphaned PremiumSubscriptions and CardDetails:
    delete from PremiumSubscriptions where PremiumSubscriptions.id not in (select premium_subscription_id from Artists where premium_subscription_id is not null);    
    delete from CardDetails where CardDetails.id not in (select card_details_id from Artists where card_details_id is not null);
    -- insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (<current timestamp>, <artist id>, 'Artists', <artist id>, 'delete');
end//


-- Загрузка артистом треков на сайт:
create procedure UploadTrack(title varchar(100), length_seconds int, artist_id int)
begin
    insert into Tracks (timestamp, title, length_seconds, artist_id) values (CURRENT_TIMESTAMP(), title, length_seconds, artist_id);
    -- insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (<current timestamp>, <artist id>, 'Tracks', LAST_INSERT_ID(), 'create');
end//


-- Прикрепить тег <tag name> к треку <track id>:
create procedure AttachTagToTrack(tag_name varchar(45), track_id int)
begin
    insert into Tags (name) values (tag_name); -- will fail if the tag exists
    insert into TagsToTracks (id_tag, id_track) select id, track_id from Tags where name = tag_name;
end//


-- Просмотреть всю информацию о треке <track id>:
-- Вьюшка для этого:
create view TracksInfo as 
select Tracks.id, Tracks.timestamp, Tracks.title, Tracks.length_seconds, Artists.nickname 
from Tracks 
inner join Artists 
on Tracks.artist_id = Artists.id;

select * from TracksInfo where Tracks.id = <track id>;


-- Количество треков у артистов:
select count(Tracks.id) total_tracks, Artists.nickname 
from Tracks 
inner join Artists on Artists.id = Tracks.artist_id  
group by artist_id;

-- Комментировать трек <track id> от <artist id>:

create procedure CommentTrack(contents varchar(500), artist_id int, track_id int)
begin
    insert into Comments (timestamp, contents, artist_id, track_id) values (CURRENT_TIMESTAMP(), contents, artist_id, <track_id);
end//

-- Отредактировать трек <track id>:
update Tracks set timestamp = <current timestamp>, title = <new title>, length_seconds = <new length> where id = <track id>;

Удалить трек <track id>:
delete from Tracks where id = <track id>;

Лайкнуть трек <track id> от <artist id>:
insert into Likes (artist_id, track_id) values (<artist id>, <track id>);


-- Пожаловаться на трек <track id> от <artist id>:
create procedure ReportTrack(title varchar(50), contents varchar(200), author_id int, track_id int)
begin
    insert into Reports (title, contents, author_id, report_type, object_id) values (title, contents, author_id, 'Tracks', track_id);
end

-- Пожаловаться на артиста <reported artist id> от <reporting artist id>:
create procedure ReportArtist(title varchar(50), contents varchar(200), author_id int, reported_artist_id int)
begin
    insert into Reports (title, contents, author_id, report_type, object_id) values (title, contents, author_id, 'Artists', reported_artist_id);
end

-- Создать плейлист от <artist id>:
insert into Playlists (title, artist_id) values (<playlist title>, <artist id>);

-- Отредактировать плейлист:
update Playlists set title = <new title> where id = <playlist id>;

-- Удалить плейлист:
delete from Playlists where id = <playlist id>;

-- Добавить трек <track id> в плейлист <playlist id>:
create procedure AddTrack(track_id int, playlist_id int)
begin
    insert into TracksToPlaylists (track_id, playlist_id) values (track_id, playlist_id);
end//

Удалить трек <track id> из плейлиста <playlist id>:
create or replace procedure DeleteFromPlaylist(p_track_id int, p_playlist_id int)
begin
    -- delete from TracksToPlaylists where track_id = track_id and playlist_id = playlist_id; -- this will delete all TracksToPlaylists
    delete from TracksToPlaylists where track_id = p_track_id and playlist_id = p_playlist_id;
end//

Информация о треках, принадлежащих плейлистам:
create view TracksPlaylistsInfo as
select TracksToPlaylists.playlist_id, TracksToPlaylists.track_id, Tracks.timestamp, Tracks.title, Tracks.length_seconds, Tracks.artist_id 
from TracksToPlaylists 
inner join Tracks 
on Tracks.id = TracksToPlaylists.track_id;

Просмотреть все треки в плейлисте <playlist id>:
select * from TracksPlaylistsInfo where playlist_id = <playlist id>;

Создать альбом <title>, <release date> от <artist id>:
insert into Playlists (title, artist_id) values (<title>, <artist id>);
insert into Albums values (last_inserted_id(), <release date>);

Вьюшка для информации об альбомах:
create view AlbumsInfo as 
select Albums.id, Albums.release_date, Playlists.title, Artists.nickname as author_name 
from Albums 
inner join Playlists on Albums.id = Playlists.id 
inner join Artists on Playlists.artist_id = Artists.id;

Просмотреть информацию об альбоме:
select * from AlbumsInfo where id = <album id>;

Удалить альбом:
delete from Playlists where id = <album id>;

Посмотреть общую продолжительность треков для каждого артиста:
select sum(Tracks.length_seconds) total_tracks_time, Artists.nickname, Tracks.artist_id  
from Tracks 
inner join Artists on Artists.id = Tracks.artist_id 
group by artist_id;

Посмотреть общую продолжительность треков для отдельного артиста:
select sum(Tracks.length_seconds) total_tracks_time, Artists.nickname, Tracks.artist_id  
from Tracks 
inner join Artists on Artists.id = Tracks.artist_id 
where artist_id = <artist id>;

Количество лайков на треке:
select count(id) num_of_likes from Likes where track_id = <track id>;

Посмотреть топ-10 самых залайканных треков:
select Tracks.id, Tracks.title, count(Likes.id) as likes_count, 
    case
        when count(Likes.id) > 2 then "superstar"
        else ""
    end as commentary
from Likes 
inner join Tracks on Tracks.id = Likes.track_id 
group by Likes.track_id 
order by likes_count desc 
limit 10;

Приобрести премиум-подписку:
insert into CardDetails (first_name, last_name, card_number, expiration) values (<first name>, <last name>, <card number>, <expiration date>);
update Artists set card_details_id = last_inserted_id() where id = <artist id>; -- а что, если инсерт зафейлился? например, card_number уже существует.

insert into PremiumSubscriptions (start_datetime, end_datetime, active, id_tariff) values (<current timestamp>, <current timestamp>, FALSE, <tariff id>);

insert into Payments (subscription_id, timestamp, status, sum, transaction_id) 
select last_insert_id(), <current timestamp>, 'successful', Tariffs.monthly_payment_dollars 
from Tariffs where id = <tariff id>;

-- после успешной оплаты:
update PremiumSubscriptions set end_datetime = DATE_ADD(end_datetime, interval 1 month), active = TRUE where id = <subscription id>;

Просмотреть все жалобы:
select * from Reports;

Просмотреть жалобы на пользователя <user id>:
select * from Reports where object_id = <user id> and report_type = "Artists";

Просмотреть жалобы на трек <track id>:
select * from Reports where object_id = <track id> and report_type = 'Tracks';

Просмотреть действия пользователя:
select * from Actions where user_id = <user id> order by timestamp;


