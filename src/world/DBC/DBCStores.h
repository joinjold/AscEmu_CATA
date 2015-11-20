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

#ifndef _DBCSTORES_H
#define _DBCSTORES_H

#include "DBCGlobals.hpp"
#include "Definitions.h"
#include "DB2/DB2Structure.hpp"

class Player;

#pragma pack(push,1)

struct WorldMapOverlay
{
    uint32 ID;              // 0
    uint32 Category;    // 1
    uint32 areaID;          // 2 - index to AreaTable
    uint32 areaID_2;        // 3 - index to AreaTable
    uint32 areaID_3;        // 4 - index to AreaTable
    uint32 areaID_4;        // 5 - index to AreaTable
    uint32 HasSubstitution; // 5
    uint32 SubstitutionId;                                  // 6
    uint32 TotalCap;                                        // 7
    uint32 WeekCap;                                         // 8
    uint32 Flags;                                           // 9	
};

#ifdef ENABLE_ACHIEVEMENTS
struct AchievementEntry
{
    uint32 ID;                 // 0
    int32 factionFlag;         // 1 -1=all, 0=horde, 1=alliance
    int32 mapID;               // 2 -1=none
    uint32 unknown1;           // 20
    const char* name;          // 3-18
    uint32 name_flags;         // 19
    const char* description;   // 21-36
    uint32 desc_flags;         // 37
    uint32 categoryId;         // 38
    uint32 points;             // 39 reward points
    uint32 orderInCategory;    // 40
    uint32 flags;              // 41
    uint32 unknown2;           // 42
    const char* rewardName;    // 43-58 title/item reward name
    uint32 rewardName_flags;   // 59
    uint32 count;              // 60
    uint32 refAchievement;     // 61
};

struct AchievementCategoryEntry
{
    uint32 ID;                 // 0
    uint32 parentCategory;     // 1 -1 for main category
    const char* name;          // 2-17
    uint32 name_flags;         // 18
    uint32 sortOrder;          // 19
};

struct AchievementCriteriaEntry
{
    uint32 ID;                     // 0
    uint32 referredAchievement;    // 1
    uint32 requiredType;           // 2
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE = 0
        ///\todo also used for player deaths..
        struct
        {
            uint32 creatureID;                             // 3
            uint32 creatureCount;                          // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG = 1
        ///\todo there are further criterias instead just winning
        struct
        {
            uint32 bgMapID;                                // 3
            uint32 winCount;                               // 4
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL = 5
        struct
        {
            uint32 unused;                                 // 3
            uint32 level;                                  // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL = 7
        struct
        {
            uint32 skillID;                                // 3
            uint32 skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT = 8
        struct
        {
            uint32 linkedAchievement;                      // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT = 9
        struct
        {
            uint32 unused;                                 // 3
            uint32 totalQuestCount;                        // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32 unused;                                 // 3
            uint32 numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32 zoneID;                                 // 3
            uint32 questCount;                             // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST = 14
        struct
        {
            uint32 unused;                                 // 3
            uint32 questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND= 15
        struct
        {
            uint32 mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP= 16
        struct
        {
            uint32 mapID;                                  // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID = 19
        struct
        {
            uint32 groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE = 20
        struct
        {
            uint32 creatureEntry;                          // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING = 24
        struct
        {
            uint32 unused;                                 // 3
            uint32 fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST = 27
        struct
        {
            uint32 questID;                                // 3
            uint32 questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2= 69
        struct
        {
            uint32 spellID;                                // 3
            uint32 spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL= 29
        struct
        {
            uint32 spellID;                                // 3
            uint32 castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32 areaID;                                 // 3 Reference to AreaTable.dbc
            uint32 killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA = 32
        struct
        {
            uint32 mapID;                                  // 3 Reference to Map.dbc
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA = 33
        struct
        {
            uint32 mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL = 34
        struct
        {
            uint32 spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM = 36
        struct
        {
            uint32 itemID;                                 // 3
            uint32 itemCount;                              // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA = 37
        struct
        {
            uint32 unused;                                 // 3
            uint32 count;                                  // 4
            uint32 flag;                                   // 5 4=in a row
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING = 38
        struct
        {
            uint32 teamtype;                               // 3 {2,3,5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING = 39
        struct
        {
            uint32 teamtype;                               // 3 {2,3,5}
            uint32 teamrating;                             // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL = 40
        struct
        {
            uint32 skillID;                                // 3
            uint32 skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM = 41
        struct
        {
            uint32 itemID;                                 // 3
            uint32 itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM = 42
        struct
        {
            uint32 itemID;                                 // 3
            uint32 itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA = 43
        struct
        {
            uint32 areaReference;                          // 3 - this is an index to WorldMapOverlay
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK= 44
        struct
        {
            ///\todo This rank is _NOT_ the index from CharTitles.dbc
            uint32 rank;                                   // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT= 45
        struct
        {
            uint32 unused;                                 // 3
            uint32 numberOfSlots;                          // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION= 46
        struct
        {
            uint32 factionID;                              // 3
            uint32 reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32 unused;                                 // 3
            uint32 numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM = 49
        ///\todo where is the required itemlevel stored?
        struct
        {
            uint32 itemSlot;                               // 3
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT= 50
        struct
        {
            uint32 rollValue;                              // 3
            uint32 count;                                  // 4
        } roll_need_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS = 52
        struct
        {
            uint32 classID;                                // 3
            uint32 count;                                  // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE = 53
        struct
        {
            uint32 raceID;                                 // 3
            uint32 count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE = 54
        ///\todo where is the information about the target stored?
        struct
        {
            uint32 emoteID;                                // 3
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE = 55
        struct
        {
            uint32 unused;                                 // 3
            uint32 count;                                  // 4
            uint32 flag;                                   // 5 =3 for battleground healing
            uint32 mapid;                                  // 6
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM = 57
        struct
        {
            uint32 itemID;                                 // 3
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD = 62
        struct
        {
            uint32 unknown;                                 // 3
            uint32 goldInCopper;                            // 4
        } quest_reward_money;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY = 67
        struct
        {
            uint32 unused;                                 // 3
            uint32 goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT = 68
        struct
        {
            uint32 goEntry;                                // 3
            uint32 useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL= 70
        ///\todo are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32 unused;                                 // 3
            uint32 killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT = 72
        struct
        {
            uint32 goEntry;                                // 3
            uint32 lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_NUMBER_OF_MOUNTS= 75
        struct
        {
            uint32 unknown;                                // 3 777=?
            uint32 mountCount;                             // 4
        } number_of_mounts;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL = 76
        struct
        {
            uint32 unused;                                 // 3
            uint32 duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER = 96
        struct
        {
            uint32 powerType;                              // 3 mana= 0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT = 97
        struct
        {
            uint32 statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER = 98
        struct
        {
            uint32 spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING = 100
        struct
        {
            uint32 ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE = 109
        struct
        {
            uint32 lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32 lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2 = 110
        struct
        {
            uint32 skillLine;                              // 3
            uint32 spellCount;                             // 4
        } cast_spell2;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE= 112
        struct
        {
            uint32 skillLine;                              // 3
            uint32 spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL = 113
        struct
        {
            uint32 unused;                                 // 3
            uint32 killCount;                              // 4
        } honorable_kill;

