
#include "items.h"
#include <string.h>
#include "myogl/log.h"


std::vector <sItemDescription> ItemsDB;

#define ADD_ITEM(item_id, item_type, item_name, item_description, item_sprite_id, dice_count, dice_name, dice_delta, item_weight, item_equip_pos) \
    tmp_item.id=item_id; \
    tmp_item.type=item_type; \
    tmp_item.name=new char[strlen(item_name)+1]; \
    strcpy(tmp_item.name, item_name); \
    tmp_item.description=new char[strlen(item_description)+1]; \
    strcpy(tmp_item.description, item_description); \
    tmp_item.sprite_id=item_sprite_id; \
    tmp_item.d_count=dice_count; tmp_item.d_name=dice_name; tmp_item.d_delta=dice_delta; \
    tmp_item.weight=item_weight; \
    tmp_item.equip_slot=item_equip_pos; \
    ItemsDB.push_back(tmp_item); \
    sprintf(tmp,"Added item_id[%d]:%s\n",tmp_item.id,tmp_item.name); \
    MyOGL::Log->puts(tmp);


void InitItemsDB(){
    sItemDescription tmp_item;
    char tmp[100];
    // Eat
    ADD_ITEM(1, itEat, "Хлеб", "Кусок заплесневешего хлеба.", tnBread, 1,6,0, 1, slNone)
    ADD_ITEM(2, itEat, "Мясо", "Свежее мясо. Еще совсем недавно бегало и пищало.", tnMeat, 10,6,0, 1, slNone)
    // Potions
    ADD_ITEM(10, itDrink, "Пустая бутылка", "Обычная пустая бутылка", tnBootleEmpty, 0,0,0, 1, slNone)
    ADD_ITEM(11, itDrink, "Бутылка воды", "Уталяет жажду", tnBootleWater, 0,0,0, 1, slNone)
    ADD_ITEM(12, itDrink, "Бутылка соленой воды", "Уталяет жажду", tnBootleWater, 0,0,0, 1, slNone)
    ADD_ITEM(13, itDrink, "Бутылка пива", "Уталяет жажду", tnBootleBeer, 0,0,0, 1, slNone)
    // One Handed Melee Weapons
    ADD_ITEM(100, itMeleeWeapon, "Нож", "Самый обычный кухонный нож для разделки мяса.", tnDagger, 1,6,0, 1, slRArm)
    ADD_ITEM(101, itMeleeWeapon, "Меч", "Одноручный меч", tnSword, 2,6,0, 10, slRArm)
    ADD_ITEM(102, itMeleeWeapon, "Булава", "Металлическое ядро, закрепленное на палке.", tnMace, 2,6,4, 50, slRArm)
    ADD_ITEM(103, itMeleeWeapon, "Копье", "Металлический наконечник на грубо остуганной палке.", tnSpear, 2,6,0, 16, slRArm)
    ADD_ITEM(104, itMeleeWeapon, "Посох", "Обычная палка, возможно, выломанная в близлежащем кустарнике.", tnStaff, 1,6,0, 8, slRArm)
    // 2 Handed Melee Weapons
    // Ranged Weapons
    // Ammunition
    // Armor
    // Shield
    // Helmet
    // Gloves
    // Boots
    // Rings
    // Amulets
}

void DeleteItemsDB(){
    MyOGL::Log->puts("Clear items database\n");
    for(unsigned int i=0;i<ItemsDB.size();i++){
       delete ItemsDB[i].name;
       delete ItemsDB[i].description;
    }
    ItemsDB.clear();
}

sItemDescription* DBItemByID(unsigned int item_id){
    for(unsigned i=0;i<ItemsDB.size();i++){
        if(ItemsDB[i].id==item_id){
            return &ItemsDB[i];
        }
    }
    Log->puts("DBItemByID - not found item ID=%d, return item 0\n ",item_id);
    return &ItemsDB[0];
}

// Items Container class ////////////////////////////////////////

CItemsContainer::CItemsContainer(){
    head = NULL;
}

// Add item in to container. stack in one field if needed
void CItemsContainer::AddItem(unsigned int item_id, int amount){
    Log->puts("Drop item %d, amount: %d\n", item_id, amount);
// fromm first to last (search id), if founded - add amount
    // else
      // create new node, search first not used letters, insert item after last used letters
}

// search specified item in container and remove
void CItemsContainer::RemoveItem(unsigned int item_id, int amount){
//
    Log->puts("Remove item %d, amount: %d\n",item_id, amount);
}

void CItemsContainer::RemoveByIndex(char index_letter, int amount){
// search for letter index
// remove
    Log->puts("Drop item %c, amount: %d\n", index_letter, amount);
}

