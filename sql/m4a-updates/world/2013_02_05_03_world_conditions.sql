-- Heavily Perfumed explicit target condition
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (71520);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(13,7,71520,0,0,31,0,3,1976,0,0,0,'','Heavily Perfumed can be casted on Stormwind City Patroler'),
(13,7,71520,0,1,31,0,3,68,0,0,0,'','Heavily Perfumed can be casted on Stormwind City Guard'),
(13,7,71520,0,2,31,0,3,3296,0,0,0,'','Heavily Perfumed can be casted on Orgrimmar Grunt');