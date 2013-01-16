/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Boss_Headless_Horseman
SD%Complete:
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "stdafx.hpp"
#include <utility>
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellMgr.h"
#include "scarlet_monastery.h"
#include "LFGMgr.h"
#include "Player.h"
#include "Group.h"
#include "SpellInfo.h"

namespace
{
    auto &&SAY_ENTRANCE         = "It is over, your search is done. Let fate choose now, the righteous one.";
    auto &&SAY_HEAD             = " Get over here, you idiot!";
    auto &&SAY_REJOINED         = "Here's my body, fit and pure! Now, your blackened souls I'll cure!";
    auto &&SAY_DEATH            = "This end have I reached before. What new adventure lies in store?";
    auto &&SAY_KILL             = "Your body lies beaten, battered and broken. Let my curse be your own, fate has spoken.";
    auto &&SAY_CONFLAGARATION   = "Harken, cur! Tis you I spurn! Now feel... the burn!";
    auto &&SAY_PUMPKINS         = "Soldiers arise, stand and fight! Bring victory at last to this fallen knight!";
}

uint32 RandomLaugh[] = {11965, 11975, 11976};

enum Entries
{
    HH_MOUNTED                  = 23682,
    HH_DISMOUNTED               = 23800,
    HEAD                        = 23775,
    PULSING_PUMPKIN             = 23694,
    PUMPKIN_FIEND               = 23545,
    HELPER                      = 23686,
    WISP_INVIS                  = 24034
};

enum Spells
{
    SPELL_CLEAVE                = 42587,
    SPELL_CONFLAGRATION         = 42380,       //Phase 2, can't find real spell(Dim Fire?)
 // SPELL_CONFL_SPEED           = 22587,       //8% increase speed, value 22587 from SPELL_CONFLAGRATION mains that spell?
    SPELL_SUMMON_PUMPKIN        = 42394,

    SPELL_WHIRLWIND             = 43116,
    SPELL_IMMUNE                = 42556,
    SPELL_BODY_REGEN            = 42403,
    SPELL_CONFUSE               = 43105,

    SPELL_FLYING_HEAD           = 42399,       //visual flying head
    SPELL_HEAD                  = 42413,       //visual buff, "head"
    SPELL_HEAD_IS_DEAD          = 42428,       //at killing head, Phase 3
    SPELL_BODY_DEATH            = 42429,

    SPELL_PUMPKIN_AURA          = 42280,
    SPELL_PUMPKIN_AURA_GREEN    = 42294,
    SPELL_SQUASH_SOUL           = 42514,
    SPELL_SPROUTING             = 42281,
    SPELL_SPROUT_BODY           = 42285,

    //Effects
    SPELL_RHYME_BIG             = 42909,
 // SPELL_RHYME_SMALL           = 42910,
    SPELL_HEAD_SPEAKS           = 43129,
    SPELL_HEAD_LANDS            = 42400,
    SPELL_BODY_FLAME            = 42074,
    SPELL_HEAD_FLAME            = 42971,
 // SPELL_ENRAGE_VISUAL         = 42438,       // he uses this spell?
    SPELL_WISP_BLUE             = 42821,
    SPELL_WISP_FLIGHT_PORT      = 42818,
 // SPELL_WISP_INVIS            = 42823,
    SPELL_SMOKE                 = 42355,
    SPELL_DEATH                 = 42566       //not correct spell
};

enum Eents
{
    //body
    EVENT_CONFLIGURATE         = 1,
    EVENT_SUMMON_PUMPKINS      = 2,
    EVENT_WHRILWIND            = 3,
    EVENT_CLEAVE               = 4, 
    EVENT_SAY_PLAYER           = 5,
    EVENT_LAUGH                = 6,
    EVENT_HP_POLLING           = 7, //Checking if body is back on 100% hp

    EVENT_MOVEMENT             = 8,

    //head
    EVENT_HEAD_LAUGH           = 9,
    EVENT_HEAD_YELL            = 10,
};

enum Phases
{
    PHASE_INTRO                 = 1,
    PHASE_ONE                   = 2,
    PHASE_TWO                   = 3,
    PHASE_THREE                 = 4,
    PHASE_HEADLESS              = 5,

