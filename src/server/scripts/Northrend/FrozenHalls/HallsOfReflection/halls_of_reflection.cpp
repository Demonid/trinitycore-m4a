/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdafx.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "halls_of_reflection.h"
#include "Player.h"
#include "ScriptedEscortAI.h"
#include "Transport.h"
#include "MapManager.h"

enum Yells
{
    SAY_JAINA_INTRO_1                   = 0,
    SAY_JAINA_INTRO_2                   = 1,
    SAY_JAINA_INTRO_3                   = 2,
    SAY_JAINA_INTRO_4                   = 3,
    SAY_JAINA_INTRO_5                   = 4,
    SAY_JAINA_INTRO_6                   = 5,
    SAY_JAINA_INTRO_7                   = 6,
    SAY_JAINA_INTRO_8                   = 7,
    SAY_JAINA_INTRO_9                   = 8,
    SAY_JAINA_INTRO_10                  = 9,
    SAY_JAINA_INTRO_11                  = 10,
    SAY_JAINA_INTRO_END                 = 11,

    SAY_SYLVANAS_INTRO_1                = 0,
    SAY_SYLVANAS_INTRO_2                = 1,
    SAY_SYLVANAS_INTRO_3                = 2,
    SAY_SYLVANAS_INTRO_4                = 3,
    SAY_SYLVANAS_INTRO_5                = 4,
    SAY_SYLVANAS_INTRO_6                = 5,
    SAY_SYLVANAS_INTRO_7                = 6,
    SAY_SYLVANAS_INTRO_8                = 7,
    SAY_SYLVANAS_INTRO_END              = 8,

    SAY_UTHER_INTRO_A2_1                = 0,
    SAY_UTHER_INTRO_A2_2                = 1,
    SAY_UTHER_INTRO_A2_3                = 2,
    SAY_UTHER_INTRO_A2_4                = 3,
    SAY_UTHER_INTRO_A2_5                = 4,
    SAY_UTHER_INTRO_A2_6                = 5,
    SAY_UTHER_INTRO_A2_7                = 6,
    SAY_UTHER_INTRO_A2_8                = 7,
    SAY_UTHER_INTRO_A2_9                = 8,
    SAY_UTHER_INTRO_H2_1                = 9,
    SAY_UTHER_INTRO_H2_2                = 10,
    SAY_UTHER_INTRO_H2_3                = 11,
    SAY_UTHER_INTRO_H2_4                = 12,
    SAY_UTHER_INTRO_H2_5                = 13,
    SAY_UTHER_INTRO_H2_6                = 14,
    SAY_UTHER_INTRO_H2_7                = 15,

    SAY_LK_INTRO_1                      = 0,
    SAY_LK_INTRO_2                      = 1,
    SAY_LK_INTRO_3                      = 2,

    SAY_FALRIC_INTRO_1                  = 5,
    SAY_FALRIC_INTRO_2                  = 6,

    SAY_MARWYN_INTRO_1                  = 4,

    SAY_LK_ALLIANCE_AGGRO               = 0,
    SAY_LK_HORDE_AGGRO                  = 1,
    SAY_LK_EVENT_START                  = 2,
    SAY_LK_EVENT_ICEWALL_01             = 3,
    SAY_LK_EVENT_ICEWALL_02             = 4,
    SAY_LK_EVENT_ICEWALL_03             = 5,
    SAY_LK_EVENT_ICEWALL_04             = 6,
    SAY_LK_EVENT_END                    = 7,

    SAY_JAINA_ESCAPE_START              = 0,
    SAY_JAINA_ESCAPE_ICEWALL_01         = 1,
    SAY_JAINA_ESCAPE_ICEWALL_02         = 2,
    SAY_JAINA_ESCAPE_ICEWALL_03         = 3,
    SAY_JAINA_ESCAPE_ICEWALL_04         = 4,
    SAY_JAINA_ESCAPE_END_01             = 5,
    SAY_JAINA_ESCAPE_END_02             = 6,
    SAY_JAINA_ESCAPE_END_03             = 7,
    SAY_JAINA_ESCAPE_END_04             = 8,

    SAY_SYLVANAS_ESCAPE_START           = 0,
    SAY_SYLVANAS_ESCAPE_ICEWALL_01      = 1,
    SAY_SYLVANAS_ESCAPE_ICEWALL_02      = 2,
    SAY_SYLVANAS_ESCAPE_ICEWALL_03      = 3,
    SAY_SYLVANAS_ESCAPE_ICEWALL_04      = 4,
    SAY_SYLVANAS_ESCAPE_END_01          = 5,
    SAY_SYLVANAS_ESCAPE_END_02          = 6,
    SAY_SYLVANAS_ESCAPE_END_03          = 7,
};

enum
{
    EVENT_NONE,

    EVENT_START_INTRO,
    EVENT_SKIP_INTRO,

    EVENT_INTRO_A2_1,
    EVENT_INTRO_A2_2,
    EVENT_INTRO_A2_3,
    EVENT_INTRO_A2_4,
    EVENT_INTRO_A2_5,
    EVENT_INTRO_A2_6,
    EVENT_INTRO_A2_7,
    EVENT_INTRO_A2_8,
    EVENT_INTRO_A2_9,
    EVENT_INTRO_A2_10,
    EVENT_INTRO_A2_11,
    EVENT_INTRO_A2_12,
    EVENT_INTRO_A2_13,
    EVENT_INTRO_A2_14,
    EVENT_INTRO_A2_15,
    EVENT_INTRO_A2_16,
    EVENT_INTRO_A2_17,
    EVENT_INTRO_A2_18,
    EVENT_INTRO_A2_19,

    EVENT_INTRO_H2_1,
    EVENT_INTRO_H2_2,
    EVENT_INTRO_H2_3,
    EVENT_INTRO_H2_4,
    EVENT_INTRO_H2_5,
    EVENT_INTRO_H2_6,
    EVENT_INTRO_H2_7,
    EVENT_INTRO_H2_8,
    EVENT_INTRO_H2_9,
    EVENT_INTRO_H2_10,
    EVENT_INTRO_H2_11,
    EVENT_INTRO_H2_12,
    EVENT_INTRO_H2_13,
    EVENT_INTRO_H2_14,
    EVENT_INTRO_H2_15,

    EVENT_INTRO_LK_1,
    EVENT_INTRO_LK_2,
    EVENT_INTRO_LK_3,
    EVENT_INTRO_LK_4,
    EVENT_INTRO_LK_5,
    EVENT_INTRO_LK_6,
    EVENT_INTRO_LK_7,
    EVENT_INTRO_LK_8,
    
    EVENT_INTRO_SKIP_END,
    EVENT_INTRO_END,

    // Ghostly Priest
    EVENT_SHADOW_WORD_PAIN,
    EVENT_CIRCLE_OF_DESTRUCTION,
    EVENT_COWER_IN_FEAR,
    EVENT_DARK_MENDING,

    // Phantom Mage
    EVENT_FIREBALL,
    EVENT_FLAMESTRIKE,
    EVENT_FROSTBOLT,
    EVENT_CHAINS_OF_ICE,
    EVENT_HALLUCINATION,

    // Shadowy Mercenary
    EVENT_SHADOW_STEP,
    EVENT_DEADLY_POISON,
    EVENT_ENVENOMED_DAGGER_THROW,
    EVENT_KIDNEY_SHOT,

    // Spectral Footman
    EVENT_SPECTRAL_STRIKE,
    EVENT_SHIELD_BASH,
    EVENT_TORTURED_ENRAGE,