        struct
        {
            uint32 field3;                                 // 3 main requirement
            uint32 field4;                                 // 4 main requirement count
            uint32 additionalRequirement1_type;            // 5 additional requirement 1 type
            uint32 additionalRequirement1_value;           // 6 additional requirement 1 value
            uint32 additionalRequirement2_type;            // 7 additional requirement 2 type
            uint32 additionalRequirement2_value;           // 8 additional requirement 1 value
        } raw;
    };
    const char* name;             // 9-24
    uint32 name_flags;            // 25
    uint32 completionFlag;        // 26
    uint32 groupFlag;             // 27
    uint32 unk1;                  // 28
    uint32 timeLimit;             // 29 time limit in seconds
    uint32 index;                 // 30
};
#endif

/*struct BattlemasterListEntry
{
    uint32 entry;                   // 0
    int32 maps[8];                  // 1-8 mapid
    uint32 instance_type;           // 9 (3 - BG, 4 - arena)
    uint32 canJoinAsGroup;          // 10 (0 or 1)
    char* name[16];                 // 11-26 name
    uint32 nameFlags;               // 27 string flag, unused
    uint32 maxGroupSize;            // 28 maxGroupSize, used for checking if queue as group
    uint32 HolidayWorldStateId;     // 29 new 3.1
    uint32 minLevel;                // 30 Min level
    uint32 maxLevel;                // 31 Max level
};*/

struct BankSlotPrice
{
    uint32 Id;
    uint32 Price;
};

struct CharTitlesEntry
{
    uint32 ID;                      // 0, title ids
    const char* name;               // 2-17, unused
    uint32 bit_index;               // 36 used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
};

struct CurrencyTypesEntry
{
    uint32 ID;                      // 0 not used
    uint32 ItemId;                  // 1 used as real index
    uint32 Category;                // 2 may be category
    uint32 BitIndex;                // 3 bit index in PLAYER_FIELD_KNOWN_CURRENCIES (1 << (index-1))
};

struct ItemSetEntry
{
    uint32 id;                      //1
    const char* name;               //2
    uint32 itemid[10];               //11 - 18
    uint32 SpellID[8];              //28 - 35
    uint32 itemscount[8];           //36 - 43
    uint32 RequiredSkillID;         //44
    uint32 RequiredSkillAmt;        //45
};

struct ItemLimitCategoryEntry
{
    uint32 Id;              // 0    - Id
    uint32 maxAmount;       // 18   - Max amount of items
    uint32 equippedFlag;    // 19   - equipped (bool?)
};

#define LOCK_NUM_CASES 5
#define LOCK_NUM_ACTIONS 8

struct Lock
{
    uint32 Id;
    uint32 locktype[LOCK_NUM_CASES];        /// If this is 1, then the next lockmisc is an item ID, if it's 2, then it's an iRef to LockTypes.dbc.
    uint32 lockmisc[LOCK_NUM_CASES];        /// Item to unlock or iRef to LockTypes.dbc depending on the locktype.
    uint32 minlockskill[LOCK_NUM_CASES];    /// Required skill needed for lockmisc (if locktype = 2).
    uint32 action[LOCK_NUM_ACTIONS];                     /// Something to do with direction / opening / closing.
};

struct emoteentry
{
    uint32 Id;
  //uint32 name;
    uint32 textid;
    uint32 textid2;
    uint32 textid3;
    uint32 textid4;
  //uint32 unk1;
    uint32 textid5;
  //uint32 unk2;
    uint32 textid6;
  //uint32 unk3;
  //uint32 unk4;
  //uint32 unk5;
  //uint32 unk6;
  //uint32 unk7;
  //uint32 unk8;
  //uint32 unk9;
  //uint32 unk10;
  //uint32 unk11;
};

//SkillLineAbility.dbc
struct skilllinespell
{
    uint32 Id;
    uint32 skilline;
    uint32 spell;
    uint32 raceMask;
    uint32 classMask;
  //uint32 excludeRace;
  //uint32 excludeClass;
    uint32 minSkillLineRank;
    uint32 next;
    uint32 acquireMethod;
    uint32 grey;
    uint32 green;
  //uint32 abandonable;
    uint32 reqTP;
    uint32 prev;
};

struct EnchantEntry
{
    uint32 Id;
    uint32 type[3];
    int32 min[3];      /// for compat, in practice min==max
    int32 max[3];
    uint32 spell[3];
    const char* Name;
    uint32 visual;
    uint32 EnchantGroups;
    uint32 GemEntry;
    uint32 unk7;        /// Gem Related
    uint32 required_skill;
    uint32 required_skill_rank;
    uint32 unk10;
    uint32 req_itemlevel;
    uint32 custom_enchant_group;
    uint32 custom_enchant_maxstack;
    uint32 custom_GearScore;
    uint32 custom_ScriptCreated;

};

struct GemPropertyEntry
{
    uint32 Entry;
    uint32 EnchantmentID;
    uint32 SocketMaskMustMatch;            /// bool
    uint32 SocketMask;
    uint32 unk3;
};

//GlyphProperties.dbc
struct GlyphPropertyEntry
{
    uint32 Entry;
    uint32 SpellID;
    uint32 Type;        /// 0 = Major, 1 = Minor
    uint32 unk;         /// some flag
};

struct GlyphSlotEntry
{
    uint32 Id;
    uint32 Type;
    uint32 Slot;
};

//SkillLine.dbc
struct skilllineentry
{
    uint32 id;
    uint32 type;
    const char* Name;
    uint32 spellIcon;
    uint32 canLink;
};

struct TalentEntry
{
    uint32 TalentID;
    uint32 TalentTree;
    uint32 Row;
    uint32 Col;
    uint32 RankID[5];
    //uint32 unk[4];
    uint32 DependsOn;
    //uint32 unk1[2];
    uint32 DependsOnRank;
    //uint32 unk2[4];
};

class Spell;
class Unit;
class Aura;
struct ProcHandlerContextShare;

enum SpellRequiredTargetTypes
{
    SPELL_TARGET_TYPE_REQUIRE_NO_CHECK = 0,
    SPELL_TARGET_TYPE_REQUIRE_FACTION_CHECK = 1,
    SPELL_TARGET_TYPE_REQUIRE_ENTRY_CHECK = 2,
};

#define MAX_SPELL_REAGENTS 8
#define MAX_SPELL_TOTEMS 2
#define MAX_SPELL_TOTEM_CATEGORIES 2

// SpellAuraOptions.dbc
struct SpellAuraOptionsEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    StackAmount;                                  // 51       m_cumulativeAura
    uint32    procChance;                                   // 38       m_procChance
    uint32    procCharges;                                  // 39       m_procCharges
    uint32    procFlags;                                    // 37       m_procTypeMask
};

// SpellAuraRestrictions.dbc
struct SpellAuraRestrictionsEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    CasterAuraState;                              // 21       m_casterAuraState
    uint32    TargetAuraState;                              // 22       m_targetAuraState
    uint32    CasterAuraStateNot;                           // 23       m_excludeCasterAuraState
    uint32    TargetAuraStateNot;                           // 24       m_excludeTargetAuraState
    uint32    casterAuraSpell;                              // 25       m_casterAuraSpell
    uint32    targetAuraSpell;                              // 26       m_targetAuraSpell
    uint32    casterAuraSpellNot;                       // 27       m_excludeCasterAuraSpell
    uint32    targetAuraSpellNot;                       // 28       m_excludeTargetAuraSpell
};

// SpellCastingRequirements.dbc
struct SpellCastingRequirementsEntry
{
    uint32    Id;                                           // not used?
    uint32    FacingCasterFlags;                            // 20       m_facingCasterFlags
    int32     RequiresAreaId;                                  // 164      m_requiredAreaGroupId
    uint32    RequiresSpellFocus;                           // 19       m_requiresSpellFocus
};

// SpellCategories.dbc
struct SpellCategoriesEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    Category;                                     // 1        m_category
    uint32    DmgClass;                                     // 153      m_defenseType
    uint32    DispelType;                                       // 2        m_dispelType
    uint32    Mechanic;                                     // 3        m_mechanic
    uint32    PreventionType;                               // 154      m_preventionType
    uint32    StartRecoveryCategory;                        // 145      m_startRecoveryCategory
};

// SpellClassOptions.dbc
struct SpellClassOptionsEntry
{
    uint32    Id;                                         // 0        m_ID
    uint32    SpellFamilyFlags[3][3];                          // 149-150  m_spellClassMask NOTE: size is 12 bytes!!!
    uint32    SpellFamilyName;                              // 148      m_spellClassSet
};

// SpellCooldowns.dbc
struct SpellCooldownsEntry
{
    uint32    Id;                                          // 0        m_ID // not used?
    int32    CategoryRecoveryTime;                         // 31       m_categoryRecoveryTime
    int32    RecoveryTime;                                 // 30       m_recoveryTime
    int32    StartRecoveryTime;                            // 146      m_startRecoveryTime
};

// SpellEffect.dbc
#define SPELL_EFFECT_MAGIC_UNDECIDED_SPELL_POWER_COEFF	0.0000001f
#define ITEM_SPELL_DEFAULT_SP_BENEFIT_COEF				0.001f

struct SpellEffectEntry
{
    //uint32    Id;                                           // 0        m_ID
    uint32    Effect;                                       // 73-75    m_effect
    float     EffectMultipleValue;                         // 106-108  m_effectAmplitude
    uint32    EffectApplyAuraName;                          // 100-102  m_effectAura
    uint32    EffectAmplitude;                              // 103-105  m_effectAuraPeriod
    int32     EffectBasePoints;                             // 82-84    m_effectBasePoints (don't must be used in spell/auras explicitly, must be used cached Spell::m_currentBasePoints)
    float	  EffectSpellPowerCoef;                         // 169-171  
    float     EffectChainMultiplier;                         // 156-158  m_effectChainAmplitude -- dmg_multiplier i guess
    int32     EffectChainTarget;                            // 109-111  m_effectChainTargets
    int32     EffectDieSides;                               // 76-78    m_effectDieSides
    uint32    EffectItemType;                               // 112-114  m_effectItemType
    uint32    EffectMechanic;                               // 85-87    m_effectMechanic
    int32     EffectMiscValue;                              // 115-117  m_effectMiscValue
    int32     EffectMiscValueB;                             // 118-120  m_effectMiscValueB
    float     EffectPointsPerComboPoint;                    // 124-126  m_effectPointsPerCombo
    uint32    EffectRadiusIndex;                            // 94-96    m_effectRadiusIndex - spellradius.dbc
    uint32    EffectRadiusMaxIndex;                        // 97-99    4.0.0
    float     EffectRealPointsPerLevel;                     // 79-81    m_effectRealPointsPerLevel
    uint32    EffectSpellGroupRelation[3];                  // 127-129  m_effectSpellClassMaskA, effect 0
    uint32    EffectTriggerSpell;                           // 121-123  m_effectTriggerSpell
    uint32    EffectImplicitTargetA;                        // 88-90    m_implicitTargetA
    uint32    EffectImplicitTargetB;                        // 91-93    m_implicitTargetB
    uint32    EffectSpellId;                                // new 4.0.0 - most important, we will attach effects to spells based on this :P
    uint32    EffectIndex;                                  // new 4.0.0
};

// SpellEquippedItems.dbc
struct SpellEquippedItemsEntry
{
    uint32    Id;                                           // 0        m_ID // not used?
    int32     EquippedItemClass;                            // 70       m_equippedItemClass (value)
    int32     RequiredItemFlags;                // 72       m_equippedItemInvTypes (mask) // this?
    int32     EquippedItemSubClass;                     // 71       m_equippedItemSubclass (mask)
};

// SpellInterrupts.dbc
struct SpellInterruptsEntry
{
    uint32    Id;                                           // 0        m_ID // not used?
    uint32    AuraInterruptFlags;                           // 33       m_auraInterruptFlags
    uint32    ChannelInterruptFlags;                        // 35       m_channelInterruptFlags
    uint32    InterruptFlags;                               // 32       m_interruptFlags
};

// SpellLevels.dbc
struct SpellLevelsEntry
{
    uint32    Id;                                           // 0        m_ID // not used?
    uint32    baseLevel;                                    // 41       m_baseLevel
    uint32    maxLevel;                                     // 40       m_maxLevel
    uint32    spellLevel;                                   // 42       m_spellLevel
};

// SpellPower.dbc
struct SpellPowerEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    manaCost;                                     // 45       m_manaCost
    uint32    manaCostPerlevel;                             // 46       m_manaCostPerLevel
    uint32    ManaCostPercentage;                           // 144      m_manaCostPct
    uint32    manaPerSecond;                                // 47       m_manaPerSecond
    uint32    manaPerSecondPerLevel;
    float     manaCostPercentagefloat;                      //           4.3.0
};

// SpellReagents.dbc
struct SpellReagentsEntry
{
    uint32 Id;                                      // 0        m_ID
    int32  Reagent[MAX_SPELL_REAGENTS];             // 54-61    m_reagent
    uint32 ReagentCount[MAX_SPELL_REAGENTS];        // 62-69    m_reagentCount
};

// SpellScaling.dbc
struct SpellScalingEntry
{
    uint32 Id;                                      // 0        m_ID
    int32  castTimeMin;                             // 1
    int32  castTimeMax;                             // 2
    int32  castScalingMaxLevel;                     // 3
    int32  playerClass;                             // 4        (index * 100) + charLevel => gtSpellScaling.dbc
    float  coeff_points_gtscale[3];                 // 5-7
    float  coeff_randompoints_gtscale[3];           // 5-7
    float  coeff_combopoint_extra_gtscale[3];       // 5-7		//formula what this will multiply is still custom. Like weapon dmg, or base value or duration ...
    float  effLevelDiffCoeff;                       // 14        some coefficient, mostly 1.0f
    int32  effScalingMinLevel;                      // 15        some level
};

// SpellShapeshift.dbc
struct SpellShapeshiftEntry
{
    uint32 Id;                                          // 0        m_ID
    uint32 Stances;                                     // 13       m_shapeshiftMask
    uint32 ShapeshiftExclude;                           // 15       m_shapeshiftExclude
};

// SpellTargetRestrictions.dbc
struct SpellTargetRestrictionsEntry
{
    uint32 Id;                                          // 0        m_ID
    uint32 MaxTargets;                                  // 152      m_maxTargets
    uint32 MaxTargetLevel;                              // 147      m_maxTargetLevel
    uint32 TargetCreatureType;                          // 18       m_targetCreatureType
    uint32 Targets;                                     // 17       m_targets
};

// SpellTotems.dbc
struct SpellTotemsEntry
{
    uint32 Id;                                          // 0        m_ID
    uint32 TotemCategory[MAX_SPELL_TOTEM_CATEGORIES];   // 162-163  m_requiredTotemCategoryID
    uint32 Totem[MAX_SPELL_TOTEMS];                     // 52-53    m_totem
};

struct SpellShapeshiftFormEntry
{
    uint32 id;                                          // 0
    uint32 Flags;                                       // 3
    int32  unit_type;                                   // 4 <=0 humanoid, other normal creature types
    uint32 AttackSpeed;                                 // 6
    uint32 modelId;                                     // 7 alliance modelid (0 means no model)
    uint32 modelId2;                                    // 8 horde modelid (but only for one form)
    uint32 spells[8];                                   // 11-18 spells which appear in the bar after shapeshifting
};

#define MAX_DIFFICULTY 4

struct SpellDifficultyEntry
{
    uint32 ID;                                          // 0        m_ID
    uint32 SpellId[MAX_DIFFICULTY];                     // 1-4      m_spellId[4]
};

struct SpellFocusObjectEntry
{
    uint32 ID;                                          // 0
};

class SpellEffectOverrideScript;
class SpellCanCastScript;
class SpellCanTargetedScript;
class Object;

#define TEMP_DISABLE_SPELL_COEFS
#define MAX_SPELL_EFFECT_COUNT 3
#define MAX_SPELL_EFFECTS 3

// Struct for the entry in Spell.dbc
struct SpellEntry
{
    uint32 Id;                                          // 0        m_ID
    uint32 Attributes;                                  // 1        m_attribute
    uint32 AttributesEx;                                // 2        m_attributesEx
    uint32 AttributesExB;                               // 3        m_attributesExB
    uint32 AttributesExC;                               // 4        m_attributesExC
    uint32 AttributesExD;                               // 5        m_attributesExD
    uint32 AttributesExE;                               // 6        m_attributesExE
    uint32 AttributesExF;                               // 7        m_attributesExF
    uint32 AttributesExG;                               // 8        m_attributesExG
    uint32 someFlags;                                   // 9        4.0.0
    uint32 unk_400_1;                                   // 10       4.0.0
    uint32 CastingTimeIndex;                            // 11       m_castingTimeIndex
    uint32 DurationIndex;                               // 12       m_durationIndex
    uint32 powerType;                                   // 13       m_powerType
    uint32 rangeIndex;                                  // 14       m_rangeIndex
    float  speed;                                       // 15       m_speed
    uint32 SpellVisual[2];                              // 16-17    m_spellVisualID
    uint32 spellIconID;                                 // 18       m_spellIconID
    uint32 activeIconID;                                // 19       m_activeIconID
    char*	Name;                                       // 20       m_name_lang
    char*	Rank;                                       // 21       m_nameSubtext_lang
    char*	Description;                                // 22       m_description_lang not used
    char*	ToolTip;                                    // 23       m_auraDescription_lang not used
    uint32  SchoolMask;                                 // 24       m_schoolMask
    uint32  RuneCostID;                                 // 25       m_runeCostID
    uint32  spellMissileID;                             // 26       m_spellMissileID not used
    uint32	spellDescriptionVariableID;                 // 27       3.2.0
    uint32  SpellDifficultyID;                          // 28       m_spellDifficultyID - id from SpellDifficulty.dbc
    float   AttackPowerToSpellDamageCoeff;              // 29       //attack power to dmg contribution
    uint32  SpellScalingId;                             // 30       SpellScaling.dbc
    uint32  SpellAuraOptionsId;                         // 31       SpellAuraOptions.dbc
    uint32  SpellAuraRestrictionsId;                    // 32       SpellAuraRestrictions.dbc
    uint32  SpellCastingRequirementsId;                 // 33       SpellCastingRequirements.dbc
    uint32  SpellCategoriesId;                          // 34       SpellCategories.dbc
    uint32  SpellClassOptionsId;                        // 35       SpellClassOptions.dbc
    uint32  SpellCooldownsId;                           // 36       SpellCooldowns.dbc
    uint32  unkIndex7;                                  // 37       all zeros...
    uint32  SpellEquippedItemsId;                       // 38       SpellEquippedItems.dbc
    uint32  SpellInterruptsId;                          // 39       SpellInterrupts.dbc
    uint32  SpellLevelsId;                              // 40       SpellLevels.dbc
    uint32  SpellPowerId;                               // 41       SpellPower.dbc
    uint32  SpellReagentsId;                            // 42       SpellReagents.dbc
    uint32  SpellShapeshiftId;                          // 43       SpellShapeshift.dbc
    uint32  SpellTargetRestrictionsId;                  // 44       SpellTargetRestrictions.dbc
    uint32  SpellTotemsId;                              // 45       SpellTotems.dbc
    uint32  ResearchProject;                            // 46  

