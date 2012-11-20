#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED

#include "items.h"
#include "myogl/sprites.h"

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

// humanoid slots
enum eWearSlots{
    slHead,
    slNeck,
    slBody,
    slBody2,
    slLArm,
    slRArm,
    slLegs,
    slLegs2,
    slRing1,
    slRing2
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
    CHudSprite *m_sprite;
    public:
        CCreature(){ m_sprite=NULL; };
        ~CCreature(){};
        void SetPosition(unsigned int x, unsigned int y){ m_x=x; m_y=y; };
        unsigned int GetPosX(){ return m_x; };
        unsigned int GetPosY(){ return m_y; };
        void SetSprite(CHudSprite *sprite){ m_sprite=sprite; };
        void Render(){ if(m_sprite!=0) m_sprite->Render();};
};

#endif // CREATURE_H_INCLUDED
