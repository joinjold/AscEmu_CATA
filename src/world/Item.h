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

#ifndef _WOWSERVER_ITEM_H
#define _WOWSERVER_ITEM_H

#include "Skill.h"

struct EnchantmentInstance
{
    EnchantEntry* Enchantment;
    bool BonusApplied;
    uint32 Slot;
    time_t ApplyTime;
    uint32 Duration;
    bool RemoveAtLogout;
    uint32 RandomSuffix;
};

const static ItemProf prof[22] = { {4, 2}, {4, 4}, {4, 8}, {4, 16}, {4, 64},
    {2, 1}, {2, 2}, {2, 4}, {2, 8}, {2, 16}, {2, 32}, {2, 64}, {2, 128}, {2, 256}, {2, 1024}, {2, 8192}, {2, 32768}, {2, 65536}, {2, 131072},
    {2, 262144}, {2, 524288}, {2, 1048576}
};

const static uint32 arm_skills[7] = { 0,
                                      SKILL_CLOTH,
                                      SKILL_LEATHER,
                                      SKILL_MAIL,
                                      SKILL_PLATE_MAIL,
                                      0,
                                      SKILL_SHIELD
                                    };

const static uint32 weap_skills[21] = { SKILL_AXES,
                                        SKILL_2H_AXES,
                                        SKILL_BOWS,
                                        SKILL_GUNS,
                                        SKILL_MACES,
                                        SKILL_2H_MACES,
                                        SKILL_POLEARMS,
                                        SKILL_SWORDS,
                                        SKILL_2H_SWORDS,
                                        0,
                                        SKILL_STAVES,
                                        0,
                                        0,
                                        SKILL_FIST_WEAPONS,
                                        0,                      /// 13
                                        SKILL_DAGGERS,
                                        SKILL_THROWN,
                                        SKILL_ASSASSINATION,
                                        SKILL_CROSSBOWS,
                                        SKILL_WANDS,
                                        SKILL_FISHING
                                      };

const static float pricemod[9] = { 1.0f,    // HATED
                                   1.0f,    // HOSTILE
                                   1.0f,    // UNFRIENDLY
                                   1.0f,    // NEUTRAL
                                   0.95f,   // FRIENDLY
                                   0.90f,   // HONORED
                                   0.85f,   // REVERED
                                   0.80f    // EXHALTED
                                 };

const static double SuffixMods[NUM_INVENTORY_TYPES] = { 0.0,
        0.46,        // HEAD
        0.26,        // NECK
        0.35,        // SHOULDERS
        0.46,        // BODY
        0.46,        // CHEST
        0.35,        // WAIST
        0.46,        // LEGS
        0.34,        // FEET
        0.26,        // WRISTS
        0.35,        // HANDS
        0.26,        // FINGER
        0.0,         // TRINKET
        0.19,        // WEAPON
        0.25,        // SHEILD
        0.14,        // RANGED
        0.26,        // CLOAK
        0.46,        // 2H-WEAPON
        0.0,         // BAG
        0.0,         // TABARD
        0.46,        // ROBE
        0.19,        // MAIN-HAND WEAPON
        0.19,        // OFF-HAND WEAPON
        0.26,        // HOLDABLE
        0.0,         // AMMO
        0.26,        // THROWN
        0.14,        // RANGED
        0.0,         // QUIVER
        0.26,        // RELIC
                                                      };

typedef std::map<uint32, EnchantmentInstance> EnchantmentMap;

#define APPLY true
#define REMOVE false

enum scalingstatmodtypes
{
    SCALINGSTATSTAT,
    SCALINGSTATARMOR,
    SCALINGSTATDAMAGE,
    SCALINGSTATSPELLPOWER
};

