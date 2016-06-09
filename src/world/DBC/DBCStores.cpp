/**
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
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
 */

#include "StdAfx.h"
#include "DBCGlobals.hpp"

typedef std::map<WMOAreaTableTripple, WMOAreaTableEntry const*> WMOAreaInfoByTripple;

#ifdef ENABLE_ACHIEVEMENTS
SERVER_DECL DBCStorage<AchievementEntry> dbcAchievementStore;
SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchievementCriteriaStore;
SERVER_DECL DBCStorage<AchievementCategoryEntry> dbcAchievementCategoryStore;
#endif
SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
SERVER_DECL DBC::DBCStorage<DBC::Structures::AreaTableEntry> sAreaStore(DBC::Structures::area_table_entry_format);
static WMOAreaInfoByTripple sWMOAreaInfoByTripple;
SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyleStore;
SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
SERVER_DECL DBCStorage<CharTitlesEntry> dbcCharTitlesEntry;
SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
SERVER_DECL DBCStorage<CurrencyTypesEntry> dbcCurrencyTypesStore;
SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
SERVER_DECL DBCStorage<GlyphSlotEntry> dbcGlyphSlot;
SERVER_DECL DBCStorage<ItemLimitCategoryEntry> dbcItemLimitCategory;
SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
SERVER_DECL DBCStorage<Lock> dbcLock;
SERVER_DECL DBCStorage<MapEntry> dbcMap;
SERVER_DECL DBCStorage<HolidaysEntry> dbcHolidayEntry;
SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;
SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
SERVER_DECL DBCStorage<SpellDifficultyEntry> dbcSpellDifficultyEntry;
SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
SERVER_DECL DBCStorage<WorldMapOverlay> dbcWorldMapOverlayStore;

SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
//SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
//SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
SERVER_DECL DBCStorage<QuestSortEntry> sQuestSortStore;
SERVER_DECL DBCStorage<QuestXP> dbcQuestXP;
SERVER_DECL DBCStorage<MailTemplateEntry> dbcMailTemplateEntry;
SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOAreaTable;
SERVER_DECL DBCStorage< SummonPropertiesEntry > dbcSummonProperties;
SERVER_DECL DBCStorage< NameGenEntry > dbcNameGen;
SERVER_DECL DBC::DBCStorage<DBC::Structures::LFGDungeonEntry> sLFGDungeonStore(DBC::Structures::LFGDungeonEntryformat);
SERVER_DECL DBCStorage< VehicleEntry > dbcVehicle;
SERVER_DECL DBCStorage< VehicleSeatEntry > dbcVehicleSeat;

SERVER_DECL DBCStorage<SpellEffectEntry>			dbcSpellEffect;
SERVER_DECL DBCStorage<SpellShapeshiftFormEntry>	dbcSpellShapeshiftForm;
SERVER_DECL DBCStorage<SpellAuraOptionsEntry>		dbcSpellAuraOptions;
SERVER_DECL DBCStorage<SpellAuraRestrictionsEntry>	dbcSpellAuraRestrictions;
SERVER_DECL DBCStorage<SpellCastingRequirementsEntry>	dbcSpellCastingRequirements;
SERVER_DECL DBCStorage<SpellCategoriesEntry>		dbcSpellCategories;
SERVER_DECL DBCStorage<SpellClassOptionsEntry>		dbcSpellClassOptions;
SERVER_DECL DBCStorage<SpellCooldownsEntry>			dbcSpellCooldowns;
SERVER_DECL DBCStorage<SpellEquippedItemsEntry>		dbcSpellEquippedItems;
SERVER_DECL DBCStorage<SpellInterruptsEntry>		dbcSpellInterrupts;
SERVER_DECL DBCStorage<SpellLevelsEntry>			dbcSpellLevels;
SERVER_DECL DBCStorage<SpellPowerEntry>				dbcSpellPower;
SERVER_DECL DBCStorage<SpellReagentsEntry>			dbcSpellReagents;
SERVER_DECL DBCStorage<SpellScalingEntry>			dbcSpellScaling;
SERVER_DECL DBCStorage<SpellShapeshiftEntry>		dbcSpellShapeshift;
SERVER_DECL DBCStorage<SpellTargetRestrictionsEntry>	dbcSpellTargetRestrictions;
SERVER_DECL DBCStorage<SpellTotemsEntry>			dbcSpellTotems;
SERVER_DECL DBCStorage<gtClassLevelFloat>			dbcGTSpellScale;
SERVER_DECL DBCStorage<FactionEntry>                sFactionStore;

