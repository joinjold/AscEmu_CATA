/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2005-2007 Ascent Team
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

#ifndef _WOWSERVER_DEFINITION_H
#define _WOWSERVER_DEFINITION_H


const unsigned NUM_MAPS = 800;
const unsigned NUM_INSTANCE_MODES = 4;


/// Sorry...need this enumeration in Player.*
enum INSTANCE_MODE
{
    MODE_NORMAL = 0,
    MODE_HEROIC = 1
};

enum RAID_MODE
{
    MODE_NORMAL_10MEN    = 0,
    MODE_NORMAL_25MEN    = 1,
    MODE_HEROIC_10MEN    = 2,
    MODE_HEROIC_25MEN    = 3,
    TOTAL_RAID_MODES     = 4
};

enum TimeConstants
{
    MINUTE          = 60,
    HOUR            = MINUTE * 60,
    DAY             = HOUR * 24,
    WEEK            = DAY * 7,
    MONTH           = DAY * 30,
    YEAR            = MONTH * 12,
    IN_MILLISECONDS = 1000
};

#define MAX_RACES         12

#define RACEMASK_ALL_PLAYABLE \
    ((1<<(RACE_HUMAN-1))    |(1<<(RACE_ORC-1))      |(1<<(RACE_DWARF-1))   | \
    (1<<(RACE_NIGHTELF-1))  |(1<<(RACE_UNDEAD-1))   |(1<<(RACE_TAUREN-1))  | \
    (1<<(RACE_GNOME-1))     |(1<<(RACE_TROLL-1))    |(1<<(RACE_BLOODELF-1))| \
    (1<<(RACE_DRAENEI-1))   |(1<<(RACE_GOBLIN-1))   |(1<<(RACE_WORGEN-1)))

#define RACEMASK_ALLIANCE \
    ((1<<(RACE_HUMAN-1))    |(1<<(RACE_DWARF-1))    |(1<<(RACE_NIGHTELF-1))| \
    (1<<(RACE_GNOME-1))     |(1<<(RACE_DRAENEI-1))  |(1<<(RACE_WORGEN-1)))

#define RACEMASK_HORDE RACEMASK_ALL_PLAYABLE & ~RACEMASK_ALLIANCE

#define CLASSMASK_ALL_PLAYABLE \
    ((1<<(WARRIOR-1))|(1<<(PALADIN-1))|(1<<(HUNTER-1))| \
    (1<<(ROGUE-1))  |(1<<(PRIEST-1)) |(1<<(SHAMAN-1))| \
    (1<<(MAGE-1))   |(1<<(WARLOCK-1))|(1<<(DRUID-1)) | \
    (1<<(DEATHKNIGHT-1)))

// values based at QuestSort.dbc
enum QuestSort
{
    QUEST_SORT_EPIC = 1,
    QUEST_SORT_WAILING_CAVERNS_OLD = 21,
    QUEST_SORT_SEASONAL = 22,
    QUEST_SORT_UNDERCITY_OLD = 23,
    QUEST_SORT_HERBALISM = 24,
    QUEST_SORT_BATTLEGROUNDS = 25,
    QUEST_SORT_ULDAMN_OLD = 41,
    QUEST_SORT_WARLOCK = 61,
    QUEST_SORT_WARRIOR = 81,
    QUEST_SORT_SHAMAN = 82,
    QUEST_SORT_FISHING = 101,
    QUEST_SORT_BLACKSMITHING = 121,
    QUEST_SORT_PALADIN = 141,
    QUEST_SORT_MAGE = 161,
    QUEST_SORT_ROGUE = 162,
    QUEST_SORT_ALCHEMY = 181,
    QUEST_SORT_LEATHERWORKING = 182,
    QUEST_SORT_ENGINERING = 201,
    QUEST_SORT_TREASURE_MAP = 221,
    QUEST_SORT_SUNKEN_TEMPLE_OLD = 241,
    QUEST_SORT_HUNTER = 261,
    QUEST_SORT_PRIEST = 262,
    QUEST_SORT_DRUID = 263,
    QUEST_SORT_TAILORING = 264,
    QUEST_SORT_SPECIAL = 284,
    QUEST_SORT_COOKING = 304,
    QUEST_SORT_FIRST_AID = 324,
    QUEST_SORT_LEGENDARY = 344,
    QUEST_SORT_DARKMOON_FAIRE = 364,
    QUEST_SORT_AHN_QIRAJ_WAR = 365,
    QUEST_SORT_LUNAR_FESTIVAL = 366,
    QUEST_SORT_REPUTATION = 367,
    QUEST_SORT_INVASION = 368,
    QUEST_SORT_MIDSUMMER = 369,
    QUEST_SORT_BREWFEST = 370,
    QUEST_SORT_INSCRIPTION = 371,
    QUEST_SORT_DEATH_KNIGHT = 372,
    QUEST_SORT_JEWELCRAFTING = 373,
    QUEST_SORT_NOBLEGARDEN = 374,
    QUEST_SORT_PILGRIMS_BOUNTY = 375,
    QUEST_SORT_LOVE_IS_IN_THE_AIR = 376,
    QUEST_SORT_ARCHAEOLOGY = 377
};

