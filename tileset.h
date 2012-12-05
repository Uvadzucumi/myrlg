#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include "myogl/texture.h"
#include "myogl/sprites.h"
#include "myogl/material.h"

using namespace MyOGL;

// tiles names in tilest
enum eTileNames{
    tnWall=0,
    tnFloorDungeonRoom=16,
    tnFloorDungeonCoridor,
    tnDoorClosedDungeon=32,
    tnDoorOpenedDungeon,
    tnDoorBrokenDungeon,
    tnFire=48,
    tnHerro=64,
    tnDagger=80,
    tnSword=82,
    tnSpear=84,
    tnStaff=86,
    tnMace=88,
    tnMeat,
    tnBread,
    tnBootleEmpty,
    tnBootleWater,
    tnBootleBeer
};


/*
enum eWorldSide{
    sNorth=0,
    sEast,
    sSouth,
    sWest
};
*/

// tileset class,
class CTileset{
        CTexture *tileset_texture;
        std::vector<CHudSprite*> TilesList;
    public:
        CTileset(CTexture *texture, int tile_x_size=32, int tile_y_size=32);
        ~CTileset();
        // get current tile sprite
        CHudSprite *Tile(unsigned int tile_num){
            if(tile_num<TilesList.size()){
                return TilesList[tile_num];
            }else{
                MyOGL::Log->puts("CTileset::Tile(num) Warning! wrong tile number %d\n",tile_num);
                return TilesList[0];
            }
        };
        void RenderAt(int x, int y, int tile_id, CMaterial *material=NULL);
        void Render(int tile_id, CMaterial *material=NULL);
};

#endif // TILES_LANDSCAPE_H_INCLUDED
