#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

#include "../tileset.h"
#include "../myogl/animation.h"
#include "level_map.h"

typedef struct{
    bool filled;
    unsigned int room_id;
} grid_field;

typedef struct{
    int left,top,width,height; // room position & size
    bool connected; // connected to first room
} sroom;

class CDungeonGeneration{

        int m_grid_w, m_grid_h;
        grid_field **m_grid;

        int m_rooms_num;
        sroom *m_rooms;
        unsigned int m_room_min_w, m_room_min_h;

        int m_width, m_height;  // map width & height
        CLevelMap *m_Map;       // map object

        int GetRoomIdByCoords(unsigned int x, unsigned int y);
        void CreateRoom(sroom *sroom, unsigned int grid_x, unsigned int grid_y);
        void RoomToMap(unsigned int room_id);
        void CreateCoridor(unsigned int start_room_id, unsigned int end_room_id);
        void AddDoors(unsigned int room_id);
        void *RandomDoorType();    // return random door type

    public:
        CDungeonGeneration(int grid_width, int grid_height, int rooms_num){
            m_width=0; m_height=0;  // map width & height
            // create grid array
            m_grid_w=grid_width; m_grid_h=grid_height;
            m_grid=new grid_field *[m_grid_w];
            for(int i=0;i<m_grid_w;i++){
                m_grid[i] = new grid_field [m_grid_h];
            }
            // create rooms array & rooms parameters
            m_rooms_num=rooms_num;
            m_rooms = new sroom [m_rooms_num];
            m_room_min_w=3;
            m_room_min_h=3;
        };

        ~CDungeonGeneration(){
            // clear grid array
            for(int i=0;i<m_grid_w;i++) delete m_grid[i];
            delete m_grid;
            // clear room array
            delete m_rooms;
        };

        void Generate(CLevelMap *Map);

};

#endif // DUNGEON_H_INCLUDED
