-- Web Wrap (cast 71010 after the 70980 aura is removed)
DELETE FROM world.spell_linked_spell WHERE spell_trigger = -70980 AND spell_effect = 71010;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(-70980, 71010, 0, 'Web Wrap');