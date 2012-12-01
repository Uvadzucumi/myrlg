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
    itTrousers,
    itBoots,
    itHat,
    itNeckJav,
    itRing,
    itMagicBook,
    itPrayerBook
};

class CItem{
    unsigned char m_item_type;
    char *m_item_name;
    bool m_examined;
    void *m_obj_data;
};


#endif // ITEMS_H_INCLUDED
