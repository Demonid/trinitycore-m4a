UPDATE `item_template` SET `spellcooldown_1`=-1 WHERE `entry`=50415; /* prev val = 1000 */
UPDATE `item_template` SET `spellppmRate_1`=2 WHERE `entry` IN (50415,50709); /* prev val = 0 */