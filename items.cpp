
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

char LetterButtonsList[]={"abcdefghijklmnopqrstuvwxyz"};

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
    // Two Handed Melee Weapons
    // Ranged Weapons
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
    Log->puts("ItemByID - not found item ID=%d, return item 0\n ",item_id);
    return &ItemsDB[0];
}

// Inventory Class. TODO - devide to Container and Inventory::Container
CInventory::CInventory(){
    m_font=NULL;
    m_item_tileset=NULL;
    // create used_buttons array
    used_buttons=new bool[strlen(LetterButtonsList)];
    for(unsigned int i=0;i<strlen(LetterButtonsList);i++){
        used_buttons[i]=false;
    }
    // add humanoid slots
    AddEquipmentSlot(slHead,"Голова");
    AddEquipmentSlot(slNeck,"Шея");
    AddEquipmentSlot(slBody,"Тело");
    AddEquipmentSlot(slBody2,"Поверх");
    AddEquipmentSlot(slRArm,"Правая рука"); // Sword
    AddEquipmentSlot(slLArm,"Левая рука");  // Shield
    AddEquipmentSlot(slRanged,"За спиной");
    AddEquipmentSlot(slRing,"Кольцо");
    AddEquipmentSlot(slRing,"Кольцо");
    AddEquipmentSlot(slLight,"Источник света");
}

// add EqSlot
void CInventory::AddEquipmentSlot(eEquipSlotNames position, const char *name){
    sInventoryEqSlots slot;
    slot.slot_position=position;
    slot.item_inv_id=-1;
    slot.displayed_name=new char[strlen(name)+1];
    strcpy(slot.displayed_name, name);
    m_slots.push_back(slot);
}

// search first unused button
int CInventory::SearchFreeButtonIndex(){
    for(unsigned int i=0;i<strlen(LetterButtonsList);i++){
        if(!used_buttons[i]){
            return i;
        }
    }
    return -1;
}

// serch button index in buttons array
int CInventory::GetButtonIndex(char button){
    for(unsigned int i=0;i<strlen(LetterButtonsList);i++){
        if(LetterButtonsList[i]==button){
            return i;
        }
    }
    return -1;
}

// get item index by item button
int CInventory::GetItemIndexByButton(char button){
    for(unsigned int i=0;i<m_items.size();i++){
        if(button==m_items[i].button){ // founded in items list
            return i;
        }
    }
    // not found
    return -1;
}

// add item to inventory
bool CInventory::AddItem(unsigned int item_id, int amount){
    int indx;
    if((indx=SearchFreeButtonIndex())<0){
        Log->puts("CInventory::AddItem() Error. Not found Free Button Index! Item not Added to inventory!\n");
        return false;
    }else{ // Add item
        sItem tmp;
        tmp.equip=false;
        tmp.id=item_id;
        tmp.amount=amount;
        tmp.button=LetterButtonsList[indx];
        used_buttons[indx]=true;    // set button to "used!"
        m_items.push_back(tmp);
   }
   return true;
}

// remove item from inventory
bool CInventory::RemoveItem(unsigned int list_id){
    if(m_items.size()>list_id){
        if(m_items[list_id].equip){
            Unequip(list_id);
        }
        // need free used button
        int indx=GetButtonIndex(m_items[list_id].button);
        if(indx!=-1){
            used_buttons[indx]=false;
            m_items.erase(m_items.begin()+list_id);
            return true;
        }else{
            Log->puts("CInventory::RemoveItem() Error! Not found button index! Item not removed\n");
            return false;
        }
    }else{
        MyOGL::Log->puts("Error CInventory::DelItem(%d) - wrong items list ID\n");
        return false;
    }
};

