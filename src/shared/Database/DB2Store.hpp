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

#ifndef DB2STORE_H
#define DB2STORE_H

#include "DB2FileLoader.hpp"

template<class T>
class DB2Storage
{
    typedef std::list<char*> StringPoolList;

    public:

        explicit DB2Storage(const char *f) : nCount(0), fieldCount(0), fmt(f), indexTable(NULL), m_dataTable(NULL) {}
        ~DB2Storage() { Clear(); }

        T* LookupEntry(uint32 id) { return (id >= nCount) ? NULL : indexTable[id]; }
        uint32  GetNumRows() const { return nCount; }
        char const* GetFormat() const { return fmt; }
        uint32 GetFieldCount() const { return fieldCount; }

        bool Load(char const* fn, LocaleConstant loc)
        {
            DB2FileLoader db2;
            // Check if load was sucessful, only then continue
            if (!db2.Load(fn, fmt))
                return false;

            fieldCount = db2.GetCols();

            // raw non-string data
            m_dataTable = (T*)db2.AutoProduceData(fmt, nCount, (char**&)indexTable);

            // string holders for string fields
            m_stringPoolList.push_back(db2.AutoProduceStringsArrayHolders(fmt, (char*)m_dataTable));

            // strings from dbc data
            m_stringPoolList.push_back(db2.AutoProduceStrings(fmt, (char*)m_dataTable, loc));

            // error in dbc file at loading if NULL(0)
            return indexTable != NULL;
        }

        bool LoadStringsFrom(char const* fn, LocaleConstant loc)
        {
            // DBC must already be loaded
            if (!indexTable)
                return false;

            DB2FileLoader db2;

            // load successful?
            if (!db2.Load(fn, fmt))
                return false;

            // strings from locale dbc data
            m_stringPoolList.push_back(db2.AutoProduceStrings(fmt, (char*)m_dataTable, loc));

            return true;
        }

        void Clear()
        {
            if (!indexTable)
                return;

            delete[]((char*)indexTable);
            indexTable = NULL;
            delete[]((char*)m_dataTable);
            m_dataTable = NULL;

            while (!m_stringPoolList.empty())
            {
                delete[] m_stringPoolList.front();
                m_stringPoolList.pop_front();
            }
            nCount = 0;
        }

        void EraseEntry(uint32 id) { indexTable[id] = NULL; }

    private:

        uint32 nCount;
        uint32 fieldCount;
        char const* fmt;
        T** indexTable;
        T* m_dataTable;
        StringPoolList m_stringPoolList;
};

#endif  //DB2STORE_H
