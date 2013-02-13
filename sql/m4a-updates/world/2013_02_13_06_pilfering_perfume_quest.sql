-- Pilfering Perfume (A:24656 H:24541)

SET @QUEST_A           := 24656;
SET @QUEST_H           := 24541;

SET @SNIP              := 38066; -- Inspector Snip Snagglebolt
SET @SNAP              := 37172; -- Detective Snap Snagglebolt
SET @MENUID            := 10976;

SET @TRIGGER_SW_OUT    := 5703; -- outside
SET @TRIGGER_SW_IN     := 5704; -- inside
SET @TRIGGER_ORG_OUT   := 5705; -- outside
SET @TRIGGER_ORG_IN    := 5706; -- inside

SET @SPELL_UNIFORM     := 71450; -- Crown Parcel Service Uniform
SET @SPELL_CONTRABAND  := 71459; -- Crown Chemical Co. Contraband

SET @SPELL_CREDIT_A    := 71522; -- Crown Chemical Co. Supplies
SET @SPELL_CREDIT_H    := 71539; -- Crown Chemical Co. Supplies

-- Snip & Snap sai
UPDATE `creature_template` SET `npcflag`=`npcflag`|1|2,`gossip_menu_id`=@MENUID,`AIName`='SmartAI' WHERE `entry` IN (@SNIP,@SNAP);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@SNIP,@SNAP) AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@SNIP,0,0,0,19,0,100,0,@QUEST_A,0,0,0,11,@SPELL_UNIFORM,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snip - On quest accept - Spellcast'),
(@SNIP,0,1,2,62,0,100,0,@MENUID,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snip - On gossip option select - Close gossip'),
(@SNIP,0,2,0,61,0,100,0,0,0,0,0,11,@SPELL_UNIFORM,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snip - On gossip option accept - Spellcast'),
--
(@SNAP,0,0,0,19,0,100,0,@QUEST_H,0,0,0,11,@SPELL_UNIFORM,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snap - On quest accept - Spellcast'),
(@SNAP,0,1,2,62,0,100,0,@MENUID,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snap - On gossip option select - Close gossip'),
(@SNAP,0,2,0,61,0,100,0,0,0,0,0,11,@SPELL_UNIFORM,0,0,0,0,0,7,0,0,0,0,0,0,0,'Snap - On gossip option accept - Spellcast');


DELETE FROM `areatrigger_involvedrelation` WHERE `id` IN (@TRIGGER_SW_OUT,@TRIGGER_SW_IN,@TRIGGER_ORG_OUT,@TRIGGER_ORG_IN);
INSERT INTO `areatrigger_involvedrelation` (`id`,`quest`) VALUES
(@TRIGGER_SW_OUT,@QUEST_A),
(@TRIGGER_SW_IN,@QUEST_A),
--
(@TRIGGER_ORG_OUT,@QUEST_H),
(@TRIGGER_ORG_IN,@QUEST_H);


DELETE FROM `areatrigger_scripts` WHERE `entry` IN (@TRIGGER_SW_OUT,@TRIGGER_SW_IN,@TRIGGER_ORG_OUT,@TRIGGER_ORG_IN);
INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES
(@TRIGGER_SW_OUT,'SmartTrigger'),
(@TRIGGER_SW_IN,'SmartTrigger'),
--
(@TRIGGER_ORG_OUT,'SmartTrigger'),
(@TRIGGER_ORG_IN,'SmartTrigger');


-- usig invoker cast because normal cast doesn't want to work(LoS issue,i think)
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@TRIGGER_SW_OUT,@TRIGGER_SW_IN,@TRIGGER_ORG_OUT,@TRIGGER_ORG_IN) AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@TRIGGER_SW_OUT,2,0,0,46,0,100,0,@TRIGGER_SW_OUT,0,0,0,75,@SPELL_CONTRABAND,0,0,0,0,0,7,0,0,0,0,0,0,0,"On Trigger - Cast - Invoker"),
(@TRIGGER_SW_IN,2,0,0,46,0,100,0,@TRIGGER_SW_IN,0,0,0,85,@SPELL_CREDIT_A,0,0,0,0,0,7,0,0,0,0,0,0,0,"On Trigger - Cast - Invoker"),
--
(@TRIGGER_ORG_OUT,2,0,0,46,0,100,0,@TRIGGER_ORG_OUT,0,0,0,75,@SPELL_CONTRABAND,0,0,0,0,0,7,0,0,0,0,0,0,0,"On Trigger - Cast - Invoker"),
(@TRIGGER_ORG_IN,2,0,0,46,0,100,0,@TRIGGER_ORG_IN,0,0,0,85,@SPELL_CREDIT_H,0,0,0,0,0,7,0,0,0,0,0,0,0,"On Trigger - Cast - Invoker");


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry` IN (@TRIGGER_SW_OUT,@TRIGGER_SW_IN,@TRIGGER_ORG_OUT,@TRIGGER_ORG_IN);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=15 AND `SourceGroup`=@MENUID;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(22,1,@TRIGGER_SW_OUT,2,0,1,0,@SPELL_UNIFORM,0,0,0,0,'',"execute sai only if player has aura"),
(22,1,@TRIGGER_SW_IN,2,0,1,0,@SPELL_CONTRABAND,0,0,0,0,'',"execute sai only if player has aura"),
(15,@MENUID,0,0,0,9,0,@QUEST_A,0,0,0,0,'',"show gossip option only if player has quest taken"),
--
(22,1,@TRIGGER_ORG_OUT,2,0,1,0,@SPELL_UNIFORM,0,0,0,0,'',"execute sai only if player has aura"),
(22,1,@TRIGGER_ORG_IN,2,0,1,0,@SPELL_CONTRABAND,0,0,0,0,'',"execute sai only if player has aura"),
(15,@MENUID,0,0,1,9,0,@QUEST_H,0,0,0,0,'',"show gossip option only if player has quest taken");


-- spell 71522/71539(Effect 1: Id 77 (SPELL_EFFECT_SCRIPT_EFFECT)) should remove spells 71450 & 71459 on hit
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (@SPELL_CREDIT_A,@SPELL_CREDIT_H);
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
(@SPELL_CREDIT_A,-@SPELL_UNIFORM,1,'Remove Crown Parcel Service Uniform on Crown Chemical Co. Supplies hit'),
(@SPELL_CREDIT_A,-@SPELL_CONTRABAND,1,'Remove Crown Chemical Co. Contraband on Crown Chemical Co. Supplies hit'),
--
(@SPELL_CREDIT_H,-@SPELL_UNIFORM,1,'Remove Crown Parcel Service Uniform on Crown Chemical Co. Supplies hit'),
(@SPELL_CREDIT_H,-@SPELL_CONTRABAND,1,'Remove Crown Chemical Co. Contraband on Crown Chemical Co. Supplies hit');