const char* WorldMapOverlayStoreFormat = "nxiiiixxxxxxxxx";
const char* BarberShopStyleEntryFormat = "nxxxxxxi";
const char* ItemSetFormat = "usuuuuuuuuxxxxxxxxxuuuuuuuuuuuuuuuuuu";
const char* LockFormat = "uuuuuuxxxuuuuuxxxuuuuuxxxxxxxxxxx";
const char* EmoteEntryFormat = "uxuuuuxuxuxxxxxxxxx";
const char* skilllinespellFormat = "iiiiiiiiiiiiix";
const char* EnchantEntrYFormat = "uxuuuuuuuuuuuusuuuuuuuu";
const char* GemPropertyEntryFormat = "uuuuux";
const char* GlyphPropertyEntryFormat = "niii";
const char* GlyphSlotEntryFormat = "nii";
const char* skilllineentrYFormat = "uusxxxx";
const char* VehicleEntryfmt = "uuffffuuuuuuuufffffffffffffffssssfufuxxx";
const char* VehicleSeatEntryfmt = "uuiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiixxxxxxxxxxxxxxxxxxxx";


const char* CharTitlesEntryfmt = "uxsxux";

const char* CurrencyTypesEntryFormat = "xnxuxxxxxxx";

#ifdef ENABLE_ACHIEVEMENTS
const char* AchievementStoreFormat = "niiussiiuiusuu";
const char* AchievementCriteriaStoreFormat = "niiiiliiisixiiixxxxxxxx";
#endif


const char* spellentryFormat = "uuuuuuuuuuuxuuuufuuuussssuuuuufuuuuuuuuuuuuuuuuu";

const char* talententryFormat = "uuuuuuuuuxuxxxxuxxx";
const char* talenttabentryFormat = "uxxuuuxxxuu";
const char* spellcasttimeFormat = "uuxx";
const char* spellradiusFormat = "ufxf";
const char* spellrangeFormat = "uffffxxx";
const char* SpellRuneCostFormat = "uuuuu";
const char* spelldurationFormat = "uuuu";
const char* randompropsFormat = "uxuuuxxx";
const char* areagroupFormat = "uuuuuuuu";
const char* areatableFormat = "uuuuuxxxxxusuxxxxxxxxxxxxx";
const char* factiontemplatedbcFormat = "uuuuuuuuuuuuuu";
const char* auctionhousedbcFormat = "uuuux";
const char* factiondbcFormat = "uiuuuuuuuuiiiiuuuuuxxxxsxx";

const char* dbctaxinodeFormat = "uufffxuuuff";
const char* dbctaxipathFormat = "uuuu";
const char* dbctaxipathnodeFormat = "uuuufffuuxx";
const char* creaturespelldataFormat = "uuuuuuuuu";
const char* charraceFormat = "nxixiixixxxxixsxxxxxixxx";
const char* charclassFormat = "uuxsxxxxxxxxxx";
const char* creaturefamilyFormat = "ufufuuuuuusx";
const char* mapentryFormat = "nxixxxsixxixiffxixxx";

