#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include "items.h"
#include "myogl/sprites.h"
#include "dungeon_map.h"   // for dungeon

enum eDirections{
    dNone=0,
    dNorth,
    dEast,
    dWest,
    dSouth,
    dDirectionsCount
};

enum eCreatureRaces{
    crHuman,
    crDwarf,
    crOrk,
    crElf
};

enum eCreatureClass{
    ccWarrior,
    ccMage,
    ccPriest,
    ccPaladin
};

class CCreature{
// attributes
    unsigned char m_str;
    unsigned char m_con;
    unsigned char m_int;
    unsigned char m_wis;
    unsigned char m_agi;
    unsigned char m_dex;
// race & class
    unsigned char m_race;
    unsigned char m_class;
// parameters
    unsigned char m_speed;
    unsigned int m_health, m_max_health;
    unsigned int m_mana, m_max_mana;
// map position
    unsigned int m_x, m_y;
//  std::vector <CItem> *inventory;
    MyOGL::CHudSprite *m_sprite;
    Vector2i GetMapCoords(eDirections direction);

    public:
        CCreature(){ m_sprite=NULL; };
        ~CCreature(){};
        void SetPosition(unsigned int x, unsigned int y){ m_x=x; m_y=y; };
        unsigned int GetPosX(){ return m_x; };
        unsigned int GetPosY(){ return m_y; };
        void SetSprite(MyOGL::CHudSprite *sprite){ m_sprite=sprite; };
        void Render(){ if(m_sprite!=0) m_sprite->Render();};
        bool Move(int dx, int dy, CDungeonLevel *dungeon);
        bool OpenDoor(CDungeonLevel *dungeon, eDirections direction=dNone);
};

// herro class
class CHerro : public CCreature{
        unsigned int m_last_mov_tick;   // for movement delay
        public:
            CInventory *inventory;
            unsigned int movement_delay;    // only for herro
            CHerro(){
                m_last_mov_tick=0;
                movement_delay=10;
                inventory=new CInventory;
            };
            ~CHerro(){
                delete inventory;
            }
            bool Move(int dx, int dy, CDungeonLevel *dungeon);
            void Render();

};


#endif // CREATURE_H_INCLUDED