    // Tortured Rifleman
    EVENT_SHOOT,
    EVENT_CURSED_ARROW,
    EVENT_FROST_TRAP,
    EVENT_ICE_SHOT,

    //Frostsworn events
    EVENT_SHEILD_THROW,
    EVENT_SPIKE,
    EVENT_CLONE,

    //Escepe events
    EVENT_ESCAPE_FREEZ_AND_MOVE,
    EVENT_SUMMON_GHOULS,
    EVENT_SUMMON_DOCTOR,
    EVENT_SUMMON_ABO,
    EVENT_SUMMON_WALL,
    EVENT_RESUME_MOVEMENT,
    EVENT_FINISH_ESCAPE,

    //Gnoul
    EVENT_GNOUL_LEAP,

    //Witch Doctor
    EVENT_DOCTOR_SHADOW_BOLT,
    EVENT_DOCTOR_SHADOW_VALLEY,
    EVENT_DOCTOR_CURSE_OF_DOOM,

    //Lumbering Abomination
    EVENT_ABO_CLEAVE,
    EVENT_ABO_VOMIT,
};

enum
{
    ACTION_START_INTRO,
    ACTION_SKIP_INTRO,
    ACTION_START_CHASE,
    ACTION_WALL_01,
    ACTION_WALL_02,
    ACTION_WALL_03,
    ACTION_WALL_04,
    ACTION_DESTROY_WALL,
    ACTION_OUTRO,

    QUEST_DELIVRANCE_FROM_THE_PIT_A2              = 24710,
    QUEST_DELIVRANCE_FROM_THE_PIT_H2              = 24712,
    QUEST_WRATH_OF_THE_LICH_KING_A2               = 24500,
    QUEST_WRATH_OF_THE_LICH_KING_H2               = 24802,
};

enum Spells
{
    // Intro spells
    SPELL_CAST_VISUAL                             = 65633, //Jaina/Sylavana lo lanzan para invocar a uther
    SPELL_BOSS_SPAWN_AURA                         = 72712, //Falric and Marwyn
    SPELL_UTHER_DESPAWN                           = 70693,
    SPELL_TAKE_FROSTMOURNE                        = 72729,
    SPELL_FROSTMOURNE_DESPAWN                     = 72726,
    SPELL_FROSTMOURNE_VISUAL                      = 73220,
    SPELL_FROSTMOURNE_SOUNDS                      = 70667,

    // Ghostly Priest
    SPELL_SHADOW_WORD_PAIN                        = 72318,
    SPELL_CIRCLE_OF_DESTRUCTION                   = 72320,
    SPELL_COWER_IN_FEAR                           = 72321,
    SPELL_DARK_MENDING                            = 72322,

    // Phantom Mage
    SPELL_FIREBALL                                = 72163,
    SPELL_FLAMESTRIKE                             = 72169,
    SPELL_FROSTBOLT                               = 72166,
    SPELL_CHAINS_OF_ICE                           = 72121,
    SPELL_HALLUCINATION                           = 72342,

    // Phantom Hallucination (same as phantom mage + HALLUCINATION_2 when dies)
    SPELL_HALLUCINATION_2                         = 72344,

    // Shadowy Mercenary
    SPELL_SHADOW_STEP                             = 72326,
    SPELL_DEADLY_POISON                           = 72329,
    SPELL_ENVENOMED_DAGGER_THROW                  = 72333,
    SPELL_KIDNEY_SHOT                             = 72335,

    // Spectral Footman
    SPELL_SPECTRAL_STRIKE                         = 72198,
    SPELL_SHIELD_BASH                             = 72194,
    SPELL_TORTURED_ENRAGE                         = 72203,

    // Tortured Rifleman
    SPELL_SHOOT                                   = 72208,
    SPELL_CURSED_ARROW                            = 72222,
    SPELL_FROST_TRAP                              = 72215,
    SPELL_ICE_SHOT                                = 72268,

    SPELL_GHOST_VISUAL                            = 69861,

    // Frostsworn
    SPELL_SHIELD_THROWN                           = 69222,
    SPELL_SPIKE                                   = 69184,
    SPELL_CLONE_NAME                              = 57507,
    SPELL_CLONE_MODEL                             = 45204,
 
    // Reflection'Spells
    SPELL_BALEFUL_STRIKE                          = 69933,
    SPELL_SPIRIT_BURST                            = 69900,

    // Escape from Lich King
    SPELL_WINTER                                  = 69780,
    SPELL_FURY_OF_FROSTMOURNE                     = 70063,
    SPELL_DARK_ARROW                              = 70194,
    SPELL_ICE_BARRIER                             = 69787,
    SPELL_SUMMON_WALL                             = 69768,
    SPELL_SOUL_REAPER                             = 73797,
    SPELL_SUMMON_RAGING_GHOUL                     = 69818,
    SPELL_SUMMON_WHICH_DOCTOR                     = 69836,
    SPELL_SUMMON_LUM_ABOMINATION                  = 69835,
    SPELL_ICE_PRISON                              = 69708,
    SPELL_HARVEST_SOUL                            = 70070,
 
    //Raging gnoul
    SPELL_GNOUL_LEAP                              = 70150,
 
    //Witch Doctor
    SPELL_COURSE_OF_DOOM                          = 70144,
    SPELL_SHADOW_BOLT_VOLLEY                      = 70145,
    SPELL_SHADOW_BOLT                             = 70080,
 
    //Lumbering Abomination
    SPELL_ABO_CLEAVE                              = 40505,
    SPELL_ABO_VOMIT                               = 70176,

    SPELL_DESTROY_ICE_WALL_01                     = 69784, //Jaina
    SPELL_DESTROY_ICE_WALL_02                     = 70225, //Sylvana
    SPELL_SYLVANA_JUMP                            = 68339,
    SPELL_SYLVANA_STEP                            = 69087,
};

const Position HallsofReflectionLocs[]=
{
    {5283.234863f, 1990.946777f, 707.695679f, 0.929097f},   // 2 Loralen Follows
    {5408.031250f, 2102.918213f, 707.695251f, 0.792756f},   // 9 Sylvanas Follows
    {5401.866699f, 2110.837402f, 707.695251f, 0.800610f},   // 10 Loralen follows
};

const Position CannonSpawns[4] =
{
    {5230.00f, 1658.75f, 802.22f, 0.00f},
    {5245.74f, 1644.44f, 802.35f, 0.00f},
    {5260.64f, 1636.07f, 802.16f, 0.00f},
    {5275.90f, 1633.22f, 802.25f, 0.00f},
};

const Position SpawnPos              = {5262.540527f, 1949.693726f, 707.695007f, 0.808736f}; // Jaina/Sylvanas Beginning Position
const Position MoveThronePos         = {5306.952148f, 1998.499023f, 709.341431f, 1.277278f}; // Jaina/Sylvanas walks to throne
const Position UtherSpawnPos         = {5308.310059f, 2003.857178f, 709.341431f, 4.650315f};
const Position LichKingSpawnPos      = {5362.917480f, 2062.307129f, 707.695374f, 3.945812f};
const Position LichKingMoveThronePos = {5312.080566f, 2009.172119f, 709.341431f, 3.973301f}; // Lich King walks to throne
const Position LichKingMoveAwayPos   = {5400.069824f, 2102.7131689f, 707.69525f, 0.843803f}; // Lich King walks away

