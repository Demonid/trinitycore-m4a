-- HoR creature texts
DELETE FROM `creature_text` WHERE `entry` IN (36954,37226,36955,37554);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
-- Lich King intro event
(36954, 0, 0, 'SILENCE, PALADIN!', 14, 0, 100, 0, 0, 17225, 'Lich King SAY_LK_INTRO_1'),
(36954, 1, 0, 'So you wish to commune with the dead? You shall have your wish.', 14, 0, 100, 0, 0, 17226, 'Lich King SAY_LK_INTRO_2'),
(36954, 2, 0, 'Falric. Marwyn. Bring their corpses to my chamber when you are through.', 14, 0, 100, 0, 0, 17227, 'Lich King SAY_LK_INTRO_3'),

-- Lich King Boss
(37226, 0, 0, 'Your allies have arrived, Jaina, just as you promised. You will all become powerful agents of the Scourge.', 14, 0, 100, 0, 0, 17212, 'HOR LK - ALLIANCE - SAY_AGGRO'),
(37226, 1, 0, 'I will not make the same mistake again, Sylvanas. This time there will be no escape. You will all serve me in death!', 14, 0, 100, 0, 0, 17213, 'HOR LK - HORDE - SAY_AGGRO'),
(37226, 2, 0, 'There is no escape!', 14, 0, 100, 0, 0, 17217, 'HOR LK - SAY_EVENT_START'),
(37226, 3, 0, 'Death\'s cold embrace awaits.', 14, 0, 100, 0, 0, 17221, 'HOR LK - SAY_ICEWALL_1'),
(37226, 4, 0, 'Succumb to the chill of the grave.', 14, 0, 100, 0, 0, 17218, 'HOR LK - SAY_ICEWALL_2'),
(37226, 5, 0, 'Another dead end.', 14, 0, 100, 0, 0, 17219, 'HOR LK - SAY_ICEWALL_3'),
(37226, 6, 0, 'How long can you fight it?', 14, 0, 100, 0, 0, 17220, 'HOR LK - SAY_ICEWALL_4'),
(37226, 7, 0, 'Nowhere to run... You\'re mine now!', 14, 0, 100, 0, 0, 17223, 'HOR LK - SAY_EVENT_DONE'),

-- Jaina
(36955, 0, 0, 'He is too powerful, we must leave this place at once! My magic will hold him in place for only a short time! Come quickly, heroes!', 14, 0, 100, 0, 0, 16606, 'HOR Jaina - SAY_EVENT_START'),
(36955, 1, 0, 'I will destroy this barrier. You must hold the undead back!', 14, 0, 100, 0, 0, 16607, 'HOR Jaina - SAY_ICE_WALL_01'),
(36955, 2, 0, 'Another ice wall! Keep the undead from interrupting my incantation so that I may bring this wall down!', 14, 0, 100, 0, 0, 16608, 'HOR Jaina - SAY_ICE_WALL_02'),
(36955, 3, 0, 'We\'re almost there... Don\'t give up!', 14, 0, 100, 0, 0, 16609, 'HOR Jaina - SAY_ICE_WALL_03'),
(36955, 4, 0, 'Your barriers can\'t hold us back much longer, monster. I will shatter them all!', 14, 0, 100, 0, 0, 16610, 'HOR Jaina - SAY_ICE_WALL_04'),
(36955, 5, 0, 'There\'s an opening up ahead. GO NOW!', 14, 0, 100, 0, 0, 16645, 'HOR Jaina - SAY_END_01'),
(36955, 6, 0, 'It... It\'s a dead end. We have no choice but to fight. Steel yourself heroes, for this is our last stand!', 14, 0, 100, 0, 0, 16647, 'HOR Jaina - SAY_END_02'),
(36955, 7, 0, 'Forgive me, heroes. I should have listened to Uther. I... I just had to see for myself. To look into his eyes one', 14, 0, 100, 0, 0, 16648, 'HOR Jaina - SAY_END_03'),
(36955, 8, 0, 'We now know what must be done. I will deliver this news to King Varian and Highlord Fordring.', 14, 0, 100, 0, 0, 16649, 'HOR Jaina - SAY_END_04'),

-- Sylvanas Windrunner
(37554, 0, 0, 'He\'s too powerful! Heroes, quickly, come to me! We must leave this place immediately! I will do what I can do hold him in place while we flee.', 14, 0, 100, 0, 0, 17028, 'HOR Sylvanas - SAY_EVENT_START'),
(37554, 1, 0, 'No wall can hold the Banshee Queen! Keep the undead at bay, heroes! I will tear this barrier down!', 14, 0, 100, 0, 0, 17029, 'HOR Sylvanas - SAY_ICE_WALL_01'),
(37554, 2, 0, 'Another barrier? Stand strong, champions! I will bring the wall down!', 14, 0, 100, 0, 0, 17030, 'HOR Sylvanas - SAY_ICE_WALL_02'),
(37554, 3, 0, 'I grow tired of these games, Arthas! Your walls can\'t stop me!', 14, 0, 100, 0, 0, 17031, 'HOR Sylvanas - SAY_ICE_WALL_03'),
(37554, 4, 0, 'You won\'t impede our escape, fiend. Keep the undead off me while I bring this barrier down!', 14, 0, 100, 0, 0, 17032, 'HOR Sylvanas - SAY_ICE_WALL_04'),
(37554, 5, 0, 'We\'re almost there! Don\'t give up!', 14, 0, 100, 0, 0, 17060, 'HOR Sylvanas - SAY_END_01'),
(37554, 6, 0, 'BLASTED DEAD END! So this is how it ends. Prepare yourselves, heroes, for today we make our final stand!', 14, 0, 100, 0, 0, 17061, 'HOR Sylvanas - SAY_END_02'),
(37554, 7, 0, 'We are safe, for now. His strength has increased ten-fold since our last battle! It will take a mighty army to destroy the Lich King, an army greater than even the Horde can rouse.', 14, 0, 100, 0, 0, 17062, 'HOR Sylvanas - SAY_END_03');