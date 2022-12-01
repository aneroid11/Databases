-- ТЕСТОВЫЕ ЗНАЧЕНИЯ:

-- Users:
insert into Users (email, password_hash, role) values ('user1@example.com', 'aaaa1111aaaa1111', 'artist');
insert into Users (email, password_hash, role) values ('user2@example.com', 'aaaa1111aaaa1111', 'artist');
insert into Users (email, password_hash, role) values ('musicserviceadmin@example.com', '9ba827640bde', 'admin');
insert into Users values (4, 'mod-1@example.com', '99aaa99a9a', 'moderator');
insert into Users values (5, 'mod-2@example.com', '194590afe0', 'moderator');
insert into Users values (6, 'junoreactor@yopmail.com', '312312311', 'artist');
insert into Users values (7, 'daniellicht@example.com', '3123aaa12', 'artist');
insert into Users values (8, 'suse@example.com', '123456', 'artist');

----------------------

--Artists:
insert into Artists values (1, "Zob Rombie", null, 'male', 1, 1);
insert into Artists values (2, "User2", '1999-01-02', 'male', null, null);
insert into Artists values (6, "Juno Reactor", null, 'male', null, null);
insert into Artists values (7, "Daniel Licht", '1957-03-13', 'male', null, null);
insert into Artists values (8, "Suse", null, 'other', null, null);

----------------------
Tariffs:
insert into Tariffs (monthly_payment_dollars) values (10.3);

----------------------
CardDetails:
insert into CardDetails (first_name, last_name, card_number, expiration) values ('John', 'John', '0000000000010000', '2023-01-01');
insert into CardDetails (first_name, last_name, card_number, expiration) values ('Batt', 'Mellamy', '1111000011110000', '2026-01-01');

----------------------
PremiumSubscriptions:
insert into PremiumSubscriptions (start_datetime, end_datetime, active, id_tariff) values ('2022-11-03 11:17:02', '2022-12-03 11:17:02', true, 1);
insert into PremiumSubscriptions (start_datetime, end_datetime, active, id_tariff) values ('2022-09-20 05:05:05', '2022-12-01 01:02:03', true, 1);

----------------------
Payments:
Подписка Zob Rombie:
insert into Payments values (1, 1, '2022-11-03 11:10:03', 'failed', 10.3, '1a2bcd3');
insert into Payments values (2, 1, '2022-11-03 11:17:02', 'successful', 10.3, '1aadc33');

Подписка Suse:
insert into Payments values (3, 2, '2021-09-20 05:05:05', 'successful', 10.3, '11213ac');
insert into Payments values (4, 2, '2022-11-01 01:02:03', 'successful', 10.3, '21aa3ac');

----------------------
Reports:
insert into Reports values (1, "Offensive title", "I am not worried at all", 1, 'Tracks', 1);
insert into Reports values (2, "Offensive offense", "I am offended by that offensive person", 2, 'Artists', 1);

----------------------
TRACKS:
User2:
insert into Tracks values (1, '2022-03-30 12:00:01', 'Don\'t worry', 200, 2);
insert into Tracks values (3, '2022-04-01 13:20:20', 'HDMI', 119, 2);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2018-06-06 00:31:00', 'Drive My Car', 164, 2);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2019-09-10 00:31:00', 'Everytime', 199, 2);

Zob Rombie:
insert into Tracks values (2, '2022-02-29 12:30:01', 'Some song', 125, 1);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2021-06-06 12:00:00', 'Dragula', 222, 1);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2020-03-01 09:00:00', 'Feel So Numb', 243, 1);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2022-03-01 09:30:00', 'Iron Head', 250, 1);

Juno Reactor:
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2020-03-01 09:30:00', 'Control', 250, 6);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2021-01-03 19:30:30', 'Mona Lisa Overdrive', 300, 6);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2011-01-03 19:22:30', 'Dakota', 120, 6);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2015-02-03 15:22:30', 'Guardian Angel', 230, 6);

