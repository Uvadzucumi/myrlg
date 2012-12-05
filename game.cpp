
#include "game.h"

MyOGL::CMaterial LightMaterials[10];  // materials for light sources
MyOGL::CMaterial TestMaterial;

MyOGL::CApplication *App;   // need delete
MyOGL::CTexture *font_texture, *tiles_texture;    // no need delete. deleted automaticly from MyOGL::TexturesList
//MyOGL::CEntity* Entity[3];                                  // no need delete. deleted automaticly from MyOGL::EntityList (2D actor)
MyOGL::CFont *font; // need delete
MyOGL::CText *text; // need delete

CTileset *Tileset;

// Player
CHerro *herro;
// Dungeon level
CDungeonLevel *dungeon;

// config file class
CConfigFile *cfg_file;

// Current Active game Window
eGameWindow ActiveWindow;