    /// CUSTOM: these fields are used for the modifications made in the world.cpp
    // uint32 eff_count;                                //will attach N effects here
    // why more then 3 ? to be able to add custom effects. Some of us are not jerks about 1 MB extra memory
    SpellEffectEntry eff[MAX_SPELL_EFFECT_COUNT];       //max 3 effects
    SpellScalingEntry ss;                               //this one is used all the time
    SpellAuraRestrictionsEntry sar;
    int32 maxstack;                                     //used for hackfixes
    uint32 procCharges;

    //this is accessed like 20 time at every spell cast. Let's not complicate it with linked lookups
    uint32 SpellGroupType[3];                           //96 bits
    uint32 School;
    uint32 AuraInterruptFlags;
    uint32 InterruptFlags;
    uint32 ChannelInterruptFlags;
    int32  RecoveryTime;
    uint32 StartRecoveryCategory;
    int32  StartRecoveryTime;
    uint32 Category;
    int32  CategoryRecoveryTime;
    uint32 MechanicsType;
    uint32 MechanicsTypeFlags;                          //combined all mechanics for removal like "imparring"
    uint32 PreventionType;
    uint32 RequiredShapeShift;
    bool   RemoveOnShapeShift;                          // vengeance / starfall

    uint32 FacingCasterFlags;
    uint32 RequiredAreaId;
    uint32 DiminishStatus;                              //
    uint32 DiminishStatusMaxPVPDur;                     // i wonder if this is set by blizz somewhere
    uint32 proc_interval;                               //!!! CUSTOM, <Fill description for variable>
    uint32 procChance;                                  //we will mod proc chance, keeping a backup mostly for debugging
    uint32 procFlags;
    uint32 procFlags2;                                  //since old flag list is too small
    uint32 in_front_status;                             //!!! CUSTOM,
    uint32 procFlagsRemove;                             //charge disipates if these proc flags are met. This is implemented wrongly.
                                                        //We should test the spell that will proc for flags. But we already register procs based
                                                        //on owner spell and not the proccing spell Buff Groupin Rule -> caster can cast this
                                                        //spell only on 1 target. Value represents the group spell is part of. Can be part of only
                                                        //1 group caster can only cast on 1 target these spells

