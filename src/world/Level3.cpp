/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org/>
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
 *
 */

#include "StdAfx.h"
#include "ObjectMgr.h"
#include "Master.h"


void ParseBanArgs(char* args, char** BanDuration, char** BanReason)
{
    char* pBanDuration = strchr(args, ' ');
    char* pReason = NULL;
    if (pBanDuration != NULL)
    {
        if (isdigit(*(pBanDuration + 1)))       // this is the duration of the ban
        {
            *pBanDuration = 0;                  // NULL-terminate the first string (character/account/ip)
            ++pBanDuration;                     // point to next arg
            pReason = strchr(pBanDuration + 1, ' ');
            if (pReason != NULL)                // BanReason is OPTIONAL
            {
                *pReason = 0;                   // BanReason was given, so NULL-terminate the duration string
                ++pReason;                      // and point to the ban reason
            }
        }
        else                                    // no duration was given (didn't start with a digit) - so this arg must be ban reason and duration defaults to permanent
        {
            pReason = pBanDuration;
            pBanDuration = NULL;
            *pReason = 0;
            ++pReason;
        }
    }
    *BanDuration = pBanDuration;
    *BanReason = pReason;
}

int32 GetSpellIDFromLink(const char* spelllink)
{
    if (spelllink == NULL)
        return 0;

    const char* ptr = strstr(spelllink, "|Hspell:");
    if (ptr == NULL)
    {
        return 0;
    }

    return atol(ptr + 8);       // spell id is just past "|Hspell:" (8 bytes)
}

bool ChatHandler::HandleWorldPortCommand(const char* args, WorldSession* m_session)
{
    float x, y, z, o = 0;
    uint32 mapid;
    if (sscanf(args, "%u %f %f %f %f", (unsigned int*)&mapid, &x, &y, &z, &o) < 4)
        return false;

    if (x >= _maxX || x <= _minX || y <= _minY || y >= _maxY)
        return false;

    LocationVector vec(x, y, z, o);
    m_session->GetPlayer()->SafeTeleport(mapid, 0, vec);
    return true;
}

bool ChatHandler::HandlePortToCreatureSpawnCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
    {
        return false;
    }

    uint32 spawn_id, spawn_map;
    float spawn_x, spawn_y, spawn_z;

    if (sscanf(args, "%u", (unsigned int*)&spawn_id) != 1)
    {
        return false;
    }
    QueryResult* QR = WorldDatabase.Query("SELECT * FROM creature_spawns WHERE id=%u", spawn_id);
    if (!QR)
    {
        RedSystemMessage(m_session, "No spawn found with id %u.", spawn_id);
        return true;
    }
    spawn_map = QR->Fetch()[2].GetUInt32();
    spawn_x = QR->Fetch()[3].GetFloat();
    spawn_y = QR->Fetch()[4].GetFloat();
    spawn_z = QR->Fetch()[5].GetFloat();
    LocationVector vec(spawn_x, spawn_y, spawn_z);
    m_session->GetPlayer()->SafeTeleport(spawn_map, 0, vec);
    delete QR;
    return true;
}

bool ChatHandler::HandlePortToGameObjectSpawnCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
    {
        return false;
    }

    uint32 spawn_id, spawn_map;
    float spawn_x, spawn_y, spawn_z;

    if (sscanf(args, "%u", (unsigned int*)&spawn_id) != 1)
    {
        return false;
    }
    QueryResult* QR = WorldDatabase.Query("SELECT * FROM gameobject_spawns WHERE id=%u", spawn_id);
    if (!QR)
    {
        RedSystemMessage(m_session, "No spawn found with id %u.", spawn_id);
        return true;
    }
    spawn_map = QR->Fetch()[2].GetUInt32();
    spawn_x = QR->Fetch()[3].GetFloat();
    spawn_y = QR->Fetch()[4].GetFloat();
    spawn_z = QR->Fetch()[5].GetFloat();
    LocationVector vec(spawn_x, spawn_y, spawn_z);
    m_session->GetPlayer()->SafeTeleport(spawn_map, 0, vec);
    delete QR;
    return true;
}

bool ChatHandler::HandleClearCooldownsCommand(const char* args, WorldSession* m_session)
{
    uint32 guid = (uint32)m_session->GetPlayer()->GetSelection();
    Player* plr = getSelectedChar(m_session, true);

    if (!plr)
    {
        plr = m_session->GetPlayer();
        SystemMessage(m_session, "Auto-targeting self.");
    }
    if (!plr) return false;

    if (plr != m_session->GetPlayer())
    {
        sGMLog.writefromsession(m_session, "Cleared all cooldowns for player %s", plr->GetName());
    }

    if (plr->getClass() == WARRIOR)
    {
        plr->ClearCooldownsOnLine(26, guid);
        plr->ClearCooldownsOnLine(256, guid);
        plr->ClearCooldownsOnLine(257, guid);
        BlueSystemMessage(m_session, "Cleared all Warrior cooldowns.");
        return true;
    }
    if (plr->getClass() == PALADIN)
    {
        plr->ClearCooldownsOnLine(594, guid);
        plr->ClearCooldownsOnLine(267, guid);
        plr->ClearCooldownsOnLine(184, guid);
        BlueSystemMessage(m_session, "Cleared all Paladin cooldowns.");
        return true;
    }
    if (plr->getClass() == HUNTER)
    {
        plr->ClearCooldownsOnLine(50, guid);
        plr->ClearCooldownsOnLine(51, guid);
        plr->ClearCooldownsOnLine(163, guid);
        BlueSystemMessage(m_session, "Cleared all Hunter cooldowns.");
        return true;
    }
    if (plr->getClass() == ROGUE)
    {
        plr->ClearCooldownsOnLine(253, guid);
        plr->ClearCooldownsOnLine(38, guid);
        plr->ClearCooldownsOnLine(39, guid);
        BlueSystemMessage(m_session, "Cleared all Rogue cooldowns.");
        return true;
    }
    if (plr->getClass() == PRIEST)
    {
        plr->ClearCooldownsOnLine(56, guid);
        plr->ClearCooldownsOnLine(78, guid);
        plr->ClearCooldownsOnLine(613, guid);
        BlueSystemMessage(m_session, "Cleared all Priest cooldowns.");
        return true;
    }
    if (plr->getClass() == DEATHKNIGHT)
    {
        plr->ClearCooldownsOnLine(770, guid);
        plr->ClearCooldownsOnLine(771, guid);
        plr->ClearCooldownsOnLine(772, guid);
        BlueSystemMessage(m_session, "Cleared all Death Knight cooldowns.");
        return true;
    }
    if (plr->getClass() == SHAMAN)
    {
        plr->ClearCooldownsOnLine(373, guid);
        plr->ClearCooldownsOnLine(374, guid);
        plr->ClearCooldownsOnLine(375, guid);
        BlueSystemMessage(m_session, "Cleared all Shaman cooldowns.");
        return true;
    }
    if (plr->getClass() == MAGE)
    {
        plr->ClearCooldownsOnLine(6, guid);
        plr->ClearCooldownsOnLine(8, guid);
        plr->ClearCooldownsOnLine(237, guid);
        BlueSystemMessage(m_session, "Cleared all Mage cooldowns.");
        return true;
    }
    if (plr->getClass() == WARLOCK)
    {
        plr->ClearCooldownsOnLine(355, guid);
        plr->ClearCooldownsOnLine(354, guid);
        plr->ClearCooldownsOnLine(593, guid);
        BlueSystemMessage(m_session, "Cleared all Warlock cooldowns.");
        return true;
    }
    if (plr->getClass() == DRUID)
    {
        plr->ClearCooldownsOnLine(573, guid);
        plr->ClearCooldownsOnLine(574, guid);
        plr->ClearCooldownsOnLine(134, guid);
        BlueSystemMessage(m_session, "Cleared all Druid cooldowns.");
        return true;
    }
    return true;
}

bool ChatHandler::HandleLearnCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
    {
        return false;
    }

    Player* plr = getSelectedChar(m_session, true);
    if (!plr)
    {
        plr = m_session->GetPlayer();
        SystemMessage(m_session, "Auto-targeting self.");
    }

    if (!plr)
    {
        return false;
    }

    if (stricmp(args, "all") == 0)
    {
        sGMLog.writefromsession(m_session, "taught %s all spells.", plr->GetName());
        SystemMessage(m_session, "Taught %s all spells.", plr->GetName());
        static uint32 spellarray[DRUID + 1][512] =
        {
            { 0 }, // N/A
            { 750, 84614, 84615, 12809, 71, 7376, 20243, 676, 49410, 3411, 12975, 3127, 6572, 2565, 23922, 871, 46968, 23920, 12678, 7386, 355, 50720, 6673, 18499, 2458, 7381, 23881, 23885, 23880, 1161, 845, 469, 12292, 1160, 55694, 5308, 60970, 6544, /*52174,*/ 1134, 20252, /*20253,*/ 5246, 12323, 6552, 85288, 96103, 85384, 97462, 1719, 1464, 34428, 1680, 2457, 21156, 46924, 100, 86346, 1715, 78, 57755, 12294, 76858, 7384, 772, 94009, 20230, 64382, 64380, 88161, 12328, 85388, 6343, 86526, 72, 86479, 0 }, // Warrior
            { 750, 53563, 53652, 4987, 19746, 26573, 54968, 82326, 54428, 879, 19750, 635, 64891, 82327, 20473, 25912, 25914, 2812, 20187, 633, 53651, 7328, 20165, 10326, 85673, 53600, 26017, 20154, 82242, 19891, 31789, 25780, 20911, 84628, 84629, 20164, 86150, 20217, 465, 498, 642, 96231, 20271, 31804, 96231, 20066, 7294, 85285, 84963, 85256, 85696, 31801, 86525, 53385, 24275, 35395, 31935, 49410, 75806, 85043, 86150, 853, 53595, 1044, 1022, 62124, 6940, 1038, 20925, 31884, 19740, 31803, 32223, 86474, 0 }, // Paladin
            { 883, 83242, 83243, 83244, 83245, 82661, 82243, 1978, 34600, 82948, 1494, 19878, 19879, 19880, 19882, 19885, 19883, 19884, 19503, 19306, 19263, 781, 53301, 13813, 13812, 82939, 5384, 1499, 60192, 13809, 82941, 13795, 13797, 82945, 34477, 77767, 51755, 51753, 82654, 93321, 79682, 86100, 2974, 19386, 24131, 3674, 19434, 82928, 3044, 75, 53209, 5116, 20736, 1543, 1130, 53351, 2643, 3045, 34490, 56641, 19801, 19506, 53254, 5118, 13165, 13159, 20043, 1462, 19574, 2641, 6197, 6991, 24406, 19577, 34026, 53271, 136, 982, 1513, 1515, 13481, 34471, 34692, 77769, 674, 86528, 2973, 9077, 8737, 86472, 0 }, // Hunter
            { 1804, 2094, 31224, 2836, 1842, 1725, 16511, 921, 14183, 1784, 57934, 57933, 1856, 53, 74001, 5277, 51723, 1966, 1776, 1766, 76577, 1860, 6770, 51713, 36554, 73981, 84617, 5938, 1752, 79327, 2983, 8676, 1833, 26679, 51722, 32645, 2098, 8647, 703, 408, 1329, /*5374, 27576,*/ 73651, 82245, 74001, 1943, 5171, 79140, 79140, 2842, 86092, 86531, 86476, 0 }, // Rogue
            { 724, 7001, 596, 33076, 33110, 139, 2006, 585, 20711, 453, 589, 586, 528, 605, 34433, 15286, 34914, 34919, 32546, 70772, 34861, 2096, 19236, 64844, /*63544,*/ 87151, 527, 6346, 56131, 56160, 84733, 588, 2061, 101062, 56161, 2060, 47788, 48153, 2050, 14914, 84733, 15237, 23455, 64904, 88625, 88625, 88685, 88684, 8122, 27683, 32379, 15407, 48045, 49821, 8092, 2944, 47585, 1706, 8129, 32375, 47540, 47666, 47750, 47758, 47757, 10060, 62618, 21562, 17, 9484, 73413, 64901, 64843, 73325, 73510, 89745, 86475, 0 }, // Priest
            { 48778, 48707, 51052, 42650, 63560, 82246, 50842, 48982, 47476, 77606, 86524, 85948, 81229, 77575, 50977, 43265, 49224, 49610, 49611, 45462, 52373, 61999, 46584, 55090, 49206, 49194, 50536, 49016, 48265, 45524, 47568, 48266, 49143, 57330, 49184, 49203, 48792, 50887, 45477, 52372, 55610, 49039, 47528, 49020, 3714, 56815, 56816, 81229, 49182, 49500, 49501, 48721, 48263, 45902, 52374, 45529, 49028, 56222, 48743, 49998, 45470, 49410, 55050, 57532, 86061, 73975, 47632, 47633, 52375, 47541, 53428, 86471, 0 }, // Death Knight
            { 8737, 131, 331, 324, 370, 403, 421, 546, 556, 974, 51730, 55533, 26364, 8512, 8232, 3738, 73899, 8017, 30823, 8071, 32175, 32176, 8075, 73680, 86629, 2062, 51523, 51524, 8184, 6196, 51533, 8227, 8024, 8033, 2645, 8177, 60103, 3599, 45284, 8190, 45297, 5675, 16190, 20608, 21169, 61295, 98008, 36936, 8143, 52127, 8042, 5730, 51490, 57994, 2008, 1064, 70809, 2484, 61882, 77478, 2894, 1535, 8349, 8050, 8056, 51514, 51505, 77451, 86529, 66844, 66842, 66843, 76780, 79206, 73899, 73680, 51886, 5394, 8004, 77472, 73920, 87718, 86477, 0 }, // Shaman
            { 10, 66, 120, 122, 116, 118, 130, 475, 543, 1463, 55342, 84721, 28271, 43987, 30449, 31589, 28272, 61025, 61305, 61721, 61780, 61780, 95969, 11426, 45438, 1449, 5143, 7268, 1953, 42955, 2139, 61316, 6117, 12051, 54646, 54648, 30455, 11113, 30482, 34913, 11366, 92315, 2948, 759, 44425, 30451, 1459, 11129, 31661, 2136, 2120, /*88148,*/ 44614, 12654, 12355, 44457, 44461, 12472, 44572, /*71757,*/ 7302, 42208, 12484, 12485, 11958, 89744, 80353, 82731, 82691, 82676, 86473, 0 }, // Mage
            { 126, 172, 348, 689, 698, 702, 980, 710, 755, 603, 687, 693, 1490, 1714, 6789, 6229, 29858, 18540, 30146, 691, 688, 1122, 712, 697, 5697, 1098, 85403, 19483, 20153, /*50590,*/ 50589, /*22703,*/ 29893, 28176, 5784, 71521, 6201, 29886, 54785, 54786, 48018, 48020, 47193, 23161, 89420, 1120, 5782, 50796, 17962, 29722, 91711, 5740, 42223, 5676, 47897, /*47960,*/ 30283, 6353, 1949, 5857, 48181, 5484, 1454, 18094, 18095, 27243, 27285, 86121, 86213, 30108, 91986, 85112, 80240, 18662, 80398, 93375, 93376, 77801, 77799, 79268, 19028, 74434, 86091, 86478, 0 }, // Warlock
            { 0 }, // N/A
            { 99, 339, 770, 774, 740, 783, 779, 33831, 24858, 24905, 16979, 49376, 80861, 33878, 33876, 84736, 24907, 33891, 50516, 61391, 48438, 50334, 81283, 81291, 62078, 5217, 5221, 78675, 80313, 80951, 48505, 50288, 78674, 93402, 61336, 5225, 1079, 40120, 52610, 2912, 467, 6785, 81170, 9005, 18960, 2908, 64801, 8936, 50769, 20484, 2782, 44203, 1066, 5421, 1178, 8998, 1850, 5229, 20719, 16857, 60089, 5209, 5211, 33943, 22842, 6807, 1126, 50464, 33745, 5185, 33763, 22570, 16914, 42231, 2637, 33786, 22812, 29166, 16689, 768, 1082, 5487, 22568, 1822, 5215, 6795, 8921, 86530, 16864, 80964, 80965, 88747, 88751, 77764, 62600, 77758, 77761, 5570, 86470, 0 }, // Druid
        };

        uint32 c = plr->getClass();
        for (uint32 i = 0; spellarray[c][i] != 0; ++i)
        {
            plr->addSpell(spellarray[c][i]);
        }

        static uint32 paladinspellarray[RACE_WORGEN + 1][3] =
        {
            { 0 },        // RACE 0
            { 13819, 23214, 0 },        // HUMAN  Summon Warhorse, Summon Charger
            { 0 },        // ORC
            { 13819, 23214, 0 },        // DWARF  Summon Warhorse, Summon Charger
            { 0 },        // NIGHTELF
            { 0 },        // UNDEAD
            { 69820, 69826, 0 },        // TAUREN Summon Sunwalker Kodo, Summon Great Sunwalker Kodo
            { 0 },        // GNOME
            { 0 },        // TROLL
            { 0 },        // GOBLIN
            { 34767, 34769, 0 },        // BLOODELF  Summon Thalassian Charger, Summon Thalassian Warhorse
            { 73629, 73630, 0 },        // DRAENEI  Summon Exarch's Elekk, Summon Great Exarch's Elekk
            { 0 },       // RACE 12
            { 0 },       // RACE 13
            { 0 },       // RACE 14
            { 0 },       // RACE 15
            { 0 },       // RACE 16
            { 0 },       // RACE 17
            { 0 },       // RACE 18
            { 0 },       // RACE 19
            { 0 },       // RACE 20
            { 0 },       // RACE 21
            { 0 },       // WORGEN
        };

        static uint32 shamanspellarray[RACE_WORGEN + 1][2] =
        {
            { 0 },        // RACE 0
            { 0 },        // HUMAN
            { 2825,  0 },              // ORC Bloodlust
            { 32182, 0 },              // DWARF Heroism
            { 0 },        // NIGHTELF
            { 0 },        // UNDEAD
            { 2825,  0 },              // TAUREN Bloodlust
            { 0 },        // GNOME
            { 2825,  0 },              // TROLL Bloodlust
            { 2825, 0 },               // GOBLIN Bloodlust
            { 0 },        // BLOODELF
            { 32182, 0 },             // DRAENEI Heroism
            { 0 },       // RACE 12
            { 0 },       // RACE 13
            { 0 },       // RACE 14
            { 0 },       // RACE 15
            { 0 },       // RACE 16
            { 0 },       // RACE 17
            { 0 },       // RACE 18
            { 0 },       // RACE 19
            { 0 },       // RACE 20
            { 0 },       // RACE 21
            { 0 },       // WORGEN
        };

        static uint32 magespellarray[RACE_WORGEN + 1][17] =
        {
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // HUMAN
            { 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 53142, 35715, 35717, 49361, 49358, 53140, 88344, 88346, 0 },       // ORC
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // DWARF
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // NIGHTELF
            { 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 53142, 35715, 35717, 49361, 49358, 53140, 88344, 88346, 0 },       // UNDEAD
            { 0 },       // TAUREN
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // GNOME
            { 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 53142, 35715, 35717, 49361, 49358, 53140, 88344, 88346, 0 },       // TROLL
            { 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 53142, 35715, 35717, 49361, 49358, 53140, 88344, 88346, 0 },       // GOBLIN
            { 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 53142, 35715, 35717, 49361, 49358, 53140, 88344, 88346, 0 },       // BLOODELF
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // DRAENEI
            { 0 },       // RACE 12
            { 0 },       // RACE 13
            { 0 },       // RACE 14
            { 0 },       // RACE 15
            { 0 },       // RACE 16
            { 0 },       // RACE 17
            { 0 },       // RACE 18
            { 0 },       // RACE 19
            { 0 },       // RACE 20
            { 0 },       // RACE 21
            { 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 53142, 33690, 33691, 49360, 49359, 53140, 88342, 88345, 0 },       // WORGEN
        };

        uint32 r = plr->getRace();
        switch (c)
        {
            case PALADIN:
                for (uint32 i = 0; paladinspellarray[r][i] != 0; ++i)
                {
                    plr->addSpell(paladinspellarray[r][i]);
                }
                break;
            case MAGE:
                for (uint32 i = 0; magespellarray[r][i] != 0; ++i)
                {
                    plr->addSpell(magespellarray[r][i]);
                }
                break;
            case SHAMAN:
                for (uint32 i = 0; shamanspellarray[r][i] != 0; ++i)
                {
                    plr->addSpell(shamanspellarray[r][i]);
                }
                break;
        }
        return true;
    }

    uint32 spell = atol((char*)args);
    if (spell == 0)
    {
        spell = GetSpellIDFromLink(args);
    }

    SpellEntry* sp = dbcSpell.LookupEntryForced(spell);
    if (!sp)
    {
        SystemMessage(m_session, "Invalid spell %u", spell);
        return true;
    }

    if (!plr->GetSession()->HasGMPermissions() && (sp->eff[0].Effect == SPELL_EFFECT_INSTANT_KILL || sp->eff[1].Effect == SPELL_EFFECT_INSTANT_KILL || sp->eff[2].Effect == SPELL_EFFECT_INSTANT_KILL))
    {
        SystemMessage(m_session, "don't be an idiot and teach players instakill spells. this action has been logged.");
        return true;
    }

    if (plr->HasSpell(spell))     // check to see if char already knows
    {
        std::string OutStr = plr->GetName();
        OutStr += " already knows that spell.";

        SystemMessage(m_session, OutStr.c_str());
        return true;
    }

    plr->addSpell(spell);
    sGMLog.writefromsession(m_session, "Taught %s spell %u", plr->GetName(), spell);
    BlueSystemMessageToPlr(plr, "%s taught you Spell %u", m_session->GetPlayer()->GetName(), spell);
    GreenSystemMessage(m_session, "Taught %s Spell %u", plr->GetName(), spell);

    return true;
}