Daniel Licht:
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2021-05-05 10:30:00', 'Wink', 129, 7);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2022-06-03 20:31:30', 'Empress Death', 65, 7);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2014-02-03 19:22:30', 'Flooded Suspense', 185, 7);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2015-03-03 15:22:30', 'Corvo Attano\'s Theme', 102, 7);

Suse:
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2010-02-05 09:30:00', 'Supermassive Black Hole', 212, 8);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2015-03-03 20:31:30', 'Hysteria', 226, 8);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2016-02-03 19:22:30', 'New Born', 170, 8);
insert into Tracks (timestamp, title, length_seconds, artist_id) values ('2017-03-03 15:22:30', 'Feeling Good', 192, 8);

----------------------
Likes:
insert into Likes values (1, 1, 1);
insert into Likes (artist_id, track_id) values (1, 19);
insert into Likes (artist_id, track_id) values (1, 10);
insert into Likes (artist_id, track_id) values (1, 14);
insert into Likes (artist_id, track_id) values (1, 16);

insert into Likes (artist_id, track_id) values (2, 6);
insert into Likes (artist_id, track_id) values (2, 7);
insert into Likes (artist_id, track_id) values (2, 8);
insert into Likes (artist_id, track_id) values (2, 14);

insert into Likes (artist_id, track_id) values (6, 13);
insert into Likes (artist_id, track_id) values (6, 14);
insert into Likes (artist_id, track_id) values (6, 15);

insert into Likes (artist_id, track_id) values (7, 4);
insert into Likes (artist_id, track_id) values (7, 5);
insert into Likes (artist_id, track_id) values (7, 6);

insert into Likes (artist_id, track_id) values (8, 11);
insert into Likes (artist_id, track_id) values (8, 13);
insert into Likes (artist_id, track_id) values (8, 14);
insert into Likes (artist_id, track_id) values (8, 15);

----------------------
Tags:
insert into Tags values (1, "rock");
insert into Tags values (2, "indie");
insert into Tags values (3, "pop");
insert into Tags (name) values ("transe");
insert into Tags (name) values ("instrumental");
insert into Tags (name) values ("soundtrack");

----------------------
TagsToTracks:
insert into TagsToTracks (id_tag, id_track) values (2, 1);
insert into TagsToTracks (id_tag, id_track) values (1, 1);
insert into TagsToTracks (id_tag, id_track) values (3, 2);

insert into TagsToTracks (id_tag, id_track) values (1, 4);
insert into TagsToTracks (id_tag, id_track) values (1, 5);
insert into TagsToTracks (id_tag, id_track) values (1, 6);
insert into TagsToTracks (id_tag, id_track) values (1, 10);
insert into TagsToTracks (id_tag, id_track) values (1, 17);
insert into TagsToTracks (id_tag, id_track) values (1, 18);
insert into TagsToTracks (id_tag, id_track) values (1, 19);
insert into TagsToTracks (id_tag, id_track) values (1, 20);

insert into TagsToTracks (id_tag, id_track) values (2, 3);
insert into TagsToTracks (id_tag, id_track) values (2, 7);
insert into TagsToTracks (id_tag, id_track) values (2, 8);

insert into TagsToTracks (id_tag, id_track) values (3, 9);
insert into TagsToTracks (id_tag, id_track) values (3, 11);

insert into TagsToTracks (id_tag, id_track) values (4, 9);
insert into TagsToTracks (id_tag, id_track) values (4, 12);

insert into TagsToTracks (id_tag, id_track) values (5, 13);
insert into TagsToTracks (id_tag, id_track) values (5, 14);
insert into TagsToTracks (id_tag, id_track) values (5, 15);
insert into TagsToTracks (id_tag, id_track) values (5, 16);

