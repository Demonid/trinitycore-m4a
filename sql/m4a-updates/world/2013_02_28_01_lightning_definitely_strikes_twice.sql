-- Quest:Lightning Definitely Strikes Twice
DELETE FROM `event_scripts` WHERE `id`=18475 AND `command`=7;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
(18475, 0, 7, 12537, 0, 0, 0, 0, 0, 0);