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
#include "QuestLogEntry.hpp"


uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, QuestRelation* qst)
{
    return CalcQuestStatus(quest_giver, plr, qst->qst, qst->type, false);
}

bool QuestMgr::isRepeatableQuestFinished(Player* plr, Quest* qst)
{
    uint32 i;

    for (i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
    {
        if (qst->ReqItemId[i])
        {
            if (plr->GetItemInterface()->GetItemCount(qst->ReqItemId[i]) < qst->ReqItemCount[i])
            {
                return false;
            }
        }
    }

    return true;
}

uint32 QuestMgr::PlayerMeetsReqs(Player* plr, Quest const* qst, bool skiplevelcheck)
{
    std::list<uint32>::iterator itr;
    uint32 status;

    if (!qst->IsRepeatable() && !qst->IsDaily())
        status = QMGR_QUEST_AVAILABLE;
    else
    {
        status = QMGR_QUEST_REPEATABLE;
        if (qst->IsRepeatable() == QUEST_SPECIAL_FLAG_REPEATABLE && plr->HasFinishedDaily(qst->GetQuestId()))
            return QMGR_QUEST_NOT_AVAILABLE;
    }

    if (plr->getLevel() < qst->GetMinLevel() && !skiplevelcheck)
        return QMGR_QUEST_AVAILABLELOW_LEVEL;

    if (qst->GetSkillOrClassMask())
    {
        if (!(qst->GetSkillOrClassMask() != plr->getClassMask()))
            return QMGR_QUEST_NOT_AVAILABLE;
    }

    if (qst->GetRequiredRaces())
    {
        if (!(qst->GetRequiredRaces() != plr->getRaceMask()))
            return QMGR_QUEST_NOT_AVAILABLE;
    }

    /*
    if (qst->GetRewSkillLineId())
    {
        if (!plr->_HasSkillLine(qst->GetRewSkillLineId()))
            return QMGR_QUEST_NOT_AVAILABLE;
        if (qst->GetRequiredSkillValue() && plr->_GetSkillLineCurrent(qst->GetRewSkillLineId()) < qst->GetRequiredSkillValue())
            return QMGR_QUEST_NOT_AVAILABLE;
    }*/

    // Check reputation
    if (qst->GetRequiredMinRepFaction() || qst->GetRequiredMaxRepFaction())
        if (plr->GetStanding(qst->GetRepObjectiveFaction()) < (int32)qst->GetRequiredMinRepFaction())
            return QMGR_QUEST_NOT_AVAILABLE;

    if (plr->HasFinishedQuest(qst->GetQuestId()) && !qst->IsRepeatable() && !qst->IsDaily())
        return QMGR_QUEST_NOT_AVAILABLE;

    // Check One of Quest Prequest
    bool questscompleted = false;
    if (!qst->GetPrevQuestId() == 0)
    {
            Quest const* questcheck = objmgr.GetQuestTemplate(qst->GetPrevQuestId());
            if (questcheck)
            {
                if (plr->HasFinishedQuest(qst->GetPrevQuestId()))
                {
                    questscompleted = true;
                }
        }
        if (!questscompleted)
            return QMGR_QUEST_NOT_AVAILABLE;
    }

    if (!qst->prevChainQuests.empty())
    {
        for (Quest::PrevChainQuests::const_iterator iter = qst->prevChainQuests.begin(); iter != qst->prevChainQuests.end(); ++iter)
        {
            if (!plr->HasFinishedQuest(*iter))
                return QMGR_QUEST_NOT_AVAILABLE;
        }
    }

    // check quest level
    if (static_cast<int32>(plr->getLevel()) >= (qst->GetQuestLevel() + 5) && (status != QMGR_QUEST_REPEATABLE))
        return QMGR_QUEST_CHAT;

    return status;
}

uint32 QuestMgr::CalcQuestStatus(Object* quest_giver, Player* plr, Quest const* qst, uint8 type, bool skiplevelcheck)
{
    QuestLogEntry* qle;

    qle = plr->GetQuestLogForEntry(qst->GetQuestId());

    if (!qle)
    {
        if (type & QUESTGIVER_QUEST_START)
        {
            return PlayerMeetsReqs(plr, qst, skiplevelcheck);
        }
    }
    else
    {
        if (qle->HasFailed())
            return QMGR_QUEST_NOT_FINISHED;

        if (type & QUESTGIVER_QUEST_END)
        {
            if (!qle->CanBeFinished())
            {
                if (qst->GetQuestFlags() == QUEST_FLAGS_REPEATABLE)
                {
                    return QMGR_QUEST_REPEATABLE;
                }
                else
                {
                    return QMGR_QUEST_NOT_FINISHED;
                }
            }
            else
            {
                return QMGR_QUEST_FINISHED;
            }
        }
    }

    return QMGR_QUEST_NOT_AVAILABLE;
}

uint32 QuestMgr::CalcQuestStatus(Player* plr, uint32 qst)
{
    QuestLogEntry* qle = plr->GetQuestLogForEntry(qst);
    if (qle != NULL)
    {
        if (!qle->CanBeFinished())
        {
            return QMGR_QUEST_NOT_FINISHED;
        }
        else
        {
            return QMGR_QUEST_FINISHED;
        }
    }

    return QMGR_QUEST_NOT_AVAILABLE;
}

uint32 QuestMgr::CalcStatus(Object* quest_giver, Player* plr)
{
    uint32 status = QMGR_QUEST_NOT_AVAILABLE;
    std::list<QuestRelation*>::const_iterator itr;
    std::list<QuestRelation*>::const_iterator q_begin;
    std::list<QuestRelation*>::const_iterator q_end;
    bool bValid = false;

    if (quest_giver->IsGameObject())
    {
        bValid = static_cast< GameObject* >(quest_giver)->HasQuests();
        if (bValid)
        {
            q_begin = static_cast< GameObject* >(quest_giver)->QuestsBegin();
            q_end = static_cast< GameObject* >(quest_giver)->QuestsEnd();
        }
    }
    else if (quest_giver->IsCreature())
    {
        bValid = static_cast< Creature* >(quest_giver)->HasQuests();
        if (bValid)
        {
            q_begin = static_cast< Creature* >(quest_giver)->QuestsBegin();
            q_end = static_cast< Creature* >(quest_giver)->QuestsEnd();
        }
    }
    else if (quest_giver->IsItem())
    {
        if (static_cast< Item* >(quest_giver)->GetProto()->QuestId)
            bValid = true;
    }
    //This will be handled at quest share so nothing important as status
    else if (quest_giver->IsPlayer())
    {
        status = QMGR_QUEST_AVAILABLE;
    }

    if (!bValid)
    {
        //annoying message that is not needed since all objects don't exactly have quests
        //LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for CalcStatus. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
        return status;
    }

    if (quest_giver->IsItem())
    {
        Quest const* pQuest = objmgr.GetQuestTemplate(static_cast< Item* >(quest_giver)->GetProto()->QuestId);
        if (pQuest)
        {
            QuestRelation qr;
            qr.qst = pQuest;
            qr.type = 1;

            uint32 tmp_status = CalcQuestStatus(quest_giver, plr, &qr);
            if (tmp_status > status)
                status = tmp_status;
        }
    }

    for (itr = q_begin; itr != q_end; ++itr)
    {
        uint32 tmp_status = CalcQuestStatus(quest_giver, plr, *itr);	// save a call
        if (tmp_status > status)
            status = tmp_status;
    }

    return status;
}

uint32 QuestMgr::ActiveQuestsCount(Object* quest_giver, Player* plr)
{
    std::list<QuestRelation*>::const_iterator itr;
    std::map<uint32, uint8> tmp_map;
    uint32 questCount = 0;

    std::list<QuestRelation*>::const_iterator q_begin;
    std::list<QuestRelation*>::const_iterator q_end;
    bool bValid = false;

    if (quest_giver->IsGameObject())
    {
        bValid = static_cast< GameObject* >(quest_giver)->HasQuests();
        if (bValid)
        {
            q_begin = static_cast< GameObject* >(quest_giver)->QuestsBegin();
            q_end = static_cast< GameObject* >(quest_giver)->QuestsEnd();

        }
    }
    else if (quest_giver->IsCreature())
    {
        bValid = static_cast< Creature* >(quest_giver)->HasQuests();
        if (bValid)
        {
            q_begin = static_cast< Creature* >(quest_giver)->QuestsBegin();
            q_end = static_cast< Creature* >(quest_giver)->QuestsEnd();
        }
    }

    if (!bValid)
    {
        LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for ActiveQuestsCount. TypeId: %d.", quest_giver->GetGUID(), quest_giver->GetTypeId());
        return 0;
    }

    for (itr = q_begin; itr != q_end; ++itr)
    {
        if (CalcQuestStatus(quest_giver, plr, *itr) >= QMGR_QUEST_CHAT)
        {
            if (tmp_map.find((*itr)->qst->GetQuestId()) == tmp_map.end())
            {
                tmp_map.insert(std::map<uint32, uint8>::value_type((*itr)->qst->GetQuestId(), 1));
                questCount++;
            }
        }
    }

    return questCount;
}

void QuestMgr::BuildOfferReward(WorldPacket* data, Quest const* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
    LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->GetQuestId(), language) : NULL;
    ItemPrototype* it;
    uint32 i = 0;
    std::string Title = qst->GetTitle();
    std::string OfferRewardText = qst->GetOfferRewardText();
    std::string QuestGiverTextWindow = qst->GetQuestGiverPortraitText();
    std::string QuestGiverName = qst->GetQuestGiverPortraitUnk();
    std::string QuestCompleteTextWindow = qst->GetQuestTurnInPortraitText();
    std::string QuestCompleteName = qst->GetQuestTurnInPortraitUnk();

    data->SetOpcode(SMSG_QUESTGIVER_OFFER_REWARD);

    *data << uint64(qst_giver->GetGUID());
    *data << uint32(qst->GetQuestId());
    *data << Title;
    *data << OfferRewardText;
    *data << QuestGiverTextWindow;
    *data << QuestGiverName;
    *data << QuestCompleteTextWindow;
    *data << QuestCompleteName;

    uint32 next = qst->NextQuestId;
    uint8 EnableNext = next ? 1 : 0; // dummy for testing

    *data << uint32(qst->GetQuestGiverPortrait());
    *data << uint32(qst->GetQuestTurnInPortrait());         // 4.0.6
    *data << uint8(EnableNext);                        // Auto Finish
    *data << uint32(qst->GetFlags());                        // 3.3.3 questFlags
    *data << uint32(qst->GetSuggestedPlayers());            // SuggestedGroupNum

    uint32 EmoteCount = 0;
    
    for (i = 0; i < QUEST_EMOTE_COUNT; i++)
    {
        if (qst->OfferRewardEmote[i] <= 0)
            break;
        ++EmoteCount;
    }

    *data << EmoteCount;

    for (uint32 i = 0; i < EmoteCount; ++i)
    {
        *data << uint32(qst->OfferRewardEmoteDelay[i]);   // Delay Emote
        *data << uint32(qst->OfferRewardEmote[i]);
    }

    *data << uint32(qst->GetRewChoiceItemsCount());
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        *data << uint32(qst->RewChoiceItemId[i]);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        *data << uint32(qst->RewChoiceItemCount[i]);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        it = ItemPrototypeStorage.LookupEntry(qst->RewChoiceItemId[i]);
        if (it)
            *data << uint32(it->DisplayInfoID);
        else
            *data << uint32(0x00);
    }

    *data << qst->GetRewItemsCount();

    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        *data << uint32(qst->RewItemId[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        *data << uint32(qst->RewItemCount[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        it = ItemPrototypeStorage.LookupEntry(qst->RewItemId[i]);

        if (it)
            *data << uint32(it->DisplayInfoID);
        else
            *data << uint32(0);
    }

    *data << uint32(qst->GetRewOrReqMoney());
    *data << uint32(qst->XPValue(plr)); // 4.0.6
    *data << uint32(qst->GetCharTitleId());
    *data << uint32(0); // Unknown 4.0.6
    *data << uint32(0); // Unknown 4.0.6
    *data << uint32(qst->GetBonusTalents());
    *data << uint32(0); // Unknown 4.0.6
    *data << uint32(0); // Unknown 4.0.6
    *data << uint32(0);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << uint32(qst->RewRepFaction[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << int32(qst->RewRepValueId[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << int32(qst->RewRepValue[i]);

    *data << int32(qst->GetRewSpellCast());
    *data << uint32(0); // Probably invisible spell cast ;/

    for (int i = 0; i < 4; i++)
        *data << uint32(0);

    for (int i = 0; i < 4; i++)
        *data << uint32(0);

    *data << uint32(0);
    *data << uint32(0);
}

void QuestMgr::BuildQuestDetails(WorldPacket* data, Quest const* qst, Object* qst_giver, uint32 menutype, uint32 language, Player* plr)
{
    LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->GetQuestId(), language) : NULL;
    std::map<uint32, uint8>::const_iterator itr;
    std::string Title = qst->GetTitle();
    std::string Details = qst->GetDetails();
    std::string Objectives = qst->GetObjectives();
    std::string EndText = qst->GetEndText();
    std::string QuestTargetTextWindow = qst->GetQuestGiverPortraitText();
    std::string QuestTargetName = qst->GetQuestGiverPortraitUnk();

    data->SetOpcode(SMSG_QUESTGIVER_QUEST_DETAILS);

    *data << qst_giver->GetGUID();			// npc guid
    *data << uint64(0);                                      // in Cata (4.0.6) sometimes npcGUID for quest sharing?
    *data << uint32(qst->GetQuestId());
   
    if (lq)
    {
        *data << lq->Title;
        *data << lq->Details;
        *data << lq->Objectives;
    }
    else
    {
        *data << qst->GetTitle();
        *data << qst->GetDetails();
        *data << qst->GetObjectives();
    }
    bool ActivateAccept = true;
    *data << QuestTargetTextWindow;
    *data << QuestTargetName;
    *data << uint16(0);                                      // Unknown Value maybe string
    *data << uint32(qst->GetQuestGiverPortrait());
    *data << uint32(0);
    *data << uint8(ActivateAccept ? 1 : 0);
    *data << uint32(qst->GetQuestFlags());
    *data << uint32(qst->GetSuggestedPlayers());
    *data << uint8(0);              //Empty?
    *data << uint8(qst->GetQuestStartType());
    *data << uint32(qst->GetRequiredSpell());

    ItemPrototype* ip;
    uint32 i = 0;

    *data << uint32(qst->GetRewChoiceItemsCount());
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        *data << uint32(qst->RewChoiceItemId[i]);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        *data << uint32(qst->RewChoiceItemCount[i]);
    for (uint32 i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
    {
        ip = ItemPrototypeStorage.LookupEntry(qst->RewChoiceItemId[i]);
        if (ip)
            *data << uint32(ip->DisplayInfoID);
        else
            *data << uint32(0x00);
    }


    *data << uint32(qst->GetRewItemsCount());

    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        *data << uint32(qst->RewItemId[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
        *data << uint32(qst->RewItemCount[i]);
    for (uint32 i = 0; i < QUEST_REWARDS_COUNT; ++i)
    {
        ip = ItemPrototypeStorage.LookupEntry(qst->RewItemId[i]);

        if (ip)
            *data << uint32(ip->DisplayInfoID);
        else
            *data << uint32(0);
    }

    *data << uint32(qst->GetRewOrReqMoney());
    *data << uint32(qst->XPValue(plr));

    *data << uint32(qst->GetCharTitleId());
    *data << uint32(0); // unknow 4.0.1
    *data << uint32(0); // unknow 4.0.1
    *data << uint32(qst->GetBonusTalents());
    *data << uint32(0); // unknow 4.0.1
    *data << uint32(0); // unknow 4.0.1
    
    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << uint32(qst->RewRepFaction[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << int32(qst->RewRepValueId[i]);

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        *data << int32(qst->RewRepValue[i]);

    *data << int32(qst->GetRewSpellCast());
    *data << uint32(0); // unknow 4.0.1 Spellcast?

    for (int i = 0; i < 4; i++)
        *data << uint32(0);

    for (int i = 0; i < 4; i++)
        *data << uint32(0);

    *data << uint32(0);
    *data << uint32(0);

    *data << uint32(QUEST_EMOTE_COUNT);
    for (uint32 i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        *data << uint32(qst->DetailsEmote[i]);
        *data << uint32(qst->DetailsEmoteDelay[i]);       // DetailsEmoteDelay (in ms)
    }
}

void QuestMgr::BuildRequestItems(WorldPacket* data, Quest const* qst, Object* qst_giver, uint32 status, uint32 language)
{
    LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest(qst->GetQuestId(), language) : NULL;
    ItemPrototype* it;
    data->SetOpcode(SMSG_QUESTGIVER_REQUEST_ITEMS);

    *data << uint64(qst_giver->GetGUID());
    *data << uint32(qst->GetQuestId());

    if (lq)
    {
        *data << lq->Title;
        *data << ((lq->IncompleteText[0]) ? lq->IncompleteText : lq->Details);
    }
    else
    {
        *data << qst->GetTitle();
        *data << qst->GetRequestItemsText();
    }

    *data << uint32(0x00);                                   // unknown

    if (status == QMGR_QUEST_FINISHED)
        *data << qst->GetCompleteEmote();
    else
        *data << qst->GetIncompleteEmote();

    // Close Window after cancel
    bool CloseOnCancel = true; // TEst
    if (CloseOnCancel)
        *data << uint32(0x01);
    else
        *data << uint32(0x00);

    *data << qst->GetQuestFlags();
    *data << qst->GetSuggestedPlayers();

    // Required Money
    *data << uint32(qst->GetRewOrReqMoney() < 0 ? -qst->GetRewOrReqMoney() : 0);

    // item count
    *data << uint32(qst->GetReqItemsCount());

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
    {
        if (!qst->ReqItemId[i])
            continue;

        it = ItemPrototypeStorage.LookupEntry(qst->ReqItemId[i]);

        *data << uint32(qst->ReqItemId[i]);
        *data << uint32(qst->ReqItemCount[i]);

        if (it)
            *data << uint32(it->DisplayInfoID);
        else
            *data << uint32(0);
    }
 
    // Added in 4.0.1
    uint32 counter = 0;
    *data << counter;
    for (uint32 i = 0; i < counter; i++)
    {
        *data << uint32(0);
        *data << uint32(0);
    }

    if (status == QMGR_QUEST_NOT_FINISHED)
        *data << uint32(0x00);
    else
        *data << uint32(0x02);

    *data << uint32(0x04);
    *data << uint32(0x08);
    *data << uint32(0x10);
    *data << uint32(0x40); // added in 4.0.1
}

void QuestMgr::BuildQuestComplete(Player* plr, Quest const* qst)
{
    uint32 xp;
    uint32 currtalentpoints = 0; // plr->GetCurrentTalentPoints();
    uint32 rewardtalents = qst->GetBonusTalents();
    uint32 playerlevel = plr->getLevel();

    if (playerlevel >= plr->GetMaxLevel())
    {
        xp = 0;
    }
    else
    {
        xp = float2int32(GenerateQuestXP(plr, qst) * sWorld.getRate(RATE_QUESTXP));
        plr->GiveXP(xp, 0, false);
    }

    /*if (currtalentpoints <= (playerlevel - 9 - rewardtalents))
        plr->AddTalentPointsToAllSpec(rewardtalents);*/

    // Reward title
    if (qst->GetCharTitleId() > 0)
        plr->SetKnownTitle(static_cast<RankTitles>(qst->GetCharTitleId()), true);

	// Some spells applied at quest reward
    SpellAreaForQuestMapBounds saBounds = sSpellFactoryMgr.GetSpellAreaForQuestMapBounds(qst->GetQuestId(), false);
	if (saBounds.first != saBounds.second)
	{
		for (SpellAreaForAreaMap::const_iterator itr = saBounds.first; itr != saBounds.second; ++itr)
		{
			if (itr->second->autocast && itr->second->IsFitToRequirements(plr, plr->GetZoneId(), plr->GetAreaID()))
				if (!plr->HasAura(itr->second->spellId))
					plr->CastSpell(plr, itr->second->spellId, true);
		}
	}

    WorldPacket data(SMSG_QUESTGIVER_QUEST_COMPLETE, (4 + 4 + 4 + 4 + 4));

    data << uint8(0x80); // unk 4.0.1 flags
    data << uint32(qst->GetRewSkillLineId());
    data << uint32(qst->GetQuestId());

    if (plr->getLevel() < sWorld.m_levelCap)
    {
        data << uint32(qst->GetRewOrReqMoney());
        data << uint32(qst->GetBonusTalents());              // bonus talents
        data << uint32(qst->GetRewSkillPoints());
        data << uint32(qst->XPValue(plr));
    }
    else
    {
        data << uint32(qst->GetRewOrReqMoney() + int32(qst->GetRewMoneyMaxLevel()));
        data << uint32(qst->GetRewSkillPoints());              // bonus talents
        data << uint32(0);

        data << uint32(0);
    }
    plr->SendPacket(&data);
}

// check this
void QuestMgr::BuildQuestList(WorldPacket* data, Object* qst_giver, Player* plr, uint32 language)
{
    if (!plr || !plr->GetSession()) return;
    uint32 status;
    std::list<QuestRelation*>::iterator it;
    std::list<QuestRelation*>::iterator st;
    std::list<QuestRelation*>::iterator ed;
    std::map<uint32, uint8> tmp_map;

    data->Initialize(SMSG_QUESTGIVER_QUEST_LIST);

    *data << qst_giver->GetGUID();
    *data << plr->GetSession()->LocalizedWorldSrv(70);//"How can I help you?"; //Hello line
    *data << uint32(1);//Emote Delay
    *data << uint32(1);//Emote

    bool bValid = false;
    if (qst_giver->IsGameObject())
    {
        bValid = static_cast< GameObject* >(qst_giver)->HasQuests();
        if (bValid)
        {
            st = static_cast< GameObject* >(qst_giver)->QuestsBegin();
            ed = static_cast< GameObject* >(qst_giver)->QuestsEnd();
        }
    }
    else if (qst_giver->IsCreature())
    {
        bValid = static_cast< Creature* >(qst_giver)->HasQuests();
        if (bValid)
        {
            st = static_cast< Creature* >(qst_giver)->QuestsBegin();
            ed = static_cast< Creature* >(qst_giver)->QuestsEnd();
        }
    }

    if (!bValid)
    {
        *data << uint8(0);
        return;
    }

    *data << uint8(sQuestMgr.ActiveQuestsCount(qst_giver, plr));

    for (it = st; it != ed; ++it)
    {
        status = sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
        if (status >= QMGR_QUEST_CHAT)
        {
            if (tmp_map.find((*it)->qst->GetQuestId()) == tmp_map.end())
            {
                tmp_map.insert(std::map<uint32, uint8>::value_type((*it)->qst->GetQuestId(), 1));
                LocalizedQuest* lq = (language > 0) ? sLocalizationMgr.GetLocalizedQuest((*it)->qst->GetQuestId(), language) : NULL;

                *data << (*it)->qst->GetQuestId();
                /**data << sQuestMgr.CalcQuestStatus(qst_giver, plr, *it);
                *data << uint32(0);*/

                switch (status)
                {
                case QMGR_QUEST_NOT_FINISHED:
                    *data << uint32(4);
                    break;

                case QMGR_QUEST_FINISHED:
                    *data << uint32(4);
                    break;

                case QMGR_QUEST_CHAT:
                    *data << uint32(QMGR_QUEST_AVAILABLE);
                    break;

                default:
                    *data << status;
                    break;
                }
                *data << int32((*it)->qst->GetQuestLevel());
                *data << uint32((*it)->qst->GetQuestFlags());
                *data << uint8(0);   // According to MANGOS: "changes icon: blue question or yellow exclamation"

                if (lq)
                    *data << lq->Title;
                else
                    *data << (*it)->qst->GetTitle();
            }
        }
    }
}

void QuestMgr::BuildQuestUpdateAddItem(WorldPacket* data, uint32 itemid, uint32 count)
{
    data->Initialize(SMSG_QUESTUPDATE_ADD_ITEM);
    *data << itemid << count;
}

void QuestMgr::SendQuestUpdateAddKill(Player* plr, uint32 questid, uint32 entry, uint32 count, uint32 tcount, uint64 guid)
{
    WorldPacket data((4 * 4 + 8));
    data.SetOpcode(SMSG_QUESTUPDATE_ADD_KILL);
    data << uint32(questid);
    data << uint32(entry);
    data << uint32(tcount + 1);
    data << uint32(count);
    data << uint64(guid);
    plr->GetSession()->SendPacket(&data);
}

void QuestMgr::BuildQuestUpdateComplete(WorldPacket* data, Quest* qst)
{
    data->Initialize(SMSG_QUESTUPDATE_COMPLETE);

    *data << qst->GetQuestId();
}

void QuestMgr::SendPushToPartyResponse(Player* plr, Player* pTarget, uint8 response)
{
    WorldPacket data(MSG_QUEST_PUSH_RESULT, 9);
    data << uint64(pTarget->GetGUID());
    data << uint8(response);
    plr->GetSession()->SendPacket(&data);
}

bool QuestMgr::OnGameObjectActivate(Player* plr, GameObject* go)
{
    uint32 i, j;
    QuestLogEntry* qle;
    uint32 entry = go->GetEntry();
    Quest const* qst;

    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        qle = plr->GetQuestLogInSlot(i);
        if (qle != NULL)
        {
            qst = qle->GetQuest();
            // don't waste time on quests without mobs
            for (uint32 k= 0; k < QUEST_OBJECTIVES_COUNT; ++k)
            {
                int32 id = qst->ReqCreatureOrGOId[k];
                uint32 count = qst->ReqCreatureOrGOCount[k];
                if (id == 0 || count == 0)
                    continue;
            }

            for (j = 0; j < 4; ++j)
            {
                if (qst->ReqCreatureOrGOId[j] == static_cast<int32>(entry) && qst->m_reqMobType[j] == QUEST_MOB_TYPE_CREATURE && qle->m_mobcount[j] < qst->ReqCreatureOrGOCount[j])
                {
                    // add another kill.
                    // (auto-dirty's it)
                    qle->IncrementMobCount(j);
                    qle->SendUpdateAddKill(j);
                    CALL_QUESTSCRIPT_EVENT(qle, OnGameObjectActivate)(entry, plr, qle);

                    if (qle->CanBeFinished())
                        qle->SendQuestComplete();

                    qle->UpdatePlayerFields();
                    return true;
                }
            }
        }
    }
    return false;
}

void QuestMgr::OnPlayerKill(Player* plr, Creature* victim, bool IsGroupKill)
{
    uint32 entry = victim->GetEntry();
    _OnPlayerKill(plr, entry, IsGroupKill);

    // Extra credit (yay we wont have to script this anymore) - Shauren
    for (uint8 i = 0; i < 2; ++i)
    {
        uint32 extracredit = victim->GetCreatureInfo()->killcredit[i];
        if (extracredit != 0)
        {
            if (CreatureNameStorage.LookupEntry(extracredit))
                _OnPlayerKill(plr, extracredit, IsGroupKill);
        }
    }
}

void QuestMgr::_OnPlayerKill(Player* plr, uint32 entry, bool IsGroupKill)
{
    if (!plr)
        return;

    uint32 i, j;
    QuestLogEntry* qle;
    Quest const* qst;

    if (plr->HasQuestMob(entry))
    {
        for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
        {
            qle = plr->GetQuestLogInSlot(i);
            if (qle != NULL)
            {
                qst = qle->GetQuest();
                for (j = 0; j < 4; ++j)
                {
                        int32 id = qst->ReqCreatureOrGOId[j];
                        uint32 count = qst->ReqCreatureOrGOCount[j];
                        if (id == 0 || count == 0)
                            continue;

                        if (qst->ReqCreatureOrGOId[j] == static_cast<int32>(entry) && qst->m_reqMobType[j] == QUEST_MOB_TYPE_CREATURE && qle->m_mobcount[j] < qst->ReqCreatureOrGOCount[j])
                    {
                        // add another kill.(auto-dirty's it)
                        qle->IncrementMobCount(j);
                        qle->SendUpdateAddKill(j);
                        CALL_QUESTSCRIPT_EVENT(qle, OnCreatureKill)(entry, plr, qle);
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
            }
        }
    }

    if (IsGroupKill)
    {
        // Shared kills
        Player* gplr = NULL;

        if (plr->InGroup())
        {
            if (Group* pGroup = plr->GetGroup())
            {
                //removed by Zack How the hell will healers get the kills then ?
                //if (pGroup->GetGroupType() != GROUP_TYPE_PARTY)
                //	return;  // Raid's don't get shared kills.

                GroupMembersSet::iterator gitr;
                pGroup->Lock();
                for (uint32 k = 0; k < pGroup->GetSubGroupCount(); k++)
                {
                    for (gitr = pGroup->GetSubGroup(k)->GetGroupMembersBegin(); gitr != pGroup->GetSubGroup(k)->GetGroupMembersEnd(); ++gitr)
                    {
                        gplr = (*gitr)->m_loggedInPlayer;
                        if (gplr && gplr != plr && plr->isInRange(gplr, 300) && gplr->HasQuestMob(entry)) // don't double kills also don't give kills to party members at another side of the world
                        {
                            for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
                            {
                                qle = gplr->GetQuestLogInSlot(i);
                                if (qle != NULL)
                                {
                                    qst = qle->GetQuest();
                                    for (j = 0; j < 4; ++j)
                                    {
                                        if (qst->ReqCreatureOrGOId[j] == 0)
                                            continue;

                                        if (qst->ReqCreatureOrGOId[j] == static_cast<int32>(entry) && qst->m_reqMobType[j] == QUEST_MOB_TYPE_CREATURE && qle->m_mobcount[j] < qst->ReqCreatureOrGOCount[j])
                                        {
                                            // add another kill.
                                            // (auto-dirty's it)
                                            qle->IncrementMobCount(j);
                                            qle->SendUpdateAddKill(j);
                                            CALL_QUESTSCRIPT_EVENT(qle, OnCreatureKill)(entry, gplr, qle);
                                            qle->UpdatePlayerFields();

                                            if (qle->CanBeFinished())
                                                qle->SendQuestComplete();
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                pGroup->Unlock();
            }
        }
    }
}

void QuestMgr::OnPlayerCast(Player* plr, uint32 spellid, uint64 & victimguid)
{
    if (!plr || !plr->HasQuestSpell(spellid))
        return;

    Unit* victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;

    uint32 i, j;
    uint32 entry = (victim) ? victim->GetEntry() : 0;
    QuestLogEntry* qle;
    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        if ((qle = plr->GetQuestLogInSlot(i)) != 0)
        {
            // don't waste time on quests without casts
            if (!qle->IsCastQuest())
                continue;

            Quest const* qst = qle->GetQuest();
            for (j = 0; j < 4; ++j)
            {
                if (qst->ReqCreatureOrGOId[j])
                {
                    if (victim && qst->ReqCreatureOrGOId[j] == static_cast<int32>(entry) && qst->ReqSpell[j] == spellid && (qle->m_mobcount[j] < qst->ReqCreatureOrGOCount[j] || qle->m_mobcount[j] == 0) && !qle->IsUnitAffected(victim))
                    {
                        qle->AddAffectedUnit(victim);
                        qle->IncrementMobCount(j);
                        qle->SendUpdateAddKill(j);
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
                // Some quests, like druid's Trial of the Lake (28/29), don't have a required target for spell cast
                else
                {
                    if (qst->ReqSpell[j] == spellid)
                    {
                        qle->IncrementMobCount(j);
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
            }
        }
    }
}

void QuestMgr::OnPlayerItemPickup(Player* plr, Item* item)
{
    uint32 i, j, k;
    uint32 pcount;
    uint32 entry = item->GetEntry();
    QuestLogEntry* qle;
    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        if ((qle = plr->GetQuestLogInSlot(i)) != 0)
        {
            for (k = 0; k < 4; ++k)
            {
                int32 id = qle->GetQuest()->ReqCreatureOrGOId[k];
                uint32 count = qle->GetQuest()->ReqCreatureOrGOCount[k];
                if (id == 0 || count == 0)
                    continue;
            }

            for (j = 0; j < MAX_REQUIRED_QUEST_ITEM; ++j)
            {
                if (qle->GetQuest()->ReqItemId[j] == entry)
                {
                    pcount = plr->GetItemInterface()->GetItemCount(entry, true);
                    CALL_QUESTSCRIPT_EVENT(qle, OnPlayerItemPickup)(entry, pcount, plr, qle);
                    if (pcount < qle->GetQuest()->ReqItemCount[j])
                    {
                        WorldPacket data(8);
                        data.SetOpcode(SMSG_QUESTUPDATE_ADD_ITEM);
                        data << qle->GetQuest()->ReqItemId[j] << uint32(1);
                        plr->GetSession()->SendPacket(&data);
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
            }
        }
    }
}

void QuestMgr::OnPlayerExploreArea(Player* plr, uint32 AreaID)
{
    uint32 i, j;
    QuestLogEntry* qle;
    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        if ((qle = plr->GetQuestLogInSlot(i)) != 0)
        {
            // don't waste time on quests without triggers
            if (qle->GetQuest()->GetZoneOrSort() == 0)
                continue;

            for (j = 0; j < 3; ++j)
            {
                if (qle->GetQuest()->m_reqExploreTrigger[j] == AreaID &&
                    !qle->m_explored_areas[j])
                {
                    qle->SetTrigger(j);
                    CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
                    qle->UpdatePlayerFields();
                    if (qle->CanBeFinished())
                        qle->SendQuestComplete();
                    break;
                }
            }
        }
    }
}

void QuestMgr::AreaExplored(Player* plr, uint32 QuestID)
{
    uint32 i, j;
    QuestLogEntry* qle;
    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        if ((qle = plr->GetQuestLogInSlot(i)) != 0)
        {
            // search for quest
            if (qle->GetQuest()->GetQuestId() == QuestID)
                for (j = 0; j < 4; ++j)
                {
                    if (qle->GetQuest()->m_reqExploreTrigger[j] &&
                        !qle->m_explored_areas[j])
                    {
                        qle->SetTrigger(j);
                        CALL_QUESTSCRIPT_EVENT(qle, OnExploreArea)(qle->m_explored_areas[j], plr, qle);
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
        }
    }
}

void QuestMgr::GiveQuestRewardReputation(Player* plr, Quest const* qst, Object* qst_giver)
{
    // Reputation reward
    for (int z = 0; z < 6; z++)
    {
        uint32 fact = 19;   // default to 19 if no factiondbc
        int32 amt = float2int32(GenerateQuestXP(plr, qst) * 0.1f); // guess
        if (!qst->RewRepFaction[z])
        {
            if (z >= 1)
                break;

            // Let's do this properly. Determine the faction of the creature, and give reputation to his faction.
            if (qst_giver->IsCreature())
                if (static_cast< Creature* >(qst_giver)->m_factionDBC != NULL)
                    fact = static_cast< Creature* >(qst_giver)->m_factionDBC->ID;
            if (qst_giver->IsGameObject())
                fact = static_cast< GameObject* >(qst_giver)->GetFaction();
        }
        else
        {
            fact = qst->RewRepFaction[z];
            if (qst->RewRepValue[z])
                amt = qst->RewRepValue[z];
        }

        //if (qst->reward_replimit)
        //    if (plr->GetStanding(fact) >= (int32)qst->reward_replimit)
         //       continue;

        amt = float2int32(amt * sWorld.getRate(RATE_QUESTREPUTATION));     // reputation rewards
        plr->ModStanding(fact, amt);
    }
}

void QuestMgr::OnQuestAccepted(Player* plr, Quest const* qst, Object* qst_giver)
{
}

void QuestMgr::OnQuestFinished(Player* plr, Quest const* qst, Object* qst_giver, uint32 reward_slot)
{
    //Re-Check for Gold Requirement (needed for possible xploit) - reward money < 0 means required money
    if (qst->GetRewOrReqMoney() < 0 && plr->GetGold() < uint32(-qst->GetRewOrReqMoney()))
        return;

    // Check they don't have more than the max gold
    if (sWorld.GoldCapEnabled && (plr->GetGold() + qst->GetRewOrReqMoney()) > sWorld.GoldLimit)
    {
        plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_TOO_MUCH_GOLD);
        return;
    }

    QuestLogEntry* qle = NULL;
    qle = plr->GetQuestLogForEntry(qst->GetQuestId());
    if (!qle)
        return;

    BuildQuestComplete(plr, qst);
    CALL_QUESTSCRIPT_EVENT(qle, OnQuestComplete)(plr, qle);
    for (uint32 x = 0; x < 4; x++)
    {
        if (qst->ReqSpell[x] != 0)
        {
            if (plr->HasQuestSpell(qst->ReqSpell[x]))
                plr->RemoveQuestSpell(qst->ReqSpell[x]);
        }
        else if (qst->ReqCreatureOrGOId[x] != 0)
        {
            if (plr->HasQuestMob(qst->ReqCreatureOrGOId[x]))
                plr->RemoveQuestMob(qst->ReqCreatureOrGOId[x]);
        }
    }
    qle->ClearAffectedUnits();
    qle->Finish();


    if (qst_giver->IsCreature())
    {
        if (!static_cast< Creature* >(qst_giver)->HasQuest(qst->GetQuestId(), 2))
        {
            //sCheatLog.writefromsession(plr->GetSession(), "tried to finish quest from invalid npc.");
            plr->GetSession()->Disconnect();
            return;
        }
    }

    //details: hmm as i can remember, repeatable quests give faction rep still after first completion
    if (qst->IsRepeatable() || qst->IsDaily())
    {
        // Reputation reward
        GiveQuestRewardReputation(plr, qst, qst_giver);
        // Static Item reward
        for (uint32 i = 0; i < 4; ++i)
        {
            if (qst->RewItemId[i])
            {
                ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewItemId[i]);
                if (!proto)
                {
                    LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->RewItemId[i], qst->GetQuestId());
                }
                else
                {
                    Item* add;
                    SlotResult slotresult;
                    add = plr->GetItemInterface()->FindItemLessMax(qst->RewItemId[i], qst->RewItemCount[i], false);
                    if (!add)
                    {
                        slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
                        if (!slotresult.Result)
                        {
                            plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
                        }
                        else
                        {
                            Item* itm = objmgr.CreateItem(qst->RewItemId[i], plr);
                            if (itm == NULL)
                                return;

                            itm->SetStackCount(uint32(qst->RewItemCount[i]));
                            if (!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
                                itm->DeleteMe();
                        }
                    }
                    else
                    {
                        add->SetStackCount(add->GetStackCount() + qst->RewItemCount[i]);
                        add->m_isDirty = true;
                    }
                }
            }
        }

        // Choice Rewards
        if (qst->RewChoiceItemId[reward_slot])
        {
            ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewChoiceItemId[reward_slot]);
            if (!proto)
            {
                Log.Error("QuestMgr", "Invalid item prototype in quest reward! ID %d, quest %d", qst->RewChoiceItemId[reward_slot], qst->GetQuestId());
            }
            else
            {
                Item* add;
                SlotResult slotresult;
                add = plr->GetItemInterface()->FindItemLessMax(qst->RewChoiceItemId[reward_slot], qst->RewChoiceItemCount[reward_slot], false);
                if (!add)
                {
                    slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
                    if (!slotresult.Result)
                    {
                        plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
                    }
                    else
                    {
                        Item* itm = objmgr.CreateItem(qst->RewChoiceItemId[reward_slot], plr);
                        if (itm == NULL)
                            return;

                        itm->SetStackCount(uint32(qst->RewChoiceItemCount[reward_slot]));
                        if (!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
                            itm->DeleteMe();

                    }
                }
                else
                {
                    add->SetStackCount(add->GetStackCount() + qst->RewChoiceItemCount[reward_slot]);
                    add->m_isDirty = true;
                }
            }
        }

        // Remove items
        for (uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
        {
            if (qst->ReqItemId[i]) plr->GetItemInterface()->RemoveItemAmt(qst->ReqItemId[i], qst->ReqItemCount[i]);
        }

        // Remove srcitem
        if (qst->GetSrcItemId() /*&& qst->GetSrcItemId() != qst->receive_items[0]*/)
            plr->GetItemInterface()->RemoveItemAmt(qst->GetSrcItemId(), qst->GetSrcItemCount() ? qst->GetSrcItemCount() : 1);

        // cast Effect Spell
        if (qst->GetRewSpellCast())
        {
            SpellEntry*   inf = dbcSpell.LookupEntryForced(qst->GetRewSpellCast());
            if (inf)
            {
                Spell* spe = sSpellFactoryMgr.NewSpell(plr, inf, true, NULL);
                SpellCastTargets tgt;
                tgt.m_unitTarget = plr->GetGUID();
                spe->prepare(&tgt);
            }
        }

        plr->ModGold(GenerateRewardMoney(plr, qst));

        // if daily then append to finished dailies
        if (qst->IsRepeatable())
            plr->PushToFinishedDailies(qst->GetQuestId());
    }
    else
    {
        plr->ModGold(GenerateRewardMoney(plr, qst));

        // Reputation reward
        GiveQuestRewardReputation(plr, qst, qst_giver);
        // Static Item reward
        for (uint32 i = 0; i < 4; ++i)
        {
            if (qst->RewItemId[i])
            {
                ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewItemId[i]);
                if (!proto)
                {
                    LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->RewItemId[i], qst->GetQuestId());
                }
                else
                {
                    Item* add;
                    SlotResult slotresult;
                    add = plr->GetItemInterface()->FindItemLessMax(qst->RewItemId[i], qst->RewItemCount[i], false);
                    if (!add)
                    {
                        slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
                        if (!slotresult.Result)
                        {
                            plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
                        }
                        else
                        {
                            Item* itm = objmgr.CreateItem(qst->RewItemId[i], plr);
                            if (itm == NULL)
                                return;

                            itm->SetStackCount(uint32(qst->RewItemCount[i]));
                            if (!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
                                itm->DeleteMe();
                        }
                    }
                    else
                    {
                        add->SetStackCount(add->GetStackCount() + qst->RewItemCount[i]);
                        add->m_isDirty = true;
                    }
                }
            }
        }

        // Choice Rewards
        if (qst->RewChoiceItemId[reward_slot])
        {
            ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewChoiceItemId[reward_slot]);
            if (!proto)
            {
                LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->RewChoiceItemId[reward_slot], qst->GetQuestId());
            }
            else
            {
                Item* add;
                SlotResult slotresult;
                add = plr->GetItemInterface()->FindItemLessMax(qst->RewChoiceItemId[reward_slot], qst->RewChoiceItemCount[reward_slot], false);
                if (!add)
                {
                    slotresult = plr->GetItemInterface()->FindFreeInventorySlot(proto);
                    if (!slotresult.Result)
                    {
                        plr->GetItemInterface()->BuildInventoryChangeError(NULL, NULL, INV_ERR_INVENTORY_FULL);
                    }
                    else
                    {
                        Item* itm = objmgr.CreateItem(qst->RewChoiceItemId[reward_slot], plr);
                        if (itm == NULL)
                            return;

                        itm->SetStackCount(uint32(qst->RewChoiceItemCount[reward_slot]));
                        if (!plr->GetItemInterface()->SafeAddItem(itm, slotresult.ContainerSlot, slotresult.Slot))
                            itm->DeleteMe();
                    }
                }
                else
                {
                    add->SetStackCount(add->GetStackCount() + qst->RewChoiceItemCount[reward_slot]);
                    add->m_isDirty = true;
                }
            }
        }

        // Remove items
        for (uint32 i = 0; i < MAX_REQUIRED_QUEST_ITEM; ++i)
        {
            if (qst->ReqItemId[i]) plr->GetItemInterface()->RemoveItemAmt(qst->ReqItemId[i], qst->ReqItemCount[i]);
        }

        // Remove srcitem
        if (qst->GetSrcItemId() /*&& qst->srcitem != qst->receive_items[0]*/)
            plr->GetItemInterface()->RemoveItemAmt(qst->GetSrcItemId(), qst->GetSrcItemCount() ? qst->GetSrcItemCount() : 1);

        // cast learning spell
        if (qst->GetRewSpell() && !qst->GetRewSpellCast()) // qst->reward_spell is the spell the quest finisher teaches you, OR the icon of the spell if effect_on_player is not 0
        {
            if (!plr->HasSpell(qst->GetRewSpell()))
            {
                // "Teaching" effect
                WorldPacket data(SMSG_SPELL_START, 42);
                data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
                data << uint32(7763);
                data << uint8(0);
                data << uint16(0);
                data << uint32(0);
                data << uint16(2);
                data << plr->GetGUID();
                plr->GetSession()->SendPacket(&data);

                data.Initialize(SMSG_SPELL_GO);
                data << qst_giver->GetNewGUID() << qst_giver->GetNewGUID();
                data << uint32(7763);		    // spellID
                data << uint8(0) << uint8(1);   // flags
                data << uint8(1);			    // amount of targets
                data << plr->GetGUID();		    // target
                data << uint8(0);
                data << uint16(2);
                data << plr->GetGUID();
                plr->GetSession()->SendPacket(&data);

                // Teach the spell
                plr->addSpell(qst->GetRewSpell());
            }
        }

        // cast Effect Spell
        if (qst->GetRewSpellCast())
        {
            SpellEntry*   inf = dbcSpell.LookupEntryForced(qst->GetRewSpellCast());
            if (inf)
            {
                Spell* spe = sSpellFactoryMgr.NewSpell(plr, inf, true, NULL);
                SpellCastTargets tgt;
                tgt.m_unitTarget = plr->GetGUID();
                spe->prepare(&tgt);
            }
        }

        //Add to finished quests
        plr->AddToFinishedQuests(qst->GetQuestId());
        /*if (qst->bonusarenapoints != 0)
        {
            plr->AddArenaPoints(qst->bonusarenapoints, true);
        }*/

#ifdef ENABLE_ACHIEVEMENTS
        plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT, 1, 0, 0);
        if (qst->GetRewOrReqMoney())
            plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_QUEST_REWARD_GOLD, qst->GetRewOrReqMoney(), 0, 0);
        plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE, qst->GetZoneOrSort(), 0, 0);
        plr->GetAchievementMgr().UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST, qst->GetQuestId(), 0, 0);
#endif
        // Remove quests that are listed to be removed on quest complete.
        std::set<uint32>::iterator iter = qst->remove_quest_list.begin();
        for (; iter != qst->remove_quest_list.end(); ++iter)
        {
            if (!plr->HasFinishedQuest((*iter)))
                plr->AddToFinishedQuests((*iter));
        }
    }

    if (qst->GetRewMailTemplateId() != 0)
    {
        MailTemplateEntry * mail = dbcMailTemplateEntry.LookupEntryForced(qst->GetRewMailTemplateId());
        if (mail != NULL)
        {
            int mailType = NORMAL;
            uint64 itemGuid = 0;
            if (qst_giver->IsCreature())
                mailType = CREATURE;
            else if (qst_giver->IsGameObject())
                mailType = GAMEOBJECT;
            /* check this
            if (qst->MailSendItem != 0)
            {
                // the way it's done in World::PollMailboxInsertQueue
                Item * pItem = objmgr.CreateItem(qst->MailSendItem, NULL);
                if (pItem != NULL)
                {
                    pItem->SetStackCount(1);
                    pItem->SaveToDB(0, 0, true, NULL);
                    itemGuid = pItem->GetGUID();
                    pItem->DeleteMe();
                }
            }*/

            sMailSystem.SendCreatureGameobjectMail(mailType, qst_giver->GetEntry(), plr->GetGUID(), mail->subject, mail->content, 0, 0, itemGuid, MAIL_STATIONERY_TEST1, MAIL_CHECK_MASK_HAS_BODY, qst->GetRewMailDelaySecs());
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Quest Management

void QuestMgr::LoadNPCQuests(Creature* qst_giver)
{
    qst_giver->SetQuestList(GetCreatureQuestList(qst_giver->GetEntry()));
}

void QuestMgr::LoadGOQuests(GameObject* go)
{
    go->SetQuestList(GetGOQuestList(go->GetEntry()));
}

QuestRelationList* QuestMgr::GetGOQuestList(uint32 entryid)
{
    std::unordered_map<uint32, QuestRelationList* > &olist = _GetList<GameObject>();
    std::unordered_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
    return (itr == olist.end()) ? 0 : itr->second;
}

QuestRelationList* QuestMgr::GetCreatureQuestList(uint32 entryid)
{
    std::unordered_map<uint32, std::list<QuestRelation*>* > &olist = _GetList<Creature>();
    std::unordered_map<uint32, QuestRelationList* >::iterator itr = olist.find(entryid);
    return (itr == olist.end()) ? 0 : itr->second;
}

template <class T> void QuestMgr::_AddQuest(uint32 entryid, Quest const* qst, uint8 type)
{
    std::unordered_map<uint32, std::list<QuestRelation*>* > &olist = _GetList<T>();
    std::list<QuestRelation*>* nlist;
    QuestRelation* ptr = NULL;

    if (olist.find(entryid) == olist.end())
    {
        nlist = new std::list < QuestRelation* > ;

        olist.insert(std::unordered_map<uint32, std::list<QuestRelation*>* >::value_type(entryid, nlist));
    }
    else
    {
        nlist = olist.find(entryid)->second;
    }

    std::list<QuestRelation*>::iterator it;
    for (it = nlist->begin(); it != nlist->end(); ++it)
    {
        if ((*it)->qst == qst)
        {
            ptr = (*it);
            break;
        }
    }

    if (ptr == NULL)
    {
        ptr = new QuestRelation;
        ptr->qst = qst;
        ptr->type = type;

        nlist->push_back(ptr);
    }
    else
    {
        ptr->type |= type;
    }
}

void QuestMgr::_CleanLine(std::string* str)
{
    _RemoveChar((char*)"\r", str);
    _RemoveChar((char*)"\n", str);

    while (str->c_str()[0] == 32)
    {
        str->erase(0, 1);
    }
}

void QuestMgr::_RemoveChar(char* c, std::string* str)
{
    std::string::size_type pos = str->find(c, 0);

    while (pos != std::string::npos)
    {
        str->erase(pos, 1);
        pos = str->find(c, 0);
    }
}

uint32 QuestMgr::GenerateQuestXP(Player* plr, Quest const* qst)
{
    if (qst->GetQuestFlags() != 0)
        return 0;

    // Leaving this for compatibility reason for the old system + custom quests ^^
    if (qst->XPValue(plr) != 0)
    {
        float modifier = 0.0f;
        uint32 playerlevel = plr->getLevel();
        int32 questlevel = qst->GetQuestLevel();

        if (static_cast<int32>(playerlevel) < (questlevel + 6))
            return qst->XPValue(plr);

        if (static_cast<int32>(playerlevel) > (questlevel + 9))
            return 0;

        if (static_cast<int32>(playerlevel) == (questlevel + 6))
            modifier = 0.8f;

        if (static_cast<int32>(playerlevel) == (questlevel + 7))
            modifier = 0.6f;

        if (static_cast<int32>(playerlevel) == (questlevel + 8))
            modifier = 0.4f;

        if (static_cast<int32>(playerlevel) == (questlevel + 9))
            modifier = 0.2f;


        return static_cast<uint32>(modifier * qst->XPValue(plr));

    }
    else
    {
        // new quest reward xp calculation mechanism based on DBC values + index taken from DB

        uint32 realXP = 0;
        uint32 xpMultiplier = 0;
        int32 baseLevel = 0;
        int32 playerLevel = plr->getLevel();
        int32 QuestLevel = qst->GetQuestLevel();

        if (QuestLevel != -1)
            baseLevel = QuestLevel;

        if (((baseLevel - playerLevel) + 10) * 2 > 10)
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10) * 2 <= 10)
            {
                if (QuestLevel == -1)
                    baseLevel = playerLevel;

                xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
            }
            else
            {
                xpMultiplier = 10;
            }
        }
        else
        {
            baseLevel = playerLevel;

            if (QuestLevel != -1)
                baseLevel = QuestLevel;

            if (((baseLevel - playerLevel) + 10) * 2 >= 1)
            {
                baseLevel = playerLevel;

                if (QuestLevel != -1)
                    baseLevel = QuestLevel;

                if (((baseLevel - playerLevel) + 10) * 2 <= 10)
                {
                    if (QuestLevel == -1)
                        baseLevel = playerLevel;

                    xpMultiplier = 2 * (baseLevel - playerLevel) + 20;
                }
                else
                {
                    xpMultiplier = 10;
                }
            }
            else
            {
                xpMultiplier = 1;
            }
        }

        if (const QuestXP* pXPData = dbcQuestXP.LookupEntry(baseLevel))
        {
            uint32 rawXP = xpMultiplier * pXPData->xpIndex[qst->GetXPId()] / 10;

            realXP = static_cast<uint32>(Arcemu::round(static_cast<double>(rawXP)));
        }
        return realXP;
    }
}

uint32 QuestMgr::GenerateRewardMoney(Player* plr, Quest const* qst)
{
        return qst->RewOrReqMoney;
}


void QuestMgr::SendQuestInvalid(QuestFailedReasons reason, Player* plyr)
{
    if (!plyr)
        return;
    plyr->GetSession()->OutPacket(SMSG_QUESTGIVER_QUEST_INVALID, 4, &reason);

    LOG_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_INVALID");
}

void QuestMgr::SendQuestFailed(FAILED_REASON failed, Quest const* qst, Player* plyr)
{
    if (!plyr)
        return;

    WorldPacket data(8);
    data.Initialize(SMSG_QUESTGIVER_QUEST_FAILED);
    data << uint32(qst->GetQuestId());
    data << failed;
    plyr->GetSession()->SendPacket(&data);
    LOG_DEBUG("WORLD:Sent SMSG_QUESTGIVER_QUEST_FAILED");
}

void QuestMgr::SendQuestUpdateFailedTimer(Quest const* pQuest, Player* plyr)
{
    if (!plyr)
        return;

    WorldPacket data(SMSG_QUESTUPDATE_FAILEDTIMER, 4);
    data << uint32(pQuest->GetQuestId());
    plyr->GetSession()->SendPacket(&data);
    LOG_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILEDTIMER");
}

void QuestMgr::SendQuestUpdateFailed(Quest const* pQuest, Player* plyr)
{
    if (!plyr)
        return;

    plyr->GetSession()->OutPacket(SMSG_QUESTUPDATE_FAILED, 4, &pQuest->QuestId);
    LOG_DEBUG("WORLD:Sent SMSG_QUESTUPDATE_FAILED");
}

void QuestMgr::SendQuestLogFull(Player* plyr)
{
    if (!plyr)
        return;

    plyr->GetSession()->OutPacket(SMSG_QUESTLOG_FULL);
    LOG_DEBUG("WORLD:Sent QUEST_LOG_FULL_MESSAGE");
}

uint32 QuestMgr::GetGameObjectLootQuest(uint32 GO_Entry)
{
    std::unordered_map<uint32, uint32>::iterator itr = m_ObjectLootQuestList.find(GO_Entry);
    if (itr == m_ObjectLootQuestList.end()) return 0;

    return itr->second;
}

void QuestMgr::SetGameObjectLootQuest(uint32 GO_Entry, uint32 Item_Entry)
{
    if (m_ObjectLootQuestList.find(GO_Entry) != m_ObjectLootQuestList.end())
    {
        //LOG_ERROR("WARNING: Gameobject %d has more than 1 quest item allocated in it's loot template!", GO_Entry);
    }

    objmgr.LoadQuestLoot(GO_Entry, Item_Entry);

    //LOG_ERROR("WARNING: No coresponding quest was found for quest item %d", Item_Entry);
}

void QuestMgr::BuildQuestFailed(WorldPacket* data, uint32 questid)
{
    data->Initialize(SMSG_QUESTUPDATE_FAILEDTIMER);
    *data << questid;
}

bool QuestMgr::OnActivateQuestGiver(Object* qst_giver, Player* plr)
{
    if (qst_giver->IsGameObject() && !static_cast< GameObject* >(qst_giver)->HasQuests())
        return false;

    uint32 questCount = sQuestMgr.ActiveQuestsCount(qst_giver, plr);
    WorldPacket data(1004);

    if (questCount == 0)
    {
        LOG_DEBUG("WORLD: Invalid NPC for CMSG_QUESTGIVER_HELLO.");
        return false;
    }
    else if (questCount == 1)
    {
        std::list<QuestRelation*>::const_iterator itr;
        std::list<QuestRelation*>::const_iterator q_begin;
        std::list<QuestRelation*>::const_iterator q_end;

        bool bValid = false;

        if (qst_giver->IsGameObject())
        {
            bValid = static_cast< GameObject* >(qst_giver)->HasQuests();
            if (bValid)
            {
                q_begin = static_cast< GameObject* >(qst_giver)->QuestsBegin();
                q_end = static_cast< GameObject* >(qst_giver)->QuestsEnd();
            }
        }
        else if (qst_giver->IsCreature())
        {
            bValid = static_cast< Creature* >(qst_giver)->HasQuests();
            if (bValid)
            {
                q_begin = static_cast< Creature* >(qst_giver)->QuestsBegin();
                q_end = static_cast< Creature* >(qst_giver)->QuestsEnd();
            }
        }

        if (!bValid)
        {
            LOG_DEBUG("QUESTS: Warning, invalid NPC " I64FMT " specified for OnActivateQuestGiver. TypeId: %d.", qst_giver->GetGUID(), qst_giver->GetTypeId());
            return false;
        }

        for (itr = q_begin; itr != q_end; ++itr)
            if (sQuestMgr.CalcQuestStatus(qst_giver, plr, *itr) >= QMGR_QUEST_CHAT)
                break;

        if (sQuestMgr.CalcStatus(qst_giver, plr) < QMGR_QUEST_CHAT)
            return false;

        ARCEMU_ASSERT(itr != q_end);

        uint32 status = sQuestMgr.CalcStatus(qst_giver, plr);

        if ((status == QMGR_QUEST_AVAILABLE) || (status == QMGR_QUEST_REPEATABLE) || (status == QMGR_QUEST_CHAT))
        {
            sQuestMgr.BuildQuestDetails(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);		// 1 because we have 1 quest, and we want goodbye to function
            plr->GetSession()->SendPacket(&data);
            LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_QUEST_DETAILS.");

            if ((*itr)->qst->HasFlag(QUEST_FLAGS_AUTO_ACCEPT))
                plr->AcceptQuest(qst_giver->GetGUID(), (*itr)->qst->GetQuestId());
        }
        else if (status == QMGR_QUEST_FINISHED)
        {
            sQuestMgr.BuildOfferReward(&data, (*itr)->qst, qst_giver, 1, plr->GetSession()->language, plr);
            plr->GetSession()->SendPacket(&data);
            //ss
            LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_OFFER_REWARD.");
        }
        else if (status == QMGR_QUEST_NOT_FINISHED)
        {
            sQuestMgr.BuildRequestItems(&data, (*itr)->qst, qst_giver, status, plr->GetSession()->language);
            plr->GetSession()->SendPacket(&data);
            LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_REQUEST_ITEMS.");
        }
    }
    else
    {
        sQuestMgr.BuildQuestList(&data, qst_giver, plr, plr->GetSession()->language);
        plr->GetSession()->SendPacket(&data);
        LOG_DEBUG("WORLD: Sent SMSG_QUESTGIVER_QUEST_LIST.");
    }
    return true;
}

QuestMgr::~QuestMgr()
{
    std::unordered_map<uint32, Quest*>::iterator itr1;
    std::unordered_map<uint32, std::list<QuestRelation*>* >::iterator itr2;
    std::list<QuestRelation*>::iterator itr3;
    std::unordered_map<uint32, std::list<QuestAssociation*>* >::iterator itr4;
    std::list<QuestAssociation*>::iterator itr5;

    // clear relations
    for (itr2 = m_obj_quests.begin(); itr2 != m_obj_quests.end(); ++itr2)
    {
        if (!itr2->second)
            continue;

        itr3 = itr2->second->begin();
        for (; itr3 != itr2->second->end(); ++itr3)
        {
            delete(*itr3);
        }
        itr2->second->clear();
        delete itr2->second;
    }

    m_obj_quests.clear();

    for (itr2 = m_npc_quests.begin(); itr2 != m_npc_quests.end(); ++itr2)
    {
        if (!itr2->second)
            continue;

        itr3 = itr2->second->begin();
        for (; itr3 != itr2->second->end(); ++itr3)
        {
            delete(*itr3);
        }
        itr2->second->clear();
        delete itr2->second;
    }

    m_npc_quests.clear();

    for (itr2 = m_itm_quests.begin(); itr2 != m_itm_quests.end(); ++itr2)
    {
        if (!itr2->second)
            continue;

        itr3 = itr2->second->begin();
        for (; itr3 != itr2->second->end(); ++itr3)
        {
            delete(*itr3);
        }
        itr2->second->clear();
        delete itr2->second;
    }
    m_itm_quests.clear();
    for (itr4 = m_quest_associations.begin(); itr4 != m_quest_associations.end(); ++itr4)
    {
        if (!itr4->second)
            continue;

        itr5 = itr4->second->begin();
        for (; itr5 != itr4->second->end(); ++itr5)
        {
            delete(*itr5);
        }
        itr4->second->clear();
        delete itr4->second;
    }
    // NTY.
    m_quest_associations.clear();
}


bool QuestMgr::CanStoreReward(Player* plyr, Quest const* qst, uint32 reward_slot)
{
    uint32 available_slots = 0;
    uint32 slotsrequired = 0;
    available_slots = plyr->GetItemInterface()->CalculateFreeSlots(NULL);
    // Static Item reward
    for (uint32 i = 0; i < 4; ++i)
    {
        if (qst->RewItemId[i])
        {
            slotsrequired++;
            ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewItemId[i]);
            if (!proto)
                LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->RewItemId[i], qst->GetQuestId());
            else if (plyr->GetItemInterface()->CanReceiveItem(proto, qst->RewItemCount[i]))
                return false;
        }
    }

    // Choice Rewards
    if (qst->RewChoiceItemId[reward_slot])
    {
        slotsrequired++;
        ItemPrototype* proto = ItemPrototypeStorage.LookupEntry(qst->RewChoiceItemId[reward_slot]);
        if (!proto)
            LOG_ERROR("Invalid item prototype in quest reward! ID %d, quest %d", qst->RewChoiceItemId[reward_slot], qst->GetQuestId());
        else if (plyr->GetItemInterface()->CanReceiveItem(proto, qst->RewChoiceItemCount[reward_slot]))
            return false;
    }
    if (available_slots < slotsrequired)
    {
        return false;
    }

    return true;
}

void QuestMgr::LoadExtraQuestStuff()
{ 
    // load creature starters
    Quest const* qst;
    uint32 creature, quest;

    QueryResult* pResult = WorldDatabase.Query("SELECT * FROM creature_quest_starter");
    uint32 pos = 0;
    uint32 total;
    if (pResult)
    {
        total = pResult->GetRowCount();
        do
        {
            Field* data = pResult->Fetch();
            creature = data[0].GetUInt32();
            quest = data[1].GetUInt32();

            qst = objmgr.GetQuestTemplate(quest);
            if (!qst)
            {
                Log.Error("ObjectMgr", "Tried to add starter to npc %d for non-existent quest %d.", creature, quest);
            }
            else
            {
                _AddQuest<Creature>(creature, qst, 1);  // 1 = starter
            }
        }
        while (pResult->NextRow());
        delete pResult;
    }

    pResult = WorldDatabase.Query("SELECT * FROM creature_quest_finisher");
    pos = 0;
    if (pResult)
    {
        total = pResult->GetRowCount();
        do
        {
            Field* data = pResult->Fetch();
            creature = data[0].GetUInt32();
            quest = data[1].GetUInt32();

            qst = objmgr.GetQuestTemplate(quest);
            if (!qst)
            {
                Log.Error("ObjectMgr", "Tried to add finisher to npc %d for non-existent quest %d.", creature, quest);
            }
            else
            {
                _AddQuest<Creature>(creature, qst, 2);  // 1 = starter
            }
        }
        while (pResult->NextRow());
        delete pResult;
    }

    pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_starter");
    pos = 0;
    if (pResult)
    {
        total = pResult->GetRowCount();
        do
        {
            Field* data = pResult->Fetch();
            creature = data[0].GetUInt32();
            quest = data[1].GetUInt32();

            qst = objmgr.GetQuestTemplate(quest);
            if (!qst)
            {
                Log.Error("ObjectMgr", "Tried to add starter to go %d for non-existent quest %d.", creature, quest);
            }
            else
            {
                _AddQuest<GameObject>(creature, qst, 1);  // 1 = starter
            }
        }
        while (pResult->NextRow());
        delete pResult;
    }

    pResult = WorldDatabase.Query("SELECT * FROM gameobject_quest_finisher");
    pos = 0;
    if (pResult)
    {
        total = pResult->GetRowCount();
        do
        {
            Field* data = pResult->Fetch();
            creature = data[0].GetUInt32();
            quest = data[1].GetUInt32();

            qst = objmgr.GetQuestTemplate(quest);
            if (!qst)
            {
                Log.Error("ObjectMgr", "Tried to add finisher to go %d for non-existent quest %d.", creature, quest);
            }
            else
            {
                _AddQuest<GameObject>(creature, qst, 2);  // 2 = finish
            }
        }
        while (pResult->NextRow());
        delete pResult;
    }
    objmgr.ProcessGameobjectQuests();

    //load item quest associations
    uint32 item;
    uint8 item_count;

    pResult = WorldDatabase.Query("SELECT * FROM item_quest_association");
    pos = 0;
    if (pResult != NULL)
    {
        total = pResult->GetRowCount();
        do
        {
            Field* data = pResult->Fetch();
            item = data[0].GetUInt32();
            quest = data[1].GetUInt32();
            item_count = data[2].GetUInt8();

            qst = objmgr.GetQuestTemplate(quest);
            if (!qst)
            {
                Log.Error("ObjectMgr", "Tried to add association to item %d for non-existent quest %d.", item, quest);
            }
            else
            {
                AddItemQuestAssociation(item, qst, item_count);
            }
        }
        while (pResult->NextRow());
        delete pResult;
    }

    m_QuestPOIMap.clear();

    QueryResult* result = WorldDatabase.Query("SELECT questId, poiId, objIndex, mapId, mapAreaId, floorId, unk3, unk4 FROM quest_poi");
    if (result != NULL)
    {
        uint32 count = 0;

        do
        {

            Field* fields = result->Fetch();

            uint32 questId = fields[0].GetUInt32();
            uint32 poiId = fields[1].GetUInt32();
            int32  objIndex = fields[2].GetInt32();
            uint32 mapId = fields[3].GetUInt32();
            uint32 mapAreaId = fields[4].GetUInt32();
            uint32 floorId = fields[5].GetUInt32();
            uint32 unk3 = fields[6].GetUInt32();
            uint32 unk4 = fields[7].GetUInt32();

            QuestPOI POI(poiId, objIndex, mapId, mapAreaId, floorId, unk3, unk4);

            m_QuestPOIMap[questId].push_back(POI);

            count++;

        }
        while (result->NextRow());

        delete result;

        Log.Notice("QuestMgr", "Point Of Interest (POI) data loaded for %u quests.", count);



        QueryResult* points = WorldDatabase.Query("SELECT questId, poiId, x, y FROM quest_poi_points");
        if (points != NULL)
        {
            count = 0;

            do
            {

                Field* pointFields = points->Fetch();

                uint32 questId = pointFields[0].GetUInt32();
                uint32 poiId = pointFields[1].GetUInt32();
                int32  x = pointFields[2].GetInt32();
                int32  y = pointFields[3].GetInt32();

                QuestPOIVector & vect = m_QuestPOIMap[questId];

                for (QuestPOIVector::iterator itr = vect.begin(); itr != vect.end(); ++itr)
                {

                    if (itr->PoiId != poiId)
                        continue;

                    QuestPOIPoint point(x, y);

                    itr->points.push_back(point);

                    break;
                }

                count++;

            }
            while (points->NextRow());

            delete points;
            Log.Success("QuestMgr", "%u quest Point Of Interest points loaded.", count);
        }

    }
}

void QuestMgr::AddItemQuestAssociation(uint32 itemId, Quest const* qst, uint8 item_count)
{
    std::unordered_map<uint32, std::list<QuestAssociation*>* > &associationList = GetQuestAssociationList();
    std::list<QuestAssociation*>* tempList;
    QuestAssociation* ptr = NULL;

    // look for the item in the associationList
    if (associationList.find(itemId) == associationList.end())
    {
        // not found. Create a new entry and QuestAssociationList
        tempList = new std::list < QuestAssociation* > ;

        associationList.insert(std::unordered_map<uint32, std::list<QuestAssociation*>* >::value_type(itemId, tempList));
    }
    else
    {
        // item found, now we'll search through its QuestAssociationList
        tempList = associationList.find(itemId)->second;
    }

    // look through this item's QuestAssociationList for a matching quest entry
    std::list<QuestAssociation*>::iterator it;
    for (it = tempList->begin(); it != tempList->end(); ++it)
    {
        if ((*it)->qst == qst)
        {
            // matching quest found
            ptr = (*it);
            break;
        }
    }

    // did we find a matching quest?
    if (ptr == NULL)
    {
        // nope, create a new QuestAssociation for this item and quest
        ptr = new QuestAssociation;
        ptr->qst = qst;
        ptr->item_count = item_count;

        tempList->push_back(ptr);
    }
    else
    {
        // yep, update the QuestAssociation with the new item_count information
        ptr->item_count = item_count;
        LOG_DEBUG("WARNING: Duplicate entries found in item_quest_association, updating item #%d with new item_count: %d.", itemId, item_count);
    }
}

QuestAssociationList* QuestMgr::GetQuestAssociationListForItemId(uint32 itemId)
{
    std::unordered_map<uint32, QuestAssociationList* > &associationList = GetQuestAssociationList();
    std::unordered_map<uint32, QuestAssociationList* >::iterator itr = associationList.find(itemId);
    if (itr == associationList.end())
    {
        return 0;
    }
    else
    {
        return itr->second;
    }
}

void QuestMgr::OnPlayerEmote(Player* plr, uint32 emoteid, uint64 & victimguid)
{
    if (!plr || !emoteid || !victimguid)
        return;

    Unit* victim = plr->GetMapMgr() ? plr->GetMapMgr()->GetUnit(victimguid) : NULL;

    uint32 i, j;
    uint32 entry = (victim) ? victim->GetEntry() : 0;
    QuestLogEntry* qle;
    for (i = 0; i < MAX_QUEST_LOG_SIZE; ++i)
    {
        if ((qle = plr->GetQuestLogInSlot(i)) != 0)
        {
            // dont waste time on quests without emotes
            if (!qle->IsEmoteQuest())
            {
                continue;
            }

            Quest const* qst = qle->GetQuest();
            for (j = 0; j < 4; ++j)
            {
                if (qst->ReqCreatureOrGOId[j])
                {
                    if (victim && qst->ReqCreatureOrGOId[j] == static_cast<int32>(entry) && qst->ReqEmoteId == emoteid && (qle->m_mobcount[j] < qst->ReqCreatureOrGOCount[j] || qle->m_mobcount[j] == 0) && !qle->IsUnitAffected(victim))
                    {
                        qle->AddAffectedUnit(victim);
                        qle->IncrementMobCount(j);
                        if (qst->GetQuestId() == 11224)   // Show progress for quest "Send Them Packing"
                        {
                            qle->SendUpdateAddKill(j);
                        }
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
                // in case some quest doesn't have a required target for the emote..
                else
                {
                    if (qst->ReqEmoteId == emoteid)
                    {
                        qle->IncrementMobCount(j);
                        qle->UpdatePlayerFields();
                        if (qle->CanBeFinished())
                            qle->SendQuestComplete();
                        break;
                    }
                }
            }
        }
    }
}

void QuestMgr::BuildQuestPOIResponse(WorldPacket& data, uint32 questid)
{
    Quest const* q = objmgr.GetQuestTemplate(questid);

    if (q != NULL)
    {
        QuestPOIVector const* POI = NULL;

        QuestPOIMap::iterator itr = m_QuestPOIMap.find(questid);

        if (itr != m_QuestPOIMap.end())
            POI = &(itr->second);

        if (POI != NULL)
        {

            data << uint32(questid);
            data << uint32(POI->size());

            for (QuestPOIVector::const_iterator itr = POI->begin(); itr != POI->end(); ++itr)
            {

                data << uint32(itr->PoiId);
                data << int32(itr->ObjectiveIndex);
                data << uint32(itr->MapId);
                data << uint32(itr->MapAreaId);
                data << uint32(itr->FloorId);
                data << uint32(itr->Unk3);
                data << uint32(itr->Unk4);
                data << uint32(itr->points.size());

                for (std::vector< QuestPOIPoint >::const_iterator itr2 = itr->points.begin(); itr2 != itr->points.end(); ++itr2)
                {

                    data << int32(itr2->x);
                    data << int32(itr2->y);
                }
            }

        }
        else
        {
            data << uint32(questid);
            data << uint32(0);
        }

    }
    else
    {
        data << uint32(questid);
        data << uint32(0);
    }
}

void QuestMgr::FillQuestMenu(Creature* giver, Player* plr, Arcemu::Gossip::Menu & menu)
{
    uint32 status;
    uint8 icon;
    if (giver->isQuestGiver() && giver->HasQuests())
    {
        for (std::list<QuestRelation*>::iterator itr = giver->QuestsBegin(); itr != giver->QuestsEnd(); ++itr)
        {
            status = sQuestMgr.CalcQuestStatus(giver, plr, *itr);
            if (status >= QMGR_QUEST_CHAT)
            {
                switch (status >= QMGR_QUEST_CHAT)
                {
                case QMGR_QUEST_NOT_FINISHED:
                    icon = QMGR_QUEST_REPEATABLE_LOWLEVEL;
                    break;

                case QMGR_QUEST_FINISHED:
                    icon = QMGR_QUEST_REPEATABLE_LOWLEVEL;
                    break;

                case QMGR_QUEST_CHAT:
                    icon = QMGR_QUEST_AVAILABLE;
                    break;

                default:
                    icon = (uint8)status;
                    break;
                }
                menu.AddQuest((*itr)->qst, icon);
            }
        }
    }
}