class npc_jaina_or_sylvanas_hor : public CreatureScript
{
private:
    bool m_isSylvana;

public:
    npc_jaina_or_sylvanas_hor(bool isSylvana, const char* name) : CreatureScript(name), m_isSylvana(isSylvana) { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->CLOSE_GOSSIP_MENU();
                if (creature->AI())
                    creature->AI()->DoAction(ACTION_START_INTRO);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->CLOSE_GOSSIP_MENU();
                if (creature->AI())
                    creature->AI()->DoAction(ACTION_SKIP_INTRO);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        QuestStatus status = player->GetQuestStatus(m_isSylvana ? QUEST_DELIVRANCE_FROM_THE_PIT_H2 : QUEST_DELIVRANCE_FROM_THE_PIT_A2);
        if (status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_REWARDED)
            player->ADD_GOSSIP_ITEM( 0, "Can you remove the sword?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        // once last quest is completed, she offers this shortcut of the starting event
        status = player->GetQuestStatus(m_isSylvana ? QUEST_WRATH_OF_THE_LICH_KING_H2 : QUEST_WRATH_OF_THE_LICH_KING_A2);
        if (status == QUEST_STATUS_COMPLETE || status == QUEST_STATUS_REWARDED)
            player->ADD_GOSSIP_ITEM( 0, "Dark Lady, I think I hear Arthas coming. Whatever you're going to do, do it quickly.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_jaina_or_sylvanas_horAI(creature);
    }

    // AI of Part1: handle the intro till start of gauntlet event.
    struct npc_jaina_or_sylvanas_horAI : public ScriptedAI
    {
        npc_jaina_or_sylvanas_horAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;
        uint64 utherGUID;
        uint64 lichkingGUID;

        EventMap events;

        void Reset()
        {
            events.Reset();

            utherGUID = 0;
            lichkingGUID = 0;

            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            me->SetVisible(true);
        }

        void DoAction(const int32 actionId)
        {
            switch (actionId)
            {
                case ACTION_START_INTRO:
                    events.ScheduleEvent(EVENT_START_INTRO, 0);
                    break;
                case ACTION_SKIP_INTRO:
                    events.ScheduleEvent(EVENT_SKIP_INTRO, 0);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            switch (events.ExecuteEvent())
            {
                case EVENT_START_INTRO:
                    me->GetMotionMaster()->MovePoint(0, MoveThronePos);
                    // Begining of intro is differents between fActions as the speech sequence and timers are differents.
                    if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        events.ScheduleEvent(EVENT_INTRO_A2_1, 0);
                    else
                        events.ScheduleEvent(EVENT_INTRO_H2_1, 0);
                    break;

            // A2 Intro Events
                case EVENT_INTRO_A2_1:
                    Talk(SAY_JAINA_INTRO_3);
                    events.ScheduleEvent(EVENT_INTRO_A2_2, 5000);
                    break;
                case EVENT_INTRO_A2_2:
                    Talk(SAY_JAINA_INTRO_4);
                    events.ScheduleEvent(EVENT_INTRO_A2_3, 10000);
                    break;
                case EVENT_INTRO_A2_3:
                    // TODO: she's doing some kind of spell casting emote
                    DoCast(me, SPELL_CAST_VISUAL);
                    instance->HandleGameObject(instance->GetData64(DATA_FROSTMOURNE), true);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    events.ScheduleEvent(EVENT_INTRO_A2_4, 10000);
                    break;
                case EVENT_INTRO_A2_4:
                    // spawn UTHER during speach 2
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->GetMotionMaster()->MoveIdle();
                        uther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                        utherGUID = uther->GetGUID();
                    }
                    events.ScheduleEvent(EVENT_INTRO_A2_5, 2000);
                    break;
                case EVENT_INTRO_A2_5:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_1);
                    events.ScheduleEvent(EVENT_INTRO_A2_6, 3000);
                    break;
                case EVENT_INTRO_A2_6:
                    Talk(SAY_JAINA_INTRO_5);
                    events.ScheduleEvent(EVENT_INTRO_A2_7, 6000);
                    break;
                case EVENT_INTRO_A2_7:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_2);
                    events.ScheduleEvent(EVENT_INTRO_A2_8, 6500);
                    break;
                case EVENT_INTRO_A2_8:
                    Talk(SAY_JAINA_INTRO_6);
                    events.ScheduleEvent(EVENT_INTRO_A2_9, 2000);
                    break;
                case EVENT_INTRO_A2_9:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_3);
                    events.ScheduleEvent(EVENT_INTRO_A2_10, 9000);
                    break;
                case EVENT_INTRO_A2_10:
                    Talk(SAY_JAINA_INTRO_7);
                    events.ScheduleEvent(EVENT_INTRO_A2_11, 5000);
                    break;
                case EVENT_INTRO_A2_11:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_4);
                    events.ScheduleEvent(EVENT_INTRO_A2_12, 11000);
                    break;
                case EVENT_INTRO_A2_12:
                    Talk(SAY_JAINA_INTRO_8);
                    events.ScheduleEvent(EVENT_INTRO_A2_13, 4000);
                    break;
                case EVENT_INTRO_A2_13:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_5);
                    events.ScheduleEvent(EVENT_INTRO_A2_14, 12500);
                    break;
                case EVENT_INTRO_A2_14:
                    Talk(SAY_JAINA_INTRO_9);
                    events.ScheduleEvent(EVENT_INTRO_A2_15, 10000);
                    break;
                case EVENT_INTRO_A2_15:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_6);
                    events.ScheduleEvent(EVENT_INTRO_A2_16, 22000);
                    break;
                case EVENT_INTRO_A2_16:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_7);
                    events.ScheduleEvent(EVENT_INTRO_A2_17, 4000);
                    break;
                case EVENT_INTRO_A2_17:
                    Talk(SAY_JAINA_INTRO_10);
                    events.ScheduleEvent(EVENT_INTRO_A2_18, 2000);
                    break;
                case EVENT_INTRO_A2_18:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_8);
                    }
                    events.ScheduleEvent(EVENT_INTRO_A2_19, 11000);
                    break;
                case EVENT_INTRO_A2_19:
                    Talk(SAY_JAINA_INTRO_11);
                    events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
                    break;

            // H2 Intro Events
                case EVENT_INTRO_H2_1:
                    Talk(SAY_SYLVANAS_INTRO_1);
                    events.ScheduleEvent(EVENT_INTRO_H2_2, 8000);
                    break;
                case EVENT_INTRO_H2_2:
                    Talk(SAY_SYLVANAS_INTRO_2);
                    events.ScheduleEvent(EVENT_INTRO_H2_3, 6000);
                    break;
                case EVENT_INTRO_H2_3:
                    Talk(SAY_SYLVANAS_INTRO_3);
                    // TODO: she's doing some kind of spell casting emote
                    DoCast(me, SPELL_CAST_VISUAL);
                    instance->HandleGameObject(instance->GetData64(DATA_FROSTMOURNE), true);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    events.ScheduleEvent(EVENT_INTRO_H2_4, 6000);
                    break;
                case EVENT_INTRO_H2_4:
                    // spawn UTHER during speach 2
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->GetMotionMaster()->MoveIdle();
                        uther->SetReactState(REACT_PASSIVE); // be sure he will not aggro arthas
                        utherGUID = uther->GetGUID();
                    }
                    events.ScheduleEvent(EVENT_INTRO_H2_5, 2000);
                    break;
                case EVENT_INTRO_H2_5:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_1);
                    events.ScheduleEvent(EVENT_INTRO_H2_6, 11000);
                    break;
                case EVENT_INTRO_H2_6:
                    Talk(SAY_SYLVANAS_INTRO_4);
                    events.ScheduleEvent(EVENT_INTRO_H2_7, 3000);
                    break;
                case EVENT_INTRO_H2_7:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_2);
                    events.ScheduleEvent(EVENT_INTRO_H2_8, 6000);
                    break;
                case EVENT_INTRO_H2_8:
                    Talk(SAY_SYLVANAS_INTRO_5);
                    events.ScheduleEvent(EVENT_INTRO_H2_9, 5000);
                    break;
                case EVENT_INTRO_H2_9:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_3);
                    events.ScheduleEvent(EVENT_INTRO_H2_10, 19000);
                    break;
                case EVENT_INTRO_H2_10:
                    Talk(SAY_SYLVANAS_INTRO_6);
                    events.ScheduleEvent(EVENT_INTRO_H2_11, 1500);
                    break;
                case EVENT_INTRO_H2_11:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_4);
                    events.ScheduleEvent(EVENT_INTRO_H2_12, 19500);
                    break;
                case EVENT_INTRO_H2_12:
                    Talk(SAY_SYLVANAS_INTRO_7);
                    events.ScheduleEvent(EVENT_INTRO_H2_13, 2000);
                    break;
                case EVENT_INTRO_H2_13:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_5);
                    }
                    events.ScheduleEvent(EVENT_INTRO_H2_14, 12000);
                    break;
                case EVENT_INTRO_H2_14:
                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_6);
                    events.ScheduleEvent(EVENT_INTRO_H2_15, 8000);
                    break;
                case EVENT_INTRO_H2_15:
                    Talk(SAY_SYLVANAS_INTRO_8);
                    events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
                    break;

            // Remaining Intro Events common for both faction
                case EVENT_INTRO_LK_1:
                    // Spawn LK in front of door, and make him move to the sword.
                    if (Creature* lichking = me->SummonCreature(NPC_LICH_KING_EVENT, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        lichking->SetWalk(true);
                        lichking->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                        lichking->SetReactState(REACT_PASSIVE);
                        lichkingGUID = lichking->GetGUID();
                    }

                    if (Creature* uther = me->GetCreature(*me, utherGUID))
                    {
                        if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            uther->AI()->Talk(SAY_UTHER_INTRO_A2_9);
                        else
                            uther->AI()->Talk(SAY_UTHER_INTRO_H2_7);
                    }

                    events.ScheduleEvent(EVENT_INTRO_LK_2, 11000);
                    break;

                case EVENT_INTRO_LK_2:
                     if (Creature* lichking = me->GetCreature(*me, lichkingGUID))
                         lichking->AI()->Talk(SAY_LK_INTRO_1);
                     events.ScheduleEvent(EVENT_INTRO_LK_3, 2000);
                     break;

                case EVENT_INTRO_LK_3:
                     // The Lich King banishes Uther to the abyss.
                     if (Creature* uther = me->GetCreature(*me, utherGUID))
                     {
                         uther->CastSpell(uther, 70693 /* Uther Despawn*/,true);
                         utherGUID = 0;
                     }

                     // He steps forward and removes the runeblade from the heap of skulls.

                     events.ScheduleEvent(EVENT_INTRO_LK_4, 4000);
                     break;

                case EVENT_INTRO_LK_4:
                      if (Creature* lichking = me->GetCreature(*me, lichkingGUID))
                          lichking->AI()->Talk(SAY_LK_INTRO_2);

                      if (GameObject *pFrostmourne = me->FindNearestGameObject(GO_FROSTMOURNE, 11.0f))
                         pFrostmourne->SetPhaseMask(0,true);
 
                      if (Creature* pLichKing = me->GetCreature(*me, lichkingGUID))
                      {
                          pLichKing->CastSpell(pLichKing, SPELL_TAKE_FROSTMOURNE, true);
                          pLichKing->CastSpell(pLichKing, SPELL_FROSTMOURNE_VISUAL, true);
                      }

                    events.ScheduleEvent(EVENT_INTRO_LK_5, 10000);
                    break;

                case EVENT_INTRO_LK_5:
                    // summon Falric and Marwyn. then go back to the door
                    
                    
                    if (Creature* falric = me->SummonCreature(NPC_FALRIC,5271.65f,2042.5f,709.32f,5.51217f))
                    {
                        falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                        falric->SetVisible(true);
                        falric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                        if (instance)
                          instance->SetData64(DATA_FALRIC, falric->GetGUID());
                    }
                    if (Creature* marwyn = me->SummonCreature(NPC_MARWYN,5344.75f,1972.87f,709.319f,2.33445f))
                    {
                        marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                        marwyn->SetVisible(true);
                        marwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                         if (instance)
                           instance->SetData64(DATA_MARWYN, marwyn->GetGUID());
                    }
                    
                    if (Creature* lichking = me->GetCreature(*me, lichkingGUID))
                    {
                        lichking->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);
                        lichking->AI()->Talk(SAY_LK_INTRO_3);
                    }

                    events.ScheduleEvent(EVENT_INTRO_LK_6, 8000);
                    break;

                case EVENT_INTRO_LK_6:
                    if (Creature* falric = me->GetCreature(*me, instance->GetData64(DATA_FALRIC)))
                        falric->AI()->Talk(SAY_FALRIC_INTRO_1);

                    events.ScheduleEvent(EVENT_INTRO_LK_7, 2000);
                    break;

                case EVENT_INTRO_LK_7:
                    if (Creature* marwyn = me->GetCreature(*me, instance->GetData64(DATA_MARWYN)))
                        marwyn->AI()->Talk(SAY_MARWYN_INTRO_1);

                    events.ScheduleEvent(EVENT_INTRO_LK_8, 5000);
                    break;

                case EVENT_INTRO_LK_8:
                     if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        Talk(SAY_JAINA_INTRO_END);
                     else
                        Talk(SAY_SYLVANAS_INTRO_END);

                    me->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);

                    events.ScheduleEvent(EVENT_INTRO_END, 10000);
                    break;
                case EVENT_INTRO_SKIP_END:

                  if (Creature* falric = me->SummonCreature(NPC_FALRIC,bossSpawnPos[0]))
                    {
                        falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                        falric->SetVisible(true);
                        falric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                        if (instance)
                          instance->SetData64(DATA_FALRIC, falric->GetGUID());
                    }
                    if (Creature* marwyn = me->SummonCreature(NPC_MARWYN,bossSpawnPos[1]))
                    {
                        marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                        marwyn->SetVisible(true);
                        marwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                         if (instance)
                           instance->SetData64(DATA_MARWYN, marwyn->GetGUID());
                    }

                    if (GameObject *pFrostmourne = me->FindNearestGameObject(GO_FROSTMOURNE, 100.0f))
                         pFrostmourne->SetPhaseMask(0,true);

                    // No break
                case EVENT_INTRO_END:
                    if (instance)
                        instance->SetData(DATA_WAVE_COUNT, SPECIAL);   // start first wave

                    if (Creature* falric = me->GetCreature(*me, instance->GetData64(DATA_FALRIC)))
                        falric->AI()->Talk(SAY_FALRIC_INTRO_2);
                    // Loralen or Koreln disappearAndDie()
                    me->DisappearAndDie();
                    if (Creature* lichking = me->GetCreature(*me, lichkingGUID))
                    {
                      lichking->DisappearAndDie();
                    }
                    break;

                case EVENT_SKIP_INTRO:
                    me->GetMotionMaster()->MovePoint(0, LichKingSpawnPos);
                    // TODO: Loralen/Koreln shall run also

                    events.ScheduleEvent(EVENT_INTRO_SKIP_END, 15000);
                    break;
            }
        }
    };

};

