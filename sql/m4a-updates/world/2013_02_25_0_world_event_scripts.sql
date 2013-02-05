-- Mistcaller Yvngar quest fix
DELETE FROM `event_scripts` WHERE `id`=21997 AND `command`=10;
INSERT INTO `event_scripts` (`id`,`delay`,`command`,`datalong`,`datalong2`,`dataint`,`x`,`y`,`z`,`o`) VALUES
(21997,0,10,34965,90000,0,10181.96,1183.417,76.12115,5.934119);