bool ChatHandler::HandleReviveCommand(const char* args, WorldSession* m_session)
{
    Player* SelectedPlayer = getSelectedChar(m_session, true);
    if (!SelectedPlayer)
        return true;

    SelectedPlayer->SetMovement(MOVE_UNROOT, 1);
    SelectedPlayer->ResurrectPlayer();
    SelectedPlayer->SetHealth(SelectedPlayer->GetMaxHealth());
    SelectedPlayer->SetPower(POWER_TYPE_MANA, SelectedPlayer->GetMaxPower(POWER_TYPE_MANA));
    SelectedPlayer->SetPower(POWER_TYPE_ENERGY, SelectedPlayer->GetMaxPower(POWER_TYPE_ENERGY));


    if (SelectedPlayer != m_session->GetPlayer())
        sGMLog.writefromsession(m_session, "revived player %s", SelectedPlayer->GetName());

    return true;
}

bool ChatHandler::HandleExploreCheatCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    Player* chr = getSelectedChar(m_session);
    if (chr == NULL)
    {
        chr = m_session->GetPlayer();
        SystemMessage(m_session, "Auto-targeting self.");
    }

    if (stricmp(args, "on") == 0)
    {
        SystemMessage(m_session, "%s has explored all zones now.", chr->GetName());
        SystemMessage(m_session, "%s has explored all zones for you.", m_session->GetPlayer()->GetName());
        sGMLog.writefromsession(m_session, "explored all zones for player %s", chr->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        SystemMessage(m_session, "%s has no more explored zones.", chr->GetName());
        SystemMessage(m_session, "%s has hidden all zones from you.", m_session->GetPlayer()->GetName());
        sGMLog.writefromsession(m_session, "hid all zones for player %s", chr->GetName());
    }
    else
        return false;

    for (uint8 i = 0; i < PLAYER_EXPLORED_ZONES_LENGTH; ++i)
    {
        if (stricmp(args, "on") == 0)
        {
            chr->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);
        }
        else if (stricmp(args, "off") == 0)
        {
            chr->RemoveFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);
        }
    }
#ifdef ENABLE_ACHIEVEMENTS
    chr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA); // update
#endif
    return true;
}

bool ChatHandler::HandleBanCharacterCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    char* pCharacter = (char*)args;
    PlayerInfo* pInfo = NULL;
    char* pReason;
    char* pDuration;
    int32 BanTime = 0;
    ParseBanArgs(pCharacter, &pDuration, &pReason);
    if (pDuration != NULL)
    {
        BanTime = GetTimePeriodFromString(pDuration);
        if (BanTime < 0) // if time is 0, ban is permanent
            return false;
    }

    Player* pPlayer = objmgr.GetPlayer(pCharacter, false);
    if (pPlayer == NULL)
    {
        pInfo = objmgr.GetPlayerInfoByName(pCharacter);
        if (pInfo == NULL)
        {
            SystemMessage(m_session, "Player not found.");
            return true;
        }
        SystemMessage(m_session, "Banning player '%s' in database for '%s'.", pCharacter, (pReason == NULL) ? "No reason." : pReason);
        std::string escaped_reason = (pReason == NULL) ? "No reason." : CharacterDatabase.EscapeString(std::string(pReason));
        CharacterDatabase.Execute("UPDATE characters SET banned = %u, banReason = '%s' WHERE guid = %u",
                                  BanTime ? BanTime + (uint32)UNIXTIME : 1, escaped_reason.c_str(), pInfo->guid);
    }
    else
    {
        SystemMessage(m_session, "Banning player '%s' ingame for '%s'.", pCharacter, (pReason == NULL) ? "No reason." : pReason);
        std::string sReason = (pReason == NULL) ? "No Reason." : std::string(pReason);
        uint32 uBanTime = BanTime ? BanTime + (uint32)UNIXTIME : 1;
        pPlayer->SetBanned(uBanTime, sReason);
        pInfo = pPlayer->getPlayerInfo();
    }
    SystemMessage(m_session, "This ban is due to expire %s%s.", BanTime ? "on " : "", BanTime ? ConvertTimeStampToDataTime(BanTime + (uint32)UNIXTIME).c_str() : "Never");

    sGMLog.writefromsession(m_session, "banned %s, reason %s, for %s", pCharacter, (pReason == NULL) ? "No reason" : pReason, BanTime ? ConvertTimeStampToString(BanTime).c_str() : "ever");
    char msg[200];
    snprintf(msg, 200, "%sGM: %s has been banned by %s for %s. Reason: %s", MSG_COLOR_RED, pCharacter, m_session->GetPlayer()->GetName(), BanTime ? ConvertTimeStampToString(BanTime).c_str() : "ever", (pReason == NULL) ? "No reason." : pReason);
    sWorld.SendWorldText(msg, NULL);
    if (sWorld.m_banTable && pInfo)
    {
        CharacterDatabase.Execute("INSERT INTO %s VALUES('%s', '%s', %u, %u, '%s')", sWorld.m_banTable, m_session->GetPlayer()->GetName(), pInfo->name, (uint32)UNIXTIME, (uint32)UNIXTIME + BanTime, (pReason == NULL) ? "No reason." : CharacterDatabase.EscapeString(std::string(pReason)).c_str());
    }

    if (pPlayer)
    {
        SystemMessage(m_session, "Kicking %s.", pPlayer->GetName());
        pPlayer->Kick();
    }
    return true;
}

bool ChatHandler::HandleBanAllCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    Player* pBanned;
    std::string pAcc;
    std::string pIP;
    std::string pArgs = args;
    char* pCharacter = (char*)args;
    char* pReason;
    char* pDuration;
    ParseBanArgs(pCharacter, &pDuration, &pReason);
    int32 BanTime = 0;
    if (pDuration != NULL)
    {
        BanTime = GetTimePeriodFromString(pDuration);
        if (BanTime < 0)
            return false;
    }
    pBanned = objmgr.GetPlayer(pCharacter, false);
    if (!pBanned || !pBanned->IsInWorld())
    {
        RedSystemMessage(m_session, "Player \'%s\' is not online or does not exists!", pCharacter);
        return true;
    }
    if (pBanned == m_session->GetPlayer())
    {
        RedSystemMessage(m_session, "You cannot ban yourself!");
        return true;
    }
    if (pBanned->GetSession() == NULL)
    {
        RedSystemMessage(m_session, "Player does not have a session!");
        return true;
    }
    if (pBanned->GetSession()->GetSocket() == NULL)
    {
        RedSystemMessage(m_session, "Player does not have a socket!");
        return true;
    }
    pAcc = pBanned->GetSession()->GetAccountName();
    pIP = pBanned->GetSession()->GetSocket()->GetRemoteIP();
    if (pIP == m_session->GetSocket()->GetRemoteIP())           //This check is there incase a gm tries to ban someone on their LAN etc.
    {
        RedSystemMessage(m_session, "That player has the same IP as you - ban failed");
        return true;
    }

    //Checks complete. time to fire it up?
    HandleBanCharacterCommand(pArgs.c_str(), m_session);
    char pIPCmd[256];
    snprintf(pIPCmd, 254, "%s %s %s", pIP.c_str(), pDuration, pReason);
    HandleIPBanCommand(pIPCmd, m_session);
    char pAccCmd[256];
    snprintf(pAccCmd, 254, "%s %s %s", pAcc.c_str(), pDuration, pReason);
    HandleAccountBannedCommand((const char*)pAccCmd, m_session);
    //GreenSystemMessage(m_session,"Successfully banned player %s with ip %s and account %s",pCharacter,pIP.c_str(),pAcc.c_str());
    return true;
}

bool ChatHandler::HandleUnBanCharacterCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    char Character[255];
    if (sscanf(args, "%s", Character) != 1)
    {
        RedSystemMessage(m_session, "A character name and reason is required.");
        return true;
    }

    // Check if player is in world.
    Player* pPlayer = ObjectMgr::getSingleton().GetPlayer(Character, false);
    if (pPlayer != 0)
    {
        GreenSystemMessage(m_session, "Unbanned player %s ingame.", pPlayer->GetName());
        pPlayer->UnSetBanned();
    }
    else
    {
        GreenSystemMessage(m_session, "Player %s not found ingame.", Character);
    }

    // Ban in database
    CharacterDatabase.Execute("UPDATE characters SET banned = 0 WHERE name = '%s'", CharacterDatabase.EscapeString(std::string(Character)).c_str());

    SystemMessage(m_session, "Unbanned character %s in database.", Character);
    sGMLog.writefromsession(m_session, "unbanned %s", Character);
    return true;
}

bool ChatHandler::HandleAddSkillCommand(const char* args, WorldSession* m_session)
{
    char buf[256];
    Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());

    if (!target)
    {
        SystemMessage(m_session, "Select A Player first.");
        return true;
    }

    uint32 skillline;
    uint16 cur, max;

    char* pSkillline = strtok((char*)args, " ");
    if (!pSkillline)
        return false;

    char* pCurrent = strtok(NULL, " ");
    if (!pCurrent)
        return false;

    char* pMax = strtok(NULL, " ");
    if (!pMax)
        return false;

    skillline = (uint32)atol(pSkillline);
    cur = (uint16)atol(pCurrent);
    max = (uint16)atol(pMax);

    target->_AddSkillLine(skillline, cur, max);

    snprintf(buf, 256, "SkillLine: %u CurrentValue %u Max Value %u Added.", (unsigned int)skillline, (unsigned int)cur, (unsigned int)max);
    sGMLog.writefromsession(m_session, "added skill line %u (%u/%u) to %s", skillline, cur, max, target->GetName());
    SystemMessage(m_session, buf);

    return true;
}

struct UnitFlagNames
{
    uint32 Flag;
    const char* Name;
};

struct UnitDynFlagNames
{
    uint32 Flag;
    const char* Name;
};

static const char* POWERTYPE[] =
{
    "Mana",
    "Rage",
    "Focus",
    "Energy",
    "Happiness",
    "Runes",
    "Runic Power"
};

static const UnitFlagNames UnitFlagToName[] =
{
    { UNIT_FLAG_SERVER_CONTROLLED, "UNIT_FLAG_SERVER_CONTROLLED" },
    { UNIT_FLAG_NOT_ATTACKABLE_2, "UNIT_FLAG_NOT_ATTACKABLE_2" },
    { UNIT_FLAG_LOCK_PLAYER, "UNIT_FLAG_LOCK_PLAYER" },
    { UNIT_FLAG_PVP_ATTACKABLE, "UNIT_FLAG_PVP_ATTACKABLE" },
    { UNIT_FLAG_UNKNOWN_5, "UNIT_FLAG_UNKNOWN_5" },
    { UNIT_FLAG_NO_REAGANT_COST, "UNIT_FLAG_NO_REAGANT_COST" },
    { UNIT_FLAG_PLUS_MOB, "UNIT_FLAG_PLUS_MOB" },
    { UNIT_FLAG_UNKNOWN_8, "UNIT_FLAG_UNKNOWN_8" },
    { UNIT_FLAG_NOT_ATTACKABLE_9, "UNIT_FLAG_NOT_ATTACKABLE_9" },
    { UNIT_FLAG_UNKNOWN_10, "UNIT_FLAG_UNKNOWN_10" },
    { UNIT_FLAG_LOOTING, "UNIT_FLAG_LOOTING" },
    { UNIT_FLAG_SELF_RES, "UNIT_FLAG_SELF_RES" },
    { UNIT_FLAG_PVP, "UNIT_FLAG_PVP" },
    { UNIT_FLAG_SILENCED, "UNIT_FLAG_SILENCED" },
    { UNIT_FLAG_DEAD, "UNIT_FLAG_DEAD" },
    { UNIT_FLAG_UNKNOWN_16, "UNIT_FLAG_UNKNOWN_16" },
    { UNIT_FLAG_ALIVE, "UNIT_FLAG_ALIVE" },
    { UNIT_FLAG_PACIFIED, "UNIT_FLAG_PACIFIED" },
    { UNIT_FLAG_STUNNED, "UNIT_FLAG_STUNNED" },
    { UNIT_FLAG_COMBAT, "UNIT_FLAG_COMBAT" },
    { UNIT_FLAG_MOUNTED_TAXI, "UNIT_FLAG_MOUNTED_TAXI" },
    { UNIT_FLAG_DISARMED, "UNIT_FLAG_DISARMED" },
    { UNIT_FLAG_CONFUSED, "UNIT_FLAG_CONFUSED" },
    { UNIT_FLAG_FLEEING, "UNIT_FLAG_FLEEING" },
    { UNIT_FLAG_PLAYER_CONTROLLED_CREATURE, "UNIT_FLAG_PLAYER_CONTROLLED_CREATURE" },
    { UNIT_FLAG_NOT_SELECTABLE, "UNIT_FLAG_NOT_SELECTABLE" },
    { UNIT_FLAG_SKINNABLE, "UNIT_FLAG_SKINNABLE" },
    { UNIT_FLAG_MOUNT, "UNIT_FLAG_MOUNT" },
    { UNIT_FLAG_UNKNOWN_29, "UNIT_FLAG_UNKNOWN_29" },
    { UNIT_FLAG_FEIGN_DEATH, "UNIT_FLAG_FEIGN_DEATH" },
    { UNIT_FLAG_UNKNOWN_31, "UNIT_FLAG_UNKNOWN_31" },
    { UNIT_FLAG_UNKNOWN_32, "UNIT_FLAG_UNKNOWN_32" }
};

static uint32 numflags = sizeof(UnitFlagToName) / sizeof(UnitFlagNames);

static const UnitDynFlagNames UnitDynFlagToName[] =
{
    { U_DYN_FLAG_LOOTABLE, "U_DYN_FLAG_LOOTABLE" },
    { U_DYN_FLAG_UNIT_TRACKABLE, "U_DYN_FLAG_UNIT_TRACKABLE" },
    { U_DYN_FLAG_TAGGED_BY_OTHER, "U_DYN_FLAG_TAGGED_BY_OTHER" },
    { U_DYN_FLAG_TAPPED_BY_PLAYER, "U_DYN_FLAG_TAPPED_BY_PLAYER" },
    { U_DYN_FLAG_PLAYER_INFO, "U_DYN_FLAG_PLAYER_INFO" },
    { U_DYN_FLAG_DEAD, "U_DYN_FLAG_DEAD" }
};

static uint32 numdynflags = sizeof(UnitDynFlagToName) / sizeof(UnitDynFlagNames);

static const char* GENDER[] =
{
    "male",
    "female",
    "neutral"
};

static const char* CLASS[] =
{
    "invalid 0",
    "warrior",
    "paladin",
    "hunter",
    "rogue",
    "priest",
    "deathknight",
    "shaman",
    "mage",
    "warlock",
    "invalid 10",
    "druid"
};

static const char* SHEATSTATE[] =
{
    "none",
    "melee",
    "Ranged"
};

struct UnitPvPFlagNames
{
    uint32 Flag;
    const char* Name;
};

static const UnitPvPFlagNames UnitPvPFlagToName[] =
{
    { U_FIELD_BYTES_FLAG_PVP, "U_FIELD_BYTES_FLAG_PVP" },
    { U_FIELD_BYTES_FLAG_FFA_PVP, "U_FIELD_BYTES_FLAG_FFA_PVP" },
    { U_FIELD_BYTES_FLAG_SANCTUARY, "U_FIELD_BYTES_FLAG_SANCTUARY" }
};

static const uint32 numpvpflags = sizeof(UnitPvPFlagToName) / sizeof(UnitPvPFlagNames);

struct PetFlagNames
{
    uint32 Flag;
    const char* Name;
};

static const PetFlagNames PetFlagToName[] =
{
    { UNIT_CAN_BE_RENAMED, "UNIT_CAN_BE_RENAMED" },
    { UNIT_CAN_BE_ABANDONED, "UNIT_CAN_BE_ABANDONED" }
};

static const uint32 numpetflags = sizeof(PetFlagToName) / sizeof(PetFlagNames);