class npc_ghostly_priest : public CreatureScript
{
public:
    npc_ghostly_priest() : CreatureScript("npc_ghostly_priest") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ghostly_priestAI(creature);
    }

    struct npc_ghostly_priestAI: public ScriptedAI
    {
        npc_ghostly_priestAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
            events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
            events.ScheduleEvent(EVENT_DARK_MENDING, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_WORD_PAIN:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_SHADOW_WORD_PAIN);
                        events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000);
                        return;
                    case EVENT_CIRCLE_OF_DESTRUCTION:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CIRCLE_OF_DESTRUCTION);
                        events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
                        return;
                    case EVENT_COWER_IN_FEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_COWER_IN_FEAR);
                        events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
                        return;
                    case EVENT_DARK_MENDING:
                        // find an ally with missing HP
                        if (Unit* target = DoSelectLowestHpFriendly(40, DUNGEON_MODE(30000, 50000)))
                        {
                            DoCast(target, SPELL_DARK_MENDING);
                            events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
                        }
                        else events.ScheduleEvent(EVENT_DARK_MENDING, 5000); // no friendly unit with missing hp. re-check in just 5 sec.                            
                        return;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class npc_phantom_mage : public CreatureScript
{
public:
    npc_phantom_mage() : CreatureScript("npc_phantom_mage") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_phantom_mageAI(creature);
    }

    struct npc_phantom_mageAI: public ScriptedAI
    {
        npc_phantom_mageAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_FIREBALL, 3000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_FLAMESTRIKE, 6000);
            events.ScheduleEvent(EVENT_FROSTBOLT, 9000);
            events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 12000);
            events.ScheduleEvent(EVENT_HALLUCINATION, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_FIREBALL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FIREBALL);
                        events.ScheduleEvent(EVENT_FIREBALL, 15000);
                        return;
                    case EVENT_FLAMESTRIKE:
                        DoCast(SPELL_FLAMESTRIKE);
                        events.ScheduleEvent(EVENT_FLAMESTRIKE, 15000);
                        return;
                    case EVENT_FROSTBOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FROSTBOLT);
                        events.ScheduleEvent(EVENT_FROSTBOLT, 15000);
                        return;
                    case EVENT_CHAINS_OF_ICE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CHAINS_OF_ICE);
                        events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 15000);
                        return;
                    case EVENT_HALLUCINATION:
                        DoCast(SPELL_HALLUCINATION);
                        return;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class npc_phantom_hallucination : public CreatureScript
{
public:
    npc_phantom_hallucination() : CreatureScript("npc_phantom_hallucination") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_phantom_hallucinationAI(creature);
    }

    struct npc_phantom_hallucinationAI : public npc_phantom_mage::npc_phantom_mageAI
    {
        npc_phantom_hallucinationAI(Creature* creature) : npc_phantom_mage::npc_phantom_mageAI(creature)
        {
        }

        void Reset()
        {
          DoZoneInCombat();
        } 

        void JustDied(Unit* /*killer*/)
        {
            DoCast(SPELL_HALLUCINATION_2);
        }
    };

};