// fill m_sel_item_actions structure for current selected item
void CInventory::BuildSelectedItemAtions(){
    sItemDescription* item;
    // set all to false
    m_sel_item_actions.drop=false;
    m_sel_item_actions.eat_drink=false;
    m_sel_item_actions.equip=false;
    m_sel_item_actions.read=false;
    m_sel_item_actions.unequip=false;
    // get item from database
    item=DBItemByID(m_items[m_selected_item].id);
    // if equip
    if(m_items[m_selected_item].equip){
        m_sel_item_actions.unequip=true;
    }else{ // not equiped
        m_sel_item_actions.drop=true;
        // check equip utem types
        if(item->type==itDrink || item->type==itEat){
            m_sel_item_actions.eat_drink=true;
        }else if(item->type==itScroll || item->type==itMagicBook || item->type==itPrayerBook){
            m_sel_item_actions.read=true;
        }else if(item->type==itArmor || item->type==itBoots || item->type==itCloth || item->type==itGloves ||
            item->type==itHelm || item->type==itLightSource || item->type==itMeleeWeapon || item->type==itNeckJav ||
            item->type==itRangedWeapon || item->type==itRing || item->type==itWand ){
            m_sel_item_actions.equip=true;
        };
    }

    if(item->type==itArmor || item->type==itBoots || item->type==itCloth || item->type==itGloves ||
       item->type==itHelm || item->type==itLightSource || item->type==itMeleeWeapon || item->type==itNeckJav ||
       item->type==itRangedWeapon || item->type==itRing || item->type==itWand ){
            m_font->Print("(ESC) - выход, (e) - экипировать, (d) - выбросить.");
    }else if(item->type==itDrink || item->type==itEat){
            m_font->Print("(ESC) - выход, (E) - съесть/выпить, (d) - выбросить.");
    }else if(item->type==itMagicBook || item->type==itScroll){
            m_font->Print("(ESC) - выход, (r) - прочитать, (d) - выбросить.");
    }else{ // unknown item type
            Log->puts("Warnind: Unknown item type in CInventory:::BuildSelItemAtions()");
            //m_font->Print("(ESC) - выход, (d) - выбросить.");
    }
}

// select items by key code, if founded in inventory - return true, else - false
bool CInventory::SelectItemByKey(int key_code){
    // check inventory for key
    int indx=GetItemIndexByButton((char)key_code);
    if(indx!=-1){
        m_selected_item=indx;
        BuildSelectedItemAtions();
        return true;
    };
    Log->puts("Not found inventory item by key %c\n",key_code);
    return false;
}

// select items by key code, if founded in inventory - return true, else - false
bool CInventory::SelectItemById(int inv_id){
    // check inventory for key
    if(inv_id<(int)m_items.size()){
        m_selected_item=inv_id;
        BuildSelectedItemAtions();
        return true;
    };
    Log->puts("Not found inventory item by index %d\n",inv_id);
    return false;
}

// return empty slot id or -1 - if slot not founded
int CInventory::SearchEmptyEqSlots(eEquipSlotNames slot_position){

    for(unsigned int i=0; i<m_slots.size(); i++){
        printf("indx: %d slot %d item %d (looking for %d)\n",i,m_slots[i].slot_position, m_slots[i].item_inv_id, slot_position);
        if(m_slots[i].slot_position==slot_position && m_slots[i].item_inv_id==-1){
            return i;
        }
    }
    // not founded
    Log->puts("Not found empty slot %d\n",slot_position);
    return -1;
}

// return first slot id or -1 - if slot not founded
int CInventory::SearchFirstEqSlot(eEquipSlotNames slot_position){
    for(unsigned int i=0; i<m_slots.size(); i++){
        if(slot_position==m_slots[i].slot_position){ // founded empty slot
            return i;
        }
    }
    // not founded
    Log->puts("Not found slot %d\n",slot_position);
    return -1;
}

// get item inventory id and return slot id if item equipped or -1 if not
int CInventory::SearchSlotIdByInvItemId(int inv_id){
    for(unsigned int i=0;i<m_slots.size();i++){
        if(m_slots[i].item_inv_id==inv_id){
            return i;
        }
    }
    Log->puts("Not found item inv_id=%d in slots list\n",inv_id);
    return -1;
}