bool ChatHandler::HandleNpcInfoCommand(const char* args, WorldSession* m_session)
{

    uint32 guid = Arcemu::Util::GUID_LOPART(m_session->GetPlayer()->GetSelection());
    Creature* crt = getSelectedCreature(m_session);
    if (!crt)
        return false;
    BlueSystemMessage(m_session, "Showing creature info for %s", crt->GetCreatureInfo()->Name);
    SystemMessage(m_session, "GUID: %d", guid);
    SystemMessage(m_session, "Faction: %d", crt->GetFaction());
    SystemMessage(m_session, "Phase: %u", crt->GetPhase());
    {
        std::string s = "";
        if (crt->isBattleMaster())
            s.append(" (Battlemaster)");
        if (crt->isTrainer())
            s.append(" (Trainer)");
        if (crt->isProf())
            s.append(" (Profession Trainer)");
        if (crt->isQuestGiver())
            s.append(" (Quests)");
        if (crt->isGossip())
            s.append(" (Gossip)");
        if (crt->isTaxi())
            s.append(" (Taxi)");
        if (crt->isCharterGiver())
            s.append(" (Charter)");
        if (crt->isGuildBank())
            s.append(" (Guild Bank)");
        if (crt->isSpiritHealer())
            s.append(" (Spirit Healer)");
        if (crt->isInnkeeper())
            s.append(" (Innkeeper)");
        if (crt->isTabardDesigner())
            s.append(" (Tabard Designer)");
        if (crt->isAuctioner())
            s.append(" (Auctioneer)");
        if (crt->isStableMaster())
            s.append(" (Stablemaster)");
        if (crt->isArmorer())
            s.append(" (Armorer)");

        SystemMessage(m_session, "NPCFlags: %d%s", crt->GetUInt32Value(UNIT_NPC_FLAGS), s.c_str());
    }
    SystemMessage(m_session, "DisplayID: %u", crt->GetDisplayId());
    SystemMessage(m_session, "VehicleID: %u", crt->GetProto()->vehicleid);

    if (crt->m_faction)
        SystemMessage(m_session, "Combat Support: 0x%.3X", crt->m_faction->FriendlyMask);
    SystemMessage(m_session, "Health (cur/max): %d/%d", crt->GetHealth(), crt->GetMaxHealth());

    uint32 powertype = crt->GetPowerType();
    if (powertype <= 6)
    {
        SystemMessage(m_session, "Powertype: %s", POWERTYPE[powertype]);
        SystemMessage(m_session, "Power (cur/max): %d/%d", crt->GetPower(powertype), crt->GetMaxPower(powertype));
    }

    SystemMessage(m_session, "Armor/Holy/Fire/Nature/Frost/Shadow/Arcane");
    SystemMessage(m_session, "%d/%d/%d/%d/%d/%d/%d", crt->GetResistance(SCHOOL_NORMAL), crt->GetResistance(SCHOOL_HOLY), crt->GetResistance(SCHOOL_FIRE), crt->GetResistance(SCHOOL_NATURE), crt->GetResistance(SCHOOL_FROST), crt->GetResistance(SCHOOL_SHADOW), crt->GetResistance(SCHOOL_ARCANE));
    SystemMessage(m_session, "Damage (min/max): %f/%f", crt->GetMinDamage(), crt->GetMaxDamage());

    ColorSystemMessage(m_session, MSG_COLOR_RED, "Entry ID: %d", crt->GetEntry());
    ColorSystemMessage(m_session, MSG_COLOR_RED, "Spawn ID: %d", crt->GetSQL_id());
    // show byte
    std::stringstream sstext;
    uint32 theBytes = crt->GetUInt32Value(UNIT_FIELD_BYTES_0);
    sstext << "UNIT_FIELD_BYTES_0 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
    sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\n';

    theBytes = crt->GetUInt32Value(UNIT_FIELD_BYTES_1);
    sstext << "UNIT_FIELD_BYTES_1 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
    sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\n';

    theBytes = crt->GetUInt32Value(UNIT_FIELD_BYTES_2);
    sstext << "UNIT_FIELD_BYTES_2 are " << uint16((uint8)theBytes & 0xFF) << " " << uint16((uint8)(theBytes >> 8) & 0xFF) << " ";
    sstext << uint16((uint8)(theBytes >> 16) & 0xFF) << " " << uint16((uint8)(theBytes >> 24) & 0xFF) << '\0';
    SendMultilineMessage(m_session, sstext.str().c_str());

    uint8 gender = crt->getGender();
    if (gender <= 2)
        SystemMessage(m_session, "Gender: %s", GENDER[gender]);
    else
        SystemMessage(m_session, "Gender: invalid %u", gender);

    uint8 crclass = crt->getClass();
    if (crclass <= 11)
        SystemMessage(m_session, "Class: %s", CLASS[crclass]);
    else
        SystemMessage(m_session, "Class: invalid %u", crclass);

    SystemMessage(m_session, "Free pet talent points: %u", crt->GetByte(UNIT_FIELD_BYTES_1, 1));

    uint8 sheat = crt->GetByte(UNIT_FIELD_BYTES_2, 0);
    if (sheat <= 2)
        SystemMessage(m_session, "Sheat state: %s", SHEATSTATE[sheat]);

    uint8 pvpflags = crt->GetByte(UNIT_FIELD_BYTES_2, 1);

    SystemMessage(m_session, "PvP flags: %u", pvpflags);

    for (uint32 i = 0; i < numpvpflags; i++)
        if ((pvpflags & UnitPvPFlagToName[i].Flag) != 0)
            SystemMessage(m_session, "%s", UnitPvPFlagToName[i].Name);

    uint8 petflags = crt->GetByte(UNIT_FIELD_BYTES_2, 2);

    SystemMessage(m_session, "Pet flags: %u", petflags);

    for (uint32 i = 0; i < numpetflags; i++)
        if ((petflags & PetFlagToName[i].Flag) != 0)
            SystemMessage(m_session, "%s", PetFlagToName[i].Name);

    if (crt->CombatStatus.IsInCombat())
        SystemMessage(m_session, "Creature is in combat");
    else
        SystemMessage(m_session, "Creature is NOT in combat");

    Unit* owner = NULL;
    if (crt->IsSummon())
        owner = static_cast< Summon* >(crt)->GetOwner();

    if (owner != NULL)
    {
        if (owner->IsPlayer())
            SystemMessage(m_session, "Owner is a %s", "player");
        if (owner->IsPet())
            SystemMessage(m_session, "Owner is a %s", "pet");
        if (owner->IsCreature())
            SystemMessage(m_session, "Owner is a %s", "creature");
    }

    SystemMessage(m_session, "Creator GUID: %u", Arcemu::Util::GUID_LOPART(crt->GetCreatedByGUID()));
    SystemMessage(m_session, "Summoner GUID: %u", Arcemu::Util::GUID_LOPART(crt->GetSummonedByGUID()));
    SystemMessage(m_session, "Charmer GUID: %u", Arcemu::Util::GUID_LOPART(crt->GetCharmedByGUID()));
    SystemMessage(m_session, "Creator Spell: %u", Arcemu::Util::GUID_LOPART(crt->GetCreatedBySpell()));



    uint32 unitflags = crt->GetUInt32Value(UNIT_FIELD_FLAGS);

    SystemMessage(m_session, "Unit flags: %u", unitflags);

    for (uint32 i = 0; i < numflags; i++)
        if ((unitflags & UnitFlagToName[i].Flag) != 0)
            SystemMessage(m_session, "%s", UnitFlagToName[i].Name);

    uint32 dynflags = crt->GetUInt32Value(UNIT_DYNAMIC_FLAGS);
    SystemMessage(m_session, "Unit dynamic flags: %u", dynflags);

    for (uint32 i = 0; i < numdynflags; i++)
        if ((dynflags & UnitDynFlagToName[i].Flag) != 0)
            SystemMessage(m_session, "%s", UnitDynFlagToName[i].Name);

    return true;
}

bool ChatHandler::HandleCreaturePhaseCommand(const char* args, WorldSession* m_session)
{
    char* sPhase = strtok((char*)args, " ");
    if (!sPhase)
        return false;

    uint32 newphase = atoi(sPhase);

    bool Save = false;
    char* pSave = strtok(NULL, " ");
    if (pSave)
        Save = (atoi(pSave) > 0 ? true : false);

    Creature* crt = getSelectedCreature(m_session);
    if (!crt) return false;

    crt->Phase(PHASE_SET, newphase);
    if (crt->m_spawn)
        crt->m_spawn->phase = newphase;
    //VLack: at this point we don't care if it has a spawn or not, as it gets one for sure in SaveToDB, that's why we don't return here from within an else statement.
    //I made this comment in case someone compares this code with the HandleGOPhaseCommand code where we have to have a spawn to be able to save it.

    // Save it to the database.
    if (Save)
    {
        crt->SaveToDB();
        crt->m_loadedFromDB = true;
    }

    sGMLog.writefromsession(m_session, "phased creature with entry %u to %u", crt->GetEntry(), newphase);

    return true;
}

bool ChatHandler::HandleIncreaseWeaponSkill(const char* args, WorldSession* m_session)
{
    char* pMin = strtok((char*)args, " ");
    uint32 cnt = 0;
    if (!pMin)
        cnt = 1;
    else
        cnt = atol(pMin);

    Player* pr = getSelectedChar(m_session, true);

    uint32 SubClassSkill = 0;
    if (!pr) pr = m_session->GetPlayer();
    if (!pr) return false;
    Item* it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_MAINHAND);
    ItemPrototype* proto = NULL;
    if (!it)
        it = pr->GetItemInterface()->GetInventoryItem(EQUIPMENT_SLOT_RANGED);
    if (it)
        proto = it->GetProto();
    if (proto)
    {
        switch (proto->SubClass)
        {
            // Weapons
            case 0:    // 1 handed axes
                SubClassSkill = SKILL_AXES;
                break;
            case 1:    // 2 handed axes
                SubClassSkill = SKILL_2H_AXES;
                break;
            case 2:    // bows
                SubClassSkill = SKILL_BOWS;
                break;
            case 3:    // guns
                SubClassSkill = SKILL_GUNS;
                break;
            case 4:    // 1 handed mace
                SubClassSkill = SKILL_MACES;
                break;
            case 5:    // 2 handed mace
                SubClassSkill = SKILL_2H_MACES;
                break;
            case 6:    // polearms
                SubClassSkill = SKILL_POLEARMS;
                break;
            case 7: // 1 handed sword
                SubClassSkill = SKILL_SWORDS;
                break;
            case 8: // 2 handed sword
                SubClassSkill = SKILL_2H_SWORDS;
                break;
            case 9: // obsolete
                SubClassSkill = 136;
                break;
            case 10: //1 handed exotic
                SubClassSkill = 136;
                break;
            case 11: // 2 handed exotic
                SubClassSkill = 0;
                break;
            case 12: // fist
                SubClassSkill = SKILL_FIST_WEAPONS;
                break;
            case 13: // misc
                SubClassSkill = 0;
                break;
            case 15: // daggers
                SubClassSkill = SKILL_DAGGERS;
                break;
            case 16: // thrown
                SubClassSkill = SKILL_THROWN;
                break;
            case 18: // crossbows
                SubClassSkill = SKILL_CROSSBOWS;
                break;
            case 19: // wands
                SubClassSkill = SKILL_WANDS;
                break;
            case 20: // fishing
                SubClassSkill = SKILL_FISHING;
                break;
        }
    }
    else
    {
        SubClassSkill = 162;
    }

    if (!SubClassSkill)
    {
        RedSystemMessage(m_session, "Can't find skill ID :-/");
        return false;
    }

    uint32 skill = SubClassSkill;

    BlueSystemMessage(m_session, "Modifying skill line %d. Advancing %d times.", skill, cnt);
    sGMLog.writefromsession(m_session, "increased weapon skill (%u) of %s by %u", skill, pr->GetName(), cnt);

    if (!pr->_HasSkillLine(skill))
    {
        SystemMessage(m_session, "Does not have skill line, adding.");
        pr->_AddSkillLine(skill, 1, 525);
    }
    else
    {
        pr->_AdvanceSkillLine(skill, cnt);
    }
    return true;
}


bool ChatHandler::HandleResetTalentsCommand(const char* args, WorldSession* m_session)
{
    Player* plr = this->getSelectedChar(m_session);
    if (!plr)
        return true;

    plr->Reset_Talents();

    SystemMessage(m_session, "Reset talents of %s.", plr->GetName());
    BlueSystemMessageToPlr(plr, "%s reset all your talents.", m_session->GetPlayer()->GetName());
    sGMLog.writefromsession(m_session, "reset talents of %s", plr->GetName());
    return true;
}

bool ChatHandler::HandleResetSpellsCommand(const char* args, WorldSession* m_session)
{
    Player* plr = this->getSelectedChar(m_session);
    if (!plr)
        return true;

    plr->Reset_Spells();

    SystemMessage(m_session, "Reset spells of %s to level 1.", plr->GetName());
    BlueSystemMessageToPlr(plr, "%s reset all your spells to starting values.", m_session->GetPlayer()->GetName());
    sGMLog.writefromsession(m_session, "reset spells of %s", plr->GetName());
    return true;
}

bool ChatHandler::HandleGetTransporterTime(const char* args, WorldSession* m_session)
{
    //Player* plyr = m_session->GetPlayer();
    Creature* crt = getSelectedCreature(m_session, false);
    if (crt == NULL)
        return false;

    WorldPacket data(SMSG_ATTACKERSTATEUPDATE, 1000);
    data << uint32(0x00000102);
    data << crt->GetNewGUID();
    data << m_session->GetPlayer()->GetNewGUID();

    data << uint32(6);
    data << uint8(1);
    data << uint32(1);
    data << uint32(0x40c00000);
    data << uint32(6);
    data << uint32(0);
    data << uint32(0);
    data << uint32(1);
    data << uint32(0x000003e8);
    data << uint32(0);
    data << uint32(0);
    m_session->SendPacket(&data);
    return true;
}

bool ChatHandler::HandleRemoveAurasCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return false;

    BlueSystemMessage(m_session, "Removing all auras...");
    for (uint32 i = MAX_REMOVABLE_AURAS_START; i < MAX_REMOVABLE_AURAS_END; ++i)
    {
        if (plr->m_auras[i] != 0) plr->m_auras[i]->Remove();
    }
    if (plr != m_session->GetPlayer())
        sGMLog.writefromsession(m_session, "Removed all of %s's auras.", plr->GetName());
    return true;
}

bool ChatHandler::HandleRemoveRessurectionSickessAuraCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return false;

    BlueSystemMessage(m_session, "Removing resurrection sickness...");
    plr->RemoveAura(15007);
    if (plr != m_session->GetPlayer())
        sGMLog.writefromsession(m_session, "Removed resurrection sickness from %s", plr->GetName());
    return true;
}

bool ChatHandler::HandleParalyzeCommand(const char* args, WorldSession* m_session)
{
    //Player* plr = getSelectedChar(m_session, true);
    //if (!plr) return false;
    Unit* plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
    if (!plr || !plr->IsPlayer())
    {
        RedSystemMessage(m_session, "Invalid target.");
        return true;
    }

    BlueSystemMessage(m_session, "Rooting target.");
    BlueSystemMessageToPlr(static_cast< Player* >(plr), "You have been rooted by %s.", m_session->GetPlayer()->GetName());
    sGMLog.writefromsession(m_session, "rooted player %s", static_cast< Player* >(plr)->GetName());
    WorldPacket data;
    data.Initialize(SMSG_FORCE_MOVE_ROOT);
    data << plr->GetNewGUID();
    data << uint32(1);

    plr->SendMessageToSet(&data, true);
    return true;
}

bool ChatHandler::HandleUnParalyzeCommand(const char* args, WorldSession* m_session)
{
    //Player* plr = getSelectedChar(m_session, true);
    //if (!plr) return false;
    Unit* plr = m_session->GetPlayer()->GetMapMgr()->GetUnit(m_session->GetPlayer()->GetSelection());
    if (!plr || !plr->IsPlayer())
    {
        RedSystemMessage(m_session, "Invalid target.");
        return true;
    }

    BlueSystemMessage(m_session, "Unrooting target.");
    BlueSystemMessageToPlr(static_cast< Player* >(plr), "You have been unrooted by %s.", m_session->GetPlayer()->GetName());
    sGMLog.writefromsession(m_session, "unrooted player %s", static_cast< Player* >(plr)->GetName());
    WorldPacket data;
    data.Initialize(SMSG_FORCE_MOVE_UNROOT);
    data << plr->GetNewGUID();
    data << uint32(5);

    plr->SendMessageToSet(&data, true);
    return true;
}

bool ChatHandler::HandleSetMotdCommand(const char* args, WorldSession* m_session)
{
    if (!args || strlen(args) < 2)
    {
        RedSystemMessage(m_session, "You must specify a message.");
        return true;
    }

    GreenSystemMessage(m_session, "Motd has been set to: %s", args);
    World::getSingleton().SetMotd(args);
    sGMLog.writefromsession(m_session, "Set MOTD to %s", args);
    return true;
}

bool ChatHandler::HandleAddItemSetCommand(const char* args, WorldSession* m_session)
{
    uint32 setid = (args ? atoi(args) : 0);
    if (!setid)
    {
        RedSystemMessage(m_session, "You must specify a setid.");
        return true;
    }

    Player* chr = getSelectedChar(m_session);
    if (chr == NULL)
    {
        RedSystemMessage(m_session, "Unable to select character.");
        return true;
    }

    ItemSetEntry* entry = dbcItemSet.LookupEntryForced(setid);
    std::list<ItemPrototype*>* l = objmgr.GetListForItemSet(setid);
    if (!entry || !l)
    {
        RedSystemMessage(m_session, "Invalid item set.");
        return true;
    }
    //const char* setname = sItemSetStore.LookupString(entry->name);
    BlueSystemMessage(m_session, "Searching item set %u...", setid);
    uint32 start = getMSTime();
    sGMLog.writefromsession(m_session, "used add item set command, set %u, target %s", setid, chr->GetName());
    for (std::list<ItemPrototype*>::iterator itr = l->begin(); itr != l->end(); ++itr)
    {
        Item* itm = objmgr.CreateItem((*itr)->ItemId, m_session->GetPlayer());
        if (!itm) continue;
        if (itm->GetProto()->Bonding == ITEM_BIND_ON_PICKUP)
        {
            if (itm->GetProto()->Flags & ITEM_FLAG_ACCOUNTBOUND) // don't "Soulbind" account-bound items
                itm->AccountBind();
            else
                itm->SoulBind();
        }

        if (!chr->GetItemInterface()->AddItemToFreeSlot(itm))
        {
            m_session->SendNotification("No free slots left!");
            itm->DeleteMe();
            return true;
        }
        else
        {
            //SystemMessage(m_session, "Added item: %s [%u]", (*itr)->Name1, (*itr)->ItemId);
            SlotResult* le = chr->GetItemInterface()->LastSearchResult();
            chr->SendItemPushResult(false, true, false, true, le->ContainerSlot, le->Slot, 1, itm->GetEntry(), itm->GetItemRandomSuffixFactor(), itm->GetItemRandomPropertyId(), itm->GetStackCount());
        }
    }
    GreenSystemMessage(m_session, "Added set to inventory complete. Time: %u ms", getMSTime() - start);
    return true;
}