class npc_shadowy_mercenary : public CreatureScript
{
public:
    npc_shadowy_mercenary() : CreatureScript("npc_shadowy_mercenary") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shadowy_mercenaryAI(creature);
    }

    struct npc_shadowy_mercenaryAI: public ScriptedAI
    {
        npc_shadowy_mercenaryAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SHADOW_STEP, 8000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
            events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
            events.ScheduleEvent(EVENT_KIDNEY_SHOT, 12000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHADOW_STEP:
                        DoCast(SPELL_SHADOW_STEP);
                        events.ScheduleEvent(EVENT_SHADOW_STEP, 8000);
                        return;
                    case EVENT_DEADLY_POISON:
                        DoCast(me->getVictim(), SPELL_DEADLY_POISON);
                        events.ScheduleEvent(EVENT_DEADLY_POISON, 10000);
                        return;
                    case EVENT_ENVENOMED_DAGGER_THROW:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_ENVENOMED_DAGGER_THROW);
                        events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
                        return;
                    case EVENT_KIDNEY_SHOT:
                        DoCast(me->getVictim(), SPELL_KIDNEY_SHOT);
                        events.ScheduleEvent(EVENT_KIDNEY_SHOT, 10000);
                        return;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class npc_spectral_footman : public CreatureScript
{
public:
    npc_spectral_footman() : CreatureScript("npc_spectral_footman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_spectral_footmanAI(creature);
    }

    struct npc_spectral_footmanAI: public ScriptedAI
    {
        npc_spectral_footmanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000); // TODO: adjust timers
            events.ScheduleEvent(EVENT_SHIELD_BASH, 10000);
            events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPECTRAL_STRIKE:
                        DoCast(me->getVictim(), SPELL_SPECTRAL_STRIKE);
                        events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000);
                        return;
                    case EVENT_SHIELD_BASH:
                        DoCast(me->getVictim(), SPELL_SHIELD_BASH);
                        events.ScheduleEvent(EVENT_SHIELD_BASH, 5000);
                        return;
                    case EVENT_TORTURED_ENRAGE:
                        DoCast(SPELL_TORTURED_ENRAGE);
                        events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
                        return;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class npc_tortured_rifleman : public CreatureScript
{
public:
    npc_tortured_rifleman() : CreatureScript("npc_tortured_rifleman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_tortured_riflemanAI(creature);
    }

    struct npc_tortured_riflemanAI  : public ScriptedAI
    {
        npc_tortured_riflemanAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
            events.ScheduleEvent(EVENT_FROST_TRAP, 1000);
            events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CURSED_ARROW:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_CURSED_ARROW);
                        events.ScheduleEvent(EVENT_CURSED_ARROW, 8000);
                        return;
                    case EVENT_FROST_TRAP:
                        DoCast(SPELL_FROST_TRAP);
                        events.ScheduleEvent(EVENT_FROST_TRAP, 10000);
                        return;
                    case EVENT_ICE_SHOT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0,0,true))
                            DoCast(target, SPELL_ICE_SHOT);
                        events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
                        return;
                }
            }

            DoSpellAttackIfReady(SPELL_SHOOT);
        }
    };

};
 