/// -1 from client enchantment slot number
enum EnchantmentSlot
{
    PERM_ENCHANTMENT_SLOT           = 0,
    TEMP_ENCHANTMENT_SLOT           = 1,
    SOCK_ENCHANTMENT_SLOT1          = 2,
    SOCK_ENCHANTMENT_SLOT2          = 3,
    SOCK_ENCHANTMENT_SLOT3          = 4,
    BONUS_ENCHANTMENT_SLOT          = 5,
    PRISMATIC_ENCHANTMENT_SLOT      = 6,

    REFORGE_ENCHANTMENT_SLOT        = 8,
    TRANSMOGRIFY_ENCHANTMENT_SLOT   = 9,
    MAX_INSPECTED_ENCHANTMENT_SLOT  = 10,

    PROP_ENCHANTMENT_SLOT_0         = 10,                   // used with RandomSuffix
    PROP_ENCHANTMENT_SLOT_1         = 11,                   // used with RandomSuffix
    PROP_ENCHANTMENT_SLOT_2         = 12,                   // used with RandomSuffix and RandomProperty
    PROP_ENCHANTMENT_SLOT_3         = 13,                   // used with RandomProperty
    PROP_ENCHANTMENT_SLOT_4         = 14,                   // used with RandomProperty
    MAX_ENCHANTMENT_SLOT            = 15
};

enum RandomEnchantmentTypes
{
    RANDOMPROPERTY = 1,
    RANDOMSUFFIX   = 2
};

