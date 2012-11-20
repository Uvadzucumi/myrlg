#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

enum eItemTypes{
    itNone=0,
    itLightSource,
    itEat,
    itDrink,
    itScroll,
    itMeleeWeapon,
    itRangedWeapon,
    itWand,
    itCloth,
    itCloth2,
    itTource,
    itBoots,
    itHat,
    itNekoJav,
    itRing,
    itMagicBook,
    itPrayeBook
};

class CItem{
    unsigned char m_item_type;
    char *m_item_name;
};


#endif // ITEMS_H_INCLUDED
