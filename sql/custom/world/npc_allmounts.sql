-- --------------------------------------------------------------------------------------
--	ALL MOUNTS VENDOR - 601014
-- --------------------------------------------------------------------------------------
SET
@Entry 		:= 601014,
@Model 		:= 26571, -- The Black Knight
-- @Model 		:= 21249, -- Armored Orc
@Name 		:= "The Mountain",
@Title 		:= "Mount Trainer",
@Icon 		:= "Speak",
@GossipMenu := 600001,
@MinLevel 	:= 255,
@MaxLevel 	:= 255,
@Faction 	:= 35,
@NPCFlag 	:= 1,
@Scale		:= 1.0,
@Rank		:= 0,
@Type 		:= 7,
@TypeFlags 	:= 0,
@FlagsExtra := 2,
@AIName		:= "SmartAI",
@Script 	:= "All_Mounts_NPC",
@CreatureTxt     := "Hail $N. I can teach you to ride.. anything!",
@NPCTxt     := 601014,
@Comment    := "Used for mount trainer npc";

-- NPC
DELETE FROM creature_template WHERE entry = @Entry;
INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `unit_class`, `unit_flags`, `type`, `type_flags`, `RegenHealth`, `type_flags2`, `AIName`, `ScriptName`) VALUES
(@Entry, @Name, @Title, @Icon, @GossipMenu, @MinLevel, @MaxLevel, @Faction, @NPCFlag, 1, 1.14286, @Scale, @Rank, 1, 2, @Type, @TypeFlags, 1, @FlagsExtra, @AIName, @Script);


DELETE FROM `creature_text` WHERE `CreatureID`=@Entry;
INSERT INTO `creature_text` (`CreatureID`, `Text`, 'Comment') VALUES (@Entry, @CreatureTxt, @comment);

DELETE FROM `creature_template_model` WHERE `CreatureID`=@Entry;
INSERT INTO `creature_template_model` (`CreatureID`, `CreatureDisplayID`) VALUES (@Entry, @Model);

DELETE FROM `gossip_menu` WHERE `MenuID`=@GossipMenu;
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES (@GossipMenu, @NPCTxt);

DELETE FROM `npc_text` WHERE `ID`=@NPCTxt;
INSERT INTO `npc_text` (`ID`) VALUES (@NPCTxt);