// unequip item inv_id
bool CInventory::Unequip(int inv_id){
    // search slot_id in slot
    int item_id=inv_id;
    if(inv_id==-1){
        inv_id=m_selected_item;
    }
    if(m_items[inv_id].equip){
        // search slot
        int slot=SearchSlotIdByInvItemId(inv_id);
        if(slot==-1){
            Log->puts("Warning CInventory::Unequip() - not found item in slots\n");
            return false;
        }
        m_slots[slot].item_inv_id=-1;
        m_items[inv_id].equip=false;
        if(item_id==-1){ // selected item - need update flags
            BuildSelectedItemAtions();
        }
        return true;
    }else{
        Log->puts("Warning CInventory::Unequip() - not equipped item\n");
        return false;
    }
};
        // TODO: check equipment position and unequip item
bool CInventory::Equip(int inv_id){
    int item_id=inv_id; // save inv_id
    if(inv_id!=-1){
        SelectItemById(inv_id);
        BuildSelectedItemAtions();
    }else{
        inv_id=m_selected_item;
    }

    if(!m_items[inv_id].equip){
        sItemDescription *item;
        item=DBItemByID(m_items[inv_id].id);
        int slot_id=SearchEmptyEqSlots(item->equip_slot);
        if(slot_id==-1){ // not founded empty
                // search not empty
            slot_id=SearchFirstEqSlot(item->equip_slot);
            if(slot_id==-1){
                // not found slots for this item
                char tmp[100];
                sprintf(tmp, "for item inv_id=%d id:%d (%s) not founded slot id: %d", inv_id, item->id, item->name, item->equip_slot);
                Log->puts(tmp);
                return false;
            }
            // not empty slot - unequip item
            Unequip(m_slots[slot_id].item_inv_id);
        }
        m_slots[slot_id].item_inv_id=inv_id;
        m_items[inv_id].equip=true;
        if(item_id==-1){ // rebuild selected item
            BuildSelectedItemAtions();
        }
        return true;
    }else{
        Log->puts("Item equipped before time.\n");
        return false;
    }
};


