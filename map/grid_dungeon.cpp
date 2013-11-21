#include <stdlib.h> // for rand
#include "grid_dungeon.h"

// return random door type
// 10% - hidden, 10% - locked, 10% - opened, 10% - broken
// 60% - closed (not locked)
void *CDungeonGeneration::RandomDoorType(){

    sTileDataDoor *data; // pointer for door data
    int tmp=rand()%100;

    data=new sTileDataDoor();

// set default door
    data->strength=rand()%100;   // for broken and found (if hidden)
    data->broken=false;
    data->opened=false;
    data->locked=false;
    data->hidden=false;
    // 10% - broken doors
// change parameters
    if(tmp>=90){ // 10% - broken doors
        data->broken=true;
    }else{
        if(tmp<=10){ // 10% - opened
            data->opened=true;
        }else{
            if(tmp<=30){    // 10% (30-20)- locked door
                data->locked=true;
            }
            // set 5% closed doors to hidden
            tmp=rand()%100;
            if(tmp<5){
                data->hidden=true;
            }
        }
    }

    return (void *)data;
}

void CDungeonGeneration::AddDoors(unsigned int room_id){
    int i;
    // vertical walls
    for(i=m_rooms[room_id].top;i<m_rooms[room_id].top+m_rooms[room_id].height;i++){
        // left wall
        if(m_Map->GetTileType(m_rooms[room_id].left-1,i)==ttCoridor){
            // check for artefact door
            if( m_Map->GetTileType(m_rooms[room_id].left-2,i)==ttWall &&
                m_Map->GetTileType(m_rooms[room_id].left-1,i-1)==ttWall &&
                m_Map->GetTileType(m_rooms[room_id].left-1,i+1)==ttWall){
                    m_Map->AddMapTile(ttWall,m_rooms[room_id].left-1,i);
            }else{
                // create random door
                    m_Map->AddMapTile(ttDoor,m_rooms[room_id].left-1,i,RandomDoorType());
            }
        }

        //right
        if(m_Map->GetTileType(m_rooms[room_id].left+m_rooms[room_id].width,i)==ttCoridor){
            if( m_Map->GetTileType(m_rooms[room_id].left+m_rooms[room_id].width+1,i)==ttWall &&
                m_Map->GetTileType(m_rooms[room_id].left+m_rooms[room_id].width,i-1)==ttWall &&
                m_Map->GetTileType(m_rooms[room_id].left+m_rooms[room_id].width,i+1)==ttWall){
                    m_Map->AddMapTile(ttWall, m_rooms[room_id].left+m_rooms[room_id].width, i);
            }else{
                m_Map->AddMapTile(ttDoor, m_rooms[room_id].left+m_rooms[room_id].width,i,RandomDoorType());
            }
        }
    }
    // horisontal walls
    for(i=m_rooms[room_id].left;i<m_rooms[room_id].left+m_rooms[room_id].width;i++){
        // top wall
        if(m_Map->GetTileType(i, m_rooms[room_id].top-1)==ttCoridor){
            if( m_Map->GetTileType(i, m_rooms[room_id].top-2)==ttWall &&
                m_Map->GetTileType(i+1, m_rooms[room_id].top-1)==ttWall &&
                m_Map->GetTileType(i-1, m_rooms[room_id].top-1)==ttWall){
                    m_Map->AddMapTile(ttWall,i,m_rooms[room_id].top-1);
            }else{
                m_Map->AddMapTile(ttDoor,i,m_rooms[room_id].top-1,RandomDoorType());
            }
        }
        // bottom
        if(m_Map->GetTileType(i, m_rooms[room_id].top+m_rooms[room_id].height)==ttCoridor){
            if(m_Map->GetTileType(i, m_rooms[room_id].top+m_rooms[room_id].height+1)==ttWall &&
               m_Map->GetTileType(i+1, m_rooms[room_id].top+m_rooms[room_id].height)==ttWall &&
               m_Map->GetTileType(i-1, m_rooms[room_id].top+m_rooms[room_id].height)==ttWall){
                   m_Map->AddMapTile(ttWall,i, m_rooms[room_id].top+m_rooms[room_id].height);
            }else{
                m_Map->AddMapTile(ttDoor,i,m_rooms[room_id].top+m_rooms[room_id].height,RandomDoorType());
            }
        }
    }
}

// add room room_id to map array
void CDungeonGeneration::RoomToMap(unsigned int room_id){
    for(int y = m_rooms[room_id].top; y < m_rooms[room_id].top + m_rooms[room_id].height; y++){
        for(int x = m_rooms[room_id].left; x < m_rooms[room_id].left + m_rooms[room_id].width; x++){
            m_Map->AddMapTile(ttRoom,x,y);
        }
    }
}