    uint32 BGR_one_buff_from_caster_on_1target;         //!!! CUSTOM, <Fill description for variable> target can have only buff of this type on self.
                                                        //Value represents the group spell is part of. Can be part of only 1 group
    uint32 BGR_one_buff_on_target;                      //!!! CUSTOM, these are related to creating a item through a spell
                                                        //all positive buffs should have this true and negative buffs as false
    bool   BGR_one_buff_on_target_skip_caster_check;	//addition for previous. 90% of the time check who casted this spell on target. Like blessings
                                                        //should stack caster can have only 1 Aura per spell group, ex pal auras
    uint32 BGR_one_buff_from_caster_on_self;            //!!! CUSTOM, these are related to creating a item through a spell
    bool   HasStackGroupFlags;

#define MAX_SPELL_STACK_GROUPS 3
    union
    {
        uint64 StackGroupMasks[MAX_SPELL_STACK_GROUPS];         //spells from same group need to apply stacking rule. Imported from trinity. bit0 from mask 1 is set to 1 if there are any values
        uint32 StackGroupMasks32[MAX_SPELL_STACK_GROUPS * 2];   //spells from same group need to apply stacking rule. Imported from trinity. bit0 from mask 1 is set to 1 if there are any values
    };

    uint32 c_is_flags;                                  //!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    uint32 c_is_flags2;                                 //!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    uint32 c_is_flags3;                                 //!!! CUSTOM, store spell checks in a static way : isdamageind,ishealing
    //float AP_spell_bonus_base[3];                     //!!! CUSTOM, mod GetAP() include it as base value so other talents can scale it 
    float AP_spell_bonus_total[3];                      //!!! CUSTOM, mod GetAP() include it as a final value without talents scaling it
    //uint32 buffType;                                  //!!! CUSTOM, these are related to creating a item through a spell
    uint32 RankNumber;                                  //!!! CUSTOM, this protects players from having >1 rank of a spell
    uint32 NameHash;                                    //!!! CUSTOM, related to custom spells, summon spell quest related spells
                                                        //this is the group the spell will affect : each effect has it's own mask
    //uint32 EffectSpellGroupRelation[3][3];            //!!! this is not contained in client dbc but server must have it
    uint32 ThreatForSpell;                              //some basic calculation for spell threath
    float ThreatForSpellCoef;                           //theoretically setting this to 0 would make a spell not do threath
    uint32 ProcOnNameHash[3];

    float base_range_or_radius_sqr;                     //!!! CUSTOM, needed for aoe spells most of the time
                                                        // love me or hate me, all "In a cone in front of the caster" spells don't necessarily mean "in front"
    float cone_width;
    uint32 Spell_Dmg_Type;
    //Spell Coefficient
    float SpellPowerCoefAvg;
    bool Spell_value_is_scripted;                       // for fixed spell value calculations this is required.
    int	ai_target_type;

    //!!!!!!!!!!! 1 spell may be present in multiple talent entries ! Ex : pet shared talents = cobra reflexes
#define SPELL_SKILL_LINK_LIMIT 5
    uint32 spell_skilline[SPELL_SKILL_LINK_LIMIT];      //skilline represents the icon in client spellbook
    uint32 spell_skilline_assoc_counter;                //spell may be present in multiple skillines
    TalentEntry* talent_entry[SPELL_SKILL_LINK_LIMIT];  //if this spell is obtained from a talent then we are able to look it up
    uint32 talent_entry_rank[SPELL_SKILL_LINK_LIMIT];   //if this spell is obtained from a talent then we are able to look it up
    uint32 talent_entry_assoc_counter;                  //count this spell in how many talent trees is present
    uint32 talent_tree;                                 //[SPELL_SKILL_LINK_LIMIT]; //multiple talents have multiple trees
    uint32 talent_remove_auras[SPELL_SKILL_LINK_LIMIT]; //if script is adding or casting auras that should get removed on talent change, add them here
    bool   spell_learn_is_linked_to_talent;             //when changing talent spec we need to know which spells to unlearn. If spell was learned by a talent then we need to remove it.
    uint8  belongs_to_player_class;                     //this was added for talent specializations that mod all fire dmg. But should not mod fire dmg procced from items
    uint32 spell_learn_next_level_spell;                //used for iterating through all levels of a spell : kinda like selecting spells with same namehash but this is based on SkillLineDBC
    skilllinespell* spell_skill;                        //in case there is one spell would be part of some skillspell
    uint32 spell_id_client;                             //use this ID fro client isntead real ID
    uint32 quick_duration_min;
    uint32 quick_tickcount;
    uint32 quick_maxAmplitude;                          //so that spells can tick last effect
    uint32 quick_ExtendDuration;
    SpellEntry* chained_cast;                           // cast this spell also when casting the original spell. Used for recently bandaged, forbearance and other spells that require more then 3 effects
    SpellEntry* aura_remove_cast;                       // if spell is dispelled, expires, canceled, this spell is casted by original caster on target(if possible). Used very rarely
    uint32 RequiredPlayerClass;                         // this was added for aura 102 and 103 to not be exploitable by other classes
    uint32 MaxAffectedTargets;
    uint32 ForcedAuraVisibleFlags;                      //should tell client to enable the casting of this spell
    uint32 ReplacingSpell;                              //some spells temp replace others while specific aura is active. Needs manual scripting

    bool always_apply;                                  // custom

    float casttime_coef;                                //!!! CUSTOM, faster spell bonus calculation
    float fixed_dddhcoef;                               //!!! CUSTOM, fixed DD-DH coefficient for some spells
    float fixed_hotdotcoef;                             //!!! CUSTOM, fixed HOT-DOT coefficient for some spells
    float Dspell_coef_override;                         //!!! CUSTOM, overrides any spell coefficient calculation and use this value in DD&DH
    float OTspell_coef_override;                        //!!! CUSTOM, overrides any spell coefficient calculation and use this value in HOT&DOT

    //uint32 SchoolMask;                                // Custom -- redefinition?
    uint32 CustomFlags;                                 // Custom
    uint32 EffectCustomFlag[MAX_SPELL_EFFECTS];         // Custom

    uint32 spell_coef_flags;

    bool self_cast_only;
    bool apply_on_shapeshift_change;
    bool is_melee_spell;                                //!!! CUSTOM,
    bool is_ranged_spell;                               //!!! CUSTOM,
    //bool spell_can_crit;                              //!!! CUSTOM,
    bool EffectCanCrit[MAX_SPELL_EFFECT_COUNT];
    bool noproc;

    void(*ProcHandler)(ProcHandlerContextShare* context);                   //i hope 1 handler / spell is enough
    void(*ChargeHandler)(ProcHandlerContextShare* context);                 //i hope 1 handler / spell is enough
    uint32(*CanCastHandler)(Spell* sp);
    uint32(*CanCastFilterHandler)(Spell* sp, SpellCanCastScript* ccs);      //caster can have filters to cast new spells
    uint32(*CanTargetedFilterHandler)(Spell* sp, Object* Caster, Object* FilterOwner, SpellCanTargetedScript* cts); //can we get targeted by some spells ? immunity handlers
    void(*TargettingOverride[3])(Spell* sp, uint8 i);                       //can script each effect how to gather their targettings
    bool(*AuraPeriodicDummyTickHook)(uint32 i, Aura* pAura, bool apply);
    bool(*SpellDummyEffectScript)(uint32 i, Spell* pSpell);
    void(*CritHandler)(ProcHandlerContextShare* context);                   //i hope 1 handler / spell is enough
    void(*EffRedirectHandler)(Spell* sp);
    bool(*AuraInterruptHandler)(Unit* owner, Aura* a, uint32 flag, uint32 skip_casted, uint32 skiphash_casted, uint32 dmg);
    
    //note this does not support more then 1 effects
    int32 required_target_type;                                             //player - 1, mob = 2, ... !!! db has value -1 to be mangos compatible

#define MAX_AURA_STATES_TO_SET 3
    uint32 SetAuraStatesCount, SetAuraStates[MAX_AURA_STATES_TO_SET], RemAuraStates[MAX_AURA_STATES_TO_SET];
    int32 RemoveCasterAuraState;                        //when casting a spell and we require to have X aurastate, it does not always mean we need to remove it
    //methods ( ofc we all know that structures do not have methods. Class does )
    uint32 *GetSpellGroupType();
    //uint32 GetChannelInterruptFlags();
    //uint32 GetRecoveryTime();
    //uint32 GetInterruptFlags();
    //uint32 GetCategoryRecoveryTime();
    //uint32 GetRequiredShapeShift();
    uint32 GetCasterAuraState() { return sar.CasterAuraState; }
    uint32 GetTargetAuraState() { return sar.TargetAuraState; }
    uint32 GetTargetAuraStateNot() { return sar.TargetAuraStateNot; }
    uint32 GetCasterAuraStateNot() { return sar.CasterAuraStateNot; }
    uint32 GetCasterAuraSpell() { return sar.casterAuraSpell; }
    uint32 GetCasterAuraSpellNot() { return sar.casterAuraSpellNot; }
    uint32 GetTargetAuraSpell() { return sar.targetAuraSpell; }
    uint32 GetTargetAuraSpellNot() { return sar.targetAuraSpellNot; }
    uint32 GetRequiresSpellFocus();
    //uint32 GetRequiresAreaId();
    uint32 GetRequiresAreaId() { return RequiredAreaId; }
    uint32* GetTotem();
    int32*  GetReagent();
    uint32* GetReagentCount();
    int32  GetEquippedItemClass();
    int32  GetEquippedItemSubClass();
    uint32 GetRequiredItemFlags();
    uint32 GetSpellDMGType() { return Spell_Dmg_Type; }
#define NO_MAX_TARGETS_DEFINED 65535 
    uint32 GetMaxTargets() { return MaxAffectedTargets; }
    uint32 GetDispelType();
    SpellPowerEntry PowerEntry;