const char* HolidayEntryFormat = "uiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxsiix";
const char* itemrandomsuffixformat = "uxxuuuuuuuuuu";
const char* chatchannelformat = "uuxsx";
const char* durabilityqualityFormat = "uf";
const char* durabilitycostsFormat = "uuuuuuuuuuuuuuuuuuuuuuuuuuuuuu";
const char* bankslotpriceformat = "uu";
const char* barbershopstyleFormat = "nxxxxxxi";
const char* gtfloatformat = "f";
const char* gtClassfloatformat = "uf";
const char* areatriggerformat = "uufffxxxxxxxx";
const char* scalingstatdistributionformat = "uiiiiiiiiiiuuuuuuuuuuxu";
const char* scalingstatvaluesformat = "iniiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char* itemlimitcategoryformat = "uxux";
const char* spellshapeshiftformformat = "uxxiixiiixxiiiiiiiixx";
const char* questxpformat = "uxuuuuuuuux";
const char* mailTemplateEntryFormat = "uss";
const char* wmoareaformat = "uiiixxxxxuuxxxx";
const char* summonpropertiesformat = "uuuuuu";
const char* namegenentryformat = "usuu";

const char* SpellEffectEntryfmt = "xufuuiffiiiiiifiifiiiiiiiix";
const char* SpellShapeshiftFormfmt = "uxxiixiiixxiiiiiiiixx";
const char* SpellShapeshiftEntryfmt = "uixixx";
const char* SpellAuraOptionsEntryfmt = "uuuuu";
const char* SpellAuraRestrictionsEntryfmt = "uiiiiiiii";
const char* SpellCastingRequirementsEntryfmt = "uixxixi";
const char* SpellCategoriesEntryfmt = "uuuuuuu";
const char* SpellClassOptionsEntryfmt = "uxiiiix";

const char* SpellCooldownsEntryfmt = "uiii";
const char* SpellEquippedItemsEntryfmt = "uiii";
const char* SpellInterruptsEntryfmt = "uixixi";
const char* SpellLevelsEntryfmt = "uiii";

const char* SpellPowerEntryfmt = "uiiiixxx";

const char* SpellReagentsEntryfmt = "uiiiiiiiiiiiiiiii";
const char* SpellScalingEntryfmt = "uiiiiffffffffffi";
const char* SpellTargetRestrictionsEntryfmt = "uixiii";
const char* SpellTotemsEntryfmt = "uiiii";
const char QuestSortEntryfmt[] = "nx";
const char FactionEntryfmt[] = "niiiiiiiiiiiiiiiiiiffixsxx";

template<class T>
bool loader_stub(const char* filename, const char* format, bool ind, T& l, bool loadstrs)
{
    Log.Notice("DBC", "Loading %s.", filename);
    return l.Load(filename, format, ind, loadstrs);
}

#define LOAD_DBC(filename, format, ind, stor, strings) if (!loader_stub(filename, format, ind, stor, strings)) { return false; }

