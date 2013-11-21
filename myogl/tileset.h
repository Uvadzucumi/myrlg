#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include "sprites.h"
#include "material.h"

namespace MyOGL{

// tileset class,
class CTileset{
        CTexture *m_tileset_texture;
        std::vector<CHudSprite*> m_tiles;
    public:
        CTileset(CTexture *texture, int tile_x_size=32, int tile_y_size=32);
        ~CTileset();
        // get current tile sprite
        CHudSprite *Tile(unsigned int tile_num){
            if(tile_num<m_tiles.size()){
                return m_tiles[tile_num];
            }else{
                MyOGL::Log->printf("CTileset::Tile(num) Warning! wrong tile number %d\n",tile_num);
                return m_tiles[0];
            }
        };
        void RenderAt(int x, int y, int tile_id, CMaterial *material=NULL);
        void Render(int tile_id, CMaterial *material=NULL);
};

}

#endif // TILES_LANDSCAPE_H_INCLUDED

