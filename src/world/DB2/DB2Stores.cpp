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

#include "DB2Stores.hpp"
#include "DBC/DBCStores.h"
#include "Log.h"

#include <map>

DB2Storage <ItemEntry>              dbcItemEntry(item_format);
DB2Storage <ItemCurrencyCostEntry>  dbcItemCurrencyCostStore(item_currency_cost_format);
DB2Storage <ItemExtendedCostEntry>  dbcItemExtendedCost(item_extended_cost_entry_format);

typedef std::list<std::string> StoreProblems;

uint32 DB2_Count = 0;

static bool LoadDB2_assert_print(uint32 fsize, uint32 rsize, const std::string& filename)
{
    sLog.outError("Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);
    return false;
}

struct LocalDB2Data
{
    LocalDB2Data(LocaleConstant loc) : defaultLocale(loc), availableDb2Locales(0xFFFFFFFF)
    {}

    LocaleConstant defaultLocale;

    // bitmasks for index of fullLocaleNameList
    uint32 availableDb2Locales;
};

template<class T>
inline void LoadDB2(LocalDB2Data& localeData, StoreProblems& errors, DB2Storage<T>& storage, std::string const& db2Path, std::string const& filename)
{
    ARCEMU_ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2_Count;

    std::string db2Filename = db2Path + filename;

    if (storage.Load(db2Filename.c_str(), localeData.defaultLocale))
    {
        for (uint8 i = 0; fullLocaleNameList[i].name; ++i)
        {
            if (!(localeData.availableDb2Locales & (1 << i)))
                continue;

            LocaleNameStr const* localStr = &fullLocaleNameList[i];
            std::string db2_dir_loc = db2Path + localStr->name + "/";
            std::string localizedName = db2Path + localStr->name + "/" + filename;

            if (!storage.LoadStringsFrom(localizedName.c_str(), localStr->locale))
                localeData.availableDb2Locales &= ~(1 << i);  // mark as not available for speedup next checks
        }
    }
    else
    {
        if (FILE* f = fopen(db2Filename.c_str(), "rb"))
        {
            char buf[100];
            sLog.outError("exist, but have more fields instead, Wrong client version DB2 file?");
            errors.push_back(db2Filename + buf);
            fclose(f);
        }
        else
            errors.push_back(db2Filename);
    }
}

void LoadDB2Stores()
{
    std::string db2Path = "./DBC/";

    LocaleNameStr const* defaultLocaleNameStr = NULL;
    StoreProblems bad_db2_file;

    LocalDB2Data availableDb2Locales(LocaleConstant(0));

    LoadDB2(availableDb2Locales, bad_db2_file, dbcItemEntry, db2Path, "Item.db2");
    LoadDB2(availableDb2Locales, bad_db2_file, dbcItemCurrencyCostStore, db2Path, "ItemCurrencyCost.db2");
    LoadDB2(availableDb2Locales, bad_db2_file, dbcItemExtendedCost, db2Path, "ItemExtendedCost.db2");

    // errors
    if (bad_db2_file.size() >= DB2_Count)
    {
        sLog.outError("Incorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found", DB2_Count);
        exit(1);
    }
    else if (!bad_db2_file.empty())
    {
        std::string str;
        for (StoreProblems::iterator i = bad_db2_file.begin(); i != bad_db2_file.end(); ++i)
            str += *i + "\n";

        sLog.outError("Some required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_file.size(), DB2_Count, str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!dbcItemEntry.LookupEntry(83086) || !dbcItemExtendedCost.LookupEntry(3872))
    {
        sLog.outError("Please extract correct db2 files from build 15595");
        exit(1);
    }
    sLog.outDebug("Initialized %d db2 stores", DB2_Count);
}