    SpellEquippedItemsEntry SpellEquippedItems;
    SpellCategoriesEntry SpellCategories;
    SpellClassOptionsEntry SpellClassOptions;
    SpellCastingRequirementsEntry scr;
    SpellTotemsEntry SpellTotems;
    SpellTargetRestrictionsEntry str;
    SpellReagentsEntry sre;
    SpellShapeshiftEntry ssh;

    //uint32 GetManaCost();
    //uint32 GetManaCostPCT();
    SpellLevelsEntry SpellLevel;
    //uint32 GetBaseLevel();
    //uint32 GetMaxLevel();
    //uint32 GetLevel();

    // Pointer to static method of a Spell subclass to create a new instance. If this is NULL, the generic Spell class will be created
    // Its type is void because class Spell is not visible here, so it'll be casted accordingly when necessary
    void* (*SpellFactoryFunc);

    // Same for Auras
    void* (*AuraFactoryFunc);

    SpellEntry()
    {
        CustomFlags = 0;

        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; i++)
            EffectCustomFlag[i] = 0;

        SpellFactoryFunc = NULL;
        AuraFactoryFunc = NULL;
    }

    bool HasEffect(uint32 effect)
    {
        for (uint32 i = 0; i < MAX_SPELL_EFFECT_COUNT; ++i)
            if (eff[i].Effect == effect)
                return true;

        return false;
    }

    bool AppliesAura(uint32 aura)
    {
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if ((eff[i].Effect == 6 || // SPELL_EFFECT_APPLY_GROUP_AREA_AURA
                eff[i].Effect == 27 || // SPELL_EFFECT_PERSISTENT_AREA_AURA
                eff[i].Effect == 35 || // SPELL_EFFECT_APPLY_GROUP_AREA_AURA
                eff[i].Effect == 65 || // SPELL_EFFECT_APPLY_RAID_AREA_AURA
                eff[i].Effect == 119 || // SPELL_EFFECT_APPLY_PET_AREA_AURA
                eff[i].Effect == 128 || // SPELL_EFFECT_APPLY_FRIEND_AREA_AURA
                eff[i].Effect == 129 || // SPELL_EFFECT_APPLY_ENEMY_AREA_AURA
                eff[i].Effect == 143) &&  // SPELL_EFFECT_APPLY_OWNER_AREA_AURA
                eff[i].EffectApplyAuraName == aura)
                return true;
        }

        return false;
    }

    bool HasCustomFlagForEffect(uint32 effect, uint32 flag)
    {
        if (effect >= MAX_SPELL_EFFECTS)
            return false;

        if ((EffectCustomFlag[effect] & flag) != 0)
            return true;
        else
            return false;
    }

    uint32 GetAAEffectId()
    {
        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; i++)
        {
            if (eff[i].Effect == 35 || // SPELL_EFFECT_APPLY_GROUP_AREA_AURA
                eff[i].Effect == 65 || // SPELL_EFFECT_APPLY_RAID_AREA_AURA
                eff[i].Effect == 119 || // SPELL_EFFECT_APPLY_PET_AREA_AURA
                eff[i].Effect == 128 || // SPELL_EFFECT_APPLY_FRIEND_AREA_AURA
                eff[i].Effect == 129 || // SPELL_EFFECT_APPLY_ENEMY_AREA_AURA
                eff[i].Effect == 143)  // SPELL_EFFECT_APPLY_OWNER_AREA_AURA
                return eff[i].Effect;
        }

        return 0;
    }

