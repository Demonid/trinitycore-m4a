-- Ragnaros loot - remove rare/uncommon item drops
UPDATE creature_loot_template SET maxcount = 0 WHERE entry = 11502 AND mincountOrRef = -34002; -- prev values 3;