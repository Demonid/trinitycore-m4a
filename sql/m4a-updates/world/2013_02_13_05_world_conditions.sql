-- Drop Cache of the Ley-Guardian if looter has aura Luck of the Draw
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=4 AND `SourceGroup`=24524 AND `SourceEntry`=52676;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(4,24524,52676,0,0,1,0,72221,0,0,0,0,0,'','on loot Cache of the Ley-Guardian - Luck of the Draw');