    bool CheckLocation(uint32 map_id, uint32 zone_id, uint32 area_id, Player* player = NULL);
};

struct SpellRuneCostEntry
{
    uint32 ID;
    uint32 bloodRuneCost;
    uint32 frostRuneCost;
    uint32 unholyRuneCost;
    uint32 runePowerGain;
};

struct TalentTabEntry
{
    uint32 TalentTabID;
    //char* Name;
    //uint32 unk3;
    //uint32 unk4;
    //uint32 unk5;
    //uint32 unk6;
    //uint32 unk7;
    //uint32 unk8;
    //uint32 unk9;
    //uint32 unk10;
    //uint32 unk11;
    //uint32 unk12;
    //uint32 unk13;
    //uint32 unk14;
    //uint32 unk15;
    //uint32 unk16;
    //uint32 unk17;
    //uint32 unk18;
    //uint32 unk19;
    //uint32 unk20;
    uint32 ClassMask;
    uint32 PetTalentMask;
    uint32 TabPage;
    //char* InternalName;
};

struct Trainerspell
{
    uint32 Id;
    uint32 skilline1;
    uint32 skilline2;
    uint32 skilline3;
    uint32 maxlvl;
    uint32 charclass;
};

struct SpellCastTime
{
    uint32 ID;
    uint32 CastTime;
    //uint32 unk1;
    //uint32 unk2;
};

struct SpellRadius
{
    uint32 ID;
    float Radius;
    //float unk1;
    float Radius2;
};

struct SpellRange
{
    uint32 ID;
    float minRange;
    float minRangeFriendly;
    float maxRange;
    float maxRangeFriendly;
    //uint32 unks[35];
};

struct SpellDuration
{
    uint32 ID;
    uint32 Duration1;
    uint32 Duration2;
    uint32 Duration3;
};

struct RandomProps
{
    uint32 ID;
    //uint32 name1;
    uint32 spells[3];
    //uint32 unk1;
    //uint32 unk2;
    //uint32 name2;
    //uint32 RankAlt1;
    //uint32 RankAlt2;
    //uint32 RankAlt3;
    //uint32 RankAlt4;
    //uint32 RankAlt5;
    //uint32 RankAlt6;
    //uint32 RankAlt7;
    //uint32 RankAlt8;
    //uint32 RankAlt9;
    //uint32 RankAlt10;
    //uint32 RankAlt11;
    //uint32 RankAlt12;
    //uint32 RankAlt13;
    //uint32 RankAlt14;
    //uint32 RankAlt15;
    //uint32 RankFlags;
};

struct AreaGroup
{
    uint32 AreaGroupId;
    uint32 AreaId[7];
};

struct FactionTemplateDBC
{
    uint32 ID;
    uint32 Faction;
    uint32 FactionGroup;
    uint32 Mask;
    uint32 FriendlyMask;
    uint32 HostileMask;
    uint32 EnemyFactions[4];
    uint32 FriendlyFactions[4];
};

struct AuctionHouseDBC
{
    uint32 id;
    uint32 faction;
    uint32 fee;
    uint32 tax;
};

struct FactionDBC
{
    uint32 ID;
    int32 RepListId;
    uint32 RaceMask[4];
    uint32 ClassMask[4];
    int32 baseRepValue[4];
    uint32 repFlags[4];
    uint32 parentFaction;
    const char* Name;
    float spilloverRateIn;
    float spilloverRateOut;
    uint32 spilloverMaxRankIn;
    uint32 groupExpansion;
};

struct DBCTaxiNode
{
    uint32 id;
    uint32 mapid;
    float x;
    float y;
    float z;
    char* name;
    uint32 horde_mount;
    uint32 alliance_mount;
    uint32 flags;
};

struct DBCTaxiPath
{
    uint32 id;
    uint32 from;
    uint32 to;
    uint32 price;
};

struct DBCTaxiPathNode
{
    uint32 id;
    uint32 path;
    uint32 seq;            /// nodeIndex
    uint32 mapid;
    float x;
    float y;
    float z;
    uint32 flags;
    uint32 waittime;
    //uint32 arivalEventID;
    //uint32 departureEventID;
};

struct CreatureSpellDataEntry
{
    uint32 id;
    uint32 Spells[3];
    uint32 PHSpell;
    uint32 Cooldowns[3];
    uint32 PH;
};

struct CharRaceEntry
{
    uint32 race_id;
    uint32 flags;
    uint32 FactionID;
    uint32 model_m;
    uint32 model_f;
    uint32 team_id;
    uint32 cinematic_id;
    const char* name1;
    uint32 expansion;
};

struct CharClassEntry
{
    uint32 class_id;
    uint32 power_type;
    const char* name;
    uint32 spellfamily;
    uint32 cinematic_id;
    uint32 expansion;
    uint32 APPerStrenth;
    uint32 APPerAgility;
    uint32 RAPPerAgility;
};

struct CreatureFamilyEntry
{
    uint32 ID;
    float minsize;
    uint32 minlevel;
    float maxsize;
    uint32 maxlevel;
    uint32 skilline;
    uint32 tameable;        /// second skill line - 270 Generic
    uint32 petdietflags;
    uint32 talenttree;      /// -1 = none, 0 = ferocity(410), 1 = tenacity(409), 2 = cunning(411)
    uint32 unk;           /// some index 0 - 63
    const char* name;
};

#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 26
#define MAX_HOLIDAY_FLAGS 10

struct HolidaysEntry
{
    uint32 Id;                                              // 0        m_ID
    uint32 Duration[MAX_HOLIDAY_DURATIONS];                 // 1-10     m_duration
    uint32 Date[MAX_HOLIDAY_DATES];                         // 11-36    m_date (dates in unix time starting at January, 1, 2000)
    uint32 Region;                                          // 37       m_region (wow region)
    uint32 Looping;                                         // 38       m_looping
    uint32 CalendarFlags[MAX_HOLIDAY_FLAGS];                // 39-48    m_calendarFlags
    //uint32 holidayNameId;                                 // 49       m_holidayNameID (HolidayNames.dbc)
    //uint32 holidayDescriptionId;                          // 50       m_holidayDescriptionID (HolidayDescriptions.dbc)
    char* TextureFilename;                                  // 51       m_textureFilename
    uint32 Priority;                                        // 52       m_priority
    uint32 CalendarFilterType;                              // 53       m_calendarFilterType (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    //uint32 flags;                                         // 54       m_flags (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

struct MapEntry
{
    uint32 id;
    uint32 map_type;
    uint32 is_pvp_zone;
    const char* real_name;
    uint32 linked_zone;             /// common zone for instance and continent map
    uint32 multimap_id;
    int32 parent_map;               /// map_id of parent map
    float start_x;                  /// enter x coordinate (if exist single entry)
    float start_y;                  /// enter y coordinate (if exist single entry)
    uint32 addon;                   /// 0-original maps, 1-tbc addon, 2-wotlk addon
    uint32 expireTime;   
    uint32 maxPlayers;
    int32 rootPhaseMap;
};

#define SUFFIX_ENCHANT_COUNT 5
struct ItemRandomSuffixEntry
{
    uint32 id;
    char* nameSuffix;
    uint32 enchantments[SUFFIX_ENCHANT_COUNT];
    uint32 prefixes[SUFFIX_ENCHANT_COUNT];
};

struct BarberShopStyleEntry
{
    uint32 id;              // 0
    uint32 type;            // 1 value 0 -> hair, value 2 -> facialhair
    //char* name;           // 2 string hairstyle name
    //char* name[15];       // 3-17 name of hair style
    //uint32 name_flags;    // 18
    //uint32 unk_name[16];  // 19-34, all empty
    //uint32 unk_flags;     // 35
    //float unk3;           // 36 values 1 and 0,75
    uint32 race;            // 37 race
    uint32 gender;          // 38 0 male, 1 female
    uint32 hair_id;         // 39 Hair ID
};

struct gtFloat
{
    float val;
};

struct gtClassLevelFloat
{
    uint32 Class100Level;
    float val;
};

struct CombatRatingDBC
{
    float val;
};

struct ChatChannelDBC
{
    uint32 id;
    uint32 flags;
    char* name_pattern;
};

struct DurabilityQualityEntry
{
    uint32 id;
    float quality_modifier;
};

struct DurabilityCostsEntry
{
    uint32 itemlevel;
    uint32 modifier[29];
};

struct SpellShapeshiftForm
{
    uint32 id;
    uint32 Flags;
    uint32 unit_type;
    uint32 AttackSpeed;
    uint32 modelId;
    uint32 modelId2;
    uint32 spells[8];
};

struct AreaTriggerEntry
{
    uint32 id;          // 0
    uint32 mapid;       // 1
    float x;            // 2
    float y;            // 3
    float z;            // 4
    float o;            // 5 radius?
    float box_x;        // 6 extent x edge
    float box_y;        // 7 extent y edge
    float box_z;        // 8 extent z edge
    float box_o;        // 9 extent rotation by about z axis
};

struct ScalingStatDistributionEntry
{
    uint32 id;
    int32 stat[10];
    uint32 statmodifier[10];
    uint32 maxlevel;
};

struct ScalingStatValuesEntry
{
    uint32 id;
    uint32 level;
    uint32 dps_mod[6];
    uint32 spellBonus;
    uint32 multiplier[5];
    uint32 armor[8][4];
    uint32 cloak_armor;
};

struct QuestXP
{
    uint32 questLevel;     // 0
    uint32 xpIndex[8];     // 1-9
    //unk                  // 10
};

struct MailTemplateEntry
{
    uint32 ID;              // 0
    char* subject;          // 1
    //float unused1[15]     // 2-16
    //uint32 flags1         // 17 name flags, unused
    char* content;          // 18
    //float unused2[15]     // 19-34
    //uint32 flags2         // 35 name flags, unused
};

struct WMOAreaTableEntry
{
    uint32 id;              // 0
    int32 rootId;           // 1
    int32 adtId;            // 2
    int32 groupId;          // 3
    uint32 flags;           // 9
    uint32 areaId;          // 10  ref -> AreaTableEntry
};

enum SummonControlTypes
{
    SUMMON_CONTROL_TYPE_WILD = 0,
    SUMMON_CONTROL_TYPE_GUARDIAN = 1,
    SUMMON_CONTROL_TYPE_PET = 2,
    SUMMON_CONTROL_TYPE_POSSESSED = 3,
    SUMMON_CONTROL_TYPE_VEHICLE = 4,
};

enum SummonTypes
{
    SUMMON_TYPE_NONE = 0,
    SUMMON_TYPE_PET = 1,
    SUMMON_TYPE_GUARDIAN = 2,
    SUMMON_TYPE_MINION = 3,
    SUMMON_TYPE_TOTEM = 4,
    SUMMON_TYPE_COMPANION = 5,
    SUMMON_TYPE_RUNEBLADE = 6,
    SUMMON_TYPE_CONSTRUCT = 7,
    SUMMON_TYPE_OPPONENT = 8,
    SUMMON_TYPE_VEHICLE = 9,
    SUMMON_TYPE_MOUNT = 10,
    SUMMON_TYPE_LIGHTWELL = 11,
    SUMMON_TYPE_BUTLER = 12
};

struct SummonPropertiesEntry
{
    uint32 ID;
    uint32 ControlType;
    uint32 FactionID;
    uint32 Type;
    uint32 Slot;
    uint32 Flags;
};

struct NameGenEntry
{
    uint32 ID;
    char* Name;
    uint32 unk1;
    uint32 unk2;
};

struct LFGDungeonEntry
{
    uint32 ID;                  // 0
    //char* name[16];             // 1-17 Name lang
    uint32 minlevel;            // 18
    uint32 maxlevel;            // 19
    uint32 reclevel;            // 20
    uint32 recminlevel;         // 21
    uint32 recmaxlevel;         // 22
    int32 map;                  // 23
    uint32 difficulty;          // 24
    //uint32 flags;               // 25
    uint32 type;                // 26
    //uint32 unk2;              // 27
    //char* unk3;               // 28
    uint32 expansion;           // 29
    //uint32 unk4;              // 30
    uint32 grouptype;           // 31
    //char* desc[16];           // 32-47 Description
    /// Helpers
    uint32 Entry() const { return ID + (type << 24); }
};


#define MAX_VEHICLE_SEATS 8

enum VehicleFlags
{
    VEHICLE_FLAG_NO_STRAFE                       = 0x00000001,           // Sets MOVEFLAG2_NO_STRAFE
    VEHICLE_FLAG_NO_JUMPING                      = 0x00000002,           // Sets MOVEFLAG2_NO_JUMPING
    VEHICLE_FLAG_FULLSPEEDTURNING                = 0x00000004,           // Sets MOVEFLAG2_FULLSPEEDTURNING
    VEHICLE_FLAG_ALLOW_PITCHING                  = 0x00000010,           // Sets MOVEFLAG2_ALLOW_PITCHING
    VEHICLE_FLAG_FULLSPEEDPITCHING               = 0x00000020,           // Sets MOVEFLAG2_FULLSPEEDPITCHING
    VEHICLE_FLAG_CUSTOM_PITCH                    = 0x00000040,           // If set use pitchMin and pitchMax from DBC, otherwise pitchMin = -pi/2, pitchMax = pi/2
    VEHICLE_FLAG_ADJUST_AIM_ANGLE                = 0x00000400,           // Lua_IsVehicleAimAngleAdjustable
    VEHICLE_FLAG_ADJUST_AIM_POWER                = 0x00000800,           // Lua_IsVehicleAimPowerAdjustable
};

struct VehicleEntry
{
    uint32 ID;                                          // 0
    uint32 flags;                                       // 1
    float turnSpeed;                                    // 2
    float pitchSpeed;                                   // 3
    float pitchMin;                                     // 4
    float pitchMax;                                     // 5
    uint32 seatID[MAX_VEHICLE_SEATS];                   // 6-13
    float mouseLookOffsetPitch;                         // 14
    float cameraFadeDistScalarMin;                      // 15
    float cameraFadeDistScalarMax;                      // 16
    float cameraPitchOffset;                            // 17
    float facingLimitRight;                             // 18
    float facingLimitLeft;                              // 19
    float msslTrgtTurnLingering;                        // 20
    float msslTrgtPitchLingering;                       // 21
    float msslTrgtMouseLingering;                       // 22
    float msslTrgtEndOpacity;                           // 23
    float msslTrgtArcSpeed;                             // 24
    float msslTrgtArcRepeat;                            // 25
    float msslTrgtArcWidth;                             // 26
    float msslTrgtImpactRadius[2];                      // 27-28
    char* msslTrgtArcTexture;                           // 29
    char* msslTrgtImpactTexture;                        // 30
    char* msslTrgtImpactModel[2];                       // 31-32
    float cameraYawOffset;                              // 33
    uint32 uiLocomotionType;                            // 34
    float msslTrgtImpactTexRadius;                      // 35
    uint32 uiSeatIndicatorType;                         // 36
    uint32 powerType;                                   // 37, new in 3.1
};

enum VehicleSeatFlags
{
    VEHICLE_SEAT_FLAG_HIDE_PASSENGER             = 0x00000200,           // Passenger is hidden
    VEHICLE_SEAT_FLAG_UNK11                      = 0x00000400,
    VEHICLE_SEAT_FLAG_CAN_CONTROL                = 0x00000800,           // Lua_UnitInVehicleControlSeat
    VEHICLE_SEAT_FLAG_CAN_ATTACK                 = 0x00004000,           // Can attack, cast spells and use items from vehicle?
    VEHICLE_SEAT_FLAG_USABLE                     = 0x02000000,           // Lua_CanExitVehicle
    VEHICLE_SEAT_FLAG_CAN_SWITCH                 = 0x04000000,           // Lua_CanSwitchVehicleSeats
    VEHICLE_SEAT_FLAG_CAN_CAST                   = 0x20000000,           // Lua_UnitHasVehicleUI
};

enum VehicleSeatFlagsB
{
    VEHICLE_SEAT_FLAG_B_NONE                     = 0x00000000,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED            = 0x00000002, 
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2          = 0x00000040,
    VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3          = 0x00000100,
};

struct VehicleSeatEntry
{
    uint32 ID;                                          // 0
    uint32 flags;                                       // 1
    int32 attachmentID;                                 // 2
    float attachmentOffsetX;                            // 3
    float attachmentOffsetY;                            // 4
    float attachmentOffsetZ;                            // 5
    float enterPreDelay;                                // 6
    float enterSpeed;                                   // 7
    float enterGravity;                                 // 8
    float enterMinDuration;                             // 9
    float enterMaxDuration;                             // 10
    float enterMinArcHeight;                            // 11
    float enterMaxArcHeight;                            // 12
    int32 enterAnimStart;                               // 13
    int32 enterAnimLoop;                                // 14
    int32 rideAnimStart;                                // 15
    int32 rideAnimLoop;                                 // 16
    int32 rideUpperAnimStart;                           // 17
    int32 rideUpperAnimLoop;                            // 18
    float exitPreDelay;                                 // 19
    float exitSpeed;                                    // 20
    float exitGravity;                                  // 21
    float exitMinDuration;                              // 22
    float exitMaxDuration;                              // 23
    float exitMinArcHeight;                             // 24
    float exitMaxArcHeight;                             // 25
    int32 exitAnimStart;                                // 26
    int32 exitAnimLoop;                                 // 27
    int32 exitAnimEnd;                                  // 28
    float passengerYaw;                                 // 29
    float passengerPitch;                               // 30
    float passengerRoll;                                // 31
    int32 passengerAttachmentID;                        // 32
    int32 vehicleEnterAnim;                             // 33
    int32 vehicleExitAnim;                              // 34
    int32 vehicleRideAnimLoop;                          // 35
    int32 vehicleEnterAnimBone;                         // 36
    int32 vehicleExitAnimBone;                          // 37
    int32 vehicleRideAnimLoopBone;                      // 38
    float vehicleEnterAnimDelay;                        // 39
    float vehicleExitAnimDelay;                         // 40
    uint32 vehicleAbilityDisplay;                       // 41
    uint32 enterUISoundID;                              // 42
    uint32 exitUISoundID;                               // 43
    int32 uiSkin;                                       // 44
    uint32 flagsB;                                      // 45

