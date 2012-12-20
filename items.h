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

struct sContainerItem{
    unsigned int id;       // database item id
    int amount;             // items count
    bool equip;             // ? equiped or not
    int button;             // button - used for this item in this container
};

class CItemsContainer{
    public:
        CItemsContainer();
        void AddItem(unsigned int item_id, int amount);
        void RemoveItem(unsigned int item_id, int amount);
        void RemoveByIndex(char index_letter, int amount);
        bool IsEmpty() const;
    private:
        class Node{
            public:
                sContainerItem item;
                Node *next;
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

#endif // ITEMS_H_INCLUDED
