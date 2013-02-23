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
#include "InstanceScript.h"
#include "halls_of_reflection.h"
#include "Player.h"

#define MAX_ENCOUNTER 3

/* Halls of Reflection encounters:
0- Falric
1- Marwyn
2- The Lich King
*/

enum eEnum
{
    ENCOUNTER_WAVE_COUNT                      = 36,
};

enum Events
{
    EVENT_NONE,
    EVENT_NEXT_WAVE,
    EVENT_RESET_LK_EVENT,
    EVENT_CHECK_WIPE,
};

static Position SpawnPos[ENCOUNTER_WAVE_COUNT] =
{
    {5276.57f, 2014.22f, 707.778f, 5.96903f},
    {5295.88f, 2040.34f, 707.778f, 5.07891f},
    {5320.37f, 1980.13f, 707.778f, 2.00713f},
    {5280.51f, 1997.84f, 707.778f, 0.296706f},
    {5302.45f, 2042.22f, 707.778f, 4.90438f},
    {5306.57f, 1977.47f, 707.778f, 1.50098f},
    {5312.75f, 2037.12f, 707.778f, 4.59022f},
    {5309.58f, 2042.67f, 707.778f, 4.69494f},
    {5275.08f, 2008.72f, 707.778f, 6.21337f},
    {5279.65f, 2004.66f, 707.778f, 0.069813f},
    {5275.48f, 2001.14f, 707.778f, 0.174533f},
    {5316.70f, 2041.55f, 707.778f, 4.50295f},
    {5302.25f, 1972.41f, 707.778f, 1.37881f},
    {5311.03f, 1972.23f, 707.778f, 1.64061f},
    {5277.36f, 1993.23f, 707.778f, 0.401426f},
    {5318.70f, 2036.11f, 707.778f, 4.2237f},
    {5335.72f, 1996.86f, 707.778f, 2.74017f},
    {5299.43f, 1979.01f, 707.778f, 1.23918f},
    {5306.06f, 2037.00f, 707.778f, 4.81711f},
    {5344.15f, 2007.17f, 707.778f, 3.15905f},
    {5337.83f, 2010.06f, 707.778f, 3.22886f},
    {5343.29f, 1999.38f, 707.778f, 2.9147f},
    {5340.84f, 1992.46f, 707.778f, 2.75762f},
    {5325.07f, 1977.60f, 707.778f, 2.07694f},
    {5336.60f, 2017.28f, 707.778f, 3.47321f},
    {5313.82f, 1978.15f, 707.778f, 1.74533f},
    {5280.63f, 2010.50f, 707.778f, 6.05629f},
    {5322.96f, 2040.29f, 707.778f, 4.34587f},
    {5343.47f, 2015.95f, 707.778f, 3.49066f},
    {5337.86f, 2003.40f, 707.778f, 2.98451f},
    {5319.16f, 1974.00f, 707.778f, 1.91986f},
    {5299.25f, 2036.00f, 707.778f, 5.02655f},
    {5295.64f, 1973.76f, 707.778f, 1.18682f},
    {5282.38f, 2016.85f, 707.778f, 5.88176f},
    {5340.74f, 2021.99f, 707.778f, 3.69204f},
    {5279.36f, 2020.48f, 707.778f, 5.96903f},
};

static uint32 WaveEntries[5] = 
{
  NPC_WAVE_MERCENARY,                            
  NPC_WAVE_FOOTMAN,                              
  NPC_WAVE_RIFLEMAN,                             
  NPC_WAVE_PRIEST,                             
  NPC_WAVE_MAGE,
};