    PHASE_MASK_INTRO            = 1 << PHASE_INTRO,
    PHASE_MASK_ONE              = 1 << PHASE_ONE,
    PHASE_MASK_TWO              = 1 << PHASE_TWO,
    PHASE_MASK_THREE            = 1 << PHASE_THREE,
    PHASE_MASK_HEADLESS         = 1 << PHASE_HEADLESS,
};

enum Actions
{
   ACTION_WAYPOINT_REACHED       = 1,
   ACTION_PHASE_TWO              = 2,
   ACTION_PHASE_THREE            = 3,
   ACTION_DEATH                  = 4,
};

struct Locations
{
    float x, y, z;
};

static Locations FlightPoint[]=
{
    {1754.00f, 1346.00f, 17.50f},
    {1765.00f, 1347.00f, 19.00f},
    {1784.00f, 1346.80f, 25.40f},
    {1803.30f, 1347.60f, 33.00f},
    {1824.00f, 1350.00f, 42.60f},
    {1838.80f, 1353.20f, 49.80f},
    {1852.00f, 1357.60f, 55.70f},
    {1861.30f, 1364.00f, 59.40f},
    {1866.30f, 1374.80f, 61.70f},
    {1864.00f, 1387.30f, 63.20f},
    {1854.80f, 1399.40f, 64.10f},
    {1844.00f, 1406.90f, 64.10f},
    {1824.30f, 1411.40f, 63.30f},
    {1801.00f, 1412.30f, 60.40f},
    {1782.00f, 1410.10f, 55.50f},
    {1770.50f, 1405.20f, 50.30f},
    {1765.20f, 1400.70f, 46.60f},
    {1761.40f, 1393.40f, 41.70f},
    {1759.10f, 1386.70f, 36.60f},
    {1757.80f, 1378.20f, 29.00f},
    {1758.00f, 1367.00f, 19.51f}
};

static Locations Spawn[]=
{
    {1776.27f, 1348.74f, 19.20f},       //spawn point for pumpkin shrine mob
    {1765.28f, 1347.46f, 17.55f}     //spawn point for smoke
};

static char const* Text[]=
{
    "Horseman rise...",
    "Your time is nigh...",
    "You felt death once...",
    "Now, know demise!"
};

#define EMOTE_LAUGHS    "Headless Horseman laughs"  // needs assigned to db.

class mob_wisp_invis : public CreatureScript
{
public:
    mob_wisp_invis() : CreatureScript("mob_wisp_invis") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_wisp_invisAI (creature);
    }

    struct mob_wisp_invisAI : public ScriptedAI
    {
        mob_wisp_invisAI(Creature* creature) : ScriptedAI(creature)
        {
            Creaturetype = delay = spell = spell2 = 0;
        }

        uint32 Creaturetype;
        uint32 delay;
        uint32 spell;
        uint32 spell2;
        void Reset() 
        {
          me->SetVisible(false);
          me->SetReactState(REACT_PASSIVE);
          me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }
        void EnterCombat(Unit* /*who*/) {}

        void SetType(uint32 _type)
        {
            switch (Creaturetype = _type)
            {
                case 1:
                    spell = SPELL_PUMPKIN_AURA_GREEN;
                    break;
                case 2:
                    delay = 15000;
                    spell = SPELL_BODY_FLAME;
                    spell2 = SPELL_DEATH;
                    break;
                case 3:
                    delay = 15000;
                    spell = SPELL_SMOKE;
                    break;
            }
            if (spell)
                DoCast(me, spell);
        }


        void MoveInLineOfSight(Unit* who)
        {
            if (!who || Creaturetype != 1 || !who->isTargetableForAttack())
                return;

            if (me->IsWithinDist(who, 0.1f, false) && !who->HasAura(SPELL_SQUASH_SOUL))
                DoCast(who, SPELL_SQUASH_SOUL);
        }

        void UpdateAI(const uint32 diff)
        {
            if (delay)
            {
                if (delay <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_SMOKE);
                    if (spell2)
                        DoCast(me, spell2);
                    delay = 0;
                } else delay -= diff;
            }
        }
    };
};

