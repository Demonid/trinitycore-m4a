-- Servant of the Throne 
UPDATE world.creature_template SET ScriptName='' WHERE entry=36724 LIMIT 1;
DELETE FROM world.smart_scripts WHERE entryorguid=36724 AND source_type=0 AND id=3; -- Removing Web Wrap cast
UPDATE world.smart_scripts SET id=1, event_flags=30, event_param1 = 3000, event_param2 = 3000, event_param3 = 4000, event_param4 = 4000 WHERE entryorguid=36724 AND source_type=0 AND id=1; -- Glacial Blast

-- Ancient Skeletal Soldier
UPDATE world.creature_template SET ScriptName='' WHERE entry=37012 LIMIT 1;
DELETE FROM world.smart_scripts WHERE entryorguid=37012 AND source_type=0 AND id=3; -- Removing Web Wrap cast
UPDATE world.smart_scripts SET id=1, event_flags=30 WHERE entryorguid=37012 AND source_type=0 AND id=0; -- Shield Bash

-- Nerub'ar Broodkeeper
UPDATE world.creature_template SET ScriptName='' WHERE entry=36725 LIMIT 1;
UPDATE world.smart_scripts SET event_param1=0, event_param2=50 WHERE entryorguid=36725 AND source_type=0 AND id=1; -- Enter combat range
UPDATE world.smart_scripts SET event_param1=9000, event_param2=11000, event_param3=9000, event_param4=11000 WHERE entryorguid=36725 AND source_type=0 AND id=3; -- Web Wrap timers

-- Deathbound Ward
UPDATE world.creature_template SET ScriptName='' WHERE entry=37007 LIMIT 1;
UPDATE world.smart_scripts SET action_param2=0 WHERE entryorguid=37007 AND source_type=0 AND id=1; -- Disrupting Shout shoult not be triggerd

-- Web Wrap
UPDATE world.creature_template SET AIName='SmartAI' WHERE entry=38028 LIMIT 1;
DELETE FROM world.smart_scripts WHERE entryorguid=38028;
INSERT INTO world.smart_scripts (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(38028, 0, 0, 0, 63, 0, 100, 30, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Web Wrap - Just Created - Set react state passive'),
(38028, 0, 0, 1, 6, 0, 100, 30, 0, 0, 0, 0, 28, 71010, 0, 0, 0, 0, 0, 21, 5, 0, 0, 0, 0, 0, 0, 'Web Wrap - on death - remove aura');