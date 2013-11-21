#ifndef TILES_DESCR_H_INCLUDED
#define TILES_DESCR_H_INCLUDED

#include "myogl/tileset.h"

using namespace MyOGL;

// tiles names in tilest
enum eTileNames{
    tnWall=0,
    tnFloorDungeonRoom=16,
    tnFloorDungeonCoridor,
    tnDoorClosedDungeon=32,
    tnDoorOpenedDungeon,
    tnDoorBrokenDungeon,
    tnBonfire=48,
    tnHerro=64,
// Prison walls
    tnWallVertical=128,
    tnWallLeftTop,
    tnWallRightTop,
    tnWallVerticalRight,
    tnWallVerticalLeft,
    // door vertical
    tnDoorVerticalClosed,
    tnDoorVerticalOpened,
    tnDoorVerticalBroken,
    tnWallEndUp,
    tnWallEndRight,
    tnWallLeftTopFillInside,
    tnWallRightTopFillInside,
    tnWallVerticalFillRight,
    tnWallVerticalFillLeft,
    tnWallLeftTopFillOutside,
    tnWallRightTopFillOutside,
// 144
    tnWallHorizontal,
    tnWallLeftBottom,
    tnWallRightBottom,
    tnWallHorizontalUp,
    tnWallHorizontalDown,
    // door horizontal
    tnDoorHorizontalClosed,
    tnDoorHorizontalOpened,
    tnDoorHorizontalBroken,
    tnWallEndDown,
    tnWallEndLeft,
    tnWallLeftBottomFillInside,
    tnWallRightBottomFillInside,
    tnWallHorizontalFillDown,
    tnWallHorizontalFillUp,
    tnWallLeftBottomFillOutside,
    tnWallRightBottomFillOutside,

    tnWallHorizontalDownFillSW,
    tnWallHorizontalDownFillSE,
    tnWallHorizontalDownFillN,

    tnWallHorizontalUpFillNW,
    tnWallHorizontalUpFillNE,
    tnWallHorizontalUpFillS,

    tnWallVerticalLeftFillNW,
    tnWallVerticalLeftFillSW,
    tnWallVerticalLeftFillE,

    tnWallVerticalRightFillNE,
    tnWallVerticalRightFillSE,
    tnWallVerticalRightFillW,

    tnWallAlone,
// Weapons
    tnDagger=80,
    tnSword=82,
    tnSpear=84,
    tnStaff=86,
    tnMace=88,
// Items
    tnBread=96,
    tnSlime,
    tnMeat,
    tnBootleEmpty,
    tnBootleWater,
    tnBootleBeer,
// Potions
    tnPotionAqua=112,
    tnPotionBlack,
    tnPotionBlue,
    tnPotionFuchsia,
    tnPotionGray,
    tnPotionGreen,
    tnPotionLime,
    tnPotionMaroon,
    tnPotionNavy,
    tnPotionOlive,
    tnPotionPurple,
    tnPotionRed,
    tnPotionSilver,
    tnPotionTeal,
    tnPotionWhite,
    tnPotionYellow
};

#endif // TILES_DESCR_H_INCLUDED