bool ChatHandler::HandleCastTimeCheatCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->CastTimeCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->CastTimeCheat = true;
        BlueSystemMessage(m_session, "activated the cast time cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "activated the cast time cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->CastTimeCheat = false;
        BlueSystemMessage(m_session, "deactivated the cast time cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "deactivated the cast time cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "god cast time on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleCooldownCheatCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->CooldownCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->CooldownCheat = true;
        //best case we could simply iterate through cooldowns or create a special function ...
        SpellSet::const_iterator itr = plyr->mSpells.begin();
        for (; itr != plyr->mSpells.end(); ++itr)
            plyr->ClearCooldownForSpell((*itr));
        BlueSystemMessage(m_session, "activated the cooldown cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "activated the cooldown cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->CooldownCheat = false;
        BlueSystemMessage(m_session, "deactivated the cooldown cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "deactivated the cooldown cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "cooldown cheat on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleGodModeCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->GodModeCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->GodModeCheat = true;
        BlueSystemMessage(m_session, "Activating the god mode cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "%s activated the god mode cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->GodModeCheat = false;
        BlueSystemMessage(m_session, "Deactivating the god mode cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "%s deactivated the god mode cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "god mode cheat on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandlePowerCheatCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->PowerCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->PowerCheat = true;
        BlueSystemMessage(m_session, "activated the power cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "activated the power cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->PowerCheat = false;
        BlueSystemMessage(m_session, "deactivated the power cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "deactivated the power cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "power cheat on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleShowCheatsCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    uint32 active = 0, inactive = 0;
#define print_cheat_status(CheatName, CheatVariable) SystemMessage(m_session, "%s%s: %s%s", MSG_COLOR_LIGHTBLUE, CheatName, \
        CheatVariable ? MSG_COLOR_LIGHTRED : MSG_COLOR_GREEN, CheatVariable ? "Active" : "Inactive");  \
        if (CheatVariable) \
        active++; \
            else \
        inactive++;

    GreenSystemMessage(m_session, "Showing cheat status for: %s", plyr->GetName());
    print_cheat_status("Cooldown", plyr->CooldownCheat);
    print_cheat_status("CastTime", plyr->CastTimeCheat);
    print_cheat_status("GodMode", plyr->GodModeCheat);
    print_cheat_status("Power", plyr->PowerCheat);
    print_cheat_status("Fly", plyr->FlyCheat);
    print_cheat_status("AuraStack", plyr->AuraStackCheat);
    print_cheat_status("ItemStack", plyr->ItemStackCheat);
    print_cheat_status("TriggerPass", plyr->TriggerpassCheat);
    if (plyr->GetSession() && plyr->GetSession()->CanUseCommand('a'))
    {
        print_cheat_status("GM Invisibility", plyr->m_isGmInvisible);
        print_cheat_status("GM Invincibility", plyr->bInvincible);
    }
    SystemMessage(m_session, "%u cheats active, %u inactive.", active, inactive);

#undef print_cheat_status

    return true;
}

bool ChatHandler::HandleFlyCommand(const char* args, WorldSession* m_session)
{
    Player* chr = getSelectedChar(m_session);

    if (!chr)
        chr = m_session->GetPlayer();

    if (!*args)
    {
        if (chr->FlyCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        WorldPacket fly(SMSG_MOVE_SET_CAN_FLY, 13);
        chr->m_setflycheat = true;
        ObjectGuid guid = chr->GetGUID();
        fly.WriteByteMask(guid[1]);
        fly.WriteByteMask(guid[6]);
        fly.WriteByteMask(guid[5]);
        fly.WriteByteMask(guid[0]);
        fly.WriteByteMask(guid[7]);
        fly.WriteByteMask(guid[4]);
        fly.WriteByteMask(guid[2]);
        fly.WriteByteMask(guid[3]);
        fly.WriteByteSeq(guid[6]);
        fly.WriteByteSeq(guid[3]);
        fly << uint32(2); // ?
        fly.WriteByteSeq(guid[2]);
        fly.WriteByteSeq(guid[1]);
        fly.WriteByteSeq(guid[4]);
        fly.WriteByteSeq(guid[7]);
        fly.WriteByteSeq(guid[0]);
        fly.WriteByteSeq(guid[5]);
        chr->SendMessageToSet(&fly, true);
        BlueSystemMessage(chr->GetSession(), "Flying mode enabled.");
        if (chr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "enabled flying mode for %s", chr->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        WorldPacket fly(SMSG_MOVE_UNSET_CAN_FLY, 13);
        chr->m_setflycheat = false;
        ObjectGuid guid = chr->GetGUID();
        fly.WriteByteMask(guid[1]);
        fly.WriteByteMask(guid[4]);
        fly.WriteByteMask(guid[2]);
        fly.WriteByteMask(guid[5]);
        fly.WriteByteMask(guid[0]);
        fly.WriteByteMask(guid[3]);
        fly.WriteByteMask(guid[6]);
        fly.WriteByteMask(guid[7]);
        fly.WriteByteSeq(guid[4]);
        fly.WriteByteSeq(guid[6]);
        fly << uint32(5); // ?
        fly.WriteByteSeq(guid[1]);
        fly.WriteByteSeq(guid[0]);
        fly.WriteByteSeq(guid[2]);
        fly.WriteByteSeq(guid[3]);
        fly.WriteByteSeq(guid[5]);
        fly.WriteByteSeq(guid[7]);
        chr->SendMessageToSet(&fly, true);
        BlueSystemMessage(chr->GetSession(), "Flying mode disabled.");
        if (chr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "disabled flying mode for %s", chr->GetName());
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleDBReloadCommand(const char* args, WorldSession* m_session)
{

    sWorld.SendWorldText("Support for reloading tables on the fly was disabled in Arcemu revision 3621. You are seeing this message because apparently reading SVN changelog or using forums search is way over the head of some of our users.", 0);
    return true;

    /*

    char str[200];
    int ret = 0;

    if (!*args || strlen(args) < 3)
    return false;


    uint32 mstime = getMSTime();
    snprintf(str, 200, "%s%s initiated server-side reload of table `%s`. The server may experience some lag while this occurs.",
    MSG_COLOR_LIGHTRED, m_session->GetPlayer()->GetName(), args);
    sWorld.SendWorldText(str, 0);

    if (0 == stricmp(args, "spell_disable"))
    {
    objmgr.ReloadDisabledSpells();
    ret = 1;
    } else
    if (0 == stricmp(args, "vendors"))
    {
    objmgr.ReloadVendors();
    ret = 1;
    }
    else
    {
    ret = Storage_ReloadTable(args);
    }

    if (ret == 0)
    snprintf(str, 200, "%sDatabase reload failed.", MSG_COLOR_LIGHTRED);
    else
    snprintf(str, 200, "%sDatabase reload completed in %u ms.", MSG_COLOR_LIGHTBLUE, getMSTime() - mstime);
    sWorld.SendWorldText(str, 0);
    sGMLog.writefromsession(m_session, "reloaded table %s", args);
    return true;

    */

}

bool ChatHandler::HandleModifyLevelCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, true);
    if (plr == 0)
        return true;

    uint32 Level = args ? atol(args) : 0;
    if (Level == 0 || Level > sWorld.m_levelCap)
    {
        RedSystemMessage(m_session, "A level (numeric) is required to be specified after this command.");
        return true;
    }

    // Set level message
    BlueSystemMessage(m_session, "Setting the level of %s to %u.", plr->GetName(), Level);
    GreenSystemMessageToPlr(plr, "%s set your level to %u.", m_session->GetPlayer()->GetName(), Level);

    sGMLog.writefromsession(m_session, "used modify level on %s, level %u", plr->GetName(), Level);

    // lookup level information
    LevelInfo* Info = objmgr.GetLevelInfo(plr->getRace(), plr->getClass(), Level);
    if (Info == NULL)
    {
        RedSystemMessage(m_session, "Levelup information not found.");
        return true;
    }

    plr->ApplyLevelInfo(Info, Level);
    if (plr->getClass() == WARLOCK)
    {
        std::list<Pet*> summons = plr->GetSummons();
        for (std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        {
            Pet* summon = *itr;
            if (summon->IsInWorld() && summon->isAlive())
            {
                summon->setLevel(Level);
                summon->ApplyStatsForLevel();
                summon->UpdateSpellList();
            }
        }
    }

    return true;
}

bool ChatHandler::HandleCreatePetCommand(const char* args, WorldSession* m_session)
{
    if ((args == NULL) || (strlen(args) < 2))
        return false;

    uint32 entry = atol(args);
    if (entry == 0)
        return false;

    CreatureInfo* ci = CreatureNameStorage.LookupEntry(entry);
    CreatureProto* cp = CreatureProtoStorage.LookupEntry(entry);

    if ((ci == NULL) || (cp == NULL))
        return false;

    Player* p = m_session->GetPlayer();

    p->DismissActivePets();
    p->RemoveFieldSummon();

    float followangle = -M_PI_FLOAT * 2;
    LocationVector v(p->GetPosition());

    v.x += (3 * (cosf(followangle + p->GetOrientation())));
    v.y += (3 * (sinf(followangle + p->GetOrientation())));

    Pet* pet = objmgr.CreatePet(entry);

    if (!pet->CreateAsSummon(entry, ci, NULL, p, NULL, 1, 0, &v, true))
    {
        pet->DeleteMe();
        return true;
    }

    pet->GetAIInterface()->SetUnitToFollowAngle(followangle);

    return true;
}


#ifdef USE_SPECIFIC_AIAGENTS
//this is custom stuff !
bool ChatHandler::HandlePetSpawnAIBot(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    if (!m_session->GetPlayer())
        return false; //wtf ?

    uint32 botprice = m_session->GetPlayer()->getLevel() * 10000; //1 gold per level ?

    if (!m_session->GetPlayer()->HasGold(botprice))
    {
        GreenSystemMessage(m_session, "You need a total of %u coins to afford a bot", botprice);
        return false;
    }

    uint8 botType = (uint8)atof((char*)args);

    if (botType != 0)
    {
        RedSystemMessage(m_session, "Incorrect value. Accepting value 0 only = healbot :)");
        return true;
    }

    uint32 Entry;
    char name[50];
    uint8 race = m_session->GetPlayer()->getRace();

    if (race == RACE_HUMAN || race == RACE_DWARF || race == RACE_NIGHTELF || race == RACE_GNOME || race == RACE_DRAENEI || race == RACE_WORGEN )
    {
        Entry = 1826;
        strcpy(name, "|cffff6060A_HealBot");
    }
    else
    {
        Entry = 5473;
        strcpy(name, "|cffff6060H_HealBot");
    }

    CreatureProto* pTemplate = CreatureProtoStorage.LookupEntry(Entry);
    CreatureInfo* pCreatureInfo = CreatureNameStorage.LookupEntry(Entry);
    if (!pTemplate || !pCreatureInfo)
    {
        RedSystemMessage(m_session, "Invalid creature spawn template: %u", Entry);
        return true;
    }
    Player* plr = m_session->GetPlayer();
    Creature* newguard = plr->create_guardian(Entry, 2 * 60 * 1000, float(-M_PI * 2), plr->getLevel());
    AiAgentHealSupport* new_interface = new AiAgentHealSupport;
    new_interface->Init(newguard, AITYPE_PET, MOVEMENTTYPE_NONE, plr);
    newguard->ReplaceAIInterface((AIInterface*) new_interface);

    /*    Pet *old_tame = plr->GetSummon();
        if (old_tame != NULL)
        {
        old_tame->Dismiss(true);
        }

        // create a pet from this creature
        Pet * pPet = objmgr.CreatePet(Entry);
        pPet->SetInstanceID(plr->GetInstanceID());
        pPet->SetMapId(plr->GetMapId());

        pPet->SetFloatValue (OBJECT_FIELD_SCALE_X, pTemplate->Scale / 2); //we do not wish to block visually other players
        AiAgentHealSupport *new_interface = new AiAgentHealSupport;
        pPet->ReplaceAIInterface((AIInterface *) new_interface);
        //    new_interface->Init(pPet,AITYPE_PET,MOVEMENTTYPE_NONE,plr); // i think this will get called automatically for pet

        pPet->CreateAsSummon(Entry, pCreatureInfo, pCreature, plr, NULL, 0x2, 0);

        pPet->Rename(name);

        //healer bot should not have any specific actions
        pPet->SetActionBarSlot(0,PET_SPELL_FOLLOW);
        pPet->SetActionBarSlot(1,PET_SPELL_STAY);
        pPet->SetActionBarSlot(2,0);
        pPet->SetActionBarSlot(3,0);
        pPet->SetActionBarSlot(4,0);
        pPet->SetActionBarSlot(5,0);
        pPet->SetActionBarSlot(6,0);
        pPet->SetActionBarSlot(7,0);
        pPet->SetActionBarSlot(8,0);
        pPet->SetActionBarSlot(9,0);
        pPet->SendSpellsToOwner();

        // remove the temp creature
        delete sp;
        delete pCreature;*/

    sGMLog.writefromsession(m_session, "used create an AI bot");
    return true;
}
#endif

bool ChatHandler::HandleAddPetSpellCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, false);
    if (!plr)
        return false;

    if (plr->GetSummon() == NULL)
    {
        RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
        return true;
    }

    uint32 SpellId = atol(args);
    SpellEntry* spell = dbcSpell.LookupEntryForced(SpellId);
    if (!SpellId || !spell)
    {
        RedSystemMessage(m_session, "Invalid spell id requested.");
        return true;
    }

    std::list<Pet*> summons = plr->GetSummons();
    for (std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
    {
        (*itr)->AddSpell(spell, true);
    }
    GreenSystemMessage(m_session, "Added spell %u to %s's pet.", SpellId, plr->GetName());
    return true;
}

bool ChatHandler::HandleRemovePetSpellCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, false);
    if (!plr)
        return false;

    if (plr->GetSummon() == NULL)
    {
        RedSystemMessage(m_session, "%s has no pet.", plr->GetName());
        return true;
    }

    uint32 SpellId = atol(args);
    SpellEntry* spell = dbcSpell.LookupEntryForced(SpellId);
    if (!SpellId || !spell)
    {
        RedSystemMessage(m_session, "Invalid spell id requested.");
        return true;
    }

    std::list<Pet*> summons = plr->GetSummons();
    for (std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
    {
        (*itr)->RemoveSpell(SpellId);
    }
    GreenSystemMessage(m_session, "Removed spell %u from %s's pet.", SpellId, plr->GetName());
    return true;
}

bool ChatHandler::HandleRenamePetCommand(const char* args, WorldSession* m_session)
{
    Player* plr = m_session->GetPlayer();
    Pet* pPet = plr->GetSummon();
    if (pPet == NULL)
    {
        RedSystemMessage(m_session, "You have no pet.");
        return true;
    }

    if (strlen(args) < 1)
    {
        RedSystemMessage(m_session, "You must specify a name.");
        return true;
    }

    GreenSystemMessage(m_session, "Renamed your pet to %s.", args);
    pPet->Rename(args);//support for only 1st pet
    return true;
}

bool ChatHandler::HandleDismissPetCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, false);
    Pet* pPet = NULL;
    if (plr)
    {
        if (plr->GetSummon() == NULL)
        {
            RedSystemMessage(m_session, "Player has no pet.");
            return true;
        }
        else
        {
            plr->DismissActivePets();
        }
    }
    else
    {
        // no player selected, see if it is a pet
        Creature* pCrt = getSelectedCreature(m_session, false);
        if (!pCrt)
        {
            // show usage string
            return false;
        }
        if (pCrt->IsPet())
        {
            pPet = static_cast< Pet* >(pCrt);
        }
        if (!pPet)
        {
            RedSystemMessage(m_session, "No player or pet selected.");
            return true;
        }
        plr = pPet->GetPetOwner();
        pPet->Dismiss();
    }

    GreenSystemMessage(m_session, "Dismissed %s's pet.", plr->GetName());
    plr->GetSession()->SystemMessage("%s dismissed your pet.", m_session->GetPlayer()->GetName());
    return true;
}

bool ChatHandler::HandlePetLevelCommand(const char* args, WorldSession* m_session)
{
    if (!args)
    {
        return false;
    }

    int32 newLevel = atol(args);
    if (newLevel < 1)
    {
        return false;
    }

    Player* plr = getSelectedChar(m_session, false);
    Pet* pPet = NULL;
    if (plr)
    {
        pPet = plr->GetSummon();
        if (!pPet)
        {
            RedSystemMessage(m_session, "Player has no pet.");
            return true;
        }
    }
    else
    {
        // no player selected, see if it is a pet
        Creature* pCrt = getSelectedCreature(m_session, false);
        if (!pCrt)
        {
            // show usage string
            return false;
        }
        if (pCrt->IsPet())
        {
            pPet = static_cast< Pet* >(pCrt);
        }
        if (!pPet)
        {
            RedSystemMessage(m_session, "No player or pet selected.");
            return true;
        }
        plr = pPet->GetPetOwner();
    }

    // Should GMs be allowed to set a pet higher than its owner?  I don't think so
    if ((uint32)newLevel > plr->getLevel())
    {
        newLevel = plr->getLevel();
    }

    //support for only 1 pet
    pPet->setLevel(newLevel);
    pPet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
    pPet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, pPet->GetNextLevelXP(newLevel));
    pPet->ApplyStatsForLevel();
    pPet->UpdateSpellList();

    GreenSystemMessage(m_session, "Set %s's pet to level %lu.", plr->GetName(), newLevel);
    plr->GetSession()->SystemMessage("%s set your pet to level %lu.", m_session->GetPlayer()->GetName(), newLevel);
    return true;
}

bool ChatHandler::HandleShutdownCommand(const char* args, WorldSession* m_session)
{
    uint32 shutdowntime;
    if (!args)
        shutdowntime = 5;
    else
        shutdowntime = atol(args);

    char msg[500];
    snprintf(msg, 500, "%sServer shutdown initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
             m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

    sWorld.SendWorldText(msg);
    sGMLog.writefromsession(m_session, "initiated server shutdown timer %u sec", shutdowntime);
    shutdowntime *= 1000;
    sMaster.m_ShutdownTimer = shutdowntime;
    sMaster.m_ShutdownEvent = true;
    sMaster.m_restartEvent = false;
    return true;
}

bool ChatHandler::HandleShutdownRestartCommand(const char* args, WorldSession* m_session)
{
    uint32 shutdowntime;
    if (!args)
        shutdowntime = 5;
    else
        shutdowntime = atol(args);

    char msg[500];
    snprintf(msg, 500, "%sServer restart initiated by %s, shutting down in %u seconds.", MSG_COLOR_LIGHTBLUE,
             m_session->GetPlayer()->GetName(), (unsigned int)shutdowntime);

    sGMLog.writefromsession(m_session, "initiated server restart timer %u sec", shutdowntime);
    sWorld.SendWorldText(msg);
    shutdowntime *= 1000;
    sMaster.m_ShutdownTimer = shutdowntime;
    sMaster.m_ShutdownEvent = true;
    sMaster.m_restartEvent = true;
    return true;
}

bool ChatHandler::HandleCancelShutdownCommand(const char* args, WorldSession* m_session)
{
    if (sMaster.m_ShutdownEvent == false)
        return false;
    char msg[500];
    snprintf(msg, 500, "%sServer %s cancelled by %s.", MSG_COLOR_LIGHTBLUE, (sMaster.m_restartEvent ? "Restart" : "Shutdown"), m_session->GetPlayer()->GetName());
    sWorld.SendWorldText(msg);

    sMaster.m_ShutdownTimer = 5000;
    sMaster.m_ShutdownEvent = false;
    sMaster.m_restartEvent = false;
    return true;

}

bool ChatHandler::HandleAllowWhispersCommand(const char* args, WorldSession* m_session)
{
    if (args == 0 || strlen(args) < 2) return false;
    PlayerCache* playercache = objmgr.GetPlayerCache(args, false);
    if (playercache == NULL)
    {
        RedSystemMessage(m_session, "Player not found.");
        return true;
    }

    m_session->GetPlayer()->m_cache->InsertValue64(CACHE_GM_TARGETS, playercache->GetUInt32Value(CACHE_PLAYER_LOWGUID));
    std::string name;
    playercache->GetStringValue(CACHE_PLAYER_NAME, name);
    BlueSystemMessage(m_session, "Now accepting whispers from %s.", name.c_str());
    playercache->DecRef();
    return true;
}

bool ChatHandler::HandleBlockWhispersCommand(const char* args, WorldSession* m_session)
{
    if (args == 0 || strlen(args) < 2) return false;
    PlayerCache* playercache = objmgr.GetPlayerCache(args, false);
    if (playercache == NULL)
    {
        RedSystemMessage(m_session, "Player not found.");
        return true;
    }

    m_session->GetPlayer()->m_cache->RemoveValue64(CACHE_GM_TARGETS, playercache->GetUInt32Value(CACHE_PLAYER_LOWGUID));
    std::string name;
    playercache->GetStringValue(CACHE_PLAYER_NAME, name);
    BlueSystemMessage(m_session, "Now blocking whispers from %s.", name.c_str());
    playercache->DecRef();
    return true;
}

bool ChatHandler::HandleAdvanceAllSkillsCommand(const char* args, WorldSession* m_session)
{
    uint32 amt = args ? atol(args) : 0;
    if (!amt)
    {
        RedSystemMessage(m_session, "An amount to increment is required.");
        return true;
    }

    Player* plr = getSelectedChar(m_session);
    if (!plr)
        return true;


    plr->_AdvanceAllSkills(amt);
    GreenSystemMessageToPlr(plr, "Advanced all your skill lines by %u points.", amt);
    sGMLog.writefromsession(m_session, "advanced all skills by %u on %s", amt, plr->GetName());
    return true;
}

bool ChatHandler::HandleKillByPlayerCommand(const char* args, WorldSession* m_session)
{
    if (!args || strlen(args) < 2)
    {
        RedSystemMessage(m_session, "A player's name is required.");
        return true;
    }

    sWorld.DisconnectUsersWithPlayerName(args, m_session);
    sGMLog.writefromsession(m_session, "disconnected player %s", args);
    return true;
}

bool ChatHandler::HandleKillBySessionCommand(const char* args, WorldSession* m_session)
{
    if (!args || strlen(args) < 2)
    {
        RedSystemMessage(m_session, "A player's name is required.");
        return true;
    }

    sWorld.DisconnectUsersWithAccount(args, m_session);
    sGMLog.writefromsession(m_session, "disconnected player with account %s", args);
    return true;
}
bool ChatHandler::HandleKillByIPCommand(const char* args, WorldSession* m_session)
{
    if (!args || strlen(args) < 2)
    {
        RedSystemMessage(m_session, "An IP is required.");
        return true;
    }

    sWorld.DisconnectUsersWithIP(args, m_session);
    sGMLog.writefromsession(m_session, "disconnected players with IP %s", args);
    return true;
}

bool ChatHandler::HandleMassSummonCommand(const char* args, WorldSession* m_session)
{
    PlayerStorageMap::const_iterator itr;
    objmgr._playerslock.AcquireReadLock();
    Player* summoner = m_session->GetPlayer();
    Player* plr;
    int faction = -1;
    char Buffer[170];
    if (*args == 'a' || *args == 'A')
    {
        faction = 0;
        snprintf(Buffer, 170, "%s%s Has requested a mass summon of all Alliance players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts", MSG_COLOR_GOLD, m_session->GetPlayer()->GetName());

    }
    else if (*args == 'h' || *args == 'H')
    {
        faction = 1;
        snprintf(Buffer, 170, "%s%s Has requested a mass summon of all Horde players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts", MSG_COLOR_GOLD, m_session->GetPlayer()->GetName());
    }
    else  snprintf(Buffer, 170, "%s%s Has requested a mass summon of all players. Do not feel obliged to accept the summon, as it is most likely for an event or a test of sorts", MSG_COLOR_GOLD, m_session->GetPlayer()->GetName());

    uint32 c = 0;

    for (itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
    {
        plr = itr->second;
        if (plr->GetSession() && plr->IsInWorld())
        {
            //plr->SafeTeleport(summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
            /* let's do this the blizz way */
            if (faction > -1 && plr->GetTeam() == static_cast<uint32>(faction))
            {
                plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
                ++c;
            }
            else if (faction == -1)
            {
                plr->SummonRequest(summoner->GetLowGUID(), summoner->GetZoneId(), summoner->GetMapId(), summoner->GetInstanceID(), summoner->GetPosition());
                ++c;
            }

        }
    }
    sGMLog.writefromsession(m_session, "requested a mass summon of %u players.", c);
    objmgr._playerslock.ReleaseReadLock();
    return true;
}

bool ChatHandler::HandleCastAllCommand(const char* args, WorldSession* m_session)
{
    if (!args || strlen(args) < 2)
    {
        RedSystemMessage(m_session, "No spellid specified.");
        return true;
    }
    Player* plr;
    uint32 spellid = atol(args);
    SpellEntry* info = dbcSpell.LookupEntryForced(spellid);
    if (!info)
    {
        RedSystemMessage(m_session, "Invalid spell specified.");
        return true;
    }

    // this makes sure no moron casts a learn spell on everybody and wrecks the server
    for (int i = 0; i < 3; i++)
    {
        if (info->eff[i].Effect == SPELL_EFFECT_LEARN_SPELL)  //SPELL_EFFECT_LEARN_SPELL - 36
        {
            sGMLog.writefromsession(m_session, "used wrong / learnall castall command, spellid %u", spellid);
            RedSystemMessage(m_session, "Learn spell specified.");
            return true;
        }
    }

    sGMLog.writefromsession(m_session, "used castall command, spellid %u", spellid);

    PlayerStorageMap::const_iterator itr;
    objmgr._playerslock.AcquireReadLock();
    for (itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
    {
        plr = itr->second;
        if (plr->GetSession() && plr->IsInWorld())
        {
            if (plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
            {
                sEventMgr.AddEvent(static_cast< Unit* >(plr), &Unit::EventCastSpell, static_cast< Unit* >(plr), info, EVENT_PLAYER_CHECKFORCHEATS, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
            }
            else
            {
                Spell* sp = sSpellFactoryMgr.NewSpell(plr, info, true, 0);
                SpellCastTargets targets(plr->GetGUID());
                sp->prepare(&targets);
            }
        }
    }
    objmgr._playerslock.ReleaseReadLock();

    BlueSystemMessage(m_session, "Casted spell %u on all players!", spellid);
    return true;
}

bool ChatHandler::HandleNpcReturnCommand(const char* args, WorldSession* m_session)
{
    Creature* creature = getSelectedCreature(m_session);
    if (!creature || !creature->m_spawn) return true;

    // return to respawn coords
    float x = creature->m_spawn->x;
    float y = creature->m_spawn->y;
    float z = creature->m_spawn->z;
    float o = creature->m_spawn->o;

    // restart movement
    creature->GetAIInterface()->SetAIState(STATE_IDLE);
    creature->GetAIInterface()->WipeHateList();
    creature->GetAIInterface()->WipeTargetList();
    creature->GetAIInterface()->MoveTo(x, y, z, o);

    sGMLog.writefromsession(m_session, "returned NPC %s, sqlid %u", creature->GetCreatureInfo()->Name, creature->GetSQL_id());

    return true;
}

bool ChatHandler::HandleModPeriodCommand(const char* args, WorldSession* m_session)
{
    Transporter* trans = m_session->GetPlayer()->m_CurrentTransporter;
    if (trans == 0)
    {
        RedSystemMessage(m_session, "You must be on a transporter.");
        return true;
    }

    uint32 np = args ? atol(args) : 0;
    if (np == 0)
    {
        RedSystemMessage(m_session, "A time in ms must be specified.");
        return true;
    }

    trans->SetPeriod(np);
    BlueSystemMessage(m_session, "Period of %s set to %u.", trans->GetInfo()->name, np);
    return true;
}

bool ChatHandler::HandleFormationLink1Command(const char* args, WorldSession* m_session)
{
    // set formation "master"
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == 0) return true;

    m_session->GetPlayer()->linkTarget = pCreature;
    BlueSystemMessage(m_session, "Linkup \"master\" set to %s.", pCreature->GetCreatureInfo()->Name);
    return true;
}

bool ChatHandler::HandleFormationLink2Command(const char* args, WorldSession* m_session)
{
    // set formation "slave" with distance and angle
    float ang, dist;
    if (*args == 0 || sscanf(args, "%f %f", &dist, &ang) != 2)
    {
        RedSystemMessage(m_session, "You must specify a distance and angle.");
        return true;
    }

    if (m_session->GetPlayer()->linkTarget == NULL || m_session->GetPlayer()->linkTarget->IsPet())
    {
        RedSystemMessage(m_session, "Master not selected. select the master, and use formationlink1.");
        return true;
    }

    Creature* slave = getSelectedCreature(m_session, true);
    if (slave == 0) return true;

    slave->GetAIInterface()->m_formationFollowDistance = dist;
    slave->GetAIInterface()->m_formationFollowAngle = ang;
    slave->GetAIInterface()->m_formationLinkTarget = m_session->GetPlayer()->linkTarget->GetGUID();
    slave->GetAIInterface()->m_formationLinkSqlId = m_session->GetPlayer()->linkTarget->GetSQL_id();
    slave->GetAIInterface()->SetUnitToFollowAngle(ang);

    // add to db
    WorldDatabase.Execute("INSERT INTO creature_formations VALUES(%u, %u, '%f', '%f')",
                          slave->GetSQL_id(), slave->GetAIInterface()->m_formationLinkSqlId, ang, dist);

    BlueSystemMessage(m_session, "%s linked up to %s with a distance of %f at %f radians.", slave->GetCreatureInfo()->Name,
                      m_session->GetPlayer()->linkTarget->GetCreatureInfo()->Name, dist, ang);

    return true;
}

bool ChatHandler::HandleNpcFollowCommand(const char* args, WorldSession* m_session)
{
    Creature* creature = getSelectedCreature(m_session, true);
    if (!creature) return true;

    creature->GetAIInterface()->SetUnitToFollow(m_session->GetPlayer());

    sGMLog.writefromsession(m_session, "used npc follow command on %s, sqlid %u", creature->GetCreatureInfo()->Name, creature->GetSQL_id());
    return true;
}

bool ChatHandler::HandleFormationClearCommand(const char* args, WorldSession* m_session)
{
    Creature* c = getSelectedCreature(m_session, true);
    if (!c) return true;

    c->GetAIInterface()->m_formationLinkSqlId = 0;
    c->GetAIInterface()->m_formationLinkTarget = 0;
    c->GetAIInterface()->m_formationFollowAngle = 0.0f;
    c->GetAIInterface()->m_formationFollowDistance = 0.0f;
    c->GetAIInterface()->ResetUnitToFollow();

    WorldDatabase.Execute("DELETE FROM creature_formations WHERE spawn_id=%u", c->GetSQL_id());
    return true;
}

bool ChatHandler::HandleNullFollowCommand(const char* args, WorldSession* m_session)
{
    Creature* c = getSelectedCreature(m_session, true);
    if (!c) return true;

    // restart movement
    c->GetAIInterface()->SetAIState(STATE_IDLE);
    c->GetAIInterface()->ResetUnitToFollow();

    sGMLog.writefromsession(m_session, "cancelled npc follow command on %s, sqlid %u", c->GetCreatureInfo()->Name, c->GetSQL_id());
    return true;
}

bool ChatHandler::HandleItemStackCheatCommand(const char* args, WorldSession* m_session)
{
    Player* p = getSelectedChar(m_session, true);
    if (!p)
        return true;

    bool turnCheatOn;
    if (!*args)
        turnCheatOn = (p->ItemStackCheat) ? false : true;
    else if (stricmp(args, "on") == 0)
        turnCheatOn = true;
    else if (stricmp(args, "off") == 0)
        turnCheatOn = false;
    else
        return false;

    p->ItemStackCheat = turnCheatOn;
    BlueSystemMessage(m_session, "%s the item stack cheat on %s.", (turnCheatOn) ? "activated" : "deactivated", p->GetName());
    GreenSystemMessageToPlr(p, "%s %s the item stack cheat on you.%s", m_session->GetPlayer()->GetName(), (turnCheatOn) ? "activated" : "deactivated", (turnCheatOn) ? "" : "  WARNING!!! Make sure all your item stacks are normal (if possible) before logging off, or else you may lose some items!");
    if (p != m_session->GetPlayer())
    {
        sGMLog.writefromsession(m_session, "item stack cheat on %s set to %s", p->GetName(), (turnCheatOn) ? "on" : "off");
    }
    return true;
}


bool ChatHandler::HandleAuraStackCheatCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->AuraStackCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->AuraStackCheat = true;
        BlueSystemMessage(m_session, "activated the aura stack cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "activated the aura stack cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->AuraStackCheat = false;
        BlueSystemMessage(m_session, "deactivated the aura stack cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "deactivated the aura stack cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "aura stack cheat on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleTriggerpassCheatCommand(const char* args, WorldSession* m_session)
{
    Player* plyr = getSelectedChar(m_session, true);
    if (!plyr)
        return true;

    if (!*args)
    {
        if (plyr->TriggerpassCheat)
            args = "off";
        else
            args = "on";
    }

    if (stricmp(args, "on") == 0)
    {
        plyr->TriggerpassCheat = true;
        BlueSystemMessage(m_session, "activated the triggerpass cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "activated the triggerpass cheat on you.", m_session->GetPlayer()->GetName());
    }
    else if (stricmp(args, "off") == 0)
    {
        plyr->TriggerpassCheat = false;
        BlueSystemMessage(m_session, "deactivated the triggerpass cheat on %s.", plyr->GetName());
        GreenSystemMessageToPlr(plyr, "deactivated the triggerpass cheat on you.", m_session->GetPlayer()->GetName());

        if (plyr != m_session->GetPlayer())
            sGMLog.writefromsession(m_session, "triggerpass cheat on %s set to %s", plyr->GetName(), args);
    }
    else
        return false;
    return true;
}

bool ChatHandler::HandleResetSkillsCommand(const char* args, WorldSession* m_session)
{
    skilllineentry* se;
    Player* plr = getSelectedChar(m_session, true);
    if (!plr)
        return true;

    plr->_RemoveAllSkills();

    // Load skills from create info.
    PlayerCreateInfo* info = objmgr.GetPlayerCreateInfo(plr->getRace(), plr->getClass());
    if (!info) return true;

    for (std::list<CreateInfo_SkillStruct>::iterator ss = info->skills.begin(); ss != info->skills.end(); ++ss)
    {
        se = dbcSkillLine.LookupEntry(ss->skillid);
        if (se->type != SKILL_TYPE_LANGUAGE && ss->skillid && ss->currentval && ss->maxval)
            plr->_AddSkillLine(ss->skillid, ss->currentval, ss->maxval);
    }
    //Chances depend on stats must be in this order!
    plr->UpdateStats();
    plr->UpdateChances();
    plr->_UpdateMaxSkillCounts();
    plr->_AddLanguages(false);
    BlueSystemMessage(m_session, "Reset skills to default.");
    sGMLog.writefromsession(m_session, "reset skills of %s to default", plr->GetName());
    return true;
}

bool ChatHandler::HandlePlayerInfo(const char* args, WorldSession* m_session)
{
    Player* plr;
    if (strlen(args) >= 2) // char name can be 2 letters
    {
        plr = objmgr.GetPlayer(args, false);
        if (!plr)
        {
            RedSystemMessage(m_session, "Unable to locate player %s.", args);
            return true;
        }
    }
    else
        plr = getSelectedChar(m_session, true);

    if (!plr) return true;
    if (!plr->GetSession())
    {
        RedSystemMessage(m_session, "ERROR: this player hasn't got any session !");
        return true;
    }
    if (!plr->GetSession()->GetSocket())
    {
        RedSystemMessage(m_session, "ERROR: this player hasn't got any socket !");
        return true;
    }
    WorldSession* sess = plr->GetSession();

    //    char* infos = new char[128];
    static const char* classes[12] =
    { "None", "Warrior", "Paladin", "Hunter", "Rogue", "Priest", "Death Knight", "Shaman", "Mage", "Warlock", "None", "Druid" };
    static const char* races[23] =
    { "None", "Human", "Orc", "Dwarf", "Night Elf", "Undead", "Tauren", "Gnome", "Troll", "Goblin", "Blood Elf", "Draenei", "None", "None", "None", "None", "None", "None", "None", "None", "None", "None", "Worgen" };

    char playedLevel[64];
    char playedTotal[64];

    int seconds = (plr->GetPlayedtime())[0];
    int mins = 0;
    int hours = 0;
    int days = 0;
    if (seconds >= 60)
    {
        mins = seconds / 60;
        if (mins)
        {
            seconds -= mins * 60;
            if (mins >= 60)
            {
                hours = mins / 60;
                if (hours)
                {
                    mins -= hours * 60;
                    if (hours >= 24)
                    {
                        days = hours / 24;
                        if (days)
                            hours -= days * 24;
                    }
                }
            }
        }
    }
    snprintf(playedLevel, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

    seconds = (plr->GetPlayedtime())[1];
    mins = 0;
    hours = 0;
    days = 0;
    if (seconds >= 60)
    {
        mins = seconds / 60;
        if (mins)
        {
            seconds -= mins * 60;
            if (mins >= 60)
            {
                hours = mins / 60;
                if (hours)
                {
                    mins -= hours * 60;
                    if (hours >= 24)
                    {
                        days = hours / 24;
                        if (days)
                            hours -= days * 24;
                    }
                }
            }
        }
    }
    snprintf(playedTotal, 64, "[%d days, %d hours, %d minutes, %d seconds]", days, hours, mins, seconds);

    GreenSystemMessage(m_session, "%s is a %s %s %s", plr->GetName(),
                       (plr->getGender() ? "Female" : "Male"), races[plr->getRace()], classes[plr->getClass()]);

    BlueSystemMessage(m_session, "%s has played %s at this level", (plr->getGender() ? "She" : "He"), playedLevel);
    BlueSystemMessage(m_session, "and %s overall", playedTotal);

    BlueSystemMessage(m_session, "%s is connecting from account '%s'[%u] with permissions '%s'",
                      (plr->getGender() ? "She" : "He"), sess->GetAccountName().c_str(), sess->GetAccountId(), sess->GetPermissions());

    const char* client;

    // Clean code says you need to work from highest combined bit to lowest. Second, you need to check if both flags exists.
    if (sess->HasFlag(ACCOUNT_FLAG_XPACK_02) && sess->HasFlag(ACCOUNT_FLAG_XPACK_01))
        client = "TBC and WotLK";
    else if (sess->HasFlag(ACCOUNT_FLAG_XPACK_02))
        client = "Wrath of the Lich King";
    else if (sess->HasFlag(ACCOUNT_FLAG_XPACK_01))
        client = "WoW Burning Crusade";
    else
        client = "WoW";

    BlueSystemMessage(m_session, "%s uses %s (build %u)", (plr->getGender() ? "She" : "He"),
                      client, sess->GetClientBuild());

    BlueSystemMessage(m_session, "%s IP is '%s', and has a latency of %ums", (plr->getGender() ? "Her" : "His"),
                      sess->GetSocket()->GetRemoteIP().c_str(), sess->GetLatency());

    return true;
}

bool ChatHandler::HandleGlobalPlaySoundCommand(const char* args, WorldSession* m_session)
{
    if (*args == '\0')
        return false;

    uint32 sound = atoi(args);
    if (sound == 0)
        return false;

    sWorld.PlaySoundToAll(sound);
    BlueSystemMessage(m_session, "Broadcasted sound %u to server.", sound);
    sGMLog.writefromsession(m_session, "used play all command soundid %u", sound);

    return true;
}

bool ChatHandler::HandleIPBanCommand(const char* args, WorldSession* m_session)
{
    char* pIp = (char*)args;
    char* pReason;
    char* pDuration;
    ParseBanArgs(pIp, &pDuration, &pReason);
    int32 timeperiod = 0;
    if (pDuration != NULL)
    {
        timeperiod = GetTimePeriodFromString(pDuration);
        if (timeperiod < 0)
            return false;
    }

    uint32 o1, o2, o3, o4;
    if (sscanf(pIp, "%3u.%3u.%3u.%3u", (unsigned int*)&o1, (unsigned int*)&o2, (unsigned int*)&o3, (unsigned int*)&o4) != 4
        || o1 > 255 || o2 > 255 || o3 > 255 || o4 > 255)
    {
        RedSystemMessage(m_session, "Invalid IPv4 address [%s]", pIp);
        return true;    // error in syntax, but we wont remind client of command usage
    }

    time_t expire_time;
    if (timeperiod == 0)        // permanent ban
        expire_time = 0;
    else
        expire_time = UNIXTIME + (time_t)timeperiod;
    std::string IP = pIp;
    std::string::size_type i = IP.find("/");
    if (i == std::string::npos)
    {
        RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, don't worry, it worked)");
        IP.append("/32");
    }

    //temporal IP or real pro flooder who will change it tomorrow ?
    char emptystring = 0;
    if (pReason == NULL)
        pReason = &emptystring;

    SystemMessage(m_session, "Adding [%s] to IP ban table, expires %s.Reason is :%s", pIp, (expire_time == 0) ? "Never" : ctime(&expire_time), pReason);
    sLogonCommHandler.IPBan_Add(IP.c_str(), (uint32)expire_time, pReason);
    sWorld.DisconnectUsersWithIP(IP.substr(0, IP.find("/")).c_str(), m_session);
    sGMLog.writefromsession(m_session, "banned ip address %s, expires %s", pIp, (expire_time == 0) ? "Never" : ctime(&expire_time));
    return true;
}

bool ChatHandler::HandleIPUnBanCommand(const char* args, WorldSession* m_session)
{
    std::string pIp = args;
    if (pIp.length() == 0)
        return false;

    if (pIp.find("/") == std::string::npos)
    {
        RedSystemMessage(m_session, "Lack of CIDR address assumes a 32bit match (if you don't understand, don't worry, it worked)");
        pIp.append("/32");
    }
    /**
    * We can afford to be less fussy with the validity of the IP address given since
    * we are only attempting to remove it.
    * Sadly, we can only blindly execute SQL statements on the logonserver so we have
    * no idea if the address existed and so the account/IPBanner cache requires reloading.
    */

    SystemMessage(m_session, "Deleting [%s] from ip ban table if it exists", pIp.c_str());
    sLogonCommHandler.IPBan_Remove(pIp.c_str());
    sGMLog.writefromsession(m_session, "unbanned ip address %s", pIp.c_str());
    return true;
}

bool ChatHandler::HandleCreatureSpawnCommand(const char* args, WorldSession* m_session)
{
    uint32 entry = atol(args);
    uint8 gender = 0;

    if (entry == 0)
        return false;

    CreatureProto* proto = CreatureProtoStorage.LookupEntry(entry);
    CreatureInfo* info = CreatureNameStorage.LookupEntry(entry);
    if (proto == NULL || info == NULL)
    {
        RedSystemMessage(m_session, "Invalid entry id.");
        return true;
    }

    CreatureSpawn* sp = new CreatureSpawn;
    //sp->displayid = info->DisplayID;
    gender = info->GenerateModelId(&sp->displayid);
    sp->entry = entry;
    sp->form = 0;
    sp->id = objmgr.GenerateCreatureSpawnID();
    sp->movetype = 0;
    sp->x = m_session->GetPlayer()->GetPositionX();
    sp->y = m_session->GetPlayer()->GetPositionY();
    sp->z = m_session->GetPlayer()->GetPositionZ();
    sp->o = m_session->GetPlayer()->GetOrientation();
    sp->emote_state = 0;
    sp->flags = 0;
    sp->factionid = proto->Faction;
    sp->bytes0 = sp->setbyte(0, 2, gender);
    sp->bytes1 = 0;
    sp->bytes2 = 0;
    //sp->respawnNpcLink = 0;
    sp->stand_state = 0;
    sp->death_state = 0;
    sp->channel_target_creature = sp->channel_target_go = sp->channel_spell = 0;
    sp->MountedDisplayID = 0;
    sp->Item1SlotDisplay = 0;
    sp->Item2SlotDisplay = 0;
    sp->Item3SlotDisplay = 0;
    sp->CanFly = 0;
    sp->phase = m_session->GetPlayer()->GetPhase();


    Creature* p = m_session->GetPlayer()->GetMapMgr()->CreateCreature(entry);
    ARCEMU_ASSERT(p != NULL);
    p->Load(sp, (uint32)NULL, NULL);
    p->m_loadedFromDB = true;
    p->PushToWorld(m_session->GetPlayer()->GetMapMgr());

    uint32 x = m_session->GetPlayer()->GetMapMgr()->GetPosX(m_session->GetPlayer()->GetPositionX());
    uint32 y = m_session->GetPlayer()->GetMapMgr()->GetPosY(m_session->GetPlayer()->GetPositionY());

    // Add spawn to map
    m_session->GetPlayer()->GetMapMgr()->GetBaseMap()->GetSpawnsListAndCreate(
        x,
        y)->CreatureSpawns.push_back(sp);

    MapCell* mCell = m_session->GetPlayer()->GetMapMgr()->GetCell(x, y);

    if (mCell != NULL)
        mCell->SetLoaded();

    BlueSystemMessage(m_session, "Spawned a creature `%s` with entry %u at %f %f %f on map %u", info->Name,
                      entry, sp->x, sp->y, sp->z, m_session->GetPlayer()->GetMapId());

    // Save it to the database.
    p->SaveToDB();

    sGMLog.writefromsession(m_session, "spawned a %s at %u %f %f %f", info->Name, m_session->GetPlayer()->GetMapId(), sp->x, sp->y, sp->z);

    return true;
}

bool ChatHandler::HandleCreatureRespawnCommand(const char* args, WorldSession* m_session)
{
    Creature* cCorpse = getSelectedCreature(m_session, false);

    if (cCorpse != NULL && cCorpse->IsCreature() && cCorpse->getDeathState() == CORPSE && cCorpse->GetSQL_id() != 0)
    {
        sEventMgr.RemoveEvents(cCorpse, EVENT_CREATURE_RESPAWN);

        BlueSystemMessage(m_session, "Respawning a Creature: `%s` with entry: %u on map: %u sqlid: %u", cCorpse->GetCreatureInfo()->Name,
                          cCorpse->GetEntry(), cCorpse->GetMapMgr()->GetMapId(), cCorpse->GetSQL_id());

        sGMLog.writefromsession(m_session, "Respawned a Creature: `%s` with entry: %u on map: %u sqlid: %u", cCorpse->GetCreatureInfo()->Name,
                                cCorpse->GetEntry(), cCorpse->GetMapMgr()->GetMapId(), cCorpse->GetSQL_id());

        cCorpse->Despawn(0, 1000);
        return true;
    }

    RedSystemMessage(m_session, "You must select a creature's corpse with a valid CreatureSpawn point.");
    return false;
}

bool ChatHandler::HandleNPCCanFlyCommand(const char* args, WorldSession* m_session)
{
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == NULL)
        return true;
    if (pCreature->GetAIInterface()->Flying())
        pCreature->GetAIInterface()->StopFlying();
    else
        pCreature->GetAIInterface()->SetFly();

    pCreature->GetAIInterface()->onGameobject = false;
    char* sSave = strtok((char*)args, " ");
    if (sSave)
    {
        bool save = (atoi(sSave) > 0 ? true : false);
        if (save)
        {
            pCreature->SaveToDB();
            pCreature->m_loadedFromDB = true;
        }
    }
    GreenSystemMessage(m_session, "You may have to leave and re-enter this zone for changes to take effect.");
    return true;
}

bool ChatHandler::HandleNPCOnGOCommand(const char* args, WorldSession* m_session)
{
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == NULL)
        return true;

    pCreature->GetAIInterface()->StopFlying();

    pCreature->GetAIInterface()->onGameobject = !pCreature->GetAIInterface()->onGameobject;
    char* sSave = strtok((char*)args, " ");
    if (sSave)
    {
        bool save = (atoi(sSave) > 0 ? true : false);
        if (save)
        {
            pCreature->SaveToDB();
            pCreature->m_loadedFromDB = true;
        }
    }
    GreenSystemMessage(m_session, "You may have to leave and re-enter this zone for changes to take effect.");
    return true;
}

bool ChatHandler::HandleRemoveItemCommand(const char* args, WorldSession* m_session)
{
    uint32 item_id;
    int32 count, ocount;
    int argc = sscanf(args, "%u %u", (unsigned int*)&item_id, (unsigned int*)&count);
    if (argc == 1)
        count = 1;
    else if (argc != 2 || !count)
        return false;

    ocount = count;
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return true;

    // loop until they're all gone.
    int32 loop_count = 0;
    int32 start_count = plr->GetItemInterface()->GetItemCount(item_id, true);
    int32 start_count2 = start_count;
    if (count > start_count)
        count = start_count;

    while (start_count >= count && (count > 0) && loop_count < 20)     // Prevent a loop here.
    {
        plr->GetItemInterface()->RemoveItemAmt(item_id, count);
        start_count2 = plr->GetItemInterface()->GetItemCount(item_id, true);
        count -= (start_count - start_count2);
        start_count = start_count2;
        ++loop_count;
    }

    ItemPrototype* iProto = ItemPrototypeStorage.LookupEntry(item_id);

    if (iProto)
    {
        sGMLog.writefromsession(m_session, "used remove item %s (id: %u) count %u from %s", iProto->Name1, item_id, ocount, plr->GetName());
        BlueSystemMessage(m_session, "Removing %u copies of item %s (id: %u) from %s's inventory.", ocount, GetItemLinkByProto(iProto, m_session->language).c_str(), item_id, plr->GetName());
        BlueSystemMessage(plr->GetSession(), "%s removed %u copies of item %s from your inventory.", m_session->GetPlayer()->GetName(), ocount, GetItemLinkByProto(iProto, plr->GetSession()->language).c_str());
    }
    else RedSystemMessage(m_session, "Cannot remove non valid item id: %u .", item_id);

    return true;
}

bool ChatHandler::HandleForceRenameCommand(const char* args, WorldSession* m_session)
{
    // prevent buffer overflow
    if (strlen(args) > 100)
        return false;

    std::string tmp = std::string(args);
    PlayerInfo* pi = objmgr.GetPlayerInfoByName(tmp.c_str());
    if (pi == 0)
    {
        RedSystemMessage(m_session, "Player with that name not found.");
        return true;
    }

    Player* plr = objmgr.GetPlayer((uint32)pi->guid);
    if (plr == 0)
    {
        CharacterDatabase.Execute("UPDATE characters SET login_flags = %u WHERE guid = %u", (uint32)LOGIN_FORCED_RENAME, (uint32)pi->guid);
    }
    else
    {
        plr->login_flags = LOGIN_FORCED_RENAME;
        plr->SaveToDB(false);
        BlueSystemMessageToPlr(plr, "%s forced your character to be renamed next logon.", m_session->GetPlayer()->GetName());
    }

    CharacterDatabase.Execute("INSERT INTO banned_names VALUES('%s')", CharacterDatabase.EscapeString(std::string(pi->name)).c_str());
    GreenSystemMessage(m_session, "Forcing %s to rename his character next logon.", args);
    sGMLog.writefromsession(m_session, "forced %s to rename his charater (%u)", pi->name, pi->guid);
    return true;
}

bool ChatHandler::HandleCustomizeCommand(const char* args, WorldSession* m_session)
{
    // prevent buffer overflow
    if (strlen(args) > 100)
        return false;

    std::string tmp = std::string(args);
    PlayerInfo* pi = objmgr.GetPlayerInfoByName(tmp.c_str());
    if (pi == 0)
    {
        RedSystemMessage(m_session, "Player with that name not found.");
        return true;
    }

    Player* plr = objmgr.GetPlayer((uint32)pi->guid);
    if (plr == 0)
    {
        CharacterDatabase.Execute("UPDATE characters SET login_flags = %u WHERE guid = %u", (uint32)LOGIN_CUSTOMIZE_LOOKS, (uint32)pi->guid);
    }
    else
    {
        plr->login_flags = LOGIN_CUSTOMIZE_LOOKS;
        plr->SaveToDB(false);
        BlueSystemMessageToPlr(plr, "%s flagged your character for customization at next login.", m_session->GetPlayer()->GetName());
    }

    GreenSystemMessage(m_session, "%s flagged to customize his character next logon.", args);
    sGMLog.writefromsession(m_session, "flagged %s for customization for charater (%u)", pi->name, pi->guid);
    return true;
}

bool ChatHandler::HandleFactionChange(const char* args, WorldSession* m_session)
{
    // prevent buffer overflow
    if (strlen(args) > 100)
        return false;

    std::string tmp = std::string(args);
    PlayerInfo* pi = objmgr.GetPlayerInfoByName(tmp.c_str());
    if (pi == 0)
    {
        RedSystemMessage(m_session, "Player with that name not found.");
        return true;
    }

    Player* plr = objmgr.GetPlayer((uint32)pi->guid);
    if (plr == 0)
    {
        CharacterDatabase.Execute("UPDATE characters SET login_flags = %u WHERE guid = %u", (uint32)LOGIN_CUSTOMIZE_FACTION, (uint32)pi->guid);
    }
    else
    {
        plr->login_flags = LOGIN_CUSTOMIZE_FACTION;
        plr->SaveToDB(false);
        BlueSystemMessageToPlr(plr, "%s flagged your character for a faction change at next login.", m_session->GetPlayer()->GetName());
    }

    GreenSystemMessage(m_session, "%s flagged for a faction change next logon.", args);
    sGMLog.writefromsession(m_session, "flagged %s for a faction change for charater (%u)", pi->name, pi->guid);
    return true;
}

bool ChatHandler::HandleRaceChange(const char* args, WorldSession* m_session)
{
    // prevent buffer overflow
    if (strlen(args) > 100)
        return false;

    std::string tmp = std::string(args);
    PlayerInfo* pi = objmgr.GetPlayerInfoByName(tmp.c_str());
    if (pi == 0)
    {
        RedSystemMessage(m_session, "Player with that name not found.");
        return true;
    }

    Player* plr = objmgr.GetPlayer((uint32)pi->guid);
    if (plr == 0)
    {
        CharacterDatabase.Execute("UPDATE characters SET login_flags = %u WHERE guid = %u", (uint32)LOGIN_CUSTOMIZE_RACE, (uint32)pi->guid);
    }
    else
    {
        plr->login_flags = LOGIN_CUSTOMIZE_RACE;
        plr->SaveToDB(false);
        BlueSystemMessageToPlr(plr, "%s flagged your character for a race change at next login.", m_session->GetPlayer()->GetName());
    }

    GreenSystemMessage(m_session, "%s flagged for a race change next logon.", args);
    sGMLog.writefromsession(m_session, "flagged %s for a race change for charater (%u)", pi->name, pi->guid);
    return true;
}

bool ChatHandler::HandleGetStandingCommand(const char* args, WorldSession* m_session)
{
    uint32 faction = atoi(args);
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return true;

    int32 standing = plr->GetStanding(faction);
    int32 bstanding = plr->GetBaseStanding(faction);

    GreenSystemMessage(m_session, "Reputation for faction %u:", faction);
    SystemMessage(m_session, "Base Standing: %d", bstanding);
    BlueSystemMessage(m_session, "Standing: %d", standing);
    return true;
}

bool ChatHandler::HandleSetStandingCommand(const char* args, WorldSession* m_session)
{
    uint32 faction;
    int32 standing;
    if (sscanf(args, "%u %d", (unsigned int*)&faction, (unsigned int*)&standing) != 2)
        return false;
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return true;

    BlueSystemMessage(m_session, "Setting standing of %u to %d on %s.", faction, standing, plr->GetName());
    plr->SetStanding(faction, standing);
    GreenSystemMessageToPlr(plr, "%s set your standing of faction %u to %d.", m_session->GetPlayer()->GetName(), faction, standing);
    sGMLog.writefromsession(m_session, "set standing of faction %u to %u for %s", faction, standing, plr->GetName());
    return true;
}

void ChatHandler::SendHighlightedName(WorldSession* m_session, const char* prefix, const char* full_name, std::string & lowercase_name, std::string & highlight, uint32 id)
{
    char message[1024];
    char start[50];
    start[0] = 0;
    message[0] = 0;

    snprintf(start, 50, "%s %u: %s", prefix, (unsigned int)id, MSG_COLOR_WHITE);

    auto highlight_length = highlight.length();
    std::string fullname = std::string(full_name);
    auto offset = lowercase_name.find(highlight);
    auto remaining = fullname.size() - offset - highlight_length;

    strcat(message, start);
    strncat(message, fullname.c_str(), offset);
    strcat(message, MSG_COLOR_LIGHTRED);
    strncat(message, (fullname.c_str() + offset), highlight_length);
    strcat(message, MSG_COLOR_WHITE);
    if (remaining > 0)
        strncat(message, (fullname.c_str() + offset + highlight_length), remaining);

    SystemMessage(m_session, message);
}

void ChatHandler::SendItemLinkToPlayer(ItemPrototype* iProto, WorldSession* pSession, bool ItemCount, Player* owner, uint32 language)
{
    if (!iProto || !pSession)
        return;
    if (ItemCount && owner == NULL)
        return;

    if (ItemCount)
    {
        int8 count = static_cast<int8>(owner->GetItemInterface()->GetItemCount(iProto->ItemId, true));
        //int8 slot = owner->GetItemInterface()->GetInventorySlotById(iProto->ItemId); //DISABLED due to being a retarded concept
        if (iProto->ContainerSlots > 0)
        {
            SystemMessage(pSession, "Item %u %s Count %u ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), count, iProto->ContainerSlots);
        }
        else
        {
            SystemMessage(pSession, "Item %u %s Count %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), count);
        }
    }
    else
    {
        if (iProto->ContainerSlots > 0)
        {
            SystemMessage(pSession, "Item %u %s ContainerSlots %u", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str(), iProto->ContainerSlots);
        }
        else
        {
            SystemMessage(pSession, "Item %u %s", iProto->ItemId, GetItemLinkByProto(iProto, language).c_str());
        }
    }
}


bool ChatHandler::HandleLookupItemCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);
    if (x.length() < 4)
    {
        RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
        return true;
    }

    BlueSystemMessage(m_session, "Starting search of item `%s`...", x.c_str());
    uint32 t = getMSTime();
    ItemPrototype* it;
    uint32 count = 0;

    StorageContainerIterator<ItemPrototype> * itr = ItemPrototypeStorage.MakeIterator();

    while (!itr->AtEnd())
    {
        it = itr->Get();
        LocalizedItem* lit = (m_session->language > 0) ? sLocalizationMgr.GetLocalizedItem(it->ItemId, m_session->language) : NULL;

        std::string litName = std::string(lit ? lit->Name : "");

        arcemu_TOLOWER(litName);

        bool localizedFound = false;
        if (FindXinYString(x, litName))
            localizedFound = true;

        if (FindXinYString(x, it->lowercase_name) || localizedFound)
        {
            // Print out the name in a cool highlighted fashion
            //SendHighlightedName(m_session, it->Name1, it->lowercase_name, x, it->ItemId, true);
            SendItemLinkToPlayer(it, m_session, false, 0, localizedFound ? m_session->language : 0);
            ++count;
            if (count == 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }

        if (!itr->Inc())
            break;
    }
    itr->Destruct();

    BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleLookupObjectCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);

    StorageContainerIterator<GameObjectInfo> * itr = GameObjectNameStorage.MakeIterator();

    GreenSystemMessage(m_session, "Starting search of object `%s`...", x.c_str());
    uint32 t = getMSTime();
    GameObjectInfo* gameobject_info;
    uint32 count = 0;
    std::string y;
    std::string recout;
    while (!itr->AtEnd())
    {
        gameobject_info = itr->Get();
        y = std::string(gameobject_info->name);
        arcemu_TOLOWER(y);
        if (FindXinYString(x, y))
        {
            //string objectID=MyConvertIntToString(i->ID);
            std::string Name;
            std::stringstream strm;
            strm << gameobject_info->entry;
            strm << ", Display ";
            strm << gameobject_info->display_id;
            //string ObjectID = i.c_str();
            const char* objectName = gameobject_info->name;
            recout = "|cfffff000Object ";
            recout += strm.str();
            recout += "|cffFFFFFF: ";
            recout += objectName;
            recout = recout + Name;
            SendMultilineMessage(m_session, recout.c_str());

            ++count;
            if (count == 25 || count > 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }
        if (!itr->Inc()) break;
    }
    itr->Destruct();
    if (count == 0)
    {
        recout = "|cff00ccffNo matches found.";
        SendMultilineMessage(m_session, recout.c_str());
    }
    BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleLookupCreatureCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);
    if (x.length() < 4)
    {
        RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
        return true;
    }

    StorageContainerIterator<CreatureInfo> * itr = CreatureNameStorage.MakeIterator();

    GreenSystemMessage(m_session, "Starting search of creature `%s`...", x.c_str());
    uint32 t = getMSTime();
    CreatureInfo* creature_info;
    uint32 count = 0;
    std::string y;
    std::string recout;
    while (!itr->AtEnd())
    {
        creature_info = itr->Get();
        y = std::string(creature_info->Name);
        arcemu_TOLOWER(y);
        if (FindXinYString(x, y))
        {
            //string objectID=MyConvertIntToString(i->ID);
            std::string Name;
            std::stringstream strm;
            strm << creature_info->Id;
            strm << ", DisplayIds: ";
            strm << creature_info->Male_DisplayID;
            if (creature_info->Female_DisplayID != 0)
            {
                strm << ", ";
                strm << creature_info->Female_DisplayID;
            }
            if (creature_info->Male_DisplayID2 != 0)
            {
                strm << ", ";
                strm << creature_info->Male_DisplayID2;
            }
            if (creature_info->Female_DisplayID2 != 0)
            {
                strm << ", ";
                strm << creature_info->Female_DisplayID2;
            }

            //string ObjectID = i.c_str();
            const char* creature_name = creature_info->Name;
            recout = "|cfffff000Creature ";
            recout += strm.str();
            recout += "|cffFFFFFF: ";
            recout += creature_name;
            recout = recout + Name;
            SendMultilineMessage(m_session, recout.c_str());

            ++count;
            if (count == 25 || count > 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }
        if (!itr->Inc()) break;
    }
    itr->Destruct();
    if (count == 0)
    {
        recout = "|cff00ccffNo matches found.";
        SendMultilineMessage(m_session, recout.c_str());
    }
    BlueSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleLookupSpellCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);
    if (x.length() < 4)
    {
        RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
        return true;
    }

    GreenSystemMessage(m_session, "Starting search of spell `%s`...", x.c_str());
    uint32 t = getMSTime();
    uint32 count = 0;
    std::string recout;
    char itoabuf[12];
    for (uint32 index = 0; index < dbcSpell.GetNumRows(); ++index)
    {
        SpellEntry* spell = dbcSpell.LookupRow(index);
        std::string y = std::string(spell->Name);
        arcemu_TOLOWER(y);
        if (FindXinYString(x, y))
        {
            // Print out the name in a cool highlighted fashion
            // SendHighlightedName(m_session, "Spell", spell->Name, y, x, spell->Id);
            // Send spell link instead
            sprintf((char*)itoabuf, "%u", spell->Id);
            recout = (const char*)itoabuf;
            recout += ": |cff71d5ff|Hspell:";
            recout += (const char*)itoabuf;
            recout += "|h[";
            recout += spell->Name;
            recout += "]|h|r";

            std::string::size_type pos = recout.find('%');
            if (pos != std::string::npos)
            {
                recout.insert(pos + 1, "%");
            }

            SendMultilineMessage(m_session, recout.c_str());

            ++count;
            if (count == 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }
    }

    GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleLookupSkillCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);
    if (x.length() < 4)
    {
        RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
        return true;
    }

    GreenSystemMessage(m_session, "Starting search of skill `%s`...", x.c_str());
    uint32 t = getMSTime();
    uint32 count = 0;
    for (uint32 index = 0; index < dbcSkillLine.GetNumRows(); ++index)
    {
        skilllineentry* skill = dbcSkillLine.LookupRow(index);
        std::string y = std::string(skill->Name);
        arcemu_TOLOWER(y);
        if (FindXinYString(x, y))
        {
            // Print out the name in a cool highlighted fashion
            SendHighlightedName(m_session, "Skill", skill->Name, y, x, skill->id);
            ++count;
            if (count == 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }
    }

    GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleLookupFactionCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    std::string x = std::string(args);
    arcemu_TOLOWER(x);
    if (x.length() < 4)
    {
        RedSystemMessage(m_session, "Your search string must be at least 4 characters long.");
        return true;
    }

    GreenSystemMessage(m_session, "Starting search of faction `%s`...", x.c_str());
    uint32 t = getMSTime();
    uint32 count = 0;
    for (uint32 index = 0; index < dbcFaction.GetNumRows(); ++index)
    {
        FactionDBC* faction = dbcFaction.LookupRow(index);
        std::string y = std::string(faction->Name);
        arcemu_TOLOWER(y);
        if (FindXinYString(x, y))
        {
            // Print out the name in a cool highlighted fashion
            SendHighlightedName(m_session, "Faction", faction->Name, y, x, faction->ID);
            ++count;
            if (count == 25)
            {
                RedSystemMessage(m_session, "More than 25 results returned. aborting.");
                break;
            }
        }
    }

    GreenSystemMessage(m_session, "Search completed in %u ms.", getMSTime() - t);
    return true;
}

bool ChatHandler::HandleGORotate(const char* args, WorldSession* m_session)
{
    char Axis;
    float deg;
    if (sscanf(args, "%c %f", &Axis, &deg) < 1) return false;
    GameObject* go = m_session->GetPlayer()->GetSelectedGo();
    if (!go)
    {
        RedSystemMessage(m_session, "No selected GameObject...");
        return true;
    }

    // float rad = deg * (float(M_PI) / 180.0f);

    switch (tolower(Axis))
    {
        case 'x':
            //        go->ModFloatValue(GAMEOBJECT_ROTATION, rad);
            break;
        case 'y':
            //        go->ModFloatValue(GAMEOBJECT_ROTATION_01, rad);
            break;
        case 'o':
            if (m_session->GetPlayer())
            {
                float ori = m_session->GetPlayer()->GetOrientation();
                go->SetParentRotation(2, sinf(ori / 2));
                go->SetParentRotation(3, cosf(ori / 2));
                go->SetOrientation(ori);
                go->UpdateRotation();
            }
            break;
        default:
            RedSystemMessage(m_session, "Invalid Axis, Please use x, y, or o.");
            return true;
    }

    uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
    go->RemoveFromWorld(true);
    go->SetNewGuid(NewGuid);
    go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
    go->SaveToDB();
    //lets reselect the object that can be really annoying...
    m_session->GetPlayer()->m_GM_SelectedGO = NewGuid;
    return true;
}

bool ChatHandler::HandleGOMove(const char* args, WorldSession* m_session)
{
    // move the go to player's coordinates
    GameObject* go = m_session->GetPlayer()->GetSelectedGo();
    if (!go)
    {
        RedSystemMessage(m_session, "No selected GameObject...");
        return true;
    }

    // new GO position (player location)
    // orientation doesn't change
    float x = m_session->GetPlayer()->GetPositionX();
    float y = m_session->GetPlayer()->GetPositionY();
    float z = m_session->GetPlayer()->GetPositionZ();
    float o = go->GetOrientation();

    go->RemoveFromWorld(true);
    go->SetPosition(x, y, z, o);
    //    go->SetFloatValue(GAMEOBJECT_POS_X, x);
    //    go->SetFloatValue(GAMEOBJECT_POS_Y, y);
    //    go->SetFloatValue(GAMEOBJECT_POS_Z, z);
    uint32 NewGuid = m_session->GetPlayer()->GetMapMgr()->GenerateGameobjectGuid();
    go->SetNewGuid(NewGuid);
    go->PushToWorld(m_session->GetPlayer()->GetMapMgr());
    go->SaveToDB();
    m_session->GetPlayer()->m_GM_SelectedGO = NewGuid;
    return true;
}

bool ChatHandler::HandleNpcPossessCommand(const char* args, WorldSession* m_session)
{
    Unit* pTarget = getSelectedChar(m_session, false);
    if (!pTarget)
    {
        pTarget = getSelectedCreature(m_session, false);
        if (pTarget && (pTarget->IsPet() || pTarget->GetCreatedByGUID() != 0))
            return false;
    }

    if (!pTarget)
    {
        RedSystemMessage(m_session, "You must select a player/creature.");
        return true;
    }

    m_session->GetPlayer()->Possess(pTarget);
    BlueSystemMessage(m_session, "Possessed " I64FMT, pTarget->GetGUID());
    switch (pTarget->GetTypeId())
    {
        case TYPEID_PLAYER:
            sGMLog.writefromsession(m_session, "used possess command on PLAYER %s", static_cast< Player* >(pTarget)->GetName());
            break;
        case TYPEID_UNIT:
            sGMLog.writefromsession(m_session, "used possess command on CREATURE %s, sqlid %u", static_cast< Creature* >(pTarget)->GetCreatureInfo()->Name, static_cast< Creature* >(pTarget)->GetSQL_id());
            break;
    }
    return true;
}

bool ChatHandler::HandleNpcUnPossessCommand(const char* args, WorldSession* m_session)
{
    Creature* creature = getSelectedCreature(m_session);
    m_session->GetPlayer()->UnPossess();

    if (creature)
    {
        // restart movement
        creature->GetAIInterface()->SetAIState(STATE_IDLE);
        creature->GetAIInterface()->WipeHateList();
        creature->GetAIInterface()->WipeTargetList();

        if (creature->m_spawn)
        {
            // return to respawn coords
            float x = creature->m_spawn->x;
            float y = creature->m_spawn->y;
            float z = creature->m_spawn->z;
            float o = creature->m_spawn->o;
            creature->GetAIInterface()->MoveTo(x, y, z, o);
        }
    }
    GreenSystemMessage(m_session, "Removed any possessed targets.");
    sGMLog.writefromsession(m_session, "used unpossess command");
    return true;
}

bool ChatHandler::HandleRehashCommand(const char* args, WorldSession* m_session)
{
    /*
    rehashes
    */
    char msg[250];
    snprintf(msg, 250, "%s is rehashing config file.", m_session->GetPlayer()->GetName());
    sWorld.SendWorldWideScreenText(msg, 0);
    sWorld.SendWorldText(msg, 0);
    sWorld.Rehash(true);
    return true;
}

struct spell_thingo
{
    uint32 type;
    uint32 target;
};

std::list<SpellEntry*> aiagent_spells;
std::map<uint32, spell_thingo> aiagent_extra;

bool ChatHandler::HandleAIAgentDebugBegin(const char* args, WorldSession* m_session)
{
    QueryResult* result = WorldDatabase.Query("SELECT DISTINCT spell FROM ai_agents");
    if (!result) return false;

    do
    {
        SpellEntry* se = dbcSpell.LookupEntryForced(result->Fetch()[0].GetUInt32());
        if (se)
            aiagent_spells.push_back(se);
    }
    while (result->NextRow());
    delete result;

    for (std::list<SpellEntry*>::iterator itr = aiagent_spells.begin(); itr != aiagent_spells.end(); ++itr)
    {
        result = WorldDatabase.Query("SELECT * FROM ai_agents WHERE spell = %u", (*itr)->Id);
        ARCEMU_ASSERT(result != NULL);
        spell_thingo t;
        t.type = result->Fetch()[6].GetUInt32();
        t.target = result->Fetch()[7].GetUInt32();
        delete result;
        aiagent_extra[(*itr)->Id] = t;
    }

    GreenSystemMessage(m_session, "Loaded %u spells for testing.", aiagent_spells.size());
    return true;
}

SpellCastTargets SetTargets(SpellEntry* sp, uint32 type, uint32 targettype, Unit* dst, Creature* src)
{
    SpellCastTargets targets;
    targets.m_unitTarget = 0;
    targets.m_itemTarget = 0;
    targets.m_srcX = 0;
    targets.m_srcY = 0;
    targets.m_srcZ = 0;
    targets.m_destX = 0;
    targets.m_destY = 0;
    targets.m_destZ = 0;

    if (targettype == TTYPE_SINGLETARGET)
    {
        targets.m_targetMask = TARGET_FLAG_UNIT;
        targets.m_unitTarget = dst->GetGUID();
    }
    else if (targettype == TTYPE_SOURCE)
    {
        targets.m_targetMask = TARGET_FLAG_SOURCE_LOCATION;
        targets.m_srcX = src->GetPositionX();
        targets.m_srcY = src->GetPositionY();
        targets.m_srcZ = src->GetPositionZ();
    }
    else if (targettype == TTYPE_DESTINATION)
    {
        targets.m_targetMask = TARGET_FLAG_DEST_LOCATION;
        targets.m_destX = dst->GetPositionX();
        targets.m_destY = dst->GetPositionY();
        targets.m_destZ = dst->GetPositionZ();
    }

    return targets;
};

bool ChatHandler::HandleAIAgentDebugContinue(const char* args, WorldSession* m_session)
{
    uint32 count = atoi(args);
    if (!count)
        return false;

    Creature* pCreature = getSelectedCreature(m_session, true);
    if (!pCreature)
        return true;

    Player* pPlayer = m_session->GetPlayer();

    for (uint32 i = 0; i < count; ++i)
    {
        if (!aiagent_spells.size())
            break;

        SpellEntry* sp = *aiagent_spells.begin();
        aiagent_spells.erase(aiagent_spells.begin());
        BlueSystemMessage(m_session, "Casting %u, " MSG_COLOR_SUBWHITE "%u remaining.", sp->Id, aiagent_spells.size());

        std::map<uint32, spell_thingo>::iterator it = aiagent_extra.find(sp->Id);
        ARCEMU_ASSERT(it != aiagent_extra.end());

        SpellCastTargets targets;
        if (it->second.type == STYPE_BUFF)
            targets = SetTargets(sp, it->second.type, it->second.type, pCreature, pCreature);
        else
            targets = SetTargets(sp, it->second.type, it->second.type, pPlayer, pCreature);

        pCreature->GetAIInterface()->CastSpell(pCreature, sp, targets);
    }

    if (!aiagent_spells.size())
        RedSystemMessage(m_session, "Finished.");
    /*else
        BlueSystemMessage(m_session, "Got %u remaining.", aiagent_spells.size());*/
    return true;
}

bool ChatHandler::HandleAIAgentDebugSkip(const char* args, WorldSession* m_session)
{
    uint32 count = atoi(args);
    if (!count) return false;

    for (uint32 i = 0; i < count; ++i)
    {
        if (!aiagent_spells.size())
            break;

        aiagent_spells.erase(aiagent_spells.begin());
    }
    BlueSystemMessage(m_session, "Erased %u spells.", count);
    return true;
}

bool ChatHandler::HandleRenameGuildCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session);
    if (!plr || !plr->GetGuildId() || !plr->GetGuild() || !args || !strlen(args))
        return false;

    Guild* pGuild = objmgr.GetGuildByGuildName(std::string(args));

    if (pGuild)
    {
        RedSystemMessage(m_session, "Guild name %s is already taken.", args);
        return false;
    }

    GreenSystemMessage(m_session, "Changed guild name of %s to %s. This will take effect next restart.", plr->GetGuild()->GetGuildName(), args);
    CharacterDatabase.Execute("UPDATE guilds SET `guildName` = \'%s\' WHERE `guildId` = '%u'", CharacterDatabase.EscapeString(std::string(args)).c_str(), plr->GetGuild()->GetGuildId());
    sGMLog.writefromsession(m_session, "Changed guild name of '%s' to '%s'", plr->GetGuild()->GetGuildName(), args);
    return true;
}

//People seem to get stuck in guilds from time to time. This should be helpful. -DGM
bool ChatHandler::HandleGuildRemovePlayerCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session);
    if (!plr || !plr->GetGuildId() || !plr->GetGuild() || plr->GetGuild()->GetGuildLeader() == plr->GetLowGUID())
        return false;

    GreenSystemMessage(m_session, "Kicked %s from Guild: %s", plr->GetName(), plr->GetGuild()->GetGuildName());

    if (plr->GetLowGUID() != m_session->GetPlayer()->GetLowGUID())
        sGMLog.writefromsession(m_session, "Kicked %s from Guild %s", plr->GetName(), plr->GetGuild()->GetGuildName());

    plr->GetGuild()->RemoveGuildMember(plr->getPlayerInfo(), plr->GetSession());
    return true;
}

//-DGM
bool ChatHandler::HandleGuildDisbandCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session);
    if (!plr || !plr->GetGuildId() || !plr->GetGuild())
        return false;

    GreenSystemMessage(m_session, "Disbanded Guild: %s", plr->GetGuild()->GetGuildName());
    sGMLog.writefromsession(m_session, "Disbanded Guild %s", plr->GetGuild()->GetGuildName());
    plr->GetGuild()->Disband();
    return true;
}

bool ChatHandler::HandleGuildJoinCommand(const char* args, WorldSession* m_session)
{
    if (!*args)
        return false;

    Player* ptarget = getSelectedChar(m_session);
    if (!ptarget) return false;

    if (ptarget->IsInGuild())
    {
        RedSystemMessage(m_session, "%s is already in a guild.", ptarget->GetName());
        return true;
    }

    Guild* pGuild = NULL;
    pGuild = objmgr.GetGuildByGuildName(std::string(args));

    if (pGuild)
    {
        pGuild->getLock().Acquire();
        uint32 memberCount = pGuild->GetNumMembers();
        pGuild->getLock().Release();

        if (memberCount >= MAX_GUILD_MEMBERS)
        {
            m_session->SystemMessage("That guild is full.");
            return true;
        }

        pGuild->AddGuildMember(ptarget->getPlayerInfo(), m_session, -2);
        GreenSystemMessage(m_session, "You have joined the guild '%s'", pGuild->GetGuildName());
        sGMLog.writefromsession(m_session, "Force joined guild '%s'", pGuild->GetGuildName());
        return true;
    }

    return false;
}

//-DGM
bool ChatHandler::HandleGuildMembersCommand(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session);
    if (!plr || !plr->GetGuildId() || !plr->GetGuild())
        return false;
    GreenSystemMessage(m_session, "Now showing guild members for %s", plr->GetGuild()->GetGuildName());
    plr->GetGuild()->Lock();
    for (GuildMemberMap::iterator itr = plr->GetGuild()->GetGuildMembersBegin(); itr != plr->GetGuild()->GetGuildMembersEnd(); ++itr)
    {
        GuildMember* member = itr->second;
        if (!member || !member->pPlayer)
            continue;

        BlueSystemMessage(m_session, "%s (Rank: %s)", member->pPlayer->name, member->pRank->szRankName);
    }
    plr->GetGuild()->Unlock();
    return true;
}

bool ChatHandler::HandleArenaCreateTeamCommand(const char* args, WorldSession* m_session)
{
    uint32 arena_team_type;
    char name[1000];
    uint32 real_type;
    Player* plr = getSelectedChar(m_session, true);
    if (sscanf(args, "%u %[^\n]", &arena_team_type, name) != 2)
    {
        SystemMessage(m_session, "Invalid syntax.");
        return true;
    }

    switch (arena_team_type)
    {
        case 2:
            real_type = 0;
            break;
        case 3:
            real_type = 1;
            break;
        case 5:
            real_type = 2;
            break;
        default:
            SystemMessage(m_session, "Invalid arena team type specified.");
            return true;
    }

    if (!plr)
        return true;

    if (plr->m_arenaTeams[real_type] != NULL)
    {
        SystemMessage(m_session, "Already in an arena team of that type.");
        return true;
    }

    ArenaTeam* t = new ArenaTeam(real_type, objmgr.GenerateArenaTeamId());
    t->m_emblemStyle = 22;
    t->m_emblemColour = 4292133532UL;
    t->m_borderColour = 4294931722UL;
    t->m_borderStyle = 1;
    t->m_backgroundColour = 4284906803UL;
    t->m_leader = plr->GetLowGUID();
    t->m_name = std::string(name);
    t->AddMember(plr->getPlayerInfo());
    objmgr.AddArenaTeam(t);
    SystemMessage(m_session, "created arena team.");
    return true;
}

bool ChatHandler::HandleArenaSetTeamLeaderCommand(const char* args, WorldSession* m_session)
{
    uint32 arena_team_type;
    uint32 real_type;
    Player* plr = getSelectedChar(m_session, true);
    if (sscanf(args, "%u", &arena_team_type) != 1)
    {
        SystemMessage(m_session, "Invalid syntax.");
        return true;
    }

    switch (arena_team_type)
    {
        case 2:
            real_type = 0;
            break;
        case 3:
            real_type = 1;
            break;
        case 5:
            real_type = 2;
            break;
        default:
            SystemMessage(m_session, "Invalid arena team type specified.");
            return true;
    }

    if (!plr)
        return true;

    if (plr->m_arenaTeams[real_type] == NULL)
    {
        SystemMessage(m_session, "Not in an arena team of that type.");
        return true;
    }

    ArenaTeam* t = plr->m_arenaTeams[real_type];
    t->SetLeader(plr->getPlayerInfo());
    SystemMessage(m_session, "player is now arena team leader.");
    return true;
}

bool ChatHandler::HandleArenaResetAllRatingsCommand(const char* args, WorldSession* m_session)
{
    objmgr.ResetArenaTeamRatings();
    return true;
}

bool ChatHandler::HandleWhisperBlockCommand(const char* args, WorldSession* m_session)
{
    if (m_session->GetPlayer()->HasFlag(PLAYER_FLAGS, PLAYER_FLAG_GM))
        return false;

    m_session->GetPlayer()->SetFlag(PLAYER_FLAGS, PLAYER_FLAG_GM);
    return true;
}

bool ChatHandler::HandleGenderChanger(const char* args, WorldSession* m_session)
{
    uint8 gender;
    Player* target = objmgr.GetPlayer((uint32)m_session->GetPlayer()->GetSelection());
    if (target == NULL)
    {
        SystemMessage(m_session, "Select A Player first.");
        return true;
    }
    uint32 displayId = target->GetNativeDisplayId();
    if (*args == 0)
        gender = target->getGender() == 1 ? 0 : 1;
    else
    {
        gender = (uint8)atoi((char*)args);
        if (gender > 1)
            gender = 1;
    }

    if (gender == target->getGender())
    {
        SystemMessage(m_session, "%s's gender is already set to %s(%u).", target->GetName(), gender ? "Female" : "Male", gender);
        return true;
    }

    target->setGender(gender);

    if (target->getGender() == 0)
    {
        target->SetDisplayId((target->getRace() == RACE_BLOODELF) ? ++displayId : --displayId);
        target->SetNativeDisplayId(displayId);
    }
    else
    {
        target->SetDisplayId((target->getRace() == RACE_BLOODELF) ? --displayId : ++displayId);
        target->SetNativeDisplayId(displayId);
    }
    target->EventModelChange();

    SystemMessage(m_session, "Set %s's gender to %s(%u).", target->GetName(), gender ? "Female" : "Male", gender);
    return true;
}

bool ChatHandler::HandleDispelAllCommand(const char* args, WorldSession* m_session)
{
    uint32 pos = 0;
    if (*args)
        pos = atoi(args);

    Player* plr;

    sGMLog.writefromsession(m_session, "used dispelall command, pos %u", pos);

    PlayerStorageMap::const_iterator itr;
    objmgr._playerslock.AcquireReadLock();
    for (itr = objmgr._players.begin(); itr != objmgr._players.end(); ++itr)
    {
        plr = itr->second;
        if (plr->GetSession() && plr->IsInWorld())
        {
            if (plr->GetMapMgr() != m_session->GetPlayer()->GetMapMgr())
            {
                sEventMgr.AddEvent(static_cast< Unit* >(plr), &Unit::DispelAll, pos ? true : false, EVENT_PLAYER_CHECKFORCHEATS, 100, 1, EVENT_FLAG_DO_NOT_EXECUTE_IN_WORLD_CONTEXT);
            }
            else
            {
                plr->DispelAll(pos ? true : false);
            }
        }
    }
    sGMLog.writefromsession(m_session, "used mass dispel");
    objmgr._playerslock.ReleaseReadLock();

    BlueSystemMessage(m_session, "Dispel action done.");
    return true;
}

bool ChatHandler::HandleShowItems(const char* args, WorldSession* m_session)
{
    std::string q;
    Player* plr = getSelectedChar(m_session, true);
    if (!plr) return true;
    BlueSystemMessage(m_session, "Listing items for player %s", plr->GetName());
    int itemcount = 0;
    ItemIterator itr(plr->GetItemInterface());
    itr.BeginSearch();
    for (; !itr.End(); itr.Increment())
    {
        if (!(*itr))
            return false;
        itemcount++;
        SendItemLinkToPlayer((*itr)->GetProto(), m_session, true, plr, m_session->language);
    }
    itr.EndSearch();
    BlueSystemMessage(m_session, "Listed %d items for player %s", itemcount, plr->GetName());

    sGMLog.writefromsession(m_session, "used show items command on %s,", plr->GetName());

    return true;
}

bool ChatHandler::HandleShowSkills(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, true);
    if (!plr)
        return true;

    BlueSystemMessage(m_session, "Listing items for player %s", plr->GetName());
    int itemcount = 0;
    SkillIterator itr2(plr);
    itr2.BeginSearch();
    for (; !itr2.End(); itr2.Increment())
    {
        itemcount++;
        SystemMessage(m_session, "Skill %u %s %u/%u", itr2->Skill->id, itr2->Skill->Name, itr2->CurrentValue, itr2->MaximumValue);
    }
    itr2.EndSearch();
    BlueSystemMessage(m_session, "Listed %d skills for player %s", itemcount, plr->GetName());
    sGMLog.writefromsession(m_session, "used show skills command on %s,", plr->GetName());

    return true;
}

bool ChatHandler::HandleCollisionTestIndoor(const char* args, WorldSession* m_session)
{
    if (sWorld.Collision)
    {
        Player* plr = m_session->GetPlayer();
        const LocationVector & loc = plr->GetPosition();
        bool res = CollideInterface.IsIndoor(plr->GetMapId(), loc.x, loc.y, loc.z + 2.0f);
        SystemMessage(m_session, "Result was: %s.", res ? "indoors" : "outside");
        return true;
    }
    else
    {
        SystemMessage(m_session, "Collision is not enabled.");
        return true;
    }
}

bool ChatHandler::HandleCollisionTestLOS(const char* args, WorldSession* m_session)
{
    if (sWorld.Collision)
    {
        Object* pObj = NULL;
        Creature* pCreature = getSelectedCreature(m_session, false);
        Player* pPlayer = getSelectedChar(m_session, false);
        if (pCreature)
            pObj = pCreature;
        else if (pPlayer)
            pObj = pPlayer;

        if (pObj == NULL)
        {
            SystemMessage(m_session, "Invalid target.");
            return true;
        }

        const LocationVector & loc2 = pObj->GetPosition();
        const LocationVector & loc1 = m_session->GetPlayer()->GetPosition();
        bool res = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z, loc2.x, loc2.y, loc2.z);
        bool res2 = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z + 2.0f, loc2.x, loc2.y, loc2.z + 2.0f);
        bool res3 = CollideInterface.CheckLOS(pObj->GetMapId(), loc1.x, loc1.y, loc1.z + 5.0f, loc2.x, loc2.y, loc2.z + 5.0f);
        SystemMessage(m_session, "Result was: %s %s %s.", res ? "in LOS" : "not in LOS", res2 ? "in LOS" : "not in LOS", res3 ? "in LOS" : "not in LOS");
        return true;
    }
    else
    {
        SystemMessage(m_session, "Collision is not enabled.");
        return true;
    }
}

bool ChatHandler::HandleGetDeathState(const char* args, WorldSession* m_session)
{
    Player* SelectedPlayer = getSelectedChar(m_session, true);
    if (!SelectedPlayer)
        return true;

    SystemMessage(m_session, "Death State: %d", SelectedPlayer->getDeathState());
    return true;
}


bool ChatHandler::HandleCollisionGetHeight(const char* args, WorldSession* m_session)
{
    if (sWorld.Collision)
    {
        Player* plr = m_session->GetPlayer();
        float radius = 5.0f;

        float posX = plr->GetPositionX();
        float posY = plr->GetPositionY();
        float posZ = plr->GetPositionZ();
        float ori = plr->GetOrientation();

        LocationVector src(posX, posY, posZ);

        LocationVector dest(posX + (radius * (cosf(ori))), posY + (radius * (sinf(ori))), posZ);
        //LocationVector destest(posX+(radius*(cosf(ori))),posY+(radius*(sinf(ori))),posZ);


        float z = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ + 2.0f);
        float z2 = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ + 5.0f);
        float z3 = CollideInterface.GetHeight(plr->GetMapId(), posX, posY, posZ);
        float z4 = plr->GetMapMgr()->GetADTLandHeight(plr->GetPositionX(), plr->GetPositionY());
        bool fp = CollideInterface.GetFirstPoint(plr->GetMapId(), src, dest, dest, -1.5f);

        SystemMessage(m_session, "Results were: %f(offset2.0f) | %f(offset5.0f) | %f(org) | landheight:%f | target radius5 FP:%d", z, z2, z3, z4, fp);
        return true;
    }
    else
    {
        SystemMessage(m_session, "Collision is not enabled.");
        return true;
    }
}
bool ChatHandler::HandleLevelUpCommand(const char* args, WorldSession* m_session)
{
    int levels = 0;

    if (!*args)
        levels = 1;
    else
        levels = atoi(args);

    if (levels <= 0)
        return false;

    Player* plr = getSelectedChar(m_session, true);

    if (!plr) plr = m_session->GetPlayer();

    if (!plr) return false;

    sGMLog.writefromsession(m_session, "used level up command on %s, with %u levels", plr->GetName(), levels);

    levels += plr->getLevel();

    if (levels > PLAYER_LEVEL_CAP)
        levels = PLAYER_LEVEL_CAP;

    LevelInfo* inf = objmgr.GetLevelInfo(plr->getRace(), plr->getClass(), levels);
    if (!inf)
        return false;
    plr->ApplyLevelInfo(inf, levels);
    if (plr->getClass() == WARLOCK)
    {
        std::list<Pet*> summons = plr->GetSummons();
        for (std::list<Pet*>::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        {
            if ((*itr)->IsInWorld() && (*itr)->isAlive())
            {
                (*itr)->setLevel(levels);
                (*itr)->ApplyStatsForLevel();
                (*itr)->UpdateSpellList();
            }
        }
    }

    WorldPacket data;
    std::stringstream sstext;
    sstext << "You have been leveled up to Level " << levels << '\0';
    SystemMessage(plr->GetSession(), sstext.str().c_str());

    plr->Social_TellFriendsOnline();

    return true;
}

bool ChatHandler::HandleFixScaleCommand(const char* args, WorldSession* m_session)
{
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == NULL)
        return true;

    float sc = (float)atof(args);
    if (sc < 0.1f)
    {
        return false;
    }

    pCreature->SetScale(sc);
    pCreature->GetProto()->Scale = sc;
    WorldDatabase.Execute("UPDATE creature_proto SET scale = '%f' WHERE entry = %u", sc, pCreature->GetEntry());
    return true;
}

