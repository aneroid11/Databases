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

create trigger ProlongSubscriptionOnPayment after insert on Payments
for each row

