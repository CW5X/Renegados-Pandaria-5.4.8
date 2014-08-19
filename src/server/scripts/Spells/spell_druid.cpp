/*
 * Copyright (C) 2011-2014 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2014 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"

enum DruidSpells
{
    SPELL_DRUID_TREANT_FORM = 114282
};

class spell_druid_glyph_of_the_treant : public SpellScriptLoader
{
public:
    spell_druid_glyph_of_the_treant() : SpellScriptLoader("spell_druid_glyph_of_the_treant") { }

    class spell_druid_glyph_of_the_treant_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_druid_glyph_of_the_treant_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_DRUID_TREANT_FORM))
                return false;
            return true;
        }

        void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->AddTemporarySpell(SPELL_DRUID_TREANT_FORM, true);
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetTarget()->ToPlayer())
                player->RemoveTemporarySpell(SPELL_DRUID_TREANT_FORM, true);
        }

        void Register() override
        {
            OnEffectApply += AuraEffectApplyFn(spell_druid_glyph_of_the_treant_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            OnEffectRemove += AuraEffectApplyFn(spell_druid_glyph_of_the_treant_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_druid_glyph_of_the_treant_AuraScript();
    }
};

void AddSC_druid_spell_scripts()
{
    new spell_druid_glyph_of_the_treant();
}
