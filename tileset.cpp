#include "tileset.h"

CTileset::CTileset(CTexture *texture, int tile_x_size, int tile_y_size){
    CHudSprite *sprite;
    int i,j;
    if(!texture){
        Log->puts("CTiles::CTiles() - empty texture\n");
    }

    tileset_texture=texture;
    for(i=0;i<texture->GetHeight();i+=tile_y_size){
        for(j=0;j<texture->GetWidth();j+=tile_x_size){
            sprite=new CHudSprite(texture);
            TilesList.push_back(sprite);
            sprite->SetUVPixelCoords(j,i,tile_x_size,tile_y_size);
            sprite->SetSize(tile_x_size,tile_y_size);
        }
    }
    Log->printf("Created %d tiles tileset\n",TilesList.size());
};

CTileset::~CTileset(){
    Log->puts("Clear TilesList\n");
    for(unsigned int i=0;i<TilesList.size();i++){
        delete TilesList[i];
    }
    TilesList.clear();
};

// render sprite in special coords
void CTileset::RenderAt(int x, int y, int tile_id, CMaterial *material){
    glTranslatef(x,y,0);
    //TilesList[tile_id]->SetMaterial(material);
    Render(tile_id, material);
    glTranslatef(-x,-y,0);
};

void CTileset::Render(int tile_id, CMaterial *material){
    TilesList[tile_id]->Render(material);
};
