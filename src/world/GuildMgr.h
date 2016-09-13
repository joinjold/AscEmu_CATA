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

#ifndef _GUILD_MGR_H
#define _GUILD_MGR_H

#include "StdAfx.h"

class SERVER_DECL GuildMgr : public Singleton < GuildMgr >
{

public:
    GuildMgr();
    ~GuildMgr();

public:
    Guild* GetGuildByLeader(uint64 guid) const;
    Guild* GetGuildById(uint32 guildId) const;
    Guild* GetGuildByName(std::string const& guildName) const;
    std::string GetGuildNameById(uint32 guildId) const;

    void LoadGuildXpForLevel();
    void LoadGuildRewards();

    void LoadGuilds();
    void AddGuild(Guild* guild);
    void RemoveGuild(uint32 guildId);

    void Update(uint32 diff);
    void SaveGuilds();

    void ResetReputationCaps();

    uint32 GenerateGuildId();
    //void SetNextGuildId(uint32 Id) { NextGuildId = Id; }

    uint32 GetXPForGuildLevel(uint8 level) const;
    std::vector<GuildReward> const& GetGuildRewards() const { return GuildRewards; }

    void ResetTimes(bool week);

    uint32 lastSave;
    bool firstSave;
protected:
    typedef std::unordered_map<uint32, Guild*> GuildContainer;
    //uint32 NextGuildId;
    GuildContainer GuildStore;
    std::vector<uint64> GuildXPperLevel;
    std::vector<GuildReward> GuildRewards;
};

#define sGuildMgr GuildMgr::getSingleton()


#endif // _GUILD_MGR_H