class mob_head : public CreatureScript
{
public:
    mob_head() : CreatureScript("mob_head") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_headAI (creature);
    }

    struct mob_headAI : public ScriptedAI
    {
        mob_headAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
        {
        }

        void Reset()
        {
          withbody = false;
          phase = 0;
          _events.ScheduleEvent(EVENT_HEAD_LAUGH, urand(10000,12000));
        }

        void EnterCombat(Unit* /*who*/) { }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage)
        {
          if (me->HealthBelowPctDamaged(67, damage) && phase == 1)   
          {
            Disappear();
            if(Creature* body = ObjectAccessor::GetCreature(*me, _instance->GetData64(ENTRY_HORSEMAN)))
            {
              DoCast(body, SPELL_FLYING_HEAD);
              body->RemoveAurasDueToSpell(SPELL_IMMUNE);
              body->AI()->DoAction(ACTION_PHASE_TWO);
            }
          }

          if (me->HealthBelowPctDamaged(34, damage) && phase == 2)
          {
            Disappear();
            if(Creature* body = ObjectAccessor::GetCreature(*me, _instance->GetData64(ENTRY_HORSEMAN)))
            {
              DoCast(body, SPELL_FLYING_HEAD);
              body->RemoveAurasDueToSpell(SPELL_IMMUNE);
              body->AI()->DoAction(ACTION_PHASE_THREE);
            }
          }
        }

          void JustDied(Unit* /*killer*/)
          {
            if(Creature* body = ObjectAccessor::GetCreature(*me, _instance->GetData64(ENTRY_HORSEMAN)))
              body->AI()->DoAction(ACTION_DEATH);
          }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {

            if (spell->Id == SPELL_FLYING_HEAD)
            {
                me->RemoveAllAuras();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                DoCast(me, SPELL_HEAD_LANDS, true);                
                DoCast(me, SPELL_HEAD, false);
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveFleeing(caster->getVictim());
                _events.ScheduleEvent(EVENT_HEAD_YELL,1000);
                withbody = false;
                switch(phase++)
                {
                  case 1:
                    me->SetHealth(me->GetMaxHealth() * 0.66);
                    break;
                  case 2:
                    me->SetHealth(me->GetMaxHealth() * 0.33);
                    break;
                }
            }
        }

        void Disappear()
        {
          me->RemoveAllAuras();
          me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
          me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
          me->GetMotionMaster()->MoveIdle();
          withbody = true;
        }

        void UpdateAI(const uint32 diff)
        {
           if(withbody)
             return;

           if (!UpdateVictim())
            return;
          
          _events.Update(diff);

          if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

          while (uint32 eventId = _events.ExecuteEvent())
          {
             switch (eventId)
             {
             case EVENT_HEAD_LAUGH:
               DoPlaySoundToSet(me, RandomLaugh[urand(0, 2)]);
               DoCast(me,SPELL_HEAD_SPEAKS);
               _events.ScheduleEvent(EVENT_HEAD_LAUGH, urand(10000,12000));
               break;
             case EVENT_HEAD_YELL:
               me->MonsterYell(SAY_HEAD,0,0);
               break;
             }
          }
           
        }
        private:

        EventMap _events;
        InstanceScript* _instance;
        bool withbody;
        uint8 phase;
    };
};