// values based at QuestInfo.dbc
enum QuestTypes
{
    QUEST_TYPE_ELITE = 1,
    QUEST_TYPE_LIFE = 21,
    QUEST_TYPE_PVP = 41,
    QUEST_TYPE_RAID = 62,
    QUEST_TYPE_DUNGEON = 81,
    QUEST_TYPE_WORLD_EVENT = 82,
    QUEST_TYPE_LEGENDARY = 83,
    QUEST_TYPE_ESCORT = 84,
    QUEST_TYPE_HEROIC = 85,
    QUEST_TYPE_RAID_10 = 88,
    QUEST_TYPE_RAID_25 = 89
};

enum SkillType
{
    SKILL_NONE = 0,

    SKILL_FROST = 6,
    SKILL_FIRE = 8,
    SKILL_ARMS = 26,
    SKILL_COMBAT = 38,
    SKILL_SUBTLETY = 39,
    SKILL_SWORDS = 43,
    SKILL_AXES = 44,
    SKILL_BOWS = 45,
    SKILL_GUNS = 46,
    SKILL_BEAST_MASTERY = 50,
    SKILL_SURVIVAL = 51,
    SKILL_MACES = 54,
    SKILL_2H_SWORDS = 55,
    SKILL_HOLY = 56,
    SKILL_SHADOW = 78,
    SKILL_DEFENSE = 95,
    SKILL_LANG_COMMON = 98,
    SKILL_RACIAL_DWARVEN = 101,
    SKILL_LANG_ORCISH = 109,
    SKILL_LANG_DWARVEN = 111,
    SKILL_LANG_DARNASSIAN = 113,
    SKILL_LANG_TAURAHE = 115,
    SKILL_DUAL_WIELD = 118,
    SKILL_RACIAL_TAUREN = 124,
    SKILL_ORC_RACIAL = 125,
    SKILL_RACIAL_NIGHT_ELF = 126,
    SKILL_FIRST_AID = 129,
    SKILL_FERAL_COMBAT = 134,
    SKILL_STAVES = 136,
    SKILL_LANG_THALASSIAN = 137,
    SKILL_LANG_DRACONIC = 138,
    SKILL_LANG_DEMON_TONGUE = 139,
    SKILL_LANG_TITAN = 140,
    SKILL_LANG_OLD_TONGUE = 141,
    SKILL_SURVIVAL2 = 142,
    SKILL_RIDING_HORSE = 148,
    SKILL_RIDING_WOLF = 149,
    SKILL_RIDING_RAM = 152,
    SKILL_RIDING_TIGER = 150,
    SKILL_SWIMING = 155,
    SKILL_2H_MACES = 160,
    SKILL_UNARMED = 162,
    SKILL_MARKSMANSHIP = 163,
    SKILL_BLACKSMITHING = 164,
    SKILL_LEATHERWORKING = 165,
    SKILL_ALCHEMY = 171,
    SKILL_2H_AXES = 172,
    SKILL_DAGGERS = 173,
    SKILL_THROWN = 176,
    SKILL_HERBALISM = 182,
    SKILL_GENERIC_DND = 183,
    SKILL_RETRIBUTION = 184,
    SKILL_COOKING = 185,
    SKILL_MINING = 186,
    SKILL_PET_IMP = 188,
    SKILL_PET_FELHUNTER = 189,
    SKILL_TAILORING = 197,
    SKILL_ENGINERING = 202,
    SKILL_PET_SPIDER = 203,
    SKILL_PET_VOIDWALKER = 204,
    SKILL_PET_SUCCUBUS = 205,
    SKILL_PET_INFERNAL = 206,
    SKILL_PET_DOOMGUARD = 207,
    SKILL_PET_WOLF = 208,
    SKILL_PET_CAT = 209,
    SKILL_PET_BEAR = 210,
    SKILL_PET_BOAR = 211,
    SKILL_PET_CROCILISK = 212,
    SKILL_PET_CARRION_BIRD = 213,
    SKILL_PET_CRAB = 214,
    SKILL_PET_GORILLA = 215,
    SKILL_PET_RAPTOR = 217,
    SKILL_PET_TALLSTRIDER = 218,
    SKILL_RACIAL_UNDED = 220,
    SKILL_CROSSBOWS = 226,
    SKILL_WANDS = 228,
    SKILL_POLEARMS = 229,
    SKILL_PET_SCORPID = 236,
    SKILL_ARCANE = 237,
    SKILL_PET_TURTLE = 251,
    SKILL_ASSASSINATION = 253,
    SKILL_FURY = 256,
    SKILL_PROTECTION = 257,
    SKILL_PROTECTION2 = 267,
    SKILL_PET_TALENTS = 270,
    SKILL_PLATE_MAIL = 293,
    SKILL_LANG_GNOMISH = 313,
    SKILL_LANG_TROLL = 315,
    SKILL_ENCHANTING = 333,
    SKILL_DEMONOLOGY = 354,
    SKILL_AFFLICTION = 355,
    SKILL_FISHING = 356,
    SKILL_ENHANCEMENT = 373,
    SKILL_RESTORATION = 374,
    SKILL_ELEMENTAL_COMBAT = 375,
    SKILL_SKINNING = 393,
    SKILL_MAIL = 413,
    SKILL_LEATHER = 414,
    SKILL_CLOTH = 415,
    SKILL_SHIELD = 433,
    SKILL_FIST_WEAPONS = 473,
    SKILL_RIDING_RAPTOR = 533,
    SKILL_RIDING_MECHANOSTRIDER = 553,
    SKILL_RIDING_UNDEAD_HORSE = 554,
    SKILL_RESTORATION2 = 573,
    SKILL_BALANCE = 574,
    SKILL_DESTRUCTION = 593,
    SKILL_HOLY2 = 594,
    SKILL_DISCIPLINE = 613,
    SKILL_LOCKPICKING = 633,
    SKILL_PET_BAT = 653,
    SKILL_PET_HYENA = 654,
    SKILL_PET_BIRD_OF_PREY = 655,
    SKILL_PET_WIND_SERPENT = 656,
    SKILL_LANG_GUTTERSPEAK = 673,
    SKILL_RIDING_KODO = 713,
    SKILL_RACIAL_TROLL = 733,
    SKILL_RACIAL_GNOME = 753,
    SKILL_RACIAL_HUMAN = 754,
    SKILL_JEWELCRAFTING = 755,
    SKILL_RACIAL_BLOODELF = 756,
    SKILL_PET_EVENT_RC = 758,
    SKILL_LANG_DRAENEI = 759,
    SKILL_RACIAL_DRAENEI = 760,
    SKILL_PET_FELGUARD = 761,
    SKILL_RIDING = 762,
    SKILL_PET_DRAGONHAWK = 763,
    SKILL_PET_NETHER_RAY = 764,
    SKILL_PET_SPOREBAT = 765,
    SKILL_PET_WARP_STALKER = 766,
    SKILL_PET_RAVAGER = 767,
    SKILL_PET_SERPENT = 768,
    SKILL_INTERNAL = 769,
    SKILL_DK_BLOOD = 770,
    SKILL_DK_FROST = 771,
    SKILL_DK_UNHOLY = 772,
    SKILL_INSCRIPTION = 773,
    SKILL_PET_MOTH = 775,
    SKILL_RUNEFORGING = 776,
    SKILL_MOUNTS = 777,
    SKILL_COMPANIONS = 778,
    SKILL_PET_EXOTIC_CHIMAERA = 780,
    SKILL_PET_EXOTIC_DEVILSAUR = 781,
    SKILL_PET_GHOUL = 782,
    SKILL_PET_EXOTIC_SILITHID = 783,
    SKILL_PET_EXOTIC_WORM = 784,
    SKILL_PET_WASP = 785,
    SKILL_PET_EXOTIC_RHINO = 786,
    SKILL_PET_EXOTIC_CORE_HOUND = 787,
    SKILL_PET_EXOTIC_SPIRIT_BEAST = 788,
    SKILL_RACIAL_WORGEN = 789,
    SKILL_RACIAL_GOBLIN = 790,
    SKILL_LANG_WORGEN = 791,
    SKILL_LANG_GOBLIN = 792,
    SKILL_ARCHAEOLOGY = 794,
    SKILL_GENERAL_HUNTER = 795,
    SKILL_GENERAL_DEATHKNIGHT = 796,
    SKILL_GENERAL_ROGUE = 797,
    SKILL_GENERAL_DRUID = 798,
    SKILL_GENERAL_MAGE = 799,
    SKILL_GENERAL_PALADIN = 800,
    SKILL_GENERAL_SHAMAN = 801,
    SKILL_GENERAL_WARLOCK = 802,
    SKILL_GENERAL_WARRIOR = 803,
    SKILL_GENERAL_PRIEST = 804,
    SKILL_PET_WATER_ELEMENTAL = 805,
    SKILL_PET_FOX = 808,
    SKILL_GLYPHS_ALL = 810,
    SKILL_PET_DOG = 811,
    SKILL_MONKEY = 815,
    SKILL_SHALE_SPIDER = 817,
    SKILL_BEETLE = 818,
    SKILL_GUILD_PERKS_ALL = 821
};

#define MAKE_NEW_GUID(l, e, h)   uint64(uint64(l) | (uint64(e) << 24) | (uint64(h) << 48))

#endif // _WOWSERVER_DEFINITION_H