enum InventoryResult
{
    EQUIP_ERR_OK = 0,
    EQUIP_ERR_CANT_EQUIP_LEVEL_I = 1,  // You must reach level %d to use that item.
    EQUIP_ERR_CANT_EQUIP_SKILL = 2,  // You aren't skilled enough to use that item.
    EQUIP_ERR_WRONG_SLOT = 3,  // That item does not go in that slot.
    EQUIP_ERR_BAG_FULL = 4,  // That bag is full.
    EQUIP_ERR_BAG_IN_BAG = 5,  // Can't put non-empty bags in other bags.
    EQUIP_ERR_TRADE_EQUIPPED_BAG = 6,  // You can't trade equipped bags.
    EQUIP_ERR_AMMO_ONLY = 7,  // Only ammo can go there.
    EQUIP_ERR_PROFICIENCY_NEEDED = 8,  // You do not have the required proficiency for that item.
    EQUIP_ERR_NO_SLOT_AVAILABLE = 9,  // No equipment slot is available for that item.
    EQUIP_ERR_CANT_EQUIP_EVER = 10, // You can never use that item.
    EQUIP_ERR_CANT_EQUIP_EVER_2 = 11, // You can never use that item.
    EQUIP_ERR_NO_SLOT_AVAILABLE_2 = 12, // No equipment slot is available for that item.
    EQUIP_ERR_2HANDED_EQUIPPED = 13, // Cannot equip that with a two-handed weapon.
    EQUIP_ERR_2HSKILLNOTFOUND = 14, // You cannot dual-wield
    EQUIP_ERR_WRONG_BAG_TYPE = 15, // That item doesn't go in that container.
    EQUIP_ERR_WRONG_BAG_TYPE_2 = 16, // That item doesn't go in that container.
    EQUIP_ERR_ITEM_MAX_COUNT = 17, // You can't carry any more of those items.
    EQUIP_ERR_NO_SLOT_AVAILABLE_3 = 18, // No equipment slot is available for that item.
    EQUIP_ERR_CANT_STACK = 19, // This item cannot stack.
    EQUIP_ERR_NOT_EQUIPPABLE = 20, // This item cannot be equipped.
    EQUIP_ERR_CANT_SWAP = 21, // These items can't be swapped.
    EQUIP_ERR_SLOT_EMPTY = 22, // That slot is empty.
    EQUIP_ERR_ITEM_NOT_FOUND = 23, // The item was not found.
    EQUIP_ERR_DROP_BOUND_ITEM = 24, // You can't drop a soulbound item.
    EQUIP_ERR_OUT_OF_RANGE = 25, // Out of range.
    EQUIP_ERR_TOO_FEW_TO_SPLIT = 26, // Tried to split more than number in stack.
    EQUIP_ERR_SPLIT_FAILED = 27, // Couldn't split those items.
    EQUIP_ERR_SPELL_FAILED_REAGENTS_GENERIC = 28, // Missing reagent
    EQUIP_ERR_NOT_ENOUGH_MONEY = 29, // You don't have enough money.
    EQUIP_ERR_NOT_A_BAG = 30, // Not a bag.
    EQUIP_ERR_DESTROY_NONEMPTY_BAG = 31, // You can only do that with empty bags.
    EQUIP_ERR_NOT_OWNER = 32, // You don't own that item.
    EQUIP_ERR_ONLY_ONE_QUIVER = 33, // You can only equip one quiver.
    EQUIP_ERR_NO_BANK_SLOT = 34, // You must purchase that bag slot first
    EQUIP_ERR_NO_BANK_HERE = 35, // You are too far away from a bank.
    EQUIP_ERR_ITEM_LOCKED = 36, // Item is locked.
    EQUIP_ERR_GENERIC_STUNNED = 37, // You are stunned
    EQUIP_ERR_PLAYER_DEAD = 38, // You can't do that when you're dead.
    EQUIP_ERR_CLIENT_LOCKED_OUT = 39, // You can't do that right now.
    EQUIP_ERR_INTERNAL_BAG_ERROR = 40, // Internal Bag Error
    EQUIP_ERR_ONLY_ONE_BOLT = 41, // You can only equip one quiver.
    EQUIP_ERR_ONLY_ONE_AMMO = 42, // You can only equip one ammo pouch.
    EQUIP_ERR_CANT_WRAP_STACKABLE = 43, // Stackable items can't be wrapped.
    EQUIP_ERR_CANT_WRAP_EQUIPPED = 44, // Equipped items can't be wrapped.
    EQUIP_ERR_CANT_WRAP_WRAPPED = 45, // Wrapped items can't be wrapped.
    EQUIP_ERR_CANT_WRAP_BOUND = 46, // Bound items can't be wrapped.
    EQUIP_ERR_CANT_WRAP_UNIQUE = 47, // Unique items can't be wrapped.
    EQUIP_ERR_CANT_WRAP_BAGS = 48, // Bags can't be wrapped.
    EQUIP_ERR_LOOT_GONE = 49, // Already looted
    EQUIP_ERR_INV_FULL = 50, // Inventory is full.
    EQUIP_ERR_BANK_FULL = 51, // Your bank is full
    EQUIP_ERR_VENDOR_SOLD_OUT = 52, // That item is currently sold out.
    EQUIP_ERR_BAG_FULL_2 = 53, // That bag is full.
    EQUIP_ERR_ITEM_NOT_FOUND_2 = 54, // The item was not found.
    EQUIP_ERR_CANT_STACK_2 = 55, // This item cannot stack.
    EQUIP_ERR_BAG_FULL_3 = 56, // That bag is full.
    EQUIP_ERR_VENDOR_SOLD_OUT_2 = 57, // That item is currently sold out.
    EQUIP_ERR_OBJECT_IS_BUSY = 58, // That object is busy.
    EQUIP_ERR_CANT_BE_DISENCHANTED = 59,
    EQUIP_ERR_NOT_IN_COMBAT = 60, // You can't do that while in combat
    EQUIP_ERR_NOT_WHILE_DISARMED = 61, // You can't do that while disarmed
    EQUIP_ERR_BAG_FULL_4 = 62, // That bag is full.
    EQUIP_ERR_CANT_EQUIP_RANK = 63, // You don't have the required rank for that item
    EQUIP_ERR_CANT_EQUIP_REPUTATION = 64, // You don't have the required reputation for that item
    EQUIP_ERR_TOO_MANY_SPECIAL_BAGS = 65, // You cannot equip another bag of that type
    EQUIP_ERR_LOOT_CANT_LOOT_THAT_NOW = 66, // You can't loot that item now.
    EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE = 67, // You cannot equip more than one of those.
    EQUIP_ERR_VENDOR_MISSING_TURNINS = 68, // You do not have the required items for that purchase
    EQUIP_ERR_NOT_ENOUGH_HONOR_POINTS = 69, // You don't have enough honor points
    EQUIP_ERR_NOT_ENOUGH_ARENA_POINTS = 70, // You don't have enough arena points
    EQUIP_ERR_ITEM_MAX_COUNT_SOCKETED = 71, // You have the maximum number of those gems in your inventory or socketed into items.
    EQUIP_ERR_MAIL_BOUND_ITEM = 72, // You can't mail soulbound items.
    EQUIP_ERR_INTERNAL_BAG_ERROR_2 = 73, // Internal Bag Error
    EQUIP_ERR_BAG_FULL_5 = 74, // That bag is full.
    EQUIP_ERR_ITEM_MAX_COUNT_EQUIPPED_SOCKETED = 75, // You have the maximum number of those gems socketed into equipped items.
    EQUIP_ERR_ITEM_UNIQUE_EQUIPPABLE_SOCKETED = 76, // You cannot socket more than one of those gems into a single item.
    EQUIP_ERR_TOO_MUCH_GOLD = 77, // At gold limit
    EQUIP_ERR_NOT_DURING_ARENA_MATCH = 78, // You can't do that while in an arena match
    EQUIP_ERR_TRADE_BOUND_ITEM = 79, // You can't trade a soulbound item.
    EQUIP_ERR_CANT_EQUIP_RATING = 80, // You don't have the personal, team, or battleground rating required to buy that item
    EQUIP_ERR_NO_OUTPUT = 81,
    EQUIP_ERR_NOT_SAME_ACCOUNT = 82, // Account-bound items can only be given to your own characters.
    EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_COUNT_EXCEEDED_IS = 84, // You can only carry %d %s
    EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_SOCKETED_EXCEEDED_IS = 85, // You can only equip %d |4item:items in the %s category
    EQUIP_ERR_SCALING_STAT_ITEM_LEVEL_EXCEEDED = 86, // Your level is too high to use that item
    EQUIP_ERR_PURCHASE_LEVEL_TOO_LOW = 87, // You must reach level %d to purchase that item.
    EQUIP_ERR_CANT_EQUIP_NEED_TALENT = 88, // You do not have the required talent to equip that.
    EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_EQUIPPED_EXCEEDED_IS = 89, // You can only equip %d |4item:items in the %s category
    EQUIP_ERR_SHAPESHIFT_FORM_CANNOT_EQUIP = 90, // Cannot equip item in this form
    EQUIP_ERR_ITEM_INVENTORY_FULL_SATCHEL = 91, // Your inventory is full. Your satchel has been delivered to your mailbox.
    EQUIP_ERR_SCALING_STAT_ITEM_LEVEL_TOO_LOW = 92, // Your level is too low to use that item
    EQUIP_ERR_CANT_BUY_QUANTITY = 93, // You can't buy the specified quantity of that item.
};