class boss_headless_horseman : public CreatureScript
{
public:
    boss_headless_horseman() : CreatureScript("boss_headless_horseman") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_headless_horsemanAI (creature);
    }

    struct boss_headless_horsemanAI : public ScriptedAI
    {
        boss_headless_horsemanAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
        {
        }
       
        void Reset()
        {
            count = 0;
            pointID = 0;
            me->SetVisible(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT | MOVEMENTFLAG_DISABLE_GRAVITY);
            me->SetSpeed(MOVE_WALK, 5.0f, true);
            me->SetReactState(REACT_PASSIVE);
            _events.SetPhase(PHASE_INTRO);
            _events.ScheduleEvent(EVENT_SAY_PLAYER, 3000, 0, PHASE_INTRO);            
            _events.ScheduleEvent(EVENT_LAUGH, urand(25000,30000));      

            DoCast(me, SPELL_HEAD);
            me->SetInCombatWithZone();
        }

        void MovementInform(uint32 type, uint32 i)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (i)
            {
                case 0:
                    me->SetVisible(true);
                    DoCast(me, SPELL_RHYME_BIG);
                    me->MonsterYell(SAY_ENTRANCE, 0,0);
                  break;
                case 1:
                {
                    if (Creature* smoke = me->SummonCreature(HELPER, Spawn[1].x, Spawn[1].y, Spawn[1].z, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
                        CAST_AI(mob_wisp_invis::mob_wisp_invisAI, smoke->AI())->SetType(3);

                    break;
                }
                case 6:
                    if (_instance)
                        _instance->SetData(GAMEOBJECT_PUMPKIN_SHRINE, 0);   //hide gameobject
                    break;
                case 19:
                    me->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT | MOVEMENTFLAG_DISABLE_GRAVITY);
                    break;
                case 20:              
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SetReactState(REACT_AGGRESSIVE);
                    _events.SetPhase(PHASE_ONE);
                    _events.ScheduleEvent(EVENT_CLEAVE,urand(2000,5000),0,PHASE_ONE);
                    
                    if (Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                    {
                       DoStartMovement(player);
                       me->AI()->AttackStart(player);
                    }
                    if (Creature* flame = me->SummonCreature(HELPER, Spawn[0].x, Spawn[0].y, Spawn[0].z, 0, TEMPSUMMON_TIMED_DESPAWN, 17000))
                       CAST_AI(mob_wisp_invis::mob_wisp_invisAI, flame->AI())->SetType(2);
                    break;
                }
            _events.ScheduleEvent(EVENT_MOVEMENT,1,0,PHASE_INTRO);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (_instance)
                _instance->SetData(DATA_HORSEMAN_EVENT, IN_PROGRESS);
        }

        void AttackStart(Unit* who)
        {
            ScriptedAI::AttackStart(who);
        }

        void MoveInLineOfSight(Unit* who)
        {
        }

        void KilledUnit(Unit* player)
        {
            if (player->GetTypeId() == TYPEID_PLAYER)
              me->MonsterYell(SAY_KILL, 0,0);
        }

        void SaySound(uint8 textEntry, Unit* target = 0)
        {
            if (target)
                Talk(textEntry, target->GetGUID());
            else
                Talk(textEntry);
        }
       
        void SpellHitTarget(Unit* unit, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_CONFLAGRATION && unit->HasAura(SPELL_CONFLAGRATION))
              me->MonsterYell(SAY_CONFLAGARATION, 0,0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->StopMoving();
            me->MonsterYell(SAY_DEATH, 0,0);

            if (Creature* flame = DoSpawnCreature(HELPER, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 60000))
                flame->CastSpell(flame, SPELL_BODY_FLAME, false);

            if (_instance)
                _instance->SetData(DATA_HORSEMAN_EVENT, DONE);

            if (Creature* Head = Unit::GetCreature((*me), _instance->GetData64(ENTRY_HEAD)))
                Head->DisappearAndDie();

            Map::PlayerList const& players = me->GetMap()->GetPlayers();
            if (!players.isEmpty())
                sLFGMgr->FinishDungeon(players.begin()->getSource()->GetGroup()->GetGUID(), 285);
        }

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {

            if (spell->Id == SPELL_FLYING_HEAD)
            {
                me->RemoveAllAuras();
                me->SetName("Headless Horseman");
                me->SetFullHealth();
                me->MonsterYell(SAY_REJOINED, 0,0);
                DoCast(me, SPELL_HEAD);
                caster->GetMotionMaster()->Clear(false);
                caster->GetMotionMaster()->MoveFollow(me, 6, float(urand(0, 5)));
                //DoResetThreat();//not sure if need
                ThreatContainer::StorageType threatlist = caster->getThreatManager().getThreatList();
                for (ThreatContainer::StorageType::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                {
                    Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                    if (unit && unit->isAlive() && unit != caster)
                        me->AddThreat(unit, caster->getThreatManager().getThreat(unit));
                }
            }
        }

        void DamageTaken(Unit* /*done_by*/, uint32 &damage)
        {
          if(_events.GetPhaseMask() == PHASE_MASK_HEADLESS)
          {
            damage = 0;
            return;
          }
          if (damage >= me->GetHealth())
          {
            damage = me->GetHealth() - 1U;
            me->RemoveAllAuras();
            me->SetName("Headless Horseman, Unhorsed");

            Creature* head = Unit::GetCreature((*me), _instance->GetData64(ENTRY_HEAD));
            if(!head)
              head = DoSpawnCreature(HEAD, float(rand()%6), float(rand()%6), 0, 0, TEMPSUMMON_DEAD_DESPAWN, 0);

            if (head && head->isAlive())
            {
                head->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                //Head->CastSpell(Head, SPELL_HEAD_INVIS, false);
                me->InterruptNonMeleeSpells(false);
                DoCast(me, SPELL_IMMUNE, true);
                DoCast(me, SPELL_BODY_REGEN, true);
                DoCast(head, SPELL_FLYING_HEAD, true);
                DoCast(me, SPELL_CONFUSE, false);                     //test
            }
            _events.SetPhase(PHASE_HEADLESS);
            _events.ScheduleEvent(EVENT_WHRILWIND, urand(5000,8000),0, PHASE_HEADLESS);
            _events.ScheduleEvent(EVENT_HP_POLLING, 1000, 0, PHASE_HEADLESS);
          }
        }

        void DoAction(int32 const action)
        {
          switch (action)
          {
          case ACTION_PHASE_TWO:
            _events.SetPhase(PHASE_TWO);
            _events.ScheduleEvent(EVENT_CONFLIGURATE,urand(15000,20000),0,PHASE_TWO);
            break;
          case ACTION_PHASE_THREE:
            _events.SetPhase(PHASE_THREE);
            _events.ScheduleEvent(EVENT_SUMMON_PUMPKINS, urand(15000,20000),0,PHASE_THREE);
            break;
          case ACTION_DEATH:
           // DoCast(me,SPELL_BODY_DEATH);
            me->Kill(me);
            break;
          }
        }

        void UpdateAI(const uint32 diff)
        {
          if(_events.GetPhaseMask() != PHASE_MASK_INTRO)
           if(!UpdateVictim())
              return;
          
          _events.Update(diff);

          if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

          while (uint32 eventId = _events.ExecuteEvent())
          {
             switch (eventId)
             {
             case EVENT_MOVEMENT:
               if(pointID < 21)
               {
                me->GetMotionMaster()->MovePoint(pointID,FlightPoint[pointID].x, FlightPoint[pointID].y, FlightPoint[pointID].z);
                pointID++;
               }
               break;
             case EVENT_SAY_PLAYER:
             {
               if(Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                 player->ToPlayer()->Say(Text[count++], 0);
               if(count < 4)
                 _events.ScheduleEvent(EVENT_SAY_PLAYER,3000,0,PHASE_INTRO);
               else _events.ScheduleEvent(EVENT_MOVEMENT, 1, 0, PHASE_INTRO);
               break;            
             }             
             case EVENT_CLEAVE:
               DoCastVictim(SPELL_CLEAVE);
               _events.ScheduleEvent(EVENT_CLEAVE, urand(2000,5000), 0, PHASE_ONE);
               break;
             case EVENT_WHRILWIND:
               if (urand(0, 1))
                    {
                        me->RemoveAurasDueToSpell(SPELL_CONFUSE);
                        DoCast(me, SPELL_WHIRLWIND, true);
                        DoCast(me, SPELL_CONFUSE);
                    }
                    else
                        me->RemoveAurasDueToSpell(SPELL_WHIRLWIND);
               _events.ScheduleEvent(EVENT_WHRILWIND, urand(5000,8000),0, PHASE_HEADLESS);
               break;
             case EVENT_HP_POLLING:
               if (me->IsFullHealth())
               {
                 if(Creature* Head = Unit::GetCreature((*me), _instance->GetData64(ENTRY_HEAD)))
                 {

                 }
               }
               _events.ScheduleEvent(EVENT_HP_POLLING, 1000, 0, PHASE_HEADLESS);
               break;
             case EVENT_CONFLIGURATE:
               if(Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                 DoCast(player, SPELL_CONFLAGRATION, false);
               _events.ScheduleEvent(EVENT_CONFLIGURATE,urand(8000,10000),0,PHASE_TWO);
               break;
             case EVENT_SUMMON_PUMPKINS:
               me->InterruptNonMeleeSpells(false);
               DoCast(me, SPELL_SUMMON_PUMPKIN);
               me->MonsterYell(SAY_PUMPKINS, 0,0);
               _events.ScheduleEvent(EVENT_SUMMON_PUMPKINS, urand(12000,15000),0,PHASE_THREE);
               break;
             case EVENT_LAUGH:
               me->MonsterTextEmote(EMOTE_LAUGHS, 0);
               DoPlaySoundToSet(me, RandomLaugh[rand()%3]);
               _events.ScheduleEvent(EVENT_LAUGH, urand(10000,12000));
               break;
             }
          }

          DoMeleeAttackIfReady(); 
        }                    

        private:
          EventMap _events;
          InstanceScript* _instance;
          int32 pointID;
          uint32 count;
    };
};

class mob_pulsing_pumpkin : public CreatureScript
{
public:
    mob_pulsing_pumpkin() : CreatureScript("mob_pulsing_pumpkin") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_pulsing_pumpkinAI (creature);
    }

    struct mob_pulsing_pumpkinAI : public ScriptedAI
    {
        mob_pulsing_pumpkinAI(Creature* creature) : ScriptedAI(creature) {}

        bool sprouted;
        uint64 debuffGUID;

        void Reset()
        {
            float x, y, z;
            me->GetPosition(x, y, z);   //this visual aura some under ground
            me->SetPosition(x, y, z + 0.35f, 0.0f);
            Despawn();
            Creature* debuff = DoSpawnCreature(HELPER, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 14500);
            if (debuff)
            {
                debuff->SetDisplayId(me->GetDisplayId());
                debuff->CastSpell(debuff, SPELL_PUMPKIN_AURA_GREEN, false);
                CAST_AI(mob_wisp_invis::mob_wisp_invisAI, debuff->AI())->SetType(1);
                debuffGUID = debuff->GetGUID();                
            }
            sprouted = false;
            DoCast(me, SPELL_PUMPKIN_AURA, true);
            DoCast(me, SPELL_SPROUTING);
            
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
        }

        void EnterCombat(Unit* /*who*/) {}

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_SPROUTING)
            {
                me->SetInCombatWithZone();
                sprouted = true;
                me->RemoveAllAuras();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_STUNNED);
                DoCast(me, SPELL_SPROUT_BODY, true);
                me->UpdateEntry(PUMPKIN_FIEND);
                me->setFaction(14);
                if (Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true))
                {
                  DoStartMovement(player);
                  me->AI()->AttackStart(player);
                }
            }
        }

        void Despawn()
        {
            if (!debuffGUID)
                return;

            Unit* debuff = Unit::GetUnit(*me, debuffGUID);
            if (debuff)
            {
                debuff->SetVisible(false);
                debuffGUID = 0;
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            if (!sprouted)
                Despawn();
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!who || !me->IsValidAttackTarget(who) || me->getVictim())
                return;

            me->AddThreat(who, 0.0f);
            if (sprouted)
                DoStartMovement(who);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (sprouted && UpdateVictim())
                DoMeleeAttackIfReady();
        }

        
    };
};

class go_loosely_turned_soil : public GameObjectScript
{
public:
    go_loosely_turned_soil() : GameObjectScript("go_loosely_turned_soil") { }

    bool OnGossipHello(Player* player, GameObject* soil)
    {
        return GameObjectScript::OnGossipHello(std::move(player), std::move(soil));
    }
};


void AddSC_boss_headless_horseman()
{
    new boss_headless_horseman();
    new mob_head();
    new mob_pulsing_pumpkin();
    new mob_wisp_invis();
    new go_loosely_turned_soil();
}
