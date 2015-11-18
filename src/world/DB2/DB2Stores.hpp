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

#ifndef DB2STORES_H
#define DB2STORES_H


#include "Common.h"
#include "Database/DB2Store.hpp"
#include "DB2Structure.hpp"

#include <list>

const char item_format[] = "niiiiiii";
const char item_currency_cost_format[] = "di";
const char item_extended_cost_entry_format[] = "nxxiiiiiiiiiiiixiiiiiiiiiixxixx";

extern SERVER_DECL DB2Storage<ItemEntry>                dbcItemEntry;
extern SERVER_DECL DB2Storage<ItemCurrencyCostEntry>    dbcItemCurrencyCostStore;
extern SERVER_DECL DB2Storage<ItemExtendedCostEntry>    dbcItemExtendedCost;

void LoadDB2Stores();

DB2Storage <ItemEntry> const* GetItemDisplayStore();

#endif  //DB2STORES_H
