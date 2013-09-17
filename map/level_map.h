#ifndef LEVEL_MAP_H_INCLUDED
#define LEVEL_MAP_H_INCLUDED

#include "../myogl/vector_types.h"
#include "../myogl/log.h"
#include "../myogl/animation.h"

//#include <stdlib.h> // NULL, rand
#include "../tileset.h" // tile names - for addTile

#include "../items.h"

#include "fov.h"
#include "light_source.h"

#include <math.h> // sqrt

#include "map_types.h"

const char* GetTileNameByType(eTileTypes tile);

// tiles data
typedef struct{
    unsigned char strength;
    bool broken;
    bool opened;
    bool locked;
    bool hidden;
}sTileDataDoor;

typedef struct{
    eTileTypes tile_type;
    short layer[3];
    //short light;
    bool viewed;
    //bool visible;
    bool skip_light;
    bool can_move;  // TODO: move_speed, if 0 - cannot move
    void *p_tile_data;
    void *p_monsters;
    //void *p_items;
    CItemsContainer items;
}sMapField;


class CMapDynamicTile{
        CAnimation *m_animation;
        std::vector<unsigned char> TileNamesList;
    public:
        MyOGL::Vector2i position;
        int layer;
        CMapDynamicTile(){
            m_animation=new CAnimation;
        };
        ~CMapDynamicTile(){
            delete m_animation;
            TileNamesList.clear();
        };
        void AddTiles(unsigned char start_sprite_id, unsigned char sprites_count=1){
            for(unsigned char i=0; i < sprites_count; i++){
                TileNamesList.push_back(start_sprite_id+i);
            }
            m_animation->FramesCount=TileNamesList.size();
        }
        // return true if frame changed
        bool Update(double DeltaTime){
            return m_animation->OnAnimate(DeltaTime);
        };
        short GetCurrentTile(){ return TileNamesList[m_animation->GetCurrentFrame()]; };
        int GetCurrentFrame(){ return m_animation->GetCurrentFrame(); };
        void SetAnimationOscillate(bool Oscillate){ m_animation->Oscillate=Oscillate; }
        void SetAnimationRate(double Rate){ m_animation->SetFrameRate(Rate); };
};

class CLevelMap{
        // map data
        int m_width, m_height;
        sMapField **m_Map;
        std::vector <CMapDynamicTile*> DynamicTilesList;
        std::vector <CMapLightSource*> LightSourcesList;
        std::vector <int> LightsNearFOV; // lights index near FOV
        // map player position
        MyOGL::Vector2i m_UnitPosition;
        bool IsWall(int x, int y, bool check_viewed=true);  // return true if wall tile type, walls, doors, windows etc...
        sTileDataDoor GetDoorData(int x, int y); // return door data from map coords x, y
    public:
        CLevelMap(int Width, int Height){

            m_width=Width; m_height=Height;

            m_Map = new sMapField *[m_width];
            for (int i = 0; i < m_width; i++){
                m_Map[i] = new sMapField[m_height];
                for(int j=0; j < m_height; j++){
                    m_Map[i][j].p_tile_data=NULL;
                    m_Map[i][j].p_monsters=NULL;
                }
            }
        };

        ~CLevelMap(){
            // clear map data
            for (int i = 0; i < m_width; i++){
                for(int j=0; j < m_height; j++){
                    if(m_Map[i][j].p_tile_data!=NULL){
                        switch(m_Map[i][j].tile_type){
                            case ttDoor:
                                if(m_Map[i][j].p_tile_data){
                                    delete ((sTileDataDoor *)m_Map[i][j].p_tile_data);
                                }
                                break;
                            default:
                                MyOGL::Log->printf("WARNING! not implementd delete *p_tile_data for tile_type %d!\n",m_Map[i][j].tile_type);
                                break;
                        }
                    }
                }
                delete [] m_Map[i];
            }
            // clear lights sources
            for(unsigned int i=0;i<LightSourcesList.size();i++){
                delete LightSourcesList[i];
            }
            LightSourcesList.clear();
            // clear animated tiles
            for(unsigned int i=0;i<DynamicTilesList.size();i++){
                delete DynamicTilesList[i];
            }
            DynamicTilesList.clear();
        };
        void CalculateAllLights();  // recalculate all light
        void CalculateNearesLights(); // recalculate lights neares hero FOV
        void LandPostprocessing(CFOV *fov, int x, int y);


        bool LineOfSight(int x1, int y1, int x2, int y2); // line on sight between x1,y1 and x2,y2
        void AddMapTile(eTileTypes TileType, int x, int y, void *tile_data=NULL);
        void AddBonfire(unsigned int x, unsigned int y, unsigned char strength);
        void Update(CFOV *fov, double DeltaTime);
        int GetWidth(){ return m_width; }
        int GetHeight() { return m_height; }

        //sMapField **GetMap(){ return m_Map; };
        //bool IsVisible(int x, int y){ return m_Map[x][y].visible; };
        bool IsSkipLight(int x, int y){ return m_Map[x][y].skip_light; };
        bool IsViewed(int x, int y){ return m_Map[x][y].viewed; };
        bool IsCanMove(int x, int y){ return m_Map[x][y].can_move; }; // TODO: add check big items, mobs - etc?
        bool IsDoor(int x, int y);
        bool IsDoorClosed(int x, int y);
        bool OpenDoor(int x, int y);
        bool CloseDoor(int x, int y);
        void SetViewed(int x, int y, bool flag){
            if(x<0 || y< 0 || x>=m_width || y>=m_height){
                MyOGL::Log->printf("RANGE ERROR CLevelMap::SetViewed(): x: %d y: %d\n",x,y);
                return;
            }
            m_Map[x][y].viewed=flag;
        }
        CItemsContainer* GetItemsInField(int x, int y){ return &m_Map[x][y].items; };   // return pointer to items list in field x, y

        MyOGL::Vector2i GetStartPosition() { return m_UnitPosition; };
        void SetStartPosition(int x, int y){ m_UnitPosition.x=x; m_UnitPosition.y=y; };
        eTileTypes GetTileType(int x, int y){ return m_Map[x][y].tile_type;}
        int GetTileID(int x, int y, int layer){
            return m_Map[x][y].layer[layer];
        }
        sMapField GetMapFiled(int x, int y){
            if(x>=0 && x<m_width && y>=0 && y<m_height){
                return m_Map[x][y];
            }
            // error return error fild
            sMapField error_fld;
            error_fld.tile_type=ttNone;
            return error_fld;
        }
        // apply light from light sources to FOV & create lights near FOV list
        void CalculateMapLight(CFOV *fov);
};

#endif // LEVEL_MAP_H_INCLUDED