class instance_halls_of_reflection : public InstanceMapScript
{
public:
    instance_halls_of_reflection() : InstanceMapScript("instance_halls_of_reflection", 668) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_halls_of_reflection_InstanceMapScript(map);
    }

    struct instance_halls_of_reflection_InstanceMapScript : public InstanceScript
    {
        instance_halls_of_reflection_InstanceMapScript(Map* map) : InstanceScript(map) {};

        uint64 uiFalric;
        uint64 uiMarwyn;
        uint64 uiLichKingEvent;
        uint64 uiJainaPart1;
        uint64 uiJainaPart2;
        uint64 uiLichKingBoss;
        uint64 uiLeader;

        uint64 uiGunship;
        uint64 uiChest;
        uint64 uiPortal;

        uint64 uiFrostmourne;
        uint64 uiFrostmourneAltar;
        uint64 uiArthasDoor;
        uint64 uiFrontDoor;
        uint64 uiRearDoor;
        uint64 uiRunDoor;
        uint64 uiWall[4];
        uint64 uiWallID[4];
        uint64 uiCaveDoor;

        uint32 uiEncounter[MAX_ENCOUNTER];
        uint32 uiTeamInInstance;
        uint32 uiWaveCount;

        uint8 spawnCount;

        bool bIntroDone;
        bool bWiped;

        std::list<uint64> _waveList;
        std::list<uint64> _activeWaveList;

        EventMap events;

        void Initialize()
        {
            events.Reset();

            uiFalric = 0;
            uiMarwyn = 0;
            uiLichKingEvent = 0;
            uiLichKingBoss = 0;
            uiJainaPart1 = 0;
            uiLeader = 0;

            uiFrostmourne = 0;
            uiFrostmourneAltar = 0;
            uiArthasDoor = 0;
            uiFrontDoor = 0;
            uiRearDoor = 0;
            uiRunDoor = 0;
            uiCaveDoor = 0;

            uiGunship = 0;
            uiChest = 0;
            uiPortal = 0;
  
            uiTeamInInstance = 0;
            uiWaveCount = 0;
            bIntroDone = false;
            bWiped = false;

            for (uint8 i = 0; i < 4; ++i)
            {
                uiWall[i] = 0;
                uiWallID[i] = 0;
            }

            for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                uiEncounter[i] = NOT_STARTED;
        }

        void OnCreatureCreate(Creature* creature)
        {
            Map::PlayerList const &players = instance->GetPlayers();
            if (!players.isEmpty())
                if (Player* player = players.begin()->getSource())
                    uiTeamInInstance = player->GetTeam();

            switch (creature->GetEntry())
            {
                case NPC_FALRIC:
                    uiFalric = creature->GetGUID();
                    break;
                case NPC_MARWYN:
                    uiMarwyn = creature->GetGUID();
                    break;
                case NPC_LICH_KING_EVENT:
                    uiLichKingEvent = creature->GetGUID();
                    break;
                case NPC_SYLVANAS_PART1:
					if (uiTeamInInstance == ALLIANCE)
                        creature->UpdateEntry(NPC_JAINA_PART1, ALLIANCE);
                    uiJainaPart1 = creature->GetGUID();
                    if(bIntroDone)
                    {
                      uiJainaPart1 = 0;
                      creature->DisappearAndDie();
                    }
                    break;
                case 37779 /* Dark Ranger Loralen */:
                    if (uiTeamInInstance == ALLIANCE)
                        creature->UpdateEntry(37582 /* Archmage Koreln */, ALLIANCE);
                    if(bIntroDone)
                      creature->DisappearAndDie();
                    break;
                case NPC_JAINA_PART2:
                    if (uiTeamInInstance == HORDE)
                        creature->UpdateEntry(NPC_SYLVANAS_PART2, HORDE);
                    creature->SetHealth(252000);
                    uiLeader = creature->GetGUID();
                    break;
                case NPC_LICH_KING_BOSS:
                    creature->SetHealth(20917000);
                    uiLichKingBoss = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go)
        {
            // TODO: init state depending on encounters
            switch (go->GetEntry())
            {
                case GO_FROSTMOURNE:
                    uiFrostmourne = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    HandleGameObject(0, false, go);
                    if(bIntroDone)
                      go->SetPhaseMask(0,true);
                    break;
                case GO_FROSTMOURNE_ALTAR:
                    uiFrostmourneAltar = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    HandleGameObject(0, true, go);
                    break;
                case GO_FRONT_DOOR:
                    uiFrontDoor = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
                    HandleGameObject(0, true, go);
                    break;
                case GO_ARTHAS_DOOR:
                    uiArthasDoor = go->GetGUID();
                    go->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);

                    if (GetData(DATA_MARWYN_EVENT) == DONE)
                        HandleGameObject(0, true, go);
                    else
                        HandleGameObject(0, false, go);
                    break;
                case GO_RUN_DOOR:
                    uiRunDoor = go->GetGUID();
                    break;
                case GO_REAR_DOOR:
                    uiRearDoor = go->GetGUID();
                    break;
                case GO_CAVE:
                    uiCaveDoor = go->GetGUID();
                    break;
                case GO_CAPTAIN_CHEST_HORDE_NM:
                    if (!instance->IsHeroic() && uiTeamInInstance == HORDE)
                        uiChest = go->GetGUID();
                    break;
                case GO_CAPTAIN_CHEST_HORDE_HC:
                    if (instance->IsHeroic() && uiTeamInInstance == HORDE)
                        uiChest = go->GetGUID();
                    break;
                case GO_CAPTAIN_CHEST_ALLIANCE_NM:
                    if (!instance->IsHeroic() && uiTeamInInstance == ALLIANCE)
                        uiChest = go->GetGUID();
                    break;
                case GO_CAPTAIN_CHEST_ALLIANCE_HC:
                    if (instance->IsHeroic() && uiTeamInInstance == ALLIANCE)
                        uiChest = go->GetGUID();
                    break;
                case GO_SKYBREAKER:
                    //go->SetPhaseMask(2, true);
                    if (uiTeamInInstance == ALLIANCE)
                        uiGunship = go->GetGUID();
                    break;
                case GO_ORGRIM_HAMMER:
                    //go->SetPhaseMask(2, true);
                    if (uiTeamInInstance == HORDE)
                        uiGunship = go->GetGUID();
                    break;
                case GO_PORTAL:
                    go->SetPhaseMask(2, true);
                    uiPortal = go->GetGUID();
                    break;
            }
        }

        void OnUnitDeath(Unit* unit)
        {
          Creature* creature = unit->ToCreature();
          if (!creature)
            return;

          switch (creature->GetEntry())
          {
            case NPC_WAVE_FOOTMAN:
            case NPC_WAVE_MAGE:
            case NPC_WAVE_MERCENARY:
            case NPC_WAVE_PRIEST:
            case NPC_WAVE_RIFLEMAN:
              SetData(DATA_WAVE_COUNT, DECREASE);
              creature->SetCorpseDelay(40000U);
              break;
          }
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_FALRIC_EVENT:
                    uiEncounter[0] = data;
                    if (data == DONE && GetData(DATA_MARWYN_EVENT) != DONE)
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 60000);
                    break;
                case DATA_MARWYN_EVENT:
                    uiEncounter[1] = data;
                    if (data == DONE)
                    {
                        DespawnWaves();
                        HandleGameObject(uiArthasDoor, true);
                        HandleGameObject(uiFrontDoor, true);

                        instance->SummonCreature(NPC_LICH_KING_BOSS, OutroSpawns[0]);
                        instance->SummonCreature(NPC_JAINA_PART2, OutroSpawns[1]);
                        instance->SummonCreature(NPC_FROSTWORN_GENERAL, bossSpawnPos[2]);

                        DoUpdateWorldState(WORLD_STATE_HOR, 0);
                    }
                    break;
                case DATA_FROSTSWORN_EVENT:
                  if (data == DONE)
                  {
                      HandleGameObject(uiRearDoor, true);
                      HandleGameObject(uiRunDoor, false);
                  }
                  break;
                case DATA_LICHKING_EVENT:
                    uiEncounter[2] = data;
                    if(data == IN_PROGRESS)
                    {
                      if(instance->IsHeroic())
                        DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                    }
                    else if(data == FAIL)
                    {
                      //events.ScheduleEvent(EVENT_RESET_LK_EVENT, 30000);
                      DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                      bWiped = false;
                    }
                    else if(data == DONE)
                    {
                      DoCastSpellOnPlayers(SPELL_ACHIEV_CHECK);
                      DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_NOT_RETREATING_EVENT);
                    }
                    break;  
                case DATA_WAVE_COUNT:
                  {
                    switch(data)
                    {
                    case SPECIAL: // Starts first wave
                      SpawnWaves(ENCOUNTER_WAVE_COUNT);
                      HandleGameObject(uiFrontDoor, false);
                      events.ScheduleEvent(EVENT_NEXT_WAVE, 10000);
                      events.ScheduleEvent(EVENT_CHECK_WIPE, 2000);
                      bIntroDone = true;
                      break;
                    case RESUME: // Hack used to spawn after wipe/load (avoiding DB spawning)
                      DespawnWaves();
                      if(GetData(DATA_MARWYN_EVENT) != DONE)
                      {
                        uint32 spawns = 22;
                        uiWaveCount = 5;
                        if(GetData(DATA_FALRIC_EVENT) != DONE)
                        {
                          if (Creature* falric = instance->SummonCreature(NPC_FALRIC,bossSpawnPos[0]))
                          {
                            falric->SetVisible(true);
                            falric->GetMotionMaster()->MovePoint(0, 5283.309f, 2031.173f, 709.319f);
                            SetData64(DATA_FALRIC, falric->GetGUID());
                          }
                          spawns = 36;
                          uiWaveCount = 0;
                        }

                        if (Creature* marwyn = instance->SummonCreature(NPC_MARWYN,bossSpawnPos[1]))
                        {
                          marwyn->SetVisible(true);
                          marwyn->GetMotionMaster()->MovePoint(0, 5335.585f, 1981.439f, 709.319f);
                          SetData64(DATA_MARWYN, marwyn->GetGUID());
                        }

                        SpawnWaves(spawns);
                        HandleGameObject(uiFrontDoor, false);
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 10000);
                      }
                      else if(GetData(DATA_LICHKING_EVENT) != DONE)
                      {                        
                        HandleGameObject(uiArthasDoor, true);
                        HandleGameObject(uiFrontDoor, true);

                        instance->SummonCreature(NPC_LICH_KING_BOSS, OutroSpawns[0]);
                        instance->SummonCreature(NPC_JAINA_PART2, OutroSpawns[1]);

                        DoUpdateWorldState(WORLD_STATE_HOR, 0);
                      }
                      events.ScheduleEvent(EVENT_CHECK_WIPE, 2000);
                      bWiped = false;
                      break;
                    case DECREASE: // Called on wave creature death
                      if(!--spawnCount)
                        events.ScheduleEvent(EVENT_NEXT_WAVE, 10000);
                      break;
                    }
                  }
            }

            if (data == DONE)
               SaveToDB();
        }

        uint32 GetData(uint32 type) const
        {
            switch (type)
            {
                case DATA_FALRIC_EVENT:         return uiEncounter[0];
                case DATA_MARWYN_EVENT:         return uiEncounter[1];
                case DATA_LICHKING_EVENT:       return uiEncounter[2];
                case DATA_WAVE_COUNT:           return bWiped && bIntroDone;
                case DATA_TEAM_IN_INSTANCE:     return uiTeamInInstance;                
            }

            return 0;
        }

        void SetData64(uint32 id, uint64 guid)
        {
            switch (id)
            {
                case DATA_FALRIC:
                    uiFalric = guid;
                    break;
                case DATA_MARWYN:
                    uiMarwyn = guid;
                    break;
                case DATA_ESCAPE_LIDER:
                    uiLeader = guid;
                    break;
            }
        }

        uint64 GetData64(uint32 identifier) const
        {
            switch (identifier)
            {
                case DATA_FALRIC:               return uiFalric;
                case DATA_MARWYN:               return uiMarwyn;
                case DATA_LICHKING:             return uiLichKingEvent;
                case DATA_FROSTMOURNE:          return uiFrostmourne;
                case NPC_LICH_KING_BOSS:        return uiLichKingBoss;
                case DATA_ESCAPE_LIDER:         return uiLeader;
            }

            return 0;
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << "H R 1 " << uiEncounter[0] << ' ' << uiEncounter[1] << ' ' << uiEncounter[2];

            OUT_SAVE_INST_DATA_COMPLETE;
            return saveStream.str();
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;
            uint16 version;
            uint16 data0, data1, data2;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> version >> data0 >> data1 >> data2;

            if (dataHead1 == 'H' && dataHead2 == 'R')
            {   

                uiEncounter[0] = data0;
                uiEncounter[1] = data1;
                uiEncounter[2] = data2;

                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    if (uiEncounter[i] == IN_PROGRESS)
                        uiEncounter[i] = NOT_STARTED;

            } else OUT_LOAD_INST_DATA_FAIL;

            if (uiEncounter[0] == DONE || uiEncounter[1] == DONE)
            {
                bIntroDone = true;
                bWiped = true;
            }

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        void AddWave()
        {
            DoUpdateWorldState(WORLD_STATE_HOR, 1);
            DoUpdateWorldState(WORLD_STATE_HOR_WAVE_COUNT, uiWaveCount);

            switch (uiWaveCount)
            {
                case 1:
                case 2:
                case 3:
                case 4:
                    ActivateWave();
                    break;
                case 5:
                    if (Creature* pFalric = instance->GetCreature(uiFalric))
                        if (pFalric->AI())
                            pFalric->AI()->DoAction(ACTION_ENTER_COMBAT);
                    break;
                case 6:
                case 7:
                case 8:
                case 9:
                    ActivateWave();
                    break;
                case 10:
                    if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
                      if (pMarwyn->AI())
                        pMarwyn->AI()->DoAction(ACTION_ENTER_COMBAT);
                    break;
            }
        }

        // Wipe has been detected. Perform cleanup.
        void DoWipe()
        {           
            events.Reset();
            DespawnWaves();

            if (Creature* pFalric = instance->GetCreature(uiFalric))
              pFalric->DespawnOrUnsummon();

            if (Creature* pMarwyn = instance->GetCreature(uiMarwyn))
              pMarwyn->DespawnOrUnsummon();

            HandleGameObject(uiFrontDoor, true);

            bWiped = true;
        }

        //Spawns all NPCs used in waves
        void SpawnWaves(uint32 count)
        {
          for(uint8 i = 0; i < count; i++)
          {
            Creature* summon = instance->SummonCreature(WaveEntries[urand(0,4)], SpawnPos[i]);
            _waveList.push_back(summon->GetGUID());
          }                 
        }

        //Despawns all spawned NPCs used in waves
        void DespawnWaves()
        {
          while(!_waveList.empty())
          {
            if(Creature* temp = instance->GetCreature(_waveList.front()))
              temp->DespawnOrUnsummon();
            _waveList.pop_front();
          }
          while(!_activeWaveList.empty())
          {
            if(Creature* temp = instance->GetCreature(_activeWaveList.front()))
              temp->DespawnOrUnsummon();
            _activeWaveList.pop_front();
          }
        }

        void ActivateWave()
        {   
          _activeWaveList.clear();
          spawnCount = 3 + uiWaveCount / 3;

          if(uiWaveCount == 8)
            spawnCount = 6;

          for(uint8 i = 0; i < spawnCount &&  !_waveList.empty(); ++i)
          {
            std::list<uint64>::iterator itr = _waveList.begin();
            std::advance(itr, urand(0, _waveList.size() - 1)); 
            if(Creature* temp = instance->GetCreature(*itr))
            {
              temp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_NOT_SELECTABLE);
              temp->SetInCombatWithZone();
              if (Unit* target = temp->SelectNearestTarget())
                temp->AI()->AttackStart(target);
              _activeWaveList.push_back(*itr);
            }
            _waveList.erase(itr);
          }
        }

        bool CheckWipe()
        {
            Map::PlayerList const &players = instance->GetPlayers();
            for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
            {
                Player* player = itr->getSource();
                if (player->isGameMaster())
                    continue;

                if (player->isAlive())
                    return false;
            }

            return true;
        }

        void Update(uint32 diff)
        {
          if (!instance->HavePlayers())
                return;

          events.Update(diff);

          switch (events.ExecuteEvent())
          {
              case EVENT_NEXT_WAVE:
                  uiWaveCount++;
                  AddWave();
                  break;
              case EVENT_RESET_LK_EVENT:
                  //instance->SummonCreature(NPC_LICH_KING_BOSS, OutroSpawns[0]);
                  //instance->SummonCreature(NPC_JAINA_PART2, OutroSpawns[1]);
                  //HandleGameObject(uiRearDoor, true);
                  break;
              case EVENT_CHECK_WIPE:
                  if(!bWiped && CheckWipe())                    
                    DoWipe();
                  events.ScheduleEvent(EVENT_CHECK_WIPE, 2000);
                  break;
            }
        }     
    };

};

class hor_altar_trigger : public AreaTriggerScript
{
    public:
        hor_altar_trigger() : AreaTriggerScript("hor_altar_trigger") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {           
            if(player->isGameMaster())
              return true;

            if (InstanceScript* instance = player->GetInstanceScript())
              if(instance->GetData(DATA_WAVE_COUNT))
               instance->SetData(DATA_WAVE_COUNT,RESUME);
            return true;
        }
};

void AddSC_instance_halls_of_reflection()
{
    new instance_halls_of_reflection();
    new hor_altar_trigger();
}