// Inventory class
void CInventory::Render(){
    // black quad

    MyOGL::Render->SetColor(0,0,0,0.8);
    MyOGL::GL.Disable(GL_TEXTURE_2D);
    MyOGL::Render->SetBlendMode(blSource);

    glBegin(GL_QUADS);
    glVertex2i(10,10);
    glVertex2i(500,10);
    glVertex2i(500,10+34*2+34*m_items.size());
    glVertex2i(10,10+34*2+34*m_items.size());
    glEnd();
    MyOGL::Render->SetBlendMode(blNone);
    MyOGL::Render->SetColor(1,1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2i(10,10);
    glVertex2i(500,10);
    glVertex2i(500,10+34*2+34*m_items.size());
    glVertex2i(10,10+34*2+34*m_items.size());
    glEnd();

    MyOGL::GL.Enable(GL_TEXTURE_2D);
    // show items list
    glTranslatef(12,20,0);
    int dy=0;
    MyOGL::Render->SetColor(0.5,0.5,0.5,1);

    char tmp[100];
    sItemDescription *item;

    // Help string
    glTranslatef(50,0,0);
    m_font->Print("Предмет");
    glTranslatef(300,0,0);
    m_font->Print("Кол-во");
    glTranslatef(80,0,0);
    m_font->Print("Вес");

    // return translate
    glTranslatef(-430,34,0);
    dy+=34;

    for(unsigned int i=0; i<m_items.size();i++){
        item=DBItemByID(m_items[i].id);
        // item ico
        if(m_item_tileset!=NULL){
            m_item_tileset->Render(item->sprite_id);    // change current color to White
        }else{
            Log->puts("Warning: not set tileset for CInventory\n");
        }
        // item name
        glTranslatef(50,0,0);
        sprintf(tmp,"%c)", m_items[i].button);
        m_font->Print(tmp); // White color
        glTranslatef(20,0,0);
        MyOGL::Render->SetColor(1.0,1.0,0.0,1.0);   // Yellow
        m_font->Print(item->name);
        glTranslatef(280,0,0);
        sprintf(tmp,"%d", m_items[i].amount);
        m_font->Print(tmp);
        // weight
        glTranslatef(80,0,0);
        sprintf(tmp,"%d", m_items[i].amount*item->weight);
        m_font->Print(tmp);

        // go to new line
        glTranslatef(-430,34,0);
        dy+=34;
        //Log->puts("print item %d\n",m_items[i].id);
    }
    // Help string
    glTranslated(10,0,0);
    MyOGL::Render->SetColor(1.0,1.0,1.0,1.0,true);   // White
    m_font->Print("ESC - выход");
    glTranslatef(-22,-20-dy,0);
}

// Show item detailed window
void CInventory::RenderItemDetail(void){
    sItemDescription* item;
    // get item from database
    item=DBItemByID(m_items[m_selected_item].id);

    // black quad

    MyOGL::Render->SetColor(0,0,0,0.8);
    MyOGL::GL.Disable(GL_TEXTURE_2D);
    MyOGL::Render->SetBlendMode(blSource);

    glBegin(GL_QUADS);
    glVertex2i(10,10);
    glVertex2i(500,10);
    glVertex2i(500,10+34*2+34*m_items.size());
    glVertex2i(10,10+34*2+34*m_items.size());
    glEnd();
    MyOGL::Render->SetBlendMode(blNone);
    MyOGL::Render->SetColor(1,1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2i(10,10);
    glVertex2i(500,10);
    glVertex2i(500,10+34*2+34*m_items.size());
    glVertex2i(10,10+34*2+34*m_items.size());
    glEnd();
    MyOGL::GL.Enable(GL_TEXTURE_2D);

    // Item Name
    glTranslatef(50,15,0);
    MyOGL::Render->SetColor(1.0,1.0,1.0,1.0);   // White
    m_font->Print(item->name);
    glTranslatef(-30,25,0);
    MyOGL::Render->SetColor(1.0,1.0,0.0,1.0);   // Yellow
    m_font->Print(item->description,470);
    Vector2i size=m_font->GetLastSizes();

    glTranslatef(0,size.height+10,0);
    MyOGL::Render->SetColor(1.0,1.0,1.0,1.0);   // White
    // TODO: show equip only for equipment items
    int dx=0, w=0;
    if(m_sel_item_actions.equip){
            w=m_font->Print("(e) - экипировать,");
            dx+=w+10;
            glTranslatef(w+10,0,0);
    }
    if(m_sel_item_actions.unequip){
            w=m_font->Print("(t) - снять,");
            dx+=w+10;
            glTranslatef(w+10,0,0);
    }
    if(m_sel_item_actions.eat_drink){
            w=m_font->Print("(E) - съесть/выпить,");
            dx+=w+10;
            glTranslatef(w+10,0,0);
    }
    if(m_sel_item_actions.read){
            w=m_font->Print("(r) - прочитать,");
            dx+=w+10;
            glTranslatef(w+10,0,0);
    }
    if(m_sel_item_actions.drop){
            w=m_font->Print("(d) - выбросить,");
            dx+=w+10;
            glTranslatef(w+10,0,0);
    }
    m_font->Print("(ESC) - выход.");
    // return translate
    glTranslatef(-20-dx,-size.height-50,0);
    return;
}

// render equipped items
void CInventory::RenderEquipped(void){
    sItemDescription* item;
    for(unsigned int i=0;i<m_slots.size();i++){
        if(m_slots[i].item_inv_id!=-1){ // found item - render
            // get item from database
            item=DBItemByID(m_items[m_slots[i].item_inv_id].id);
            // render item sprite
            m_item_tileset->Render(item->sprite_id+1);
        }
    }
}
