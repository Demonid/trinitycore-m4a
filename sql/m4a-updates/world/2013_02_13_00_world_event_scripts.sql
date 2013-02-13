-- Event script for Terrok's summoning
DELETE FROM `event_scripts` WHERE id = 15014;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
(15014, 0, 10, 21838, 90000, 0, -3789.4, 3507.63, 286.982, -0.994838);