#define RANDOM_SUFFIX_MAGIC_CALCULATION(__suffix, __scale) float2int32(float(__suffix) * float(__scale) / 10000.0f);

class SERVER_DECL Item : public Object
{
    public:
        Item();
        void Init(uint32 high, uint32 low);
        virtual ~Item();
        void Create(uint32 itemid, Player* owner);

        ItemPrototype* GetProto() const { return m_itemProto; }
        void SetProto(ItemPrototype* pr) { m_itemProto = pr; }

        Player* GetOwner() const { return m_owner; }
        void SetOwner(Player* owner);

        void SetOwnerGUID(uint64 GUID) { SetUInt64Value(ITEM_FIELD_OWNER, GUID); }
        uint64 GetOwnerGUID() { return GetUInt64Value(ITEM_FIELD_OWNER); }

        void SetContainerGUID(uint64 GUID) { SetUInt64Value(ITEM_FIELD_CONTAINED, GUID); }
        uint64 GetContainerGUID() { return GetUInt64Value(ITEM_FIELD_CONTAINED); }

        void SetCreatorGUID(uint64 GUID) { SetUInt64Value(ITEM_FIELD_CREATOR, GUID); }
        void SetGiftCreatorGUID(uint64 GUID) { SetUInt64Value(ITEM_FIELD_GIFTCREATOR, GUID); }

