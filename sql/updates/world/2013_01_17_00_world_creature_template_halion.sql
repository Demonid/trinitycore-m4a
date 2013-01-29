/* Setting right trigger spawns */
UPDATE world.creature SET spawnMask = 15 WHERE id = 40081;
UPDATE world.creature_template SET difficulty_entry_1 = 40470, difficulty_entry_2 = 40471, difficulty_entry_3 = 40472 WHERE entry = 40081;

/* Set correct model IDs */
UPDATE world.creature_template SET modelid1=169,modelid2=11686, flags_extra= flags_extra | 128 WHERE entry IN (40081, 40470, 40471, 40472);
UPDATE world.creature_template SET modelid1=169, modelid2=11686, flags_extra = flags_extra | 128 WHERE entry IN (40091, 43280, 43281, 43282);

/* Set correct Immune Masks */
UPDATE world.creature_template SET mechanic_immune_mask = mechanic_immune_mask | 650854271 WHERE entry IN (39863, 40142, 40681, 40683);