class boss_lich_king_hor : public CreatureScript
{
public:
    boss_lich_king_hor() : CreatureScript("boss_lich_king_hor") { }
 
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_lich_king_horAI(creature);
    }
 
    struct boss_lich_king_horAI : public npc_escortAI
    {
        boss_lich_king_horAI(Creature *creature) : npc_escortAI(creature), summons(creature), instance(creature->GetInstanceScript())
        {
        }
 
        void Reset()
        {
          events.Reset();
          me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
          //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
          me->SetReactState(REACT_PASSIVE);
          me->SetHealth(me->GetMaxHealth()/2);

          if(instance->GetData(DATA_LICHKING_EVENT) == DONE)
            me->DisappearAndDie();

          if(Creature* pLider = me->GetCreature(*me, instance->GetData64(DATA_ESCAPE_LIDER)))
            me->Attack(pLider,true);         
        }
 
        void JustDied(Unit* pKiller) { }
 
        void DoAction(const int32 actionId)
        {
            switch (actionId)
            {
            case ACTION_START_CHASE:
              me->RemoveAurasDueToSpell(SPELL_ICE_PRISON);
              me->RemoveAurasDueToSpell(SPELL_DARK_ARROW);
              me->SetSpeed(MOVE_RUN, 0.8f, false);
              Start(false, true);
              break;
            case ACTION_WALL_04:  
              events.ScheduleEvent(EVENT_SUMMON_DOCTOR,1);
              events.ScheduleEvent(EVENT_SUMMON_DOCTOR,1);
              // no break
            case ACTION_WALL_03:
              events.ScheduleEvent(EVENT_SUMMON_DOCTOR,1);
              events.ScheduleEvent(EVENT_SUMMON_ABO,1);
              // no break
            case ACTION_WALL_02:
              events.ScheduleEvent(EVENT_SUMMON_DOCTOR,1);
              events.ScheduleEvent(EVENT_SUMMON_ABO,1);
              // no break
            case ACTION_WALL_01:
              events.ScheduleEvent(EVENT_SUMMON_DOCTOR,1);
              events.ScheduleEvent(EVENT_SUMMON_GHOULS,1);
              events.ScheduleEvent(EVENT_RESUME_MOVEMENT,5000);
              DoCast(me,SPELL_WINTER);
              SetEscortPaused(true);
              break;
            }
        }

        void WaypointReached(uint32 i)
        {
            switch(i)
            {
                case 66:
                    SetEscortPaused(true);
                    if(Creature* pLider = me->GetCreature(*me, instance->GetData64(DATA_ESCAPE_LIDER)))
                    {
                        me->CastSpell(pLider, SPELL_HARVEST_SOUL, false);
                        pLider->AI()->DoAction(ACTION_OUTRO);
                    }
                    instance->SetData(DATA_LICHKING_EVENT,DONE);
                    Talk(SAY_LK_EVENT_END);
                    break;
            }
        }
        
        void SummonedCreatureDespawn(Creature* summoned)
        {
          summons.Despawn(summoned);

          if (summons.empty())
          {
            me->RemoveAurasDueToSpell(SPELL_WINTER);
            me->SetSpeed(MOVE_RUN, 0.8f, false);            
            if(Creature* pLider = me->GetCreature(*me, instance->GetData64(DATA_ESCAPE_LIDER)))
              pLider->AI()->DoAction(ACTION_DESTROY_WALL);

          }
        }
 
        void JustSummoned(Creature* summoned)
        {        
            summons.Summon(summoned);

            summoned->SetInCombatWithZone();
        }
 
  
        void UpdateEscortAI(const uint32 diff)
        {
          if(instance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
            return;

          events.Update(diff);

          while(!me->HasUnitState(UNIT_STATE_CASTING))
              if(uint32 eventId = events.ExecuteEvent()) 
                switch(eventId)
                {               
                case EVENT_SUMMON_GHOULS:
                  DoCast(SPELL_SUMMON_RAGING_GHOUL);
                  break;
                case EVENT_SUMMON_DOCTOR:
                  DoCast(SPELL_SUMMON_WHICH_DOCTOR);
                  break;
                case EVENT_SUMMON_ABO:
                  DoCast(SPELL_SUMMON_LUM_ABOMINATION);
                  break;
                case EVENT_RESUME_MOVEMENT:
                  SetEscortPaused(false);
                  break;
                }
              else break;     

          DoMeleeAttackIfReady();
              
          // Leader caught, wipe
          if(Creature* pLider = me->GetCreature(*me, instance->GetData64(DATA_ESCAPE_LIDER)))              
            if (pLider->IsWithinDistInMap(me, 2.0f) && instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
            {
                    me->CastSpell(me, SPELL_FURY_OF_FROSTMOURNE, true);     
                    summons.DespawnAll();
                    instance->SetData(DATA_LICHKING_EVENT, FAIL);
                    pLider->DisappearAndDie();
                    me->DisappearAndDie();
            }

        }

        private:
        InstanceScript* instance;        
        SummonList summons;
        EventMap events;
    };
};
 
class npc_raging_gnoul : public CreatureScript
{
public:
    npc_raging_gnoul() : CreatureScript("npc_raging_gnoul") { }
 
    struct npc_raging_gnoulAI : public ScriptedAI
    {
        npc_raging_gnoulAI(Creature *creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
        }
  
        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_GNOUL_LEAP, 500);
            me->SetCorpseDelay(0);
        }
        
  
        void UpdateAI(const uint32 diff)
        {
           if(!UpdateVictim())
                return;
            
            events.Update(diff);
            
            while(!me->HasUnitState(UNIT_STATE_CASTING))
              if(uint32 eventId = events.ExecuteEvent()) 
                switch(eventId)
                {               
                case EVENT_GNOUL_LEAP:
                  if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM,0,25.0f,true))
                    DoCast(target, SPELL_GNOUL_LEAP);
                  else events.ScheduleEvent(EVENT_GNOUL_LEAP, 500); //Polling for target in 25 yd range;
                  break;
                }
              else break;          
            
            DoMeleeAttackIfReady();
        }

        private: 
          InstanceScript* instance;
          EventMap events;
    };
 
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_raging_gnoulAI(creature);
    }
 
};
 
class npc_risen_witch_doctor : public CreatureScript
{
public:
    npc_risen_witch_doctor() : CreatureScript("npc_risen_witch_doctor") { }
 
    struct npc_risen_witch_doctorAI : public ScriptedAI
    {
        npc_risen_witch_doctorAI(Creature *creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
        }
  
        void Reset()
        {
            events.Reset();
            events.ScheduleEvent(EVENT_DOCTOR_SHADOW_BOLT, 3000);
            events.ScheduleEvent(EVENT_DOCTOR_SHADOW_VALLEY, 8000);
            events.ScheduleEvent(EVENT_DOCTOR_CURSE_OF_DOOM, 5000);
            me->SetCorpseDelay(0);
        }
        
        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            
            events.Update(diff);
            
            while(!me->HasUnitState(UNIT_STATE_CASTING))
              if(uint32 eventId = events.ExecuteEvent()) 
                switch(eventId)
                {
                case EVENT_DOCTOR_SHADOW_BOLT:
                  DoCastVictim(SPELL_SHADOW_BOLT);
                  events.ScheduleEvent(EVENT_DOCTOR_SHADOW_BOLT,10000);
                  break;
                case EVENT_DOCTOR_SHADOW_VALLEY:
                  DoCastVictim(SPELL_SHADOW_BOLT_VOLLEY);
                  events.ScheduleEvent(EVENT_DOCTOR_SHADOW_VALLEY, 6000);
                  break;
                case EVENT_DOCTOR_CURSE_OF_DOOM:
                  if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM,0,0,true))
                        DoCast(target, SPELL_COURSE_OF_DOOM);
                  events.ScheduleEvent(EVENT_DOCTOR_CURSE_OF_DOOM, 20000);
                  break;
                }
              else break;
  
            DoMeleeAttackIfReady();
        }
    private: 
      InstanceScript* instance;
      EventMap events;
    };
 
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_risen_witch_doctorAI(creature);
    }
 
};
 
class npc_lumbering_abomination : public CreatureScript
{
public:
    npc_lumbering_abomination() : CreatureScript("npc_lumbering_abomination") { }
 
    struct npc_lumbering_abominationAI : public ScriptedAI
    {
        npc_lumbering_abominationAI(Creature *creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
        }
         
        void Reset()
        {
          events.Reset();
          events.ScheduleEvent(EVENT_ABO_VOMIT,10000);
          events.ScheduleEvent(EVENT_ABO_CLEAVE, 6000);

          if(Creature* pLider = me->GetCreature(*me, instance->GetData64(DATA_ESCAPE_LIDER)))
            AttackStart(pLider);

          me->SetCorpseDelay(0);
        }
        
        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            
            events.Update(diff);
            