bool LoadDBCs()
{
    uint32 available_dbc_locales = 0xFFFFFFFF;
    DBC::StoreProblemList bad_dbc_files;
    std::string dbc_path = "./DBC/";

    LOAD_DBC("DBC/WorldMapOverlay.dbc", WorldMapOverlayStoreFormat, true, dbcWorldMapOverlayStore, true);
#ifdef ENABLE_ACHIEVEMENTS
    LOAD_DBC("DBC/Achievement_Criteria.dbc", AchievementCriteriaStoreFormat, true, dbcAchievementCriteriaStore, true);
    LOAD_DBC("DBC/Achievement.dbc", AchievementStoreFormat, true, dbcAchievementStore, true);
#endif
    LOAD_DBC("DBC/CharTitles.dbc", CharTitlesEntryfmt, true, dbcCharTitlesEntry, true);
    LOAD_DBC("DBC/CurrencyTypes.dbc", CurrencyTypesEntryFormat, true, dbcCurrencyTypesStore, true);
    LOAD_DBC("DBC/BarberShopStyle.dbc", BarberShopStyleEntryFormat, true, dbcBarberShopStyleStore, true);
    LOAD_DBC("DBC/ItemSet.dbc", ItemSetFormat, true, dbcItemSet, true);
    LOAD_DBC("DBC/Lock.dbc", LockFormat, true, dbcLock, false);
    LOAD_DBC("DBC/EmotesText.dbc", EmoteEntryFormat, true, dbcEmoteEntry, false);
    LOAD_DBC("DBC/SkillLineAbility.dbc", skilllinespellFormat, false, dbcSkillLineSpell, false);
    LOAD_DBC("DBC/SpellItemEnchantment.dbc", EnchantEntrYFormat, true, dbcEnchant, true);
    LOAD_DBC("DBC/GemProperties.dbc", GemPropertyEntryFormat, true, dbcGemProperty, false);
    LOAD_DBC("DBC/GlyphProperties.dbc", GlyphPropertyEntryFormat, true, dbcGlyphProperty, true);
    LOAD_DBC("DBC/GlyphSlot.dbc", GlyphSlotEntryFormat, true, dbcGlyphSlot, true);
    LOAD_DBC("DBC/SkillLine.dbc", skilllineentrYFormat, true, dbcSkillLine, true);
    LOAD_DBC("DBC/Spell.dbc", spellentryFormat, true, dbcSpell, true);
    LOAD_DBC("DBC/Talent.dbc", talententryFormat, true, dbcTalent, false);
    LOAD_DBC("DBC/TalentTab.dbc", talenttabentryFormat, true, dbcTalentTab, false);
    LOAD_DBC("DBC/SpellCastTimes.dbc", spellcasttimeFormat, true, dbcSpellCastTime, false);
    LOAD_DBC("DBC/SpellRadius.dbc", spellradiusFormat, true, dbcSpellRadius, false);
    LOAD_DBC("DBC/SpellRange.dbc", spellrangeFormat, true, dbcSpellRange, false);
    LOAD_DBC("DBC/SpellRuneCost.dbc", SpellRuneCostFormat, true, dbcSpellRuneCost, false);
    LOAD_DBC("DBC/SpellDuration.dbc", spelldurationFormat, true, dbcSpellDuration, false);
    LOAD_DBC("DBC/SpellShapeshiftForm.dbc", spellshapeshiftformformat, false, dbcSpellShapeshiftForm, false);
    LOAD_DBC("DBC/ItemRandomProperties.dbc", randompropsFormat, true, dbcRandomProps, false);
    LOAD_DBC("DBC/AreaGroup.dbc", areagroupFormat, true, dbcAreaGroup, true);

    DBC::LoadDBC(available_dbc_locales, bad_dbc_files, sAreaStore, dbc_path, "AreaTable.dbc");

    LOAD_DBC("DBC/FactionTemplate.dbc", factiontemplatedbcFormat, true, dbcFactionTemplate, false);
    LOAD_DBC("DBC/Faction.dbc", factiondbcFormat, true, dbcFaction, true);
    LOAD_DBC("DBC/TaxiNodes.dbc", dbctaxinodeFormat, false, dbcTaxiNode, false);
    LOAD_DBC("DBC/TaxiPath.dbc", dbctaxipathFormat, false, dbcTaxiPath, false);
    LOAD_DBC("DBC/TaxiPathNode.dbc", dbctaxipathnodeFormat, false, dbcTaxiPathNode, false);
    LOAD_DBC("DBC/CreatureSpellData.dbc", creaturespelldataFormat, true, dbcCreatureSpellData, false);
    LOAD_DBC("DBC/CreatureFamily.dbc", creaturefamilyFormat, true, dbcCreatureFamily, true);
    LOAD_DBC("DBC/ChrRaces.dbc", charraceFormat, true, dbcCharRace, true);
    LOAD_DBC("DBC/ChrClasses.dbc", charclassFormat, true, dbcCharClass, true);
    LOAD_DBC("DBC/Map.dbc", mapentryFormat, true, dbcMap, true);
    LOAD_DBC("DBC/AuctionHouse.dbc", auctionhousedbcFormat, true, dbcAuctionHouse, false);
    LOAD_DBC("DBC/ItemRandomSuffix.dbc", itemrandomsuffixformat, true, dbcItemRandomSuffix, false);
    LOAD_DBC("DBC/gtCombatRatings.dbc", gtClassfloatformat, false, dbcCombatRating, false);
    LOAD_DBC("DBC/ChatChannels.dbc", chatchannelformat, true, dbcChatChannels, true);
    LOAD_DBC("DBC/DurabilityQuality.dbc", durabilityqualityFormat, true, dbcDurabilityQuality, false);
    LOAD_DBC("DBC/DurabilityCosts.dbc", durabilitycostsFormat, true, dbcDurabilityCosts, false);
    LOAD_DBC("DBC/BankBagSlotPrices.dbc", bankslotpriceformat, true, dbcBankSlotPrices, false);
    LOAD_DBC("DBC/gtBarberShopCostBase.dbc", gtClassfloatformat, false, dbcBarberShopPrices, false);
    LOAD_DBC("DBC/gtChanceToMeleeCrit.dbc", gtClassfloatformat, false, dbcMeleeCrit, false);
    LOAD_DBC("DBC/gtChanceToMeleeCritBase.dbc", gtClassfloatformat, false, dbcMeleeCritBase, false);
    LOAD_DBC("DBC/gtChanceToSpellCrit.dbc", gtClassfloatformat, false, dbcSpellCrit, false);
    LOAD_DBC("DBC/gtChanceToSpellCritBase.dbc", gtClassfloatformat, false, dbcSpellCritBase, false);
    LOAD_DBC("DBC/gtRegenMPPerSpt.dbc", gtClassfloatformat, false, dbcManaRegenBase, false); //it's not a mistake.
    LOAD_DBC("DBC/AreaTrigger.dbc", areatriggerformat, true, dbcAreaTrigger, true);
    LOAD_DBC("DBC/ScalingStatDistribution.dbc", scalingstatdistributionformat, true, dbcScalingStatDistribution, false);
    LOAD_DBC("DBC/ScalingStatValues.dbc", scalingstatvaluesformat, true, dbcScalingStatValues, false);
    LOAD_DBC("DBC/ItemLimitCategory.dbc", itemlimitcategoryformat, true, dbcItemLimitCategory, true);
    LOAD_DBC("DBC/QuestSort.dbc", QuestSortEntryfmt, false, sQuestSortStore, false);
    LOAD_DBC("DBC/QuestXP.dbc", questxpformat, false, dbcQuestXP, false);
    LOAD_DBC("DBC/MailTemplate.dbc", mailTemplateEntryFormat, true, dbcMailTemplateEntry, true);
    LOAD_DBC("DBC/WMOAreaTable.dbc", wmoareaformat, true, dbcWMOAreaTable, false);
    LOAD_DBC("DBC/SummonProperties.dbc", summonpropertiesformat, true, dbcSummonProperties, false);

    LOAD_DBC("DBC/SpellEffect.dbc", SpellEffectEntryfmt, false, dbcSpellEffect, false);
    LOAD_DBC("DBC/SpellShapeshiftForm.dbc", SpellShapeshiftFormfmt, false, dbcSpellShapeshiftForm, false);
    LOAD_DBC("DBC/SpellShapeshift.dbc", SpellShapeshiftEntryfmt, false, dbcSpellShapeshift, false);
    LOAD_DBC("DBC/SpellAuraOptions.dbc", SpellAuraOptionsEntryfmt, false, dbcSpellAuraOptions, false);
    LOAD_DBC("DBC/SpellAuraRestrictions.dbc", SpellAuraRestrictionsEntryfmt, false, dbcSpellAuraRestrictions, false);
    LOAD_DBC("DBC/SpellCastingRequirements.dbc", SpellCastingRequirementsEntryfmt, false, dbcSpellCastingRequirements, false);
    LOAD_DBC("DBC/SpellCategories.dbc", SpellCategoriesEntryfmt, false, dbcSpellCategories, false);
    LOAD_DBC("DBC/SpellClassOptions.dbc", SpellClassOptionsEntryfmt, false, dbcSpellClassOptions, false);
    LOAD_DBC("DBC/SpellCooldowns.dbc", SpellCooldownsEntryfmt, false, dbcSpellCooldowns, false);
    LOAD_DBC("DBC/SpellEquippedItems.dbc", SpellEquippedItemsEntryfmt, false, dbcSpellEquippedItems, false);
    LOAD_DBC("DBC/SpellInterrupts.dbc", SpellInterruptsEntryfmt, false, dbcSpellInterrupts, false);
    LOAD_DBC("DBC/SpellLevels.dbc", SpellLevelsEntryfmt, false, dbcSpellLevels, false);
    LOAD_DBC("DBC/SpellPower.dbc", SpellPowerEntryfmt, false, dbcSpellPower, false);
    LOAD_DBC("DBC/SpellReagents.dbc", SpellReagentsEntryfmt, false, dbcSpellReagents, false);
    LOAD_DBC("DBC/SpellScaling.dbc", SpellScalingEntryfmt, false, dbcSpellScaling, false);
    LOAD_DBC("DBC/SpellTargetRestrictions.dbc", SpellTargetRestrictionsEntryfmt, false, dbcSpellTargetRestrictions, false);
    LOAD_DBC("DBC/SpellTotems.dbc", SpellTotemsEntryfmt, false, dbcSpellTotems, false);

    //LOAD_DBC("DBC/NameGen.dbc", namegenentryformat, true, dbcNameGen, true);
    //DBC::LoadDBC(available_dbc_locales, bad_dbc_files, sLFGDungeonStore, dbc_path, "LFGDungeons.dbc");
    LOAD_DBC("DBC/Vehicle.dbc", VehicleEntryfmt, true, dbcVehicle, true);
    LOAD_DBC("DBC/VehicleSeat.dbc", VehicleSeatEntryfmt, true, dbcVehicleSeat, false);
    LOAD_DBC("DBC/Faction.dbc", FactionEntryfmt, false, sFactionStore, false);




    MapManagement::AreaManagement::AreaStorage::Initialise(&sAreaStore);
    auto area_map_collection = MapManagement::AreaManagement::AreaStorage::GetMapCollection();
    for (auto map_object : dbcMap)
    {
        area_map_collection->insert(std::pair<uint32, uint32>(map_object->id, map_object->linked_zone));
    }
    //auto wmo_row_count = dbcWMOAreaTable.GetNumRows();
    for (auto i = 0; i < 51119; ++i) // This is a hack, dbc loading needs rework
    {
        if (auto entry = dbcWMOAreaTable.LookupEntry(i))
        {
            sWMOAreaInfoByTripple.insert(WMOAreaInfoByTripple::value_type(WMOAreaTableTripple(entry->rootId, entry->adtId, entry->groupId), entry));
            MapManagement::AreaManagement::AreaStorage::AddWMOTripleEntry(entry->groupId, entry->rootId, entry->adtId, entry->areaId);
        }
    }
    return true;
}

const WMOAreaTableEntry* GetWMOAreaTableEntryByTriple(int32 root_id, int32 adt_id, int32 group_id)
{
    auto iter = sWMOAreaInfoByTripple.find(WMOAreaTableTripple(root_id, adt_id, group_id));
    if (iter == sWMOAreaInfoByTripple.end())
        return 0;
    return iter->second;
}

uint32 SpellEntry::GetRequiredItemFlags()
{
    if (SpellEquippedItemsId)
    {
        SpellEquippedItemsEntry* see = dbcSpellEquippedItems.LookupEntryForced(SpellEquippedItemsId);
        if (see)
            return see->RequiredItemFlags;
    }
    return 0;
}