    bool IsUsable() const
    {
        if ((flags & VEHICLE_SEAT_FLAG_USABLE) != 0)
            return true;
        else
            return false;
    }

    bool IsController() const
    {
        if ((flags & VEHICLE_SEAT_FLAG_CAN_CONTROL) != 0)
            return true;
        else
            return false;
    }

    bool HidesPassenger() const
    {
        if ((flags & VEHICLE_SEAT_FLAG_HIDE_PASSENGER) != 0)
            return true;
        else
            return false;
    }
};


#pragma pack(pop)

inline float GetRadius(SpellRadius* radius)
{
    return radius->Radius;
}
inline uint32 GetCastTime(SpellCastTime* time)
{
    return time->CastTime;
}
inline float GetMaxRange(SpellRange* range)
{
    return range->maxRange;
}
inline float GetMinRange(SpellRange* range)
{
    return range->minRange;
}
inline uint32 GetDuration(SpellDuration* dur)
{
    return dur->Duration1;
}

#define SAFE_DBC_CODE_RETURNS       /// undefine this to make out of range/nulls return null. */

template<class T>
class SERVER_DECL DBCStorage
{
        T* m_heapBlock;
        T* m_firstEntry;

        T** m_entries;
        uint32 m_max;
        uint32 m_numrows;
        uint32 m_stringlength;
        char* m_stringData;

        uint32 rows;
        uint32 cols;
        uint32 useless_shit;
        uint32 header;

    public:

        class iterator
        {
            private:
                T* p;
            public:
                iterator(T* ip = 0) : p(ip) { }
                iterator & operator++() { ++p; return *this; }
                iterator & operator--() { --p; return *this; }
                bool operator!=(const iterator & i) { return (p != i.p); }
                T* operator*() { return p; }
        };

        iterator begin()
        {
            return iterator(&m_heapBlock[0]);
        }
        iterator end()
        {
            return iterator(&m_heapBlock[m_numrows]);
        }

        DBCStorage()
        {
            m_heapBlock = NULL;
            m_entries = NULL;
            m_firstEntry = NULL;
            m_max = 0;
            m_numrows = 0;
            m_stringlength = 0;
            m_stringData = NULL;
            rows = 0;
            cols = 0;
            useless_shit = 0;
            header = 0;
        }

        ~DBCStorage()
        {
            Cleanup();
        }

        void Cleanup()
        {
            if (m_heapBlock)
            {
                free(m_heapBlock);
                m_heapBlock = NULL;
            }
            if (m_entries)
            {
                free(m_entries);
                m_entries = NULL;
            }
            if (m_stringData != NULL)
            {
                free(m_stringData);
                m_stringData = NULL;
            }
        }

        bool Load(const char* filename, const char* format, bool load_indexed, bool load_strings)
        {
            uint32 i;
            uint32 string_length;
            long pos;

            FILE* f = fopen(filename, "rb");
            if (f == NULL)
                return false;

            /* read the number of rows, and allocate our block on the heap */
            if (fread(&header, 4, 1, f) != 1)
            {
                fclose(f);
                return false;
            }
            if (fread(&rows, 4, 1, f) != 1)
            {
                fclose(f);
                return false;
            }
            if (fread(&cols, 4, 1, f) != 1)
            {
                fclose(f);
                return false;
            }
            if (fread(&useless_shit, 4, 1, f) != 1)
            {
                fclose(f);
                return false;
            }
            if (fread(&string_length, 4, 1, f) != 1)
            {
                fclose(f);
                return false;
            }
            pos = ftell(f);

            if (load_strings)
            {
                fseek(f, 20 + (rows * cols * 4), SEEK_SET);
                m_stringData = (char*)malloc(string_length);
                m_stringlength = string_length;
                if (m_stringData)
                    if (fread(m_stringData, string_length, 1, f) != 1)
                    {
                        fclose(f);
                        return false;
                    }
            }

            if (fseek(f, pos, SEEK_SET) != 0)
            {
                fclose(f);
                return false;
            }

            m_heapBlock = (T*)malloc(rows * sizeof(T));
            ASSERT(m_heapBlock);

            /* read the data for each row */
            for (i = 0; i < rows; ++i)
            {
                memset(&m_heapBlock[i], 0, sizeof(T));
                bool read_success = ReadEntry(f, &m_heapBlock[i], format, cols, filename);
                if (!read_success)
                    continue;

                if (load_indexed)
                {
                    /* all the time the first field in the dbc is our unique entry */
                    if (*(uint32*)&m_heapBlock[i] > m_max)
                        m_max = *(uint32*)&m_heapBlock[i];
                }
            }

            if (load_indexed)
            {
                m_entries = (T**)malloc(sizeof(T*) * (m_max + 1));
                ASSERT(m_entries);

                memset(m_entries, 0, (sizeof(T*) * (m_max + 1)));
                for (i = 0; i < rows; ++i)
                {
                    if (m_firstEntry == NULL)
                        m_firstEntry = &m_heapBlock[i];

                    m_entries[*(uint32*)&m_heapBlock[i]] = &m_heapBlock[i];
                }
            }

            m_numrows = rows;

            fclose(f);
            return true;
        }

        bool ReadEntry(FILE* f, T* dest, const char* format, uint32 cols, const char* filename)
        {
            const char* t = format;
            uint32* dest_ptr = (uint32*)dest;
            uint32 c = 0;
            uint32 val;
            size_t len = strlen(format);
            if (len != cols)
            {
                sLog.outError("!!! possible invalid format in file %s (us: %u, them: %u)", filename, len, cols);
                return false;
            }

            while (*t != 0)
            {
                if ((++c) > cols)
                {
                    ++t;
                    //sLog.outError("!!! Read buffer overflow in DBC reading of file %s", filename);
                    continue;
                }

                if (fread(&val, 4, 1, f) != 1)
                {
                    ++t;
                    continue;
                }

                if (*t == 'x')
                {
                    ++t;
                    continue;        // skip!
                }

                if ((*t == 's') || (*t == 'l'))
                {
                    char** new_ptr = (char**)dest_ptr;
                    static const char* null_str = "";
                    char* ptr;
                    // if t == 'lxxxxxxxxxxxxxxxx' use localized strings in case
                    // the english one is empty. *t ends at most on the locale flag
                    for (int count = (*t == 'l') ? 16 : 0 ;
                            val == 0 && count > 0 && *(t + 1) == 'x'; t++, count--)
                    {
                        fread(&val, 4, 1, f);

                    }
                    if (val < m_stringlength)
                        ptr = m_stringData + val;
                    else
                        ptr = (char*)null_str;

                    *new_ptr = ptr;
                    new_ptr++;
                    dest_ptr = (uint32*)new_ptr;
                }
                else
                {
                    *dest_ptr = val;
                    dest_ptr++;
                }

                ++t;
            }
            return true;
        }

        inline uint32 GetNumRows()
        {
            return m_numrows;
        }