            while(!me->HasUnitState(UNIT_STATE_CASTING))
              if(uint32 eventId = events.ExecuteEvent()) 
                switch(eventId)
                {
                case EVENT_ABO_VOMIT:
                  DoCastVictim(SPELL_ABO_VOMIT);
                  events.ScheduleEvent(EVENT_ABO_VOMIT,10000);
                  break;
                case EVENT_ABO_CLEAVE:
                  DoCastVictim(SPELL_ABO_CLEAVE);
                  events.ScheduleEvent(EVENT_ABO_CLEAVE, 6000);
                  break;
                }
              else break;
             
            DoMeleeAttackIfReady();
        }
 
    private: 
      InstanceScript* instance;
      EventMap events;
    };
 
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_lumbering_abominationAI(creature);
    }
};

class npc_frostworn_general : public CreatureScript
{
public:
    npc_frostworn_general() : CreatureScript("npc_frostworn_general") { }
 
    struct npc_frostworn_generalAI : public ScriptedAI
    {
        npc_frostworn_generalAI(Creature *creature) : ScriptedAI(creature), instance(creature->GetInstanceScript())
        {
        }      
 
        void Reset()
        {
            if (!instance)
                return;
            
            events.Reset();
        }
        
        void JustDied(Unit* killer)
        {
            if(!instance)
                return;   

            instance->SetData(DATA_FROSTSWORN_EVENT,DONE);
        }

        void EnterCombat(Unit* who)
        {
          events.ScheduleEvent(EVENT_SHEILD_THROW,8000);
          events.ScheduleEvent(EVENT_SPIKE,12000);
          events.ScheduleEvent(EVENT_CLONE,20000);
        }

        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            
            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
              switch(eventId)
              {
              case EVENT_SHEILD_THROW:
                if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                   DoCast(target, SPELL_SHIELD_THROWN);
                events.ScheduleEvent(EVENT_SHEILD_THROW,8000);
                break;
              case EVENT_SPIKE:
                if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                    DoCast(target, SPELL_SPIKE);
                events.ScheduleEvent(EVENT_SPIKE,12000);
                break;
              case EVENT_CLONE:
                {
                  std::list<Unit *> playerList;
                  SelectTargetList(playerList, 5, SELECT_TARGET_TOPAGGRO, 0, true);
                  for (std::list<Unit*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                  {
                    Creature* pReflection = me->SummonCreature(NPC_REFLECTION, (*itr)->GetPositionX(), (*itr)->GetPositionY(), (*itr)->GetPositionZ(), (*itr)->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                    pReflection->SetName((*itr)->GetName());
                    (*itr)->CastSpell(pReflection, SPELL_CLONE_NAME, true);
                    (*itr)->CastSpell(pReflection, SPELL_CLONE_MODEL, true);
                    pReflection->setFaction(me->getFaction());
                    pReflection->AI()->AttackStart((*itr));
                   }
                }
                break;
              }
            }
            
            DoMeleeAttackIfReady();
        }
 
    private:
      InstanceScript* instance;
      EventMap events;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_frostworn_generalAI(creature);
    }
};
 
class npc_spiritual_reflection : public CreatureScript
{
public:
    npc_spiritual_reflection() : CreatureScript("npc_spiritual_reflection") { }
 
    struct npc_spiritual_reflectionAI : public ScriptedAI
    {
        npc_spiritual_reflectionAI(Creature *creature) : ScriptedAI(creature)
        {
            Reset();
        }
 
        InstanceScript* instance;
        uint32 uiStrikeTimer;
 
        void Reset()
        {
            uiStrikeTimer = urand(1000,3000);
        }
 
        void JustDied(Unit* pKiller)
        {
            DoCast(pKiller, SPELL_SPIRIT_BURST);
        }
 
        void UpdateAI(const uint32 uiDiff)
        {
            if(!UpdateVictim())
                return;
 
            if(uiStrikeTimer < uiDiff)
            {
                if(Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                   DoCast(target, SPELL_BALEFUL_STRIKE);
                uiStrikeTimer = urand(3000, 8000);
            }
            else
                uiStrikeTimer -= uiDiff;
 
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_spiritual_reflectionAI(creature);
    }
};

class npc_jaina_and_sylvana_hor_part2 : public CreatureScript
{
public:
    npc_jaina_and_sylvana_hor_part2() : CreatureScript("npc_jaina_and_sylvana_hor_part2") { }
 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        InstanceScript* instance = (InstanceScript*)creature->GetInstanceScript();
        auto ai = CAST_AI(npc_jaina_and_sylvana_hor_part2::npc_jaina_and_sylvana_hor_part2AI, creature->AI());
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->CLOSE_GOSSIP_MENU();                
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
 
                if(instance)
                {
                    instance->SetData64(DATA_ESCAPE_LIDER, creature->GetGUID());
                    instance->SetData(DATA_LICHKING_EVENT, IN_PROGRESS);
                }

                if(Creature* pLichKingBoss = creature->GetCreature(*creature, instance->GetData64(NPC_LICH_KING_BOSS)))
                  pLichKingBoss->AI()->DoAction(ACTION_START_CHASE);

                ai->Start(false, true);
                return true;
            default:
                return false;
        }
    }
 
