/* Zul Gurub - Vilebranch - Speaker AI */
UPDATE creature_template 
SET ScriptName = "npc_vilebranch_speaker", AIName = ""
WHERE entry = 11391;

/* Removing unused SAI */
DELETE FROM smart_scripts
WHERE entryorguid = 11391;