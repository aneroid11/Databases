delimiter //

-- ПРОЦЕДУРЫ:

-- Процедура для логирования:
create procedure Log (user_id int, object_type enum('Users', 'Artists', 'CardDetails', 'Likes', 'Comments', 'Reports', 'PremiumSubscriptions', 'Payments', 'Tags', 'Tracks', 'Playlists', 'Albums', 'TagsToTracks', 'TagsToPlaylists', 'TracksToPlaylists'), object_id int, action_type enum('create', 'update', 'delete'))
begin
    insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (CURRENT_TIMESTAMP(), user_id, object_type, object_id, action_type);
end//


-- Регистрация артиста:

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


--Поиск артиста по никнейму:
select * from Artists where LOWER(nickname) like LOWER("%<search>%");
select * from ArtistsInfo where LOWER(nickname) like LOWER("%<search>%");


-- Изменить информацию об артисте:
update Artists set nickname = <nickname> where id = <artist id>;
-- insert into Actions (timestamp, user_id, object_type, object_id, action_type) values (<current timestamp>, <artist id>, 'Artists', <artist id>, 'update');


-- Удалить аккаунт артиста:
create procedure DeleteArtistAccount(artist_id int)
begin
    delete from Users where id = artist_id;
    
    -- delete all orphaned PremiumSubscriptions and CardDetails:
    delete from PremiumSubscriptions where id not in (select premium_subscription_id from Artists where premium_subscription_id is not null);    
    delete from CardDetails where id not in (select card_details_id from Artists where card_details_id is not null);
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
    insert into Comments (timestamp, contents, artist_id, track_id) values (CURRENT_TIMESTAMP(), contents, artist_id, track_id);
end//

-- комментарии к треку:
select Comments.contents, Artists.nickname from Comments inner join Artists on Comments.artist_id = Artists.id where track_id = 5;

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
create procedure AddTrackToPlaylist(track_id int, playlist_id int)
begin
    insert into TracksToPlaylists (track_id, playlist_id) values (track_id, playlist_id);
end//

-- Удалить трек <track id> из плейлиста <playlist id>:
create procedure DeleteFromPlaylist(p_track_id int, p_playlist_id int)
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

-- Просмотреть все треки в плейлисте <playlist id>:
select * from TracksPlaylistsInfo where playlist_id = <playlist id>;

-- Создать альбом <title>, <release date> от <artist id>:
create procedure CreateAlbum(title varchar(100), artist_id int, release_date date)
begin
    insert into Playlists (title, artist_id) values (title, artist_id);
    
    if row_count() > 0 then
        -- previous insert was successful
        insert into Albums values (last_insert_id(), release_date);
    end if;
end//

-- Просмотреть все плейлисты и альбомы
select Playlists.id, Playlists.title, Playlists.artist_id, Albums.release_date, Artists.nickname from Playlists left join Albums on Playlists.id = Albums.id left join Artists on artist_id = Artists.id;

-- Вьюшка для информации об альбомах:
create view AlbumsInfo as 
select Albums.id, Albums.release_date, Playlists.title, Artists.nickname as author_name 
from Albums 
inner join Playlists on Albums.id = Playlists.id 
inner join Artists on Playlists.artist_id = Artists.id;

-- Просмотреть информацию об альбоме:
select * from AlbumsInfo where id = <album id>;

-- Удалить плейлист/альбом:
create procedure DeletePlaylist(playlist_id int)
begin
    delete from Playlists where id = playlist_id;
end//

-- Посмотреть общую продолжительность треков для каждого артиста:
create procedure TracksTotalLength()
begin
    select sum(Tracks.length_seconds) total_tracks_time, Artists.nickname, Tracks.artist_id  
    from Tracks 
    inner join Artists on Artists.id = Tracks.artist_id 
    group by artist_id;
end//

-- Посмотреть общую продолжительность треков для отдельного артиста:
create procedure ArtistTracksTotalLength(p_artist_id int)
begin
    -- why is this not working when in procedure, but working when not?
    select sum(Tracks.length_seconds) total_tracks_time, Artists.nickname, Tracks.artist_id  
    -- select sum(Tracks.length_seconds) total_tracks_time
    from Tracks 
    inner join Artists on Artists.id = Tracks.artist_id 
    where artist_id = p_artist_id
    -- doesn't work without group by (in procedure)
    group by artist_id;
end//


-- Количество лайков на треке:
create procedure LikesOnTrack(p_track_id int)
begin
    select count(id) num_of_likes from Likes where track_id = p_track_id;
end//


-- Посмотреть топ-10 самых залайканных треков:
create procedure TopLikedTracks(num_of_tracks int)
begin
    select Tracks.id, Tracks.title, count(Likes.id) as likes_count, 
    case
        when count(Likes.id) > 2 then "superstar"
        else ""
    end as commentary
    from Likes 
    inner join Tracks on Tracks.id = Likes.track_id 
    group by Likes.track_id 
    order by likes_count desc 
    limit num_of_tracks;
end//


-- данные премиум-подписки пользователя
create procedure PremiumSubscriptionInfo(artist_id int)
begin
    select * from PremiumSubscriptions where id = (select premium_subscription_id from Artists where id = artist_id);
end//


-- есть ли у пользователя подписка?
create function ArtistHasSubscription(artist_id int)
returns boolean
deterministic
begin
    return (select (premium_subscription_id is not null) as has_subscription from Artists where id = artist_id);
end//


-- отвязать данные карты у пользователя
create procedure DeattachCardDetails(artist_id int)
begin
    delete from CardDetails where id = (select card_details_id from Artists where id = artist_id);
end//


-- привязать данные карты к пользователю
create procedure AttachCardDetails(artist_id int, first_name varchar(60), last_name varchar(80), card_number varchar(16), expiration date)
begin
    -- delete previous card details
    call DeattachCardDetails(artist_id);
    
    insert into CardDetails (first_name, last_name, card_number, expiration) values (first_name, last_name, card_number, expiration);
    
    if row_count() > 0 then
        -- previous insert was successful
        update Artists set card_details_id = last_insert_id() where id = artist_id;
    end if;
end//


-- Создать премиум-подписку для пользователя
create procedure CreatePremiumSubscription(artist_id int)
begin
    insert into PremiumSubscriptions (start_datetime, end_datetime, active, id_tariff) values (CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), FALSE, 1);
    
    if row_count() > 0 then
        update Artists set premium_subscription_id = last_insert_id() where id = artist_id;
    end if;
end//


-- закрыть подписку
update PremiumSubscriptions set active = FALSE where id = <id>;//


-- Произвести оплату для подписки subscription_id
create procedure CreatePayment(subscription_id int, amount float, status enum('failed', 'successful'), transaction_id varchar(100))
begin
    insert into Payments (subscription_id, timestamp, status, sum, transaction_id) 
    values (subscription_id, CURRENT_TIMESTAMP(), status, amount, transaction_id);
end//

create procedure CreatePaymentForMonth(subscription_id int, status enum('failed', 'successful'), transaction_id varchar(100))
begin
    call CreatePayment(
        subscription_id, 
        (select monthly_payment_dollars from Tariffs where id = (
            select id_tariff from PremiumSubscriptions where id = subscription_id
        )),
        status,
        transaction_id
    );
end//


-- Просмотреть все жалобы:
select * from Reports;

-- Просмотреть жалобы на пользователя <user id>:
select * from Reports where object_id = <user id> and report_type = "Artists";

-- Просмотреть жалобы на трек <track id>:
select * from Reports where object_id = <track id> and report_type = 'Tracks';

-- Просмотреть действия пользователя:
select * from Actions where user_id = <user id> order by timestamp;
