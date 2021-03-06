/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"


void World::InitPriestSpells()
{
    SpellEntry * sp = NULL;
    if (sp = dbcSpell.LookupEntryForced(41635))
        sp->procFlags = PROC_ON_PHYSICAL_ATTACK_VICTIM | PROC_ON_MELEE_ATTACK_VICTIM | PROC_ON_RANGED_CRIT_ATTACK_VICTIM | PROC_ON_RANGED_ATTACK_VICTIM;

    if (sp = dbcSpell.LookupEntryForced(33174))
    {
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
        sp->eff[1].EffectApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
    }
    if (sp = dbcSpell.LookupEntryForced(33182))
    {
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
        sp->eff[1].EffectApplyAuraName = SPELL_AURA_ADD_PCT_MODIFIER;
    }

    // still figuring :X - remove if you want.
    //static uint32 spellids[3] = {47535,47536,47537};
    //for (uint8 i = 0;i<3;i++)
    //{
    //      if (sp = dbcSpell.LookupEntryForced(spellids[i]))
    //      {
    //              sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;
    //              sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;
    //              sp->eff[2].Effect = 0;
    //              sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
    //              sp->eff[1].EffectApplyAuraName = SPELL_AURA_DUMMY;
    //              sp->eff[0].EffectTriggerSpell = 47755;
    //              sp->procFlags = PROC_ON_CAST_SPELL;
    //              sp->procChance = 100;
    //              sp->proc_interval = 1500;
    //      }
    //}
    if (sp = dbcSpell.LookupEntryForced(47755))
        //sp->logsId = 47755;

        if (sp = dbcSpell.LookupEntryForced(47509))
        {
            sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
            sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
            sp->eff[0].EffectTriggerSpell = 47753;
        }

    if (sp = dbcSpell.LookupEntryForced(47511))
    {
        sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 47753;
    }

    if (sp = dbcSpell.LookupEntryForced(47515))
    {
        sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 47753;
    }

    if (sp = dbcSpell.LookupEntryForced(47516))
    {
        sp->procFlags = PROC_ON_CAST_SPELL;
    }

    if (sp = dbcSpell.LookupEntryForced(47517))
    {
        sp->procFlags = PROC_ON_CAST_SPELL;
    }

    if (sp = dbcSpell.LookupEntryForced(47930))
    {
        sp->eff[1].EffectApplyAuraName = SPELL_AURA_MOD_HEALING_DONE_PERCENT;
        sp->eff[0].EffectBasePoints = -3;
        sp->eff[1].EffectBasePoints = 3;
        sp->eff[1].EffectMiscValue = 127;
    }

    if (sp = dbcSpell.LookupEntryForced(33150))
    {
        sp->procCharges = 0;
    }

    if (sp = dbcSpell.LookupEntryForced(33154))
    {
        sp->procCharges = 0;
    }

    if (sp = dbcSpell.LookupEntryForced(33151))
    {
        sp->procCharges = 0;
    }

    if (sp = dbcSpell.LookupEntryForced(34753))
    {
        sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
    }

    if (sp = dbcSpell.LookupEntryForced(34859))
    {
        sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
    }

    if (sp = dbcSpell.LookupEntryForced(34860))
    {
        sp->procFlags = PROC_ON_SPELL_CRIT_HIT;
    }

    if (sp = dbcSpell.LookupEntryForced(47788))
    {
        sp->eff[2].Effect = SPELL_EFFECT_APPLY_AURA;
        sp->eff[2].EffectApplyAuraName = SPELL_AURA_DUMMY;
    }

    if (sp = dbcSpell.LookupEntryForced(724))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181102;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(27870))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181105;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(27871))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181106;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(28275))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181165;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(48086))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181165;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(48087))
    {
        sp->eff[0].Effect = SPELL_EFFECT_SUMMON_OBJECT;
        sp->eff[0].EffectMiscValue = 181165;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_LOCATION_NEAR_CASTER;
        sp->eff[0].EffectBasePoints = 0;
    }

    if (sp = dbcSpell.LookupEntryForced(15258))
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_MOD_DAMAGE_PERCENT_DONE;

    if (sp = dbcSpell.LookupEntryForced(64044))
    {
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_MOD_FEAR;
        auto spell_entry = dbcSpell.LookupEntryForced(64058);
        if (spell_entry != nullptr)
            spell_entry->NameHash += 1;
        //dbcSpell.LookupEntryForced(64058)->NotRmoveTrigger = true;
    }

    if (sp = dbcSpell.LookupEntryForced(47948))
        sp->eff[0].Effect = SPELL_EFFECT_DUMMY;

    if (sp = dbcSpell.LookupEntryForced(15407))
        sp->eff[0].EffectBasePoints = 14;

    if (sp = dbcSpell.LookupEntryForced(48156))
        sp->eff[0].EffectBasePoints = 150;
}
