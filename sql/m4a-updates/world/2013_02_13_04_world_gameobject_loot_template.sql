-- Oculus loot - Cache of the Ley-Guardian
DELETE FROM `gameobject_loot_template` WHERE `entry`=24524 AND `item`=52676;
INSERT INTO `gameobject_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(24524,52676,100,1,0,1,1);