    bool OnGossipHello(Player* player, Creature* creature)
    {
        InstanceScript* instance = creature->GetInstanceScript();
 
        if(!instance)
            return false;
 
        if(instance->GetData(DATA_LICHKING_EVENT) == DONE)
            return false;
 
        if(creature->isQuestGiver())
           player->PrepareQuestMenu( creature->GetGUID());
 
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We're ready! Let's go!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
 
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
 
        return true;
    }
 
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_jaina_and_sylvana_hor_part2AI(creature);
    }
 
    struct npc_jaina_and_sylvana_hor_part2AI : public npc_escortAI
    {
        npc_jaina_and_sylvana_hor_part2AI(Creature *creature) : npc_escortAI(creature), instance(creature->GetInstanceScript())
        {
        }
     
        void Reset()
        {
            if(!instance)
                return;
            events.Reset();

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

            if(instance->GetData(DATA_LICHKING_EVENT) == DONE)
               me->DisappearAndDie();

            SetDespawnAtFar(false);

            DoCast(me,SPELL_ICE_BARRIER);
           
            triggered = false;
            //if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
              //me->Attack(pLichKingBoss,true);
        }
 
        void WaypointReached(uint32 i)
        {
            switch(i)
            {
              case 3:  // WP3 - Summon first Ice wall
                  summonWall(5540.39f, 2086.48f, 731.066f, 1.00057f);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->Talk(SAY_LK_EVENT_ICEWALL_01); 
                  break;
              case 4: // WP4 - Start destroying wall
                  SetEscortPaused(true);
                  me->CastSpell(me, me->GetEntry() == NPC_JAINA_PART2? SPELL_DESTROY_ICE_WALL_01 : SPELL_DESTROY_ICE_WALL_02, false);  
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->DoAction(ACTION_WALL_01);
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_ICEWALL_01 : SAY_SYLVANAS_ESCAPE_ICEWALL_01);
                  break;
              case 7: // WP8 - Summon second wall
                  summonWall(5494.3f, 1978.27f, 736.689f, 1.0885f);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->Talk(SAY_LK_EVENT_ICEWALL_02);
                  break;
              case 8: // WP9 - Start destroying wall
                  SetEscortPaused(true);
                  me->CastSpell(me, me->GetEntry() == NPC_JAINA_PART2? SPELL_DESTROY_ICE_WALL_01 : SPELL_DESTROY_ICE_WALL_02, false);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->DoAction(ACTION_WALL_02);
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_ICEWALL_02 : SAY_SYLVANAS_ESCAPE_ICEWALL_02);
                  break;
              case 11: // WP11 - Summon third wall                 
                  summonWall(5426.27f, 1888.12f, 751.303f, 0.923328f);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->Talk(SAY_LK_EVENT_ICEWALL_03);
                  break;
              case 12: // WP12 - Start destroying wall
                  SetEscortPaused(true);
                  me->CastSpell(me, me->GetEntry() == NPC_JAINA_PART2? SPELL_DESTROY_ICE_WALL_01 : SPELL_DESTROY_ICE_WALL_02, false);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->DoAction(ACTION_WALL_03);
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_ICEWALL_03 : SAY_SYLVANAS_ESCAPE_ICEWALL_03);
                  break;
              case 15: // WP15 - Summon 4th wall                 
                  summonWall(5323.61f, 1755.85f, 770.305f, 0.784186f); 
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->Talk(SAY_LK_EVENT_ICEWALL_04);
                  break;
              case 16: // WP16 - Start destroying wall
                  SetEscortPaused(true);
                  me->CastSpell(me, me->GetEntry() == NPC_JAINA_PART2? SPELL_DESTROY_ICE_WALL_01 : SPELL_DESTROY_ICE_WALL_02, false);
                  if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                    pLichKingBoss->AI()->DoAction(ACTION_WALL_04);
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_ICEWALL_04 : SAY_SYLVANAS_ESCAPE_ICEWALL_04);
                  break;
              case 17:
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_END_01 : SAY_SYLVANAS_ESCAPE_END_01);
                  break;
              case 20:
                  SetEscortPaused(true);
                  me->SetUInt32Value(UNIT_NPC_EMOTESTATE,me->GetEntry() == NPC_JAINA_PART2 ? EMOTE_STATE_READY2HL : EMOTE_STATE_READY1H);
                  Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_END_02 : SAY_SYLVANAS_ESCAPE_END_02);
                  break;
            }
        }
        
        void DoAction(const int32 actionId) override
        {
            switch (actionId)
            {
                case ACTION_DESTROY_WALL:
                  {
                    me->FindNearestGameObject(GO_ICE_WALL, 50.0f)->SetGoState(GO_STATE_ACTIVE);                 
                    SetEscortPaused(false);
                    me->InterruptNonMeleeSpells(false);
                    break;
                  }
                case ACTION_OUTRO:
                  {
                    ship = createTransport(me->GetEntry() == NPC_JAINA_PART2? GO_SKYBREAKER : GO_ORGRIM_HAMMER);
                    ship->BuildStartMovePacket(me->GetMap());
                    events.ScheduleEvent(EVENT_FINISH_ESCAPE,13000);
                    SetEscortPaused(true);
                  }
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who || who->GetTypeId() != TYPEID_PLAYER)
                return;
            
            if(triggered)
              return;
 
            if(me->IsWithinDistInMap(who, 50.0f))
            {
                events.ScheduleEvent(EVENT_ESCAPE_FREEZ_AND_MOVE,15000);
                if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                  pLichKingBoss->AI()->Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_LK_ALLIANCE_AGGRO : SAY_LK_HORDE_AGGRO);
                triggered = true;
            }
        }
  
        void HoRQuestComplete(uint32 killCredit)
        {
            if(instance)
            {
                Map::PlayerList const &PlayerList = instance->instance->GetPlayers();
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    i->getSource()->KilledMonsterCredit(killCredit, 0);
            }
        }
 
        void UpdateEscortAI(const uint32 diff)
        {
            if(!instance)
                return; 

            events.Update(diff);

            while(uint32 eventId = events.ExecuteEvent())
            {
              switch(eventId)
              {
              case EVENT_ESCAPE_FREEZ_AND_MOVE:
                me->RemoveAurasDueToSpell(SPELL_ICE_BARRIER);

                if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                {
                  pLichKingBoss->AttackStop();
                  pLichKingBoss->CastSpell(pLichKingBoss, me->GetEntry() == NPC_JAINA_PART2 ? SPELL_ICE_PRISON : SPELL_DARK_ARROW, true);                  
                }

                me->AttackStop();

                me->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->FindNearestGameObject(GO_RUN_DOOR, 50.0f)->SetGoState(GO_STATE_ACTIVE);
                Talk(SAY_JAINA_ESCAPE_START);
                break;
              case EVENT_FINISH_ESCAPE:
                ship->BuildStopMovePacket(me->GetMap());
                me->RemoveAllAuras();
                me->SummonGameObject(GO_PORTAL, 5297.91f, 1642.62f, 784.302f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0);
                me->SummonGameObject(me->GetEntry() == NPC_JAINA_PART2 ? DUNGEON_MODE<uint32>(GO_CAPTAIN_CHEST_ALLIANCE_NM,GO_CAPTAIN_CHEST_ALLIANCE_HC) : DUNGEON_MODE<uint32>(GO_CAPTAIN_CHEST_HORDE_NM,GO_CAPTAIN_CHEST_HORDE_HC),5303.36f, 1646.82f, 784.30f, 2.5064f,0.0f, 0.0f,0.0f,0.0f,0);
                me->FindNearestGameObject(GO_CAVE, 50.0f)->UseDoorOrButton();
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

                if(Creature* pLichKingBoss = me->GetCreature(*me, instance->GetData64(NPC_LICH_KING_BOSS)))
                  pLichKingBoss->DisappearAndDie();
                Talk(me->GetEntry() == NPC_JAINA_PART2? SAY_JAINA_ESCAPE_END_03 : SAY_SYLVANAS_ESCAPE_END_03);
                HoRQuestComplete(38211);
                break;
              }
            }
        }

        private:

          void summonWall(float pos_x, float pos_y, float pos_z, float ang)
          {
            if(Creature* wall = me->SummonCreature(NPC_ICE_WALL,pos_x, pos_y, pos_z, ang))
              wall->CastSpell((Unit*)NULL,69767,false);
          }

          Transport* createTransport(uint32 entry)
          {

            GameObjectTemplate const* goinfo = sObjectMgr->GetGameObjectTemplate(entry);
     
            std::set<uint32> mapsUsed;
            Transport* t = new Transport(60000, goinfo->ScriptId);
            t->GenerateWaypoints(goinfo->moTransport.taxiPathId, mapsUsed);

            if(t->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry, t->m_WayPoints[0].mapid, t->m_WayPoints[0].x, t->m_WayPoints[0].y, t->m_WayPoints[0].z, 0.0f, 255, 0))
            {
              sMapMgr->m_Transports.insert(t);

              for (std::set<uint32>::const_iterator i = mapsUsed.begin(); i != mapsUsed.end(); ++i)
                sMapMgr->m_TransportsByMap[*i].insert(t);

              Map* map = me->GetMap();

              t->SetMap(map);
              t->AddToWorld();                                       
            }

            return t;
          }

          EventMap events;
          InstanceScript* instance;
          Transport* ship;
          bool triggered;
    };
};

void AddSC_halls_of_reflection()
{
    new npc_jaina_or_sylvanas_hor(true, "npc_sylvanas_hor_part1");
    new npc_jaina_or_sylvanas_hor(false, "npc_jaina_hor_part1");
    new npc_ghostly_priest();
    new npc_phantom_mage();
    new npc_phantom_hallucination();
    new npc_shadowy_mercenary();
    new npc_spectral_footman();
    new npc_tortured_rifleman();

    new npc_frostworn_general();
    new npc_spiritual_reflection();

    new boss_lich_king_hor();
    new npc_lumbering_abomination();
    new npc_risen_witch_doctor();
    new npc_raging_gnoul();
    new npc_jaina_and_sylvana_hor_part2();
}