// add coridor in map array from start to end point
void CDungeonGeneration::CreateCoridor(unsigned int start_room_id, unsigned int end_room_id){
    //
    //char logstr[100];
    unsigned int curr_x, curr_y;
    int dx, dy;

    unsigned int start_x=m_rooms[start_room_id].left+(int)m_rooms[start_room_id].width/2;
    unsigned int end_x=m_rooms[end_room_id].left+(int)m_rooms[end_room_id].width/2;
    unsigned int start_y=m_rooms[start_room_id].top+(int)m_rooms[start_room_id].height/2;
    unsigned int end_y=m_rooms[end_room_id].top+(int)m_rooms[end_room_id].height/2;

    if(start_x<end_x){ dx=1; }else{ dx=-1;}
    if(start_y<end_y){ dy=1; }else{ dy=-1;}
    //sprintf(logstr,"Coridor [%d,%d] - [%d,%d] dx: %d dy: %d\n", start_x,start_y, end_x, end_y, dx, dy);
    //Log->puts(logstr);

    // set connected!

    // horisontal
    bool cor_near_room=false;
    for(curr_x=start_x, curr_y=start_y;curr_x!=end_x;curr_x+=dx){
        if(m_Map->GetTileType(curr_x, curr_y)==ttWall){
            if(!cor_near_room){
                m_Map->AddMapTile(ttCoridor,curr_x,curr_y);
            }
            // check top and bottom. if founded room - resize room
            if(m_Map->GetTileType(curr_x,curr_y-1)==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x,curr_y-1);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            if(m_Map->GetTileType(curr_x, curr_y+1)==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x,curr_y+1);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            // check - if room finished - need add coridor in previous and current location
            if(cor_near_room){
                m_Map->AddMapTile(ttCoridor,curr_x-dx,curr_y);
                m_Map->AddMapTile(ttCoridor,curr_x,curr_y);
                cor_near_room=false;
            }
        }else{
            if(m_Map->GetTileType(curr_x, curr_y)==ttRoom){
                int room_id=GetRoomIdByCoords(curr_x,curr_y);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                }
            }
        }
    }
    // vertical
    cor_near_room=false;
    for(;curr_y!=end_y;curr_y+=dy){
        if(m_Map->GetTileType(curr_x, curr_y)==ttWall){
            if(!cor_near_room){
                m_Map->AddMapTile(ttCoridor,curr_x,curr_y);
            }
            if(m_Map->GetTileType(curr_x-1, curr_y)==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x-1,curr_y);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            if(m_Map->GetTileType(curr_x+1, curr_y)==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x+1,curr_y);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            // if room finished - add coridor in previous and current location
            if(cor_near_room){
                m_Map->AddMapTile(ttCoridor, curr_x, curr_y-dy);
                m_Map->AddMapTile(ttCoridor, curr_x, curr_y);
                cor_near_room=false;
            }
        }else{
            if(m_Map->GetTileType(curr_x, curr_y)==ttRoom){
                int room_id=GetRoomIdByCoords(curr_x,curr_y);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                }
            }
        }
    }
    m_rooms[start_room_id].connected=true;
}

// fill roms array and add rooms in map
void CDungeonGeneration::CreateRoom(sroom *room, unsigned int grid_x, unsigned int grid_y){
    // set room left, top, width, height
    unsigned int grid_size_w=(int)(m_width/m_grid_w);
    unsigned int grid_size_h=(int)(m_height/m_grid_h);
    room->width=rand()%(int)(grid_size_w-2-m_room_min_w)+m_room_min_w;
    room->height=rand()%(int)(grid_size_h-2-m_room_min_h)+m_room_min_h;
    room->left=rand()%(grid_size_w-room->width-1)+1+grid_x*grid_size_w;
    room->top=rand()%(grid_size_h-room->height-1)+1+grid_y*grid_size_h;
    room->connected=false;
    //char logstr[100];
    //sprintf(logstr, "room: l:%d t:%d w: %d h:%d right_bottom: [%d,%d]\n", room->left, room->top, room->width, room->height, room->left+room->width, room->top+room->height);
    //Log->puts(logstr);
};


void CDungeonGeneration::Generate(CLevelMap *Map){
    int x,y,i;
    // fill level to wall block

    m_Map=Map;
    m_width=m_Map->GetWidth();
    m_height=m_Map->GetHeight();

    for(y=0; y<m_height; y++){
        for(x=0; x<m_width; x++){
            m_Map->AddMapTile(ttWall,x,y);
            //m_Map->GetMap()[x][y].viewed=false;
            m_Map->SetViewed(x,y,false);
        }
    }

    // clear grid array
    for(y=0; y<m_grid_h; y++){
        for(x=0;x<m_grid_w;x++){
            m_grid[x][y].filled=false;
        }
    }

    // create rooms
    for(i=0; i<m_rooms_num; i++){
        // search empty grid field
        do{
            x=rand()%m_grid_w;
            y=rand()%m_grid_h;
        }while(m_grid[x][y].filled);
        // create room
        CreateRoom(&m_rooms[i],x,y);
        RoomToMap(i);
        m_grid[x][y].filled=true;
        m_grid[x][y].room_id=i;
        // add light source for room center
        m_Map->AddBonfire(m_rooms[i].left+(int)m_rooms[i].width/2,m_rooms[i].top+(int)m_rooms[i].height/2,9);
    }
    m_rooms[0].connected=true;

    // create coridors
    for(i=1;i<m_rooms_num;i++){
        CreateCoridor(i,i-1);
    }
    // add doors for room
    for(i=0;i<m_rooms_num;i++){
        AddDoors(i);
    }

    m_Map->SetStartPosition(rand()%m_rooms[0].width+m_rooms[0].left, rand()%m_rooms[0].height+m_rooms[0].top);

    m_Map->CalculateAllLights();

};

int CDungeonGeneration::GetRoomIdByCoords(unsigned int x, unsigned int y){
    // get grid x and y
    int grid_x=(int)(x/(m_width/m_grid_w));
    int grid_y=(int)(y/(m_height/m_grid_h));
    return m_grid[grid_x][grid_y].room_id;
}
