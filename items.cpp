
#include "items.h"
#include <string.h>
#include "myogl/log.h"


std::vector <sItemDescription> ItemsDB;

char LetterButtonsList[]={"abcdefghijklmnopqrstuvwxyz\0"};

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
    ADD_ITEM(1, itEat, "Хлеб", "Кусок заплесневешего хлеба.", tnBread, 3,6,0, 1, slNone)
    ADD_ITEM(2, itEat, "Мясо", "Свежее мясо. Еще совсем недавно бегало и пищало.", tnMeat, 10,6,0, 1, slNone)
    ADD_ITEM(2, itEat, "Слизь", "Странно пахнущая, бесформенная масса.", tnSlime, 10,6,0, 1, slNone)
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
    Log->printf("DBItemByID - not found item ID=%d, return item 0\n ",item_id);
    return &ItemsDB[0];
}

// Items Container class ////////////////////////////////////////

CItemsContainer::CItemsContainer(){
    head = NULL;
}

// return element count in container
unsigned int CItemsContainer::size() const{
    unsigned int count=0;
    Node *node;
    node=head;
    while(node!=NULL){
        count++;
        node=node->next;
    }
    return count;
}

// search items in container and merge if posible
bool CItemsContainer::MergeItem(unsigned int item_id, int amount){
    Node *node;
    node=head;
    while(node!=NULL){
        if(node->item==item_id){
            node->amount+=amount;
            return true;
        }
        node=node->next;
    }
    return false;
}

// clear container
void CItemsContainer::clear(){
    Node *node, *next_node;
    node=head;
    while(node!=NULL){
        next_node=node->next;
        delete node;
        node=next_node;
    }
}

// return index by button
int CItemsContainer::GetIndexByButton(char button){
    Node *node;
    int index=0;
    node=head;
    while(node!=NULL){
        if(node->button==button){
            return index;
        }
        node=node->next;
        index++;
    }
    // not found
    return -1;
}

//
char CItemsContainer::GetFirstFreeButton(){
    int button_index;
    bool founded=true;
    Node *node;
    for(button_index=0;button_index<(int)strlen(LetterButtonsList);button_index++){
        node=head;
        while(node!=NULL){
            // check current button
            if(LetterButtonsList[button_index]==node->button){
                founded=false;
                break;
            }
            node=node->next;
        }
        if(founded){
            Log->printf("Found free button %c\n",LetterButtonsList[button_index]);
            return LetterButtonsList[button_index];
        }else{
            founded=true;
        }
    }
    return '_';
}

// Get By ID
int CItemsContainer::GetByIndex(int index){
    Node *node=head;
    int count=0;
    while(node!=NULL){
        if(count==index){
            return node->item;
        }
        node=node->next;
        count++;
    }
    return -1;
}

// return button
char CItemsContainer::ButtonByIndex(int index){
    Node *node=head;
    int count=0;
    while(node!=NULL){
        if(count==index){
            return node->button;
        }
        count++;
        node=node->next;
    }
    return '_';
}

// return amount
int CItemsContainer::AmountByIndex(int index){
    Node *node=head;
    int count=0;
    while(node!=NULL){
        if(count==index){
            return node->amount;
        }
        node=node->next;
        count++;
    }
    return -1;
}

// add item to end of container
bool CItemsContainer::push_back(int item_id, int amount){
    Node *node, *prev;
    if(!head){ // if list empty
        head=new Node;
        head->item=item_id;
        head->amount=amount;
        head->button=LetterButtonsList[0];
        return true;
    }else{
        // search last
        node=head;
        while(node!=NULL){
            prev=node;
            node=node->next;
        }
    // in prev - last node
        prev->next=new Node;
        if(!prev->next){
            Log->puts("Error: not created new item! in CItemsContainer::push_back()");
            return false;
        }
        prev->next->item=item_id;
        prev->next->amount=amount;
        prev->next->button=GetFirstFreeButton();
        return true;
    }
}

// Add item in to container. stack in one field if needed
bool CItemsContainer::AddItem(unsigned int item_id, int amount){
    if(!size()){    // first item
        return push_back(item_id, amount);
    }
    if(MergeItem(item_id, amount)){
        return true;    // item merged
    }
    // push_back, TODO - search prev button index (automatictly sort)
    return push_back(item_id, amount);
}

bool CItemsContainer::RemoveByIndex(int index){
// search for letter index
    Node *node, *prev;
    int count=0;
    node=head;
    prev=head;
    while(node!=NULL){
        if(count==index){ // remove
            prev->next=node->next;   // connect prex and next
            delete node;            // delete current
            Log->printf("Delete from index=%d\n",index);
            return true;
        }
        count++;
        prev=node;
        node=node->next;
    }
    Log->printf("CItemsContainer::RemoveByIndex - item not removed, not founded by index %d\n", index);
    return false;
}

