#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include "tileset.h"
#include "myogl/ui/bitmap_font.h"
#include <vector>

enum eItemTypes{
    itNone=0,
    itLightSource,
    itEat,
    itDrink,
    itScroll,
    itMeleeWeapon,
    itRangedWeapon,
    itBullets,
    itWand,
    itCloth,
    itArmor,
    itGloves,
    itBoots,
    itHelm,
    itNeckJav,
    itRing,
    itMagicBook,
    itPrayerBook
};

// humanoid slots
enum eEquipSlotNames{
    slNone,
    slHead,
    slNeck,
    slBody,
    slBody2,
    // OneHanded Melee, 2Handed Mellee, Shields, Trow
    slLArm,
    slRArm,
    // Ranged Weapon
    slRanged,
    // Boots
    slLegs,
    // Rings
    slRing,
    slLight
};

struct sItemDescription{
    unsigned int id;
    eItemTypes type;
    char *name;
    char *description;
    int sprite_id;
    int d_count, d_name, d_delta;   // dice
    int modif1, modif2;             // modificator (+x, +y) or [+X, +Y] for armor
    int weight;
    eEquipSlotNames equip_slot;
};

struct sItem{
    unsigned int id;
    int amount;
    bool equip;
    int button;
};

struct sInventoryEqSlots{
    char *displayed_name;
    eEquipSlotNames slot_position;
    int item_inv_id;
};

class CInventory{
        std::vector <sItem> m_items;    // indexes - inv_id
        std::vector <sInventoryEqSlots> m_slots; // indexes slot_id
        struct {
            bool drop;
            bool eat_drink;
            bool equip;
            bool unequip;
            bool read;
        } m_sel_item_actions;
        unsigned int m_selected_item;   // need for display item detailed information

        bool *used_buttons;
        // for render
        CFont *m_font;
        CTileset *m_item_tileset;

        int SearchFreeButtonIndex();  // return first free button for item
        int GetButtonIndex(char button); // get button index in all button array. need for set used/unused
        int GetItemIndexByButton(char button);  // return item index (inv_id) by button index. (for remove item from inventory)
        void BuildSelectedItemAtions(void); // build selected item actions and set item_action structure
        int SearchEmptyEqSlots(eEquipSlotNames slot_position); // return empty slot or -1 if not founded
        int SearchFirstEqSlot(eEquipSlotNames slot_name);   // return first slot (empty/not empty) or -1 if not fount in m_slots
        void AddEquipmentSlot(eEquipSlotNames position, const char *name);  // add slots for equip items
        int SearchSlotIdByInvItemId(int inv_id); // return slot itendex for item inv_id, or -1 - if item not equipped

    public:
        CInventory();
        ~CInventory(){
            m_font=NULL;
            m_item_tileset=NULL;
            m_items.clear();
            delete used_buttons;
            for(unsigned i=0;i<m_slots.size();i++){
                delete m_slots[i].displayed_name;
            }
            m_slots.clear();
        }
        // add item to inventory
        bool AddItem(unsigned int item_id, int amount=0);
        // remove from inventory
        bool RemoveItem(unsigned int list_id);
        // remove from inventory
        bool RemoveItemByButton(char button);
        // equip/unequip item
        bool Equip(int inv_id=-1); // inv_id
        bool Unequip(int inv_id=-1); // inv_id


        void SetFont(CFont *font){ m_font=font; };
        void SetItemTileset(CTileset *tileset){ m_item_tileset=tileset; };
        bool SelectItemByKey(int key_code);
        bool SelectItemById(int inv_id);
        void Render(); // render items list
        void RenderItemDetail(void); // render selected item detailed info
        void RenderEquipped(void);
};

extern std::vector <sItemDescription> ItemsDB;
extern void InitItemsDB();
extern void DeleteItemsDB();
extern int ItemsDB_SelectedItem;
extern void ItemsDB_RenderDescription(int item_id);

#endif // ITEMS_H_INCLUDED
