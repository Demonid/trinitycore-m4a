/* Solace trinket proc */
DELETE FROM spell_proc_event WHERE entry in (67752,67698);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES (67752, 0, 0, 0, 0, 0, 0, 65536, 0, 0, 0);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES (67698, 0, 0, 0, 0, 0, 0, 65536, 0, 0, 0);
