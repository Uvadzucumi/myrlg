#ifndef MAP_TYPES_H_INCLUDED
#define MAP_TYPES_H_INCLUDED

// tile types
enum eTileTypes{
    ttNone=0,
    ttWall,
    ttCoridor,
    ttRoom,
    ttDoor,/*
    ttDoorClosed, // closed door
    ttDoorOpened,
    ttDoorBroken,
    ttDoorHidden,
    ttDoorLocked,*/
    ttContainer,
    ttStairs,
    ttFire
};

#endif // MAP_TYPES_H_INCLUDED
