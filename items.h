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
    itAmmunition,
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
    // Light items
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
/*
struct sContainerItem{
    unsigned int id;       // database item id
//    char button;
//    bool equip;             // ? equiped or not
};
*/

class CItemsContainer{
    public:
        CItemsContainer();
        bool push_back(int item_id, int amount);            // add item to end
        bool AddItem(unsigned int item_id, int amount);     // add item to list (merge if posible)
        //void InsertByIndex(unsigned int item_id, int amount, int index);      // insert new item to container (used index)
        int GetByIndex(int index);              // return item id
        //bool IsEquipped();
        bool RemoveByIndex(int index);
        unsigned int size() const;
        bool IsEmpty() const;
        void clear();
        int GetIndexByButton(char button);  // return item index or -1 if not have
        char ButtonByIndex(int index);
        int AmountByIndex(int index);
//        eEquipSlotNames EquippedSlot(int index);
    private:
        char GetFirstFreeButton();
        bool MergeItem(unsigned int item_id, int amount);   // if posible
        class Node{
            public:
                unsigned int item;
                char button;
                int amount;
//                eEquipSlotNames eq_slot;
                Node *next;
                Node(){next=NULL;}
             //   Node *prev;
        };
        Node *head;
};



// Items Database
extern std::vector <sItemDescription> ItemsDB;
extern void InitItemsDB(); // Fill Items Database
extern void DeleteItemsDB(); // Free Items Database
// get item_id, return pointer to item from database
extern sItemDescription* DBItemByID(unsigned int item_id);

// selected item
//extern int ItemsDB_SelectedItem;
// Show items detail description window
extern void ItemsDB_RenderDescription(int item_id);

//extern char LetterButtonsList[];

#endif // ITEMS_H_INCLUDED
