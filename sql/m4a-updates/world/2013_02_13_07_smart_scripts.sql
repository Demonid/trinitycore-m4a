-- Suitable Disguise quest fix
DELETE FROM world.smart_scripts WHERE entryorguid IN (36669,36670) AND id = 5;
INSERT INTO world.smart_scripts (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(36669, 0, 5, 0, 19, 0, 100, 0, 20438, 0, 0, 0, 50, 201384, 300000, 0, 0, 0, 0, 8, 0, 0, 0, 5796.428, 696.332, 657.948, 0.1256, 'Clean Laundry - On Quest Accept - Summon Gameobject Clean Laundry'), -- Alliance quest
(36670, 0, 5, 0, 19, 0, 100, 0, 24556, 0, 0, 0, 50, 201384, 300000, 0, 0, 0, 0, 8, 0, 0, 0, 5796.428, 696.332, 657.948, 0.1256, 'Clean Laundry - On Quest Accept - Summon Gameobject Clean Laundry'); -- Horde quest