bool ChatHandler::HandleAddTrainerSpellCommand(const char* args, WorldSession* m_session)
{
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == NULL)
        return true;

    uint32 spellid, cost, reqlevel;
    if (sscanf(args, "%u %u %u", &spellid, &cost, &reqlevel ) != 5)
        return false;

    Trainer* pTrainer = pCreature->GetTrainer();
    if (pTrainer == NULL)
    {
        RedSystemMessage(m_session, "Target is not a trainer.");
        return true;
    }

    SpellEntry* pSpell = dbcSpell.LookupEntryForced(spellid);
    if (pSpell == NULL)
    {
        RedSystemMessage(m_session, "Invalid spell.");
        return true;
    }

    if (pSpell->eff[0].Effect == SPELL_EFFECT_INSTANT_KILL || pSpell->eff[1].Effect == SPELL_EFFECT_INSTANT_KILL || pSpell->eff[2].Effect == SPELL_EFFECT_INSTANT_KILL)
    {
        RedSystemMessage(m_session, "No. You're not doing that.");
        return true;
    }

    TrainerSpell sp;
    sp.spellCost = cost;
    sp.spell = pSpell->Id;
    sp.reqLevel = reqlevel;

    pTrainer->Spells.push_back(sp);
    pTrainer->SpellCount++;

    SystemMessage(m_session, "Added spell %u (%s) to trainer.", pSpell->Id, pSpell->Name);
    sGMLog.writefromsession(m_session, "added spell %u to trainer %u", spellid, pCreature->GetEntry());
    WorldDatabase.Execute("INSERT INTO trainer_spells VALUES(%u, %u, %u, %u, %u ,%u)",
                          pCreature->GetEntry(), pSpell->Id, cost, uint32(0), uint32(0), reqlevel);

    return true;
}