        uint64 GetCreatorGUID() { return GetUInt64Value(ITEM_FIELD_CREATOR); }
        uint64 GetGiftCreatorGUID() { return GetUInt64Value(ITEM_FIELD_GIFTCREATOR); }

        void SetStackCount(uint32 amt) { SetUInt32Value(ITEM_FIELD_STACK_COUNT, amt); }
        uint32 GetStackCount() { return GetUInt32Value(ITEM_FIELD_STACK_COUNT); }
        void ModStackCount(int32 val) { ModUnsigned32Value(ITEM_FIELD_STACK_COUNT, val); }

        void SetDuration(uint32 durationseconds) { SetUInt32Value(ITEM_FIELD_DURATION, durationseconds); }
        uint32 GetDuration() { return GetUInt32Value(ITEM_FIELD_DURATION); }

        void SetCharges(uint32 index, uint32 charges) { SetUInt32Value(ITEM_FIELD_SPELL_CHARGES + index, charges); }
        void ModCharges(uint32 index, int32 val) { ModSignedInt32Value(ITEM_FIELD_SPELL_CHARGES + index, val); }
        uint32 GetCharges(uint32 index) const { return GetUInt32Value(ITEM_FIELD_SPELL_CHARGES + index); }

        /////////////////////////////////////////////////// FLAGS ////////////////////////////////////////////////////////////