insert into TagsToTracks (id_tag, id_track) values (6, 13);
insert into TagsToTracks (id_tag, id_track) values (6, 14);
insert into TagsToTracks (id_tag, id_track) values (6, 15);
insert into TagsToTracks (id_tag, id_track) values (6, 16);

----------------------
Comments:
insert into Comments values (1, '2022-03-30 12:00:30', 'I am not worried, I am going to report this track', 1, 1);
insert into Comments (timestamp, contents, artist_id, track_id) values ('2022-04-02 12:31:31', 'Good track!', 1, 3);
insert into Comments (timestamp, contents, artist_id, track_id) values ('2021-03-01 12:31:31', 'Another good track from you', 1, 5);
insert into Comments (timestamp, contents, artist_id, track_id) values ('2021-05-06 11:10:20', 'I really like the strings', 7, 5);

----------------------
Playlists:
insert into Playlists values (1, "Good tracks", 1);
insert into Playlists values (2, "Example album containing two tracks", 2);
insert into Playlists (title, artist_id) values ("Dishonored Soundtrack", 7);
insert into Playlists values (4, "Revelations and Black Holes", 8);

----------------------
TracksToPlaylists:
insert into TracksToPlaylists values (1, 1, 1);
insert into TracksToPlaylists values (2, 2, 1);
insert into TracksToPlaylists (track_id, playlist_id) values (3, 1);
insert into TracksToPlaylists (track_id, playlist_id) values (1, 2);
insert into TracksToPlaylists (track_id, playlist_id) values (3, 2);

insert into TracksToPlaylists (track_id, playlist_id) values (14, 3);
insert into TracksToPlaylists (track_id, playlist_id) values (15, 3);
insert into TracksToPlaylists (track_id, playlist_id) values (16, 3);

insert into TracksToPlaylists (track_id, playlist_id) values (17, 4);
insert into TracksToPlaylists (track_id, playlist_id) values (19, 4);
insert into TracksToPlaylists (track_id, playlist_id) values (20, 4);

----------------------
TagsToPlaylists:
insert into TagsToPlaylists (id_tag, id_playlist) values (1, 1);
insert into TagsToPlaylists values (2, 2, 2);
insert into TagsToPlaylists (id_tag, id_playlist) values (1, 4);

----------------------
Albums:
insert into Albums values (2, '2022-04-01');
insert into Albums values (3, '2022-04-02');
insert into Albums values (4, '2017-03-04');