        T* LookupEntryForced(uint32 i)
        {
#if 0
            if (m_entries)
            {
                if (i > m_max || m_entries[i] == NULL)
                {
                    printf("LookupEntryForced failed for entry %u\n", i);
                    return NULL;
                }
                else
                    return m_entries[i];
            }
            else
            {
                if (i >= m_numrows)
                    return NULL;
                else
                    return &m_heapBlock[i];
            }
#else
            if (m_entries)
            {
                if (i > m_max || m_entries[i] == NULL)
                    return NULL;
                else
                    return m_entries[i];
            }
            else
            {
                if (i >= m_numrows)
                    return NULL;
                else
                    return &m_heapBlock[i];
            }
#endif
        }

        T* LookupRowForced(uint32 i)
        {
            if (i >= m_numrows)
                return NULL;
            else
                return &m_heapBlock[i];
        }

        T* CreateCopy(T* obj)
        {
            T* oCopy = (T*)malloc(sizeof(T));
            ASSERT(oCopy);
            memcpy(oCopy, obj, sizeof(T));
            return oCopy;
        }
        void SetRow(uint32 i, T* t)
        {
            if (i < m_max && m_entries)
                m_entries[i] = t;
        }

        T* LookupEntry(uint32 i)
        {
            if (m_entries)
            {
                if (i > m_max || m_entries[i] == NULL)
                    return m_firstEntry;
                else
                    return m_entries[i];
            }
            else
            {
                if (i >= m_numrows)
                    return &m_heapBlock[0];
                else
                    return &m_heapBlock[i];
            }
        }

        T* LookupRow(uint32 i)
        {
            if (i >= m_numrows)
                return &m_heapBlock[0];
            else
                return &m_heapBlock[i];
        }
};

extern SERVER_DECL DBCStorage<WorldMapOverlay> dbcWorldMapOverlayStore;
#ifdef ENABLE_ACHIEVEMENTS
extern SERVER_DECL DBCStorage<AchievementEntry> dbcAchievementStore;
extern SERVER_DECL DBCStorage<AchievementCriteriaEntry> dbcAchievementCriteriaStore;
#endif
extern SERVER_DECL DBCStorage<CharTitlesEntry> dbcCharTitlesEntry;
extern SERVER_DECL DBCStorage<CurrencyTypesEntry> dbcCurrencyTypesStore;
extern SERVER_DECL DBCStorage<BarberShopStyleEntry> dbcBarberShopStyleStore;
extern SERVER_DECL DBCStorage<GemPropertyEntry> dbcGemProperty;
extern SERVER_DECL DBCStorage<GlyphPropertyEntry> dbcGlyphProperty;
extern SERVER_DECL DBCStorage<GlyphSlotEntry> dbcGlyphSlot;
extern SERVER_DECL DBCStorage<ItemSetEntry> dbcItemSet;
extern SERVER_DECL DBCStorage<Lock> dbcLock;
extern SERVER_DECL DBCStorage<SpellEntry> dbcSpell;
extern SERVER_DECL DBCStorage<SpellDifficultyEntry> dbcSpellDifficultyEntry;
extern SERVER_DECL DBCStorage<SpellDuration> dbcSpellDuration;
extern SERVER_DECL DBCStorage<SpellRange> dbcSpellRange;
extern SERVER_DECL DBCStorage<emoteentry> dbcEmoteEntry;
extern SERVER_DECL DBCStorage<SpellRadius> dbcSpellRadius;
extern SERVER_DECL DBCStorage<SpellCastTime> dbcSpellCastTime;
extern SERVER_DECL DBCStorage<AreaGroup> dbcAreaGroup;
extern SERVER_DECL DBC::DBCStorage<DBC::Structures::AreaTableEntry> sAreaStore;
extern SERVER_DECL DBCStorage<FactionTemplateDBC> dbcFactionTemplate;
extern SERVER_DECL DBCStorage<FactionDBC> dbcFaction;
extern SERVER_DECL DBCStorage<EnchantEntry> dbcEnchant;
extern SERVER_DECL DBCStorage<RandomProps> dbcRandomProps;
extern SERVER_DECL DBCStorage<skilllinespell> dbcSkillLineSpell;
extern SERVER_DECL DBCStorage<skilllineentry> dbcSkillLine;
extern SERVER_DECL DBCStorage<DBCTaxiNode> dbcTaxiNode;
extern SERVER_DECL DBCStorage<DBCTaxiPath> dbcTaxiPath;
extern SERVER_DECL DBCStorage<DBCTaxiPathNode> dbcTaxiPathNode;
extern SERVER_DECL DBCStorage<AuctionHouseDBC> dbcAuctionHouse;
extern SERVER_DECL DBCStorage<TalentEntry> dbcTalent;
extern SERVER_DECL DBCStorage<TalentTabEntry> dbcTalentTab;
extern SERVER_DECL DBCStorage<CreatureSpellDataEntry> dbcCreatureSpellData;
extern SERVER_DECL DBCStorage<CreatureFamilyEntry> dbcCreatureFamily;
extern SERVER_DECL DBCStorage<CharClassEntry> dbcCharClass;
extern SERVER_DECL DBCStorage<CharRaceEntry> dbcCharRace;
extern SERVER_DECL DBCStorage<MapEntry> dbcMap;
extern SERVER_DECL DBCStorage <HolidaysEntry> dbcHolidaysStore;
extern SERVER_DECL DBCStorage<SpellRuneCostEntry> dbcSpellRuneCost;
extern SERVER_DECL DBCStorage<ItemRandomSuffixEntry> dbcItemRandomSuffix;
extern SERVER_DECL DBCStorage<CombatRatingDBC> dbcCombatRating;
extern SERVER_DECL DBCStorage<ChatChannelDBC> dbcChatChannels;
extern SERVER_DECL DBCStorage<DurabilityCostsEntry> dbcDurabilityCosts;
extern SERVER_DECL DBCStorage<DurabilityQualityEntry> dbcDurabilityQuality;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcBankSlotPrices;
extern SERVER_DECL DBCStorage<BankSlotPrice> dbcStableSlotPrices;
extern SERVER_DECL DBCStorage<gtFloat> dbcBarberShopPrices;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcMeleeCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCrit;
extern SERVER_DECL DBCStorage<gtFloat> dbcSpellCritBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcManaRegenBase;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegen;
extern SERVER_DECL DBCStorage<gtFloat> dbcHPRegenBase;
extern SERVER_DECL DBCStorage<AreaTriggerEntry> dbcAreaTrigger;
extern SERVER_DECL DBCStorage<ScalingStatDistributionEntry> dbcScalingStatDistribution;
extern SERVER_DECL DBCStorage<ScalingStatValuesEntry> dbcScalingStatValues;
extern SERVER_DECL DBCStorage<ItemLimitCategoryEntry> dbcItemLimitCategory;
extern SERVER_DECL DBCStorage<QuestXP> dbcQuestXP;
extern SERVER_DECL DBCStorage<MailTemplateEntry> dbcMailTemplateEntry;
extern SERVER_DECL DBCStorage<WMOAreaTableEntry> dbcWMOAreaTable;
extern SERVER_DECL DBCStorage<SummonPropertiesEntry> dbcSummonProperties;
extern SERVER_DECL DBCStorage<NameGenEntry> dbcNameGen;
extern SERVER_DECL DBC::DBCStorage<DBC::Structures::LFGDungeonEntry> sLFGDungeonStore;
extern SERVER_DECL DBCStorage<VehicleEntry> dbcVehicle;
extern SERVER_DECL DBCStorage<VehicleSeatEntry> dbcVehicleSeat;

extern SERVER_DECL DBCStorage<SpellShapeshiftFormEntry>				dbcSpellShapeshiftForm;
extern SERVER_DECL DBCStorage<SpellShapeshiftEntry>					dbcSpellShapeshift;
extern SERVER_DECL DBCStorage<SpellAuraOptionsEntry>				dbcSpellAuraOptions;
extern SERVER_DECL DBCStorage<SpellAuraRestrictionsEntry>			dbcSpellAuraRestrictions;
extern SERVER_DECL DBCStorage<SpellCastingRequirementsEntry>		dbcSpellCastingRequirements;
extern SERVER_DECL DBCStorage<SpellCategoriesEntry>					dbcSpellCategories;
extern SERVER_DECL DBCStorage<SpellClassOptionsEntry>				dbcSpellClassOptions;
extern SERVER_DECL DBCStorage<SpellCooldownsEntry>					dbcSpellCooldowns;
extern SERVER_DECL DBCStorage<SpellEquippedItemsEntry>				dbcSpellEquippedItems;
extern SERVER_DECL DBCStorage<SpellInterruptsEntry>					dbcSpellInterrupts;
extern SERVER_DECL DBCStorage<SpellLevelsEntry>						dbcSpellLevels;
extern SERVER_DECL DBCStorage<SpellPowerEntry>						dbcSpellPower;
extern SERVER_DECL DBCStorage<SpellReagentsEntry>					dbcSpellReagents;
extern SERVER_DECL DBCStorage<SpellScalingEntry>					dbcSpellScaling;
extern SERVER_DECL DBCStorage<SpellTargetRestrictionsEntry>			dbcSpellTargetRestrictions;
extern SERVER_DECL DBCStorage<SpellTotemsEntry>						dbcSpellTotems;
extern SERVER_DECL DBCStorage<gtClassLevelFloat>					dbcGTSpellScale;

bool LoadDBCs();

const WMOAreaTableEntry* GetWMOAreaTableEntryByTriple(int32 root_id, int32 adt_id, int32 group_id);

#endif // _DBCSTORES_H