        void SoulBind() { SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_SOULBOUND); }
        uint32 IsSoulbound() { return HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_SOULBOUND); }

        void AccountBind() { SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_ACCOUNTBOUND); }
        uint32 IsAccountbound() { return HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_ACCOUNTBOUND);  }

        void MakeConjured() { SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_CONJURED); }
        uint32 IsConjured() { return HasFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_CONJURED); }

        void Lock() { RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_LOOTABLE); }
        void UnLock() { SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_LOOTABLE); }

        void Wrap() { SetFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_WRAPPED); }
        void UnWrap() { RemoveFlag(ITEM_FIELD_FLAGS, ITEM_FLAG_WRAPPED); }

        void ClearFlags() { SetFlag(ITEM_FIELD_FLAGS, 0); }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void SetDirty(){ m_isDirty = true; }

        uint32 GetItemRandomPropertyId() const { return m_uint32Values[ITEM_FIELD_RANDOM_PROPERTIES_ID]; }
        uint32 GetItemRandomSuffixFactor() { return m_uint32Values[ITEM_FIELD_PROPERTY_SEED]; }

        void SetItemRandomPropertyId(uint32 id)
        {
            random_prop = id;
            SetUInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, id);
        }

        void SetItemRandomSuffixFactor(uint32 factor)
        {
            random_suffix = factor;
            SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, factor);
        }

        void SetRandomSuffix(uint32 id)
        {
            int32 r_id = -(int32(id));
            uint32 v = Item::GenerateRandomSuffixFactor(m_itemProto);
            SetItemRandomPropertyId((uint32)r_id);
            SetItemRandomSuffixFactor(v);
            random_suffix = id;
        }

        void SetDurability(uint32 Value) { SetUInt32Value(ITEM_FIELD_DURABILITY, Value); };
        void SetDurabilityMax(uint32 Value) { SetUInt32Value(ITEM_FIELD_MAXDURABILITY, Value); };

        uint32 GetDurability() { return GetUInt32Value(ITEM_FIELD_DURABILITY); }
        uint32 GetDurabilityMax() { return GetUInt32Value(ITEM_FIELD_MAXDURABILITY); }

        void SetDurabilityToMax() { SetUInt32Value(ITEM_FIELD_DURABILITY, GetUInt32Value(ITEM_FIELD_MAXDURABILITY)); }

        uint32 GetEnchantmentId(uint32 index) { return GetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 3 * index); }
        void SetEnchantmentId(uint32 index, uint32 value) { SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 3 * index, value); }

        uint32 GetEnchantmentDuration(uint32 index) { return GetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 1 + 3 * index); }
        void SetEnchantmentDuration(uint32 index, uint32 value) { SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 1 + 3 * index, value); }

        uint32 GetEnchantmentCharges(uint32 index) { return GetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 2 + 3 * index); }
        void SetEnchantmentCharges(uint32 index, uint32 value) { SetUInt32Value(ITEM_FIELD_ENCHANTMENT_1_1 + 2 + 3 * index, value); }

        //////////////////////////////////////////////////////////
        // Creation time in terms of played time
        /////////////////////////////////////////////////////////
        void SetCreationTime(uint32 time) { SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, time); }
        uint32 GetCreationTime() { return GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME); }

        // DB Serialization
        void LoadFromDB(Field* fields, Player* plr, bool light);
        void SaveToDB(int8 containerslot, int8 slot, bool firstsave, QueryBuffer* buf);
        bool LoadAuctionItemFromDB(uint64 guid);
        void DeleteFromDB();
        void DeleteMe();
        bool IsEligibleForRefund();

        //////////////////////////////////////////////////////////////////////////////////////////
        /// uint32 GetChargesLeft()
        /// Finds an on-use spell on the item and returns the charges left
        ///
        /// \param none
        ///
        /// \returns the charges left if an on-use spell is found, 0 if no such spell found.
        ///
        //////////////////////////////////////////////////////////////////////////////////////////
        uint32 GetChargesLeft() const
        {
            for (uint32 x = 0; x < 5; x++)
                if ((m_itemProto->Spells[x].Id != 0) && (m_itemProto->Spells[x].Trigger == USE))
                    return GetCharges(x);

            return 0;
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        /// void SetChargesLeft(uint32 charges)
        /// Finds an on-use spell on the item, and sets the remaining charges.
        /// If no such spell found, nothing changes.
        ///
        /// \param uint32 charges  -  Number to be set as charges left.
        ///
        /// \returns none
        ///
        //////////////////////////////////////////////////////////////////////////////////////////
        void SetChargesLeft(uint32 charges)
        {
            for (uint32 x = 0; x < 5; x++)
            {
                if ((m_itemProto->Spells[x].Id != 0) && (m_itemProto->Spells[x].Trigger == USE))
                {
                    SetCharges(x, charges);
                    break;
                }
            }
        }

        time_t GetEnchantmentApplytime(uint32 slot)
        {
            EnchantmentMap::iterator itr = Enchantments.find(slot);
            if (itr == Enchantments.end())
                return 0;
            else
                return itr->second.ApplyTime;
        }

        /// Adds an enchantment to the item.
        int32 AddEnchantment(EnchantEntry* Enchantment, uint32 Duration, bool Perm = false, bool apply = true, bool RemoveAtLogout = false, uint32 Slot_ = 0, uint32 RandomSuffix = 0);
        uint32 GetSocketsCount();

        /// Removes an enchantment from the item.
        void RemoveEnchantment(uint32 EnchantmentSlot);

        // Removes related temporary enchants
        void RemoveRelatedEnchants(EnchantEntry* newEnchant);

        /// Adds the bonus on an enchanted item.
        void ApplyEnchantmentBonus(uint32 Slot, bool Apply);

        /// Applies all enchantment bonuses (use on equip)
        void ApplyEnchantmentBonuses();

        /// Removes all enchantment bonuses (use on dequip)
        void RemoveEnchantmentBonuses();

        /// Event to remove an enchantment.
        void EventRemoveEnchantment(uint32 Slot);

        /// Check if we have an enchantment of this id?
        int32 HasEnchantment(uint32 Id);

        /// Check if we have an enchantment on that slot
        bool HasEnchantmentOnSlot(uint32 slot);

        /// Modify the time of an existing enchantment.
        void ModifyEnchantmentTime(uint32 Slot, uint32 Duration);

        /// Find free enchantment slot.
        int32 FindFreeEnchantSlot(EnchantEntry* Enchantment, uint32 random_type);

        /// Removes all enchantments.
        void RemoveAllEnchantments(bool OnlyTemporary);

        /// Sends SMSG_ITEM_UPDATE_ENCHANT_TIME
        void SendEnchantTimeUpdate(uint32 Slot, uint32 Duration);

        void SendDurationUpdate();

        /// Applies any random properties the item has.
        void ApplyRandomProperties(bool apply);

        void RemoveProfessionEnchant();
        void RemoveSocketBonusEnchant();

        /// gets the itemlink for a message to the player
        std::string GetItemLink(uint32 language);

        bool IsAmmoBag() { return (m_itemProto->Class == ITEM_CLASS_QUIVER); }

        uint32 CountGemsWithLimitId(uint32 Limit);

        void RemoveFromWorld();

        Loot* loot;
        bool locked;
        bool m_isDirty;

        EnchantmentInstance* GetEnchantment(uint32 slot);
        bool IsGemRelated(EnchantEntry* Enchantment);

        static uint32 GenerateRandomSuffixFactor(ItemPrototype* m_itemProto);

        bool HasEnchantments() { return (Enchantments.size() > 0) ? true : false; }

        uint32 wrapped_item_id;

        time_t GetItemExpireTime() { return ItemExpiresOn; }
        void SetItemExpireTime(time_t timesec) { ItemExpiresOn = timesec; }
        void EventRemoveItem();
        void RemoveFromRefundableMap();
        bool RepairItem(Player* pPlayer, bool guildmoney = false, int32* pCost = NULL);
        uint32 RepairItemCost();

        uint32 GetOnUseSpellID(uint32 index) { return OnUseSpellIDs[index]; }
        bool HasOnUseSpellID(uint32 id)
        {
            for (uint32 i = 0; i < 3; ++i)
                if (OnUseSpellIDs[i] == id)
                    return true;

            return false;
        }

        void SetText(std::string &text){ this->text = text; }
        const std::string& GetText() const{ return this->text; }

    protected:

        ItemPrototype* m_itemProto;
        EnchantmentMap Enchantments;
        uint32 _fields[ITEM_END];   /// this mem is wasted in case of container... but this will be fixed in future
        Player* m_owner;            /// let's not bother the manager with unneeded requests
        uint32 random_prop;
        uint32 random_suffix;
        time_t ItemExpiresOn;       /// this is for existingduration

    private:
        /// Enchant type 3 spellids, like engineering gadgets appliable to items.
        uint32 OnUseSpellIDs[3];
        std::string text;
};

uint32 GetSkillByProto(uint32, uint32);

uint32 GetSellPriceForItem(ItemPrototype* proto, uint32 count);
uint32 GetBuyPriceForItem(ItemPrototype* proto, uint32 count, Player* plr, Creature* vendor);

uint32 GetSellPriceForItem(uint32 itemid, uint32 count);
uint32 GetBuyPriceForItem(uint32 itemid, uint32 count, Player* plr, Creature* vendor);

std::string GetItemLinkByProto(ItemPrototype* iProto, uint32 language);

int32 GetStatScalingStatValueColumn(ItemPrototype* proto, uint32 type);

#endif // _WOWSERVER_ITEM_H
