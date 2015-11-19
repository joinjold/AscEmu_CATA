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

void World::InitHunterSpells()
{
    SpellEntry * sp = NULL;
    if (sp = dbcSpell.LookupEntryForced(6991))
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_GAMEOBJECT_ITEM;

    if (sp = dbcSpell.LookupEntryForced(61847))
    {
        sp->eff[2].Effect = SPELL_EFFECT_APPLY_AURA;
        sp->eff[2].EffectApplyAuraName = SPELL_AURA_MOD_DODGE_PERCENT;
    }

    if (sp = dbcSpell.LookupEntryForced(19552))
        sp->BGR_one_buff_on_target = 0;
    if (sp = dbcSpell.LookupEntryForced(19553))
        sp->BGR_one_buff_on_target = 0;
    if (sp = dbcSpell.LookupEntryForced(19554))
        sp->BGR_one_buff_on_target = 0;
    if (sp = dbcSpell.LookupEntryForced(19555))
        sp->BGR_one_buff_on_target = 0;
    if (sp = dbcSpell.LookupEntryForced(19556))
        sp->BGR_one_buff_on_target = 0;

    if (sp = dbcSpell.LookupEntryForced(53252))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_PET;
        sp->eff[0].EffectTriggerSpell = 53398;
        sp->procChance = 50;
        sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
    }

    if (sp = dbcSpell.LookupEntryForced(53253))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_ON_PET;
        sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL;
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_PET;
        sp->eff[0].EffectTriggerSpell = 53398;
        sp->procChance = 100;
        sp->procFlags = PROC_ON_CRIT_ATTACK | PROC_ON_SPELL_CRIT_HIT | PROC_TARGET_SELF;
    }

    if (sp = dbcSpell.LookupEntryForced(53398))
    {
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_PET_MASTER;
    }

    if (sp = dbcSpell.LookupEntryForced(34456))
    {
        sp->eff[0].EffectBasePoints = 3;
    }

    if (sp = dbcSpell.LookupEntryForced(53257))
    {
        sp->eff[0].EffectImplicitTargetA = EFF_TARGET_PET;
    }

    if (sp = dbcSpell.LookupEntryForced(56314))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
        sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 57447;
        sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[1].EffectTriggerSpell = 57475;
    }

    if (sp = dbcSpell.LookupEntryForced(56315))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
        sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 57452;
        sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[1].EffectTriggerSpell = 57482;
    }

    if (sp = dbcSpell.LookupEntryForced(56316))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
        sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 57453;
        sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[1].EffectTriggerSpell = 57483;
    }

    if (sp = dbcSpell.LookupEntryForced(56317))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
        sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 57457;
        sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[1].EffectTriggerSpell = 57484;
    }

    if (sp = dbcSpell.LookupEntryForced(56318))
    {
        sp->c_is_flags |= SPELL_FLAG_IS_CASTED_ON_PET_SUMMON_PET_OWNER | SPELL_FLAG_IS_EXPIREING_WITH_PET;
        sp->eff[0].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[0].EffectTriggerSpell = 57458;
        sp->eff[1].Effect = SPELL_EFFECT_TRIGGER_SPELL;
        sp->eff[1].EffectTriggerSpell = 57485;
    }

    if (sp = dbcSpell.LookupEntryForced(53220))
    {
        sp->NameHash += 1;
    }

    if (sp = dbcSpell.LookupEntryForced(34720))
        if (sp = dbcSpell.LookupEntryForced(781))
            sp->eff[0].Effect = SPELL_EFFECT_DUMMY;
}
