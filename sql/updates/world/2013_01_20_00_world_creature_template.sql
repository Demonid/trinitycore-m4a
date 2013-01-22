-- Halion damage multipliers
UPDATE world.creature_template SET dmg_multiplier = 70 WHERE entry IN (39863, 40142);  -- 10 nm
UPDATE world.creature_template SET dmg_multiplier = 100 WHERE entry IN (39864, 40143); -- 25 nm
UPDATE world.creature_template SET dmg_multiplier = 100 WHERE entry IN (39944, 40144); -- 10 hc
UPDATE world.creature_template SET dmg_multiplier = 170 WHERE entry IN (39945, 40145); -- 25 hc