bool ChatHandler::HandleSetTitle(const char* args, WorldSession* m_session)
{
    Player* plr = getSelectedChar(m_session, true);
    if (plr == NULL)
        return false;

    int32 title = atol(args);
    if (title > int32(PVPTITLE_END) || title < -int32(PVPTITLE_END))
    {
        RedSystemMessage(m_session, "Argument %i is out of range!", title);
        return false;
    }
    if (title == 0)
    {
        plr->SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES, 0);
        plr->SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES1, 0);
        plr->SetUInt64Value(PLAYER__FIELD_KNOWN_TITLES2, 0);
    }
    else if (title > 0)
        plr->SetKnownTitle(static_cast<RankTitles>(title), true);
    else
        plr->SetKnownTitle(static_cast<RankTitles>(-title), false);

    plr->SetChosenTitle(0);  // better remove chosen one
    SystemMessage(m_session, "Title has been %s.", title > 0 ? "set" : "reset");

    std::stringstream logtext;
    logtext << "has ";
    if (title > 0)
        logtext << "set the title field of " << plr->GetName() << "to " << title << ".";
    else
        logtext << "reset the title field of " << plr->GetName();

    sGMLog.writefromsession(m_session, logtext.str().c_str());
    return true;
}

bool ChatHandler::HandleNPCLootCommand(const char* args, WorldSession* m_session)
{
    Creature* pCreature = getSelectedCreature(m_session, true);
    if (pCreature == NULL)
    {
        return false;
    }

    QueryResult* _result = WorldDatabase.Query("SELECT itemid, normal10percentchance, heroic10percentchance, normal25percentchance, heroic25percentchance, mincount, maxcount FROM loot_creatures WHERE entryid=%u;", pCreature->GetEntry());
    if (_result != NULL)
    {
        Field* _field;
        std::stringstream ss;
        ItemPrototype* proto;
        std::string color;
        int32 minQuality = 0;
        uint8 numFound = 0;

        if (*args)
        {
            minQuality = atol(args);
        }

        do
        {
            _field = _result->Fetch();
            ss.str("");
            proto = ItemPrototypeStorage.LookupEntry(_field[0].GetUInt32());
            if (proto == NULL || (int32)proto->Quality < minQuality)
            {
                continue;
            }
            ++numFound;
            ss << "(N10 " << _field[1].GetFloat() << "%%) (N25 " << _field[3].GetFloat() << "%%) (H10 " << _field[2].GetFloat() << "%%) (H25 " << _field[4].GetFloat() << "%%): ";

            switch (proto->Quality)
            {
                case 0: //Poor,gray
                    color = "cff9d9d9d";
                    break;
                case 1: //Common,white
                    color = "cffffffff";
                    break;
                case 2: //Uncommon,green
                    color = "cff1eff00";
                    break;
                case 3: //Rare,blue
                    color = "cff0070dd";
                    break;
                case 4: //Epic,purple
                    color = "cffa335ee";
                    break;
                case 5: //Legendary,orange
                    color = "cffff8000";
                    break;
                case 6: //Artifact,pale gold
                    color = "c00fce080";
                    break;
                case 7: //Heirloom,pale gold
                    color = "c00fce080";
                    break;
                default:
                    color = "cff9d9d9d";
                    break;
            }

            ss << "|" << color.c_str() << "|Hitem:" << proto->ItemId << ":0:0:0:0:0:0:0|h[" << proto->Name1;
            ss << "]|h|r (" << _field[5].GetUInt32() << "-" << _field[6].GetUInt32() << ")";
            SystemMessage(m_session, ss.str().c_str(), '%', '%');
        }
        while (_result->NextRow() && (numFound <= 25));
        delete _result;
        if (numFound > 25)
        {
            SystemMessage(m_session, "More than 25 results found.");
        }
        else
        {
            SystemMessage(m_session, "%lu results found.", numFound);
        }
    }
    else
    {
        SystemMessage(m_session, "0 results found.");
    }
    return true;
}

bool ChatHandler::HandleNPCCastCommand(const char* args, WorldSession* m_session)
{
    if (*args == '\0')
        return false;

    Creature* c = getSelectedCreature(m_session);
    if (c == NULL)
        return false;

    uint32 spellid = atol(args);
    if (spellid == 0)
        return false;

    SpellEntry* sp = dbcSpell.LookupEntry(spellid);
    if (sp == NULL)
        return false;

    c->CastSpell(reinterpret_cast<Unit*>(NULL), sp, false);

    return true;
}
