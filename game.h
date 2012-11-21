#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "myogl/myogl.h"
#include "myogl/bitmap_font.h"
#include "myogl/vector_types.h" // vector type size
#include "myogl/entity.h"   // for animation
#include "myogl/material.h"

#include "dungeon_map.h"  // dungeon map generation/render
#include "creature.h"       // for player
#include "myogl/config_file.h"  // for read confin

extern MyOGL::CMaterial LightMaterials[10];  // materials for light sources

extern MyOGL::CApplication *App;   // need delete
extern MyOGL::CTexture *font_texture, *tiles_texture;    // no need delete. deleted automaticly from MyOGL::TexturesList
//MyOGL::CEntity* Entity[3];                                  // no need delete. deleted automaticly from MyOGL::EntityList (2D actor)
extern MyOGL::CFont *font; // need delete
extern MyOGL::CText *text; // need delete

extern CTileset *Tileset;
extern CHerro *herro;

extern CDungeonLevel *dungeon;

extern CConfigFile *cfg_file;

#endif // GAME_H_INCLUDED
