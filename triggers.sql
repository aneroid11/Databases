-- при заходе пользователя в аккаунт:
set @curr_session_user_id = <user_id>;

-- ТРИГГЕРЫ:

-- для логов:
create trigger ArtistRegister before insert on Artists
for each row
    set @curr_session_user_id = new.id;//

    
create procedure LogWithCurrUser(object_type enum('Users', 'Artists', 'CardDetails', 'Likes', 'Comments', 'Reports', 'PremiumSubscriptions', 'Payments', 'Tags', 'Tracks', 'Playlists', 'Albums', 'TagsToTracks', 'TagsToPlaylists', 'TracksToPlaylists'), object_id int, action_type enum('create', 'update', 'delete'))
begin
    if @curr_session_user_id is not NULL then
        call Log(@curr_session_user_id, object_type, object_id, action_type);
    end if;
end//
    
A665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3
A665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3

create trigger LogUsersCreate after insert on Users 
for each row
    call LogWithCurrUser("Users", new.id, 'create')//
    
create trigger LogUsersUpdate after update on Users 
for each row
    call LogWithCurrUser("Users", new.id, 'update')//
    
create trigger LogUsersDelete after delete on Users 
for each row
begin
    call LogWithCurrUser("Users", old.id, 'delete');
    
    if old.id = @curr_session_user_id then
        set @curr_session_user_id = NULL;
    end if;
end//

    
create trigger LogArtistsCreate after insert on Artists
for each row
    call LogWithCurrUser("Artists", new.id, 'create')//
    
create trigger LogArtistsUpdate after update on Artists
for each row
    call LogWithCurrUser("Artists", new.id, 'update')//
    
create trigger LogArtistsDelete after delete on Artists
for each row
    call LogWithCurrUser("Artists", old.id, 'delete')//
    
    
create trigger LogCardDetailsCreate after insert on CardDetails
for each row
    call LogWithCurrUser("CardDetails", new.id, 'create')//
    
create trigger LogCardDetailsUpdate after update on CardDetails
for each row
    call LogWithCurrUser("CardDetails", new.id, 'update')//
    
create trigger LogCardDetailsDelete after delete on CardDetails
for each row
    call LogWithCurrUser("CardDetails", old.id, 'delete')//
    

create trigger LogPaymentsCreate after insert on Payments
for each row
    call LogWithCurrUser("Payments", new.id, 'create')//
    
create trigger LogPaymentsUpdate after update on Payments
for each row
    call LogWithCurrUser("Payments", new.id, 'update')//
    
create trigger LogPaymentsDelete after delete on Payments
for each row
    call LogWithCurrUser("Payments", old.id, 'delete')//
   
   
create trigger LogLikesCreate after insert on Likes
for each row
    call LogWithCurrUser("Likes", new.id, 'create')//
    
create trigger LogLikesUpdate after update on Likes
for each row
    call LogWithCurrUser("Likes", new.id, 'update')//
    
create trigger LogLikesDelete after delete on Likes
for each row
    call LogWithCurrUser("Likes", old.id, 'delete')//
    

create trigger LogCommentsCreate after insert on Comments
for each row
    call LogWithCurrUser("Comments", new.id, 'create')//
    
create trigger LogCommentsUpdate after update on Comments
for each row
    call LogWithCurrUser("Comments", new.id, 'update')//
    
create trigger LogCommentsDelete after delete on Comments
for each row
    call LogWithCurrUser("Comments", old.id, 'delete')//
    
    
create trigger LogReportsCreate after insert on Reports
for each row
    call LogWithCurrUser("Reports", new.id, 'create')//
    
create trigger LogReportsUpdate after update on Reports
for each row
    call LogWithCurrUser("Reports", new.id, 'update')//
    
create trigger LogReportsDelete after delete on Reports
for each row
    call LogWithCurrUser("Reports", old.id, 'delete')//

-- остальные триггеры для логов аналогично
-- конец триггеров для логов

-- если оплата прошла успешно, продлить подписку
create trigger ProlongSubscriptionOnPayment after insert on Payments
for each row
begin
    declare prolong_months float;

    -- после успешной оплаты
    if new.status = "successful" then
        -- сколько месяцев оплатил пользователь?
        set prolong_months = new.sum / (
            select monthly_payment_dollars from Tariffs where id = (
                select id_tariff from PremiumSubscriptions where id = new.subscription_id
            )
        );
    
        update PremiumSubscriptions 
        -- set end_datetime = DATE_ADD(end_datetime, interval 1 month), active = TRUE 
        set end_datetime = DATE_ADD(
            case 
                when end_datetime < sysdate() then sysdate()
                else end_datetime
            end,
            interval prolong_months month
        ), active = TRUE 
        where id = new.subscription_id;
    end if;
end//


-- Проверка на саморепорт
create procedure CheckSelfReportProc(report_type enum('Tracks', 'Artists'), author_id int, object_id int)
begin
    if report_type = "Artists" and author_id = object_id then
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Self-report is not permitted!';
    end if;
end//

create trigger CheckSelfReportIns before insert on Reports
for each row
begin
    call CheckSelfReportProc(new.report_type, new.author_id, new.object_id);
end//

create trigger CheckSelfReportUpd before update on Reports
for each row
begin
    call CheckSelfReportProc(new.report_type, new.author_id, new.object_id);
end//


-- проверка на самолайк
create procedure CheckSelfLikeProc(track_id int, author_id int)
begin
    if author_id = (select artist_id from Tracks where id = track_id) then
        SIGNAL SQLSTATE '45000' -- user-defined exceptions
        SET MESSAGE_TEXT = 'Self-like is not permitted!';
    end if;
end//

create trigger CheckSelfLikeIns before insert on Likes
for each row
begin
    call CheckSelfLikeProc(new.track_id, new.artist_id);
end//

create trigger CheckSelfLikeUpd before update on Likes
for each row
begin
    call CheckSelfLikeProc(new.track_id, new.artist_id);
end//

-- проверка, не пытается ли пользователь добавить не свой трек в альбом
create trigger CheckAddingTrackToPlaylist before insert on TracksToPlaylists
for each row
begin
    declare playlist_artist_id int;
    declare track_artist_id int;

    -- если это альбом
    if exists (select 1 from Albums where id = new.playlist_id) then
        -- проверить, совпадает ли айдишник артиста для трека с айдишником артиста для плейлиста
        set playlist_artist_id = (select artist_id from Playlists where id = new.playlist_id);
        set track_artist_id = (select artist_id from Tracks where id = new.track_id);
        
        if playlist_artist_id != track_artist_id then
            SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'You cannot add a track that is not yours to an album!';
        end if;
    end if;
end//