----------------------
Actions:
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-01 02:00:00', 3, 'Users', 4, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-01 02:01:00', 3, 'Users', 5, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-02-10 12:01:00', 1, 'Users', 1, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-02-10 12:01:00', 1, 'Artists', 1, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-15 13:01:00', 2, 'Users', 2, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-15 13:01:00', 2, 'Artists', 2, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-16 13:01:00', 6, 'Users', 6, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-16 13:01:00', 6, 'Artists', 6, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-16 11:00:30', 7, 'Users', 7, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-16 11:00:30', 7, 'Artists', 7, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-17 11:00:30', 8, 'Users', 8, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-01-17 11:00:30', 8, 'Artists', 8, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2020-03-01 09:00:00', 1, 'Tracks', 5, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-06-06 12:00:00', 1, 'Tracks', 4, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-02-28 12:30:01', 1, 'Tracks', 2, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-01 09:30:00', 1, 'Tracks', 6, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2018-06-06 00:31:00', 2, 'Tracks', 7, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2019-09-10 00:31:00', 2, 'Tracks', 8, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-30 12:00:01', 2, 'Tracks', 1, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-04-01 13:20:20', 2, 'Tracks', 3, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2011-01-03 19:22:30', 6, 'Tracks', 11, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-02-03 15:22:30', 6, 'Tracks', 12, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2020-03-01 09:30:00', 6, 'Tracks', 9, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-01-03 19:30:30', 6, 'Tracks', 10, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2014-02-03 19:22:30', 7, 'Tracks', 15, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-03-03 15:22:30', 7, 'Tracks', 16, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-05-05 10:30:00', 7, 'Tracks', 13, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-03 20:31:30', 7, 'Tracks', 14, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-02-05 09:30:00', 8, 'Tracks', 17, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-03-03 20:31:30', 8, 'Tracks', 18, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2016-02-03 19:22:30', 8, 'Tracks', 19, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2017-03-03 15:22:30', 8, 'Tracks', 20, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-02-05 09:30:00', 8, 'Tags', 1, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2011-01-03 19:22:30', 6, 'Tags', 3, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2014-02-03 19:22:30', 7, 'Tags', 5, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2014-02-03 19:22:30', 7, 'Tags', 6, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-02-03 15:22:30', 6, 'Tags', 4, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2018-06-06 00:31:00', 2, 'Tags', 2, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-30 12:00:01', 2, 'TagsToTracks', 1, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-30 12:00:01', 2, 'TagsToTracks', 2, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-02-28 12:30:01', 1, 'TagsToTracks', 3, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-04-01 13:20:20', 2, 'TagsToTracks', 15, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-06-06 12:00:00', 1, 'TagsToTracks', 7, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2020-03-01 09:00:00', 1, 'TagsToTracks', 8, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-01 09:30:00', 1, 'TagsToTracks', 9, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2018-06-06 00:31:00', 2, 'TagsToTracks', 16, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2019-09-10 00:31:00', 2, 'TagsToTracks', 17, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2020-03-01 09:30:00', 6, 'TagsToTracks', 18, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2020-03-01 09:30:00', 6, 'TagsToTracks', 20, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-01-03 19:30:30', 6, 'TagsToTracks', 10, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2011-01-03 19:22:30', 6, 'TagsToTracks', 19, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-02-03 15:22:30', 6, 'TagsToTracks', 21, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-02-03 15:22:30', 7, 'TagsToTracks', 23, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-05-05 10:30:00', 7, 'TagsToTracks', 27, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-03 20:31:30', 7, 'TagsToTracks', 24, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-03 20:31:30', 7, 'TagsToTracks', 28, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2014-02-03 19:22:30', 7, 'TagsToTracks', 25, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2014-02-03 19:22:30', 7, 'TagsToTracks', 29, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-03-03 15:22:30', 7, 'TagsToTracks', 26, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-03-03 15:22:30', 7, 'TagsToTracks', 30, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2010-02-05 09:30:00', 8, 'TagsToTracks', 11, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-03-03 20:31:30', 8, 'TagsToTracks', 12, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2016-02-03 19:22:30', 8, 'TagsToTracks', 13, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2017-03-03 15:22:30', 8, 'TagsToTracks', 14, 'create');

insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-04-30 12:00:01', 1, 'Likes', 1, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2016-04-03 19:22:30', 1, 'Likes', 2, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2021-02-03 19:30:30', 1, 'Likes', 3, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-07-03 20:31:30', 1, 'Likes', 4, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2015-04-03 15:22:30', 1, 'Likes', 5, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-03-01 09:30:00', 2, 'Likes', 6, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2018-07-06 00:31:00', 2, 'Likes', 7, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2019-10-10 00:31:00', 2, 'Likes', 8, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-07-03 20:31:30', 2, 'Likes', 9, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-04 20:31:30', 6, 'Likes', 10, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-05 20:31:30', 6, 'Likes', 11, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-06 20:31:30', 6, 'Likes', 12, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-07 20:31:30', 7, 'Likes', 13, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-08 20:31:30', 7, 'Likes', 14, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-09 20:31:30', 7, 'Likes', 15, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-10 20:31:30', 8, 'Likes', 16, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-11 20:31:30', 8, 'Likes', 17, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-12 20:31:30', 8, 'Likes', 18, 'create');
insert into Actions (timestamp, user_id, object_type, object_id, action_type) values ('2022-06-13 20:31:30', 8, 'Likes', 19, 'create');
