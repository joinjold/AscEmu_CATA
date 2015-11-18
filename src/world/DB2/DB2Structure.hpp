/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DB2STRUCTURE_H
#define DB2STRUCTURE_H

#include "Common.h"
#include "AchievementMgr.h"

#include <map>
#include <set>
#include <vector>


struct ItemEntry
{
    uint32   ID;                                             // 0
    uint32   Class;                                          // 1
    uint32   SubClass;                                       // 2
    int32    Unk0;                                           // 3
    int32    Material;                                       // 4
    uint32   DisplayId;                                      // 5
    uint32   InventoryType;                                  // 6
    uint32   Sheath;                                         // 7
};

struct ItemCurrencyCostEntry
{
    //uint32 id;                                            // 0
    uint32 itemid;                                          // 1
};

#define MAX_EXTENDED_COST_ITEMS         5
#define MAX_EXTENDED_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32 costid;                                      // 0
    uint32 honor;                                       // 1
    uint32 arena;                                       // 2
    //uint32 unk                                        // 3
    uint32 item[MAX_EXTENDED_COST_ITEMS];               // 4-8
    uint32 count[MAX_EXTENDED_COST_ITEMS];              // 9-13
    uint32 personalrating;                              // 14
    //uint32 unk                                        // 15
    uint32 reqcur[MAX_EXTENDED_COST_CURRENCIES];        // 16-20
    uint32 reqcurrcount[MAX_EXTENDED_COST_CURRENCIES];  // 21-25
    //uint32 unk[5];                                    // 26-30
};

#endif  //DB2STRUCTURE_H
