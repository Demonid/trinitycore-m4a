DELETE FROM `command` WHERE `name`='reviveifdead' LIMIT 1;
INSERT INTO `command` (`name`, `security`, `help`) VALUES ('reviveifdead', 1, 'Revives character who meets criteria');

DELETE FROM `trinity_string` WHERE `entry` IN (11002, 11003, 11004);
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES 
(11002, 'Character needs to be online in order to revive it', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(11003, 'Revive can not be done while in battleground or arena', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL),
(11004, 'Characters spirit has to be released in order to revive it', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);