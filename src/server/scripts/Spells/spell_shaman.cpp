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
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "Player.h"
#include "ScriptMgr.h"
#include "GridNotifiers.h"
#include "Unit.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SPELL_SHAMAN_FLAME_SHOCK                = 8050,
    SPELL_SHAMAN_FROZEN_POWER_TALENT        = 63374,
    SPELL_SHAMAN_FREEZE                     = 63685,
    SPELL_SHAMAN_EARTHQUAKE                 = 61882,
    SPELL_SHAMAN_EARTHQUAKE_DAMAGE          = 77478,
    SPELL_SHAMAN_EARTHQUAKE_STUN            = 77505,
};

enum ShamanSpellIcons
{
    SHAMAN_ICON_ID_SOOTHING_RAIN                = 2011,
    SHAMAN_ICON_ID_SHAMAN_LAVA_FLOW             = 3087
};

class spell_sha_lava_burst : public SpellScriptLoader
{
public:
    spell_sha_lava_burst() : SpellScriptLoader("spell_sha_lava_burst") { }

    class spell_sha_lava_burst_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_lava_burst_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FLAME_SHOCK))
                return false;
            return true;
        }

        void RecalculateDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetHitUnit())
                return;

            if (AuraEffect *aurEff = GetHitUnit()->GetAuraEffect(SPELL_SHAMAN_FLAME_SHOCK, EFFECT_2, GetCaster()->GetGUID()))
            {
                int32 damage = GetHitDamage();
                AddPct(damage, aurEff->GetAmount());
                SetHitDamage(damage);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_lava_burst_SpellScript::RecalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_sha_lava_burst_SpellScript();
    }
};

class spell_sha_frost_shock : public SpellScriptLoader
{
public:
    spell_sha_frost_shock() : SpellScriptLoader("spell_sha_frost_shock") { }

    class spell_sha_frost_shock_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_frost_shock_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FROZEN_POWER_TALENT) || !sSpellMgr->GetSpellInfo(SPELL_SHAMAN_FREEZE))
                return false;
            return true;
        }

        void ApplyFreeze()
        {
            if (!GetHitUnit())
                return;

            if (GetCaster()->HasAura(SPELL_SHAMAN_FROZEN_POWER_TALENT))
                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHAMAN_FREEZE, true);
        }

        void Register() override
        {
            AfterHit += SpellHitFn(spell_sha_frost_shock_SpellScript::ApplyFreeze);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_sha_frost_shock_SpellScript();
    }
};

class spell_sha_earthquake : public SpellScriptLoader
{
public:
    spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

    class spell_sha_earthquake_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_sha_earthquake_AuraScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHQUAKE_DAMAGE))
                return false;
            return true;
        }

        void OnPeriodic(AuraEffect const* aurEff)
        {
            if (!GetCaster())
                return;

            if (DynamicObject* dynamicObject = GetCaster()->GetDynObject(SPELL_SHAMAN_EARTHQUAKE))
            {
                float x, y, z;
                dynamicObject->GetPosition(x, y, z);
                GetCaster()->CastSpell(x, y, z, SPELL_SHAMAN_EARTHQUAKE_DAMAGE, true);
            }
        }

        void Register() override
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnPeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new spell_sha_earthquake_AuraScript();
    }
};

class spell_sha_earthquake_damage : public SpellScriptLoader
{
public:
    spell_sha_earthquake_damage() : SpellScriptLoader("spell_sha_earthquake_damage") { }

    class spell_sha_earthquake_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_earthquake_damage_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHAMAN_EARTHQUAKE_STUN))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            if (!GetHitUnit() || GetHitUnit()->HasAura(SPELL_SHAMAN_EARTHQUAKE_STUN))
                return;

            if (roll_chance_i(GetEffectValue()))
                GetCaster()->AddAura(SPELL_SHAMAN_EARTHQUAKE_STUN, GetHitUnit());
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_sha_earthquake_damage_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_sha_earthquake_damage_SpellScript();
    }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_lava_burst();
    new spell_sha_frost_shock();
    new spell_sha_earthquake();
    new spell_sha_earthquake_damage();
}
