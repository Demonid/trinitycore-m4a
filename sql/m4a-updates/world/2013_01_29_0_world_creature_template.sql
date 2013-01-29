/* Zul Gurub - Vilebranch - Speaker AI */
UPDATE world.creature_template SET ScriptName = "npc_vilebranch_speaker", AIName = "" WHERE entry = 11391;

/* Removing unused SAI */
DELETE FROM world.smart_scripts WHERE entryorguid = 11391;