/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
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

#include "ScriptMgr.h"
#include "Battlefield.h"
#include "Containers.h"
#include "Battlefield/BattlefieldTB.h"
#include "DB2Stores.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptSystem.h"
#include "SpellScript.h"

enum TBSpiritGuide
{
    GOSSIP_OPTION_ID_SLAGWORKS = 0,
    GOSSIP_OPTION_ID_IRONCLAD_GARRISON = 1,
    GOSSIP_OPTION_ID_WARDENS_VIGIL = 2,
    GOSSIP_OPTION_ID_EAST_SPIRE = 3,
    GOSSIP_OPTION_ID_WEST_SPIRE = 4,
    GOSSIP_OPTION_ID_SOUTH_SPIRE = 5,
};

struct npc_tb_spirit_guide : public ScriptedAI
{
    npc_tb_spirit_guide(Creature* creature) : ScriptedAI(creature) { }

    bool OnGossipSelect(Player* player, uint32 /*menuId*/, uint32 gossipListId) override
    {
        player->PlayerTalkClass->SendCloseGossip();

        uint32 areaId = 0;
        switch (gossipListId)
        {
            case GOSSIP_OPTION_ID_SLAGWORKS:
                areaId = TB_GY_SLAGWORKS;
                break;
            case GOSSIP_OPTION_ID_IRONCLAD_GARRISON:
                areaId = TB_GY_IRONCLAD_GARRISON;
                break;
            case GOSSIP_OPTION_ID_WARDENS_VIGIL:
                areaId = TB_GY_WARDENS_VIGIL;
                break;
            case GOSSIP_OPTION_ID_EAST_SPIRE:
                areaId = TB_GY_EAST_SPIRE;
                break;
            case GOSSIP_OPTION_ID_WEST_SPIRE:
                areaId = TB_GY_WEST_SPIRE;
                break;
            case GOSSIP_OPTION_ID_SOUTH_SPIRE:
                areaId = TB_GY_SOUTH_SPIRE;
                break;
            default:
                return true;
        }

        if (WorldSafeLocsEntry const* safeLoc = sObjectMgr->GetWorldSafeLoc(areaId))
            player->TeleportTo(safeLoc->Loc);

        return false;
    }
};

// 85123 - Siege Cannon - selects random target
class spell_siege_cannon : public SpellScriptLoader
{
public:
    spell_siege_cannon() : SpellScriptLoader("spell_siege_cannon") { }

    class spell_siege_cannon_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_siege_cannon_SpellScript);

        void SelectRandomTarget(std::list<WorldObject*>& targets)
        {
            if (targets.empty())
                return;

            WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
            targets.clear();
            targets.push_back(target);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_siege_cannon_SpellScript::SelectRandomTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_siege_cannon_SpellScript();
    }
};

void AddSC_tol_barad()
{
    RegisterCreatureAI(npc_tb_spirit_guide);
    new spell_siege_cannon();
}
