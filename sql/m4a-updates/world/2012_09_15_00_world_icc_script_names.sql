UPDATE creature_template
SET ScriptName = "npc_servant_of_the_throne"
WHERE entry = 36724;

UPDATE creature_template
SET ScriptName = "npc_ancient_skeletal_soldier"
WHERE entry = 37012;

UPDATE creature_template
SET ScriptName = "npc_deathbound_ward"
WHERE entry = 37007;

UPDATE creature_template
SET ScriptName = "npc_nerubar_broodkeeper"
WHERE entry = 36725;

DELETE FROM spell_script_names WHERE spell_id = 70964;
INSERT INTO spell_script_names
VALUES (70964,"spell_icc_shield_bash");

DELETE FROM spell_script_names WHERE spell_id = 70980;
INSERT INTO spell_script_names
VALUES (70980,"spell_icc_web_wrap");