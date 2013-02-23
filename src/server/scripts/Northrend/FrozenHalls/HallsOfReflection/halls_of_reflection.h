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

#ifndef DEF_HALLS_OF_REFLECTION_H
#define DEF_HALLS_OF_REFLECTION_H

enum Data
{
    DATA_FALRIC_EVENT,
    DATA_MARWYN_EVENT,
    DATA_LICHKING_EVENT,
    DATA_WAVE_COUNT,
    DATA_TEAM_IN_INSTANCE,
    DATA_FALRIC,
    DATA_MARWYN,
    DATA_LICHKING,
    DATA_FROSTMOURNE,
    DATA_FROSTSWORN_EVENT,

    DATA_ESCAPE_LIDER,

    DATA_SUMMONS,
    DATA_ICE_WALL_1,       
    DATA_ICE_WALL_2,        
    DATA_ICE_WALL_3,        
    DATA_ICE_WALL_4,        

    DECREASE                   = 100,
    RESUME                     = 101,
};

enum Creatures
{
    NPC_FALRIC                                    = 38112,
    NPC_MARWYN                                    = 38113,
    NPC_LICH_KING_BOSS                            = 37226,
    NPC_LICH_KING_EVENT                           = 36954,

    NPC_UTHER                                     = 37225,
    NPC_JAINA_PART1                               = 37221,
    NPC_JAINA_PART2                               = 36955,
    NPC_SYLVANAS_PART1                            = 37223,
    NPC_SYLVANAS_PART2                            = 37554,

    NPC_WAVE_MERCENARY                            = 38177,
    NPC_WAVE_FOOTMAN                              = 38173,
    NPC_WAVE_RIFLEMAN                             = 38176,
    NPC_WAVE_PRIEST                               = 38175,
    NPC_WAVE_MAGE                                 = 38172,

    NPC_FROSTWORN_GENERAL                         = 36723,
    NPC_REFLECTION                                = 37107,

    NPC_ICE_WALL                                  = 37014,
    NPC_RAGING_GNOUL                              = 36940,
    NPC_RISEN_WITCH_DOCTOR                        = 36941,
    NPC_ABON                                      = 37069,

    NPC_BARTLETT                                  = 37182,
    NPC_KORM                                      = 37833,
};

enum GameObjects
{
    GO_FROSTMOURNE               = 202302,
    GO_FROSTMOURNE_ALTAR         = 202236,
    GO_FRONT_DOOR                = 201976,
    GO_ARTHAS_DOOR               = 197341,
    GO_REAR_DOOR                 = 197342,
    GO_RUN_DOOR                  = 197343,
 
    GO_ICE_WALL                  = 201385,
    GO_CAVE                      = 201596,
 
    GO_STAIRS_SKYBREAKER         = 201709,
    GO_SKYBREAKER                = 201598,
    GO_STAIRS_ORGRIM_HAMMER      = 202211,
    GO_ORGRIM_HAMMER             = 201599,

    GO_PORTAL                    = 202079,

    GO_CAPTAIN_CHEST_HORDE_NM              = 202212, 
    GO_CAPTAIN_CHEST_ALLIANCE_NM           = 201710, 
    GO_CAPTAIN_CHEST_HORDE_HC              = 202337, 
    GO_CAPTAIN_CHEST_ALLIANCE_HC           = 202336, 
};

enum Achievements
{
    ACHIEV_NOT_RETREATING_EVENT    = 22615,
    SPELL_ACHIEV_CHECK             = 72830,
};

enum HorWorldStates
{
    WORLD_STATE_HOR                               = 4884,
    WORLD_STATE_HOR_WAVE_COUNT                    = 4882,
};

// Common actions from Instance Script to Boss Script
enum Actions
{
    ACTION_ENTER_COMBAT,
};

static Position bossSpawnPos[3] =
{
  {5271.65f,2042.50f,709.320f,5.51217f},
  {5344.75f,1972.87f,709.319f,2.33445f},
  {5413.89f,2116.61f,709.319f,3.95703f},
};

const Position OutroSpawns[2] =
{
  {5564.25f, 2274.69f, 733.01f, 3.93f}, // Lich King
  {5556.27f, 2266.28f, 733.01f, 0.8f},  // Jaina/Sylvana
};
// Base class for FALRIC and MARWYN
// handled the summonList and the notification events to/from the InstanceScript
struct boss_horAI : ScriptedAI
{
    boss_horAI(Creature* creature) : ScriptedAI(creature), summons(creature)
    {
        instance = me->GetInstanceScript();
    }

    InstanceScript* instance;
    EventMap events;
    SummonList summons;

    void Reset()
    {
        events.Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
        me->SetReactState(REACT_PASSIVE);
    }

    void DamageTaken(Unit* /*who*/, uint32 &uiDamage)
    {
        if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            uiDamage = 0;
    }

    void DoAction(const int32 actionID)
    {
        switch (actionID)
        {
            case ACTION_ENTER_COMBAT:  

                // Reset flags
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_AGGRESSIVE);

                if (Unit* unit = me->SelectNearestTarget())
                    AttackStart(unit);

                DoZoneInCombat();
                break;
        }
    }
};

#endif
