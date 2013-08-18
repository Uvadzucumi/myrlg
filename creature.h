#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include "inventory.h"
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
    private:
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

//  std::vector <CItem> *inventory;
    MyOGL::CHudSprite *m_sprite;
    Vector2i GetMapCoords(eDirections direction);
    protected:
        int m_x, m_y;
    public:
        CCreature(){ m_sprite=NULL; };
        ~CCreature(){};
        void SetPosition(unsigned int x, unsigned int y){ m_x=x; m_y=y; };
        unsigned int GetPosX(){ return m_x; };
        unsigned int GetPosY(){ return m_y; };
        void SetSprite(MyOGL::CHudSprite *sprite){ m_sprite=sprite; };
        void Render(){ if(m_sprite) m_sprite->Render();};
        bool Move(int dx, int dy, CDungeonLevel *dungeon);
        bool OpenDoor(CDungeonLevel *dungeon, eDirections direction=dNone);
};

// herro class
class CHero : public CCreature{
            unsigned int m_last_mov_tick;   // for movement delay
            unsigned int m_movement_delay;    // only for herro
            bool m_autoopen_doors;
        public:
            CInventory *inventory;
            CHero(){
                m_last_mov_tick=0;
                m_movement_delay=10;
                m_autoopen_doors=false;
                inventory=new CInventory;
            };
            ~CHero(){
                delete inventory;
            }
            void SetMovementDelay(unsigned int delay){ m_movement_delay=delay;};
            void SetAutoopenDoors(bool auto_open){ m_autoopen_doors=auto_open;}
            void Render();
            // Commands
            void PikUp(CDungeonLevel *dungeon);
            void Drop(CDungeonLevel *dungeon);
            bool Move(int dx, int dy, CDungeonLevel *dungeon);

};


#endif // CREATURE_H_INCLUDED
