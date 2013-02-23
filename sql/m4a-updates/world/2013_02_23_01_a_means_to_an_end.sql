-- A Means to an End
-- Event script of spell casted on quest item use
DELETE FROM `event_scripts` WHERE `id` = 17868;
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES 
(17868, 0, 10, 27238, 90000, 0, 2784.13, -205.31, 140.57, 6.07248);

-- Trigger NPC, forces correct moddle IDs
UPDATE `creature_template` SET `flags_extra` = `flags_extra` | 128 WHERE `entry` = 27353;

-- EventAI, setting correct faction
DELETE FROM `creature_ai_scripts` WHERE `id` = 2723803;
INSERT INTO `creature_ai_scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES 
(2723803, 27238, 11, 0, 100, 1, 0, 0, 0, 0, 2, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'Foreman Kaleiki - On spawn - Set facrion');

