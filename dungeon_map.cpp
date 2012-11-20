#include "dungeon_map.h"

#include <math.h>
#include "game.h"

int CDungeonLevel::GetRoomIdByCoords(unsigned int x, unsigned int y){
    // get grid x and y
    int grid_x=(int)(x/(m_width/m_grid_w));
    int grid_y=(int)(y/(m_height/m_grid_h));
    return m_grid[grid_x][grid_y].room_id;
}

void CDungeonLevel::AddMapTile(eTileTypes TileType, unsigned int x, unsigned int y){
    int tmp;
    void *data;
    m_Map[x][y].tile_type=TileType;
    m_Map[x][y].layer[1]=255;
    m_Map[x][y].layer[2]=255;
    switch(TileType){
        case ttDoor: // create door tile
            // create door data
            data=new sTileDataDoor();
            m_Map[x][y].skip_light=false;
            m_Map[x][y].can_move=true;
            //((sTileDataDoor *)data)->hidden=true;
            //(sTileDataDoor)(*p)->locked=100;
//            p->strength=rand()%100;
            tmp=rand()%100;
            if(tmp<=20){
                ((sTileDataDoor *)data)->locked=true;
                ((sTileDataDoor *)data)->opened=false;
            }else{
                ((sTileDataDoor *)data)->locked=false;
                if(tmp<=50){
                    ((sTileDataDoor *)data)->opened=false;
                }else{
                    ((sTileDataDoor *)data)->opened=true;
                }
            }
            if(tmp>90){ ((sTileDataDoor *)data)->broken=true; }else{ ((sTileDataDoor *)data)->broken=false; }
            if(tmp<=5){ ((sTileDataDoor *)data)->hidden=true; }else{ ((sTileDataDoor *)data)->hidden=false; }
            //m_Map[x][y].p_tile_data=door;
            if(((sTileDataDoor *)data)->hidden){
                m_Map[x][y].layer[0]=tnWall;
                m_Map[x][y].can_move=false;
            }else{
                m_Map[x][y].layer[0]=tnFloorDungeonCoridor;
                if(((sTileDataDoor *)data)->broken){
                    m_Map[x][y].skip_light=true;
                    m_Map[x][y].layer[1]=tnDoorBrokenDungeon;
                }else if(((sTileDataDoor *)data)->opened){
                    m_Map[x][y].skip_light=true;
                    m_Map[x][y].layer[1]=tnDoorOpenedDungeon;
                }else{
                    m_Map[x][y].can_move=false;
                    m_Map[x][y].layer[1]=tnDoorClosedDungeon;
                }
            }
            //delete ((sTileDataDoor *)data);
            break;
        case ttFire: // create fire tile
            m_Map[x][y].layer[0]=tnFloorDungeonRoom;
            m_Map[x][y].layer[1]=tnFire;
            m_Map[x][y].skip_light=true;
            m_Map[x][y].can_move=true;
            break;
        case ttCoridor:
            m_Map[x][y].layer[0]=tnFloorDungeonCoridor;
            m_Map[x][y].skip_light=true;
            m_Map[x][y].can_move=true;
            break;
        case ttRoom:
            m_Map[x][y].layer[0]=tnFloorDungeonRoom;
            m_Map[x][y].skip_light=true;
            m_Map[x][y].can_move=true;
            break;
        case ttWall:
            m_Map[x][y].layer[0]=tnWall;
            m_Map[x][y].skip_light=false;
            m_Map[x][y].can_move=false;
            break;
        default:
            MyOGL::Log->puts("Warning: unknow AddMapTile(TileType) = [%d]\n",(int)TileType);
    }
}

void CDungeonLevel::AddDoors(unsigned int room_id){
    unsigned int i;
    // vertical walls
    for(i=m_rooms[room_id].top;i<m_rooms[room_id].top+m_rooms[room_id].height;i++){
        // left wall
        if(m_Map[m_rooms[room_id].left-1][i].tile_type==ttCoridor){
            // check for artefact door
            if( m_Map[m_rooms[room_id].left-2][i].tile_type==ttWall &&
                m_Map[m_rooms[room_id].left-1][i-1].tile_type==ttWall &&
                m_Map[m_rooms[room_id].left-1][i+1].tile_type==ttWall){
                    m_Map[m_rooms[room_id].left-1][i].tile_type=ttWall;
            }else{
                //m_Map[m_rooms[room_id].left-1][i].tile_type=ttDoor;
                AddMapTile(ttDoor,m_rooms[room_id].left-1,i);
            }
        }
        //right
        if(m_Map[m_rooms[room_id].left+m_rooms[room_id].width][i].tile_type==ttCoridor){
            if( m_Map[m_rooms[room_id].left+m_rooms[room_id].width+1][i].tile_type==ttWall &&
                m_Map[m_rooms[room_id].left+m_rooms[room_id].width][i-1].tile_type==ttWall &&
                m_Map[m_rooms[room_id].left+m_rooms[room_id].width][i+1].tile_type==ttWall){
                    m_Map[m_rooms[room_id].left+m_rooms[room_id].width][i].tile_type=ttWall;
            }else{
                AddMapTile(ttDoor, m_rooms[room_id].left+m_rooms[room_id].width,i);
            }
        }
    }
    // horisontal walls
    for(i=m_rooms[room_id].left;i<m_rooms[room_id].left+m_rooms[room_id].width;i++){
        // top wall
        if(m_Map[i][m_rooms[room_id].top-1].tile_type==ttCoridor){
            if( m_Map[i][m_rooms[room_id].top-2].tile_type==ttWall &&
                m_Map[i+1][m_rooms[room_id].top-1].tile_type==ttWall &&
                m_Map[i-1][m_rooms[room_id].top-1].tile_type==ttWall){
                    m_Map[i][m_rooms[room_id].top-1].tile_type=ttWall;
            }else{
                AddMapTile(ttDoor,i,m_rooms[room_id].top-1);
            }
        }
        // bottom
        if(m_Map[i][m_rooms[room_id].top+m_rooms[room_id].height].tile_type==ttCoridor){
            if(m_Map[i][m_rooms[room_id].top+m_rooms[room_id].height+1].tile_type==ttWall &&
               m_Map[i+1][m_rooms[room_id].top+m_rooms[room_id].height].tile_type==ttWall &&
               m_Map[i-1][m_rooms[room_id].top+m_rooms[room_id].height].tile_type==ttWall){
                   m_Map[i][m_rooms[room_id].top+m_rooms[room_id].height].tile_type=ttWall;
            }else{
                AddMapTile(ttDoor,i,m_rooms[room_id].top+m_rooms[room_id].height);
            }
        }
    }
}

// add room room_id to map array
void CDungeonLevel::RoomToMap(unsigned int room_id){
    for(unsigned int y = m_rooms[room_id].top; y < m_rooms[room_id].top + m_rooms[room_id].height; y++){
        for(unsigned int x = m_rooms[room_id].left; x < m_rooms[room_id].left + m_rooms[room_id].width; x++){
            AddMapTile(ttRoom,x,y);
        }
    }
}

// add coridor in map array from start to end point
void CDungeonLevel::CreateCoridor(unsigned int start_room_id, unsigned int end_room_id){
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
        if(m_Map[curr_x][curr_y].tile_type==ttWall){
            if(!cor_near_room){
                AddMapTile(ttCoridor,curr_x,curr_y);
            }
            // check top and bottom. if founded room - resize room
            if(m_Map[curr_x][curr_y-1].tile_type==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x,curr_y-1);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            if(m_Map[curr_x][curr_y+1].tile_type==ttRoom){
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
                AddMapTile(ttCoridor,curr_x-dx,curr_y);
                AddMapTile(ttCoridor,curr_x,curr_y);
                cor_near_room=false;
            }
        }else{
            if(m_Map[curr_x][curr_y].tile_type==ttRoom){
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
        if(m_Map[curr_x][curr_y].tile_type==ttWall){
            if(!cor_near_room){
                AddMapTile(ttCoridor,curr_x,curr_y);
            }
            if(m_Map[curr_x-1][curr_y].tile_type==ttRoom){
                cor_near_room=true;
                int room_id=GetRoomIdByCoords(curr_x-1,curr_y);
                if(m_rooms[room_id].connected){
                    m_rooms[start_room_id].connected=true;
                    return;
                };
                continue;
            }
            if(m_Map[curr_x+1][curr_y].tile_type==ttRoom){
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
                AddMapTile(ttCoridor, curr_x, curr_y-dy);
                AddMapTile(ttCoridor, curr_x, curr_y);
                cor_near_room=false;
            }
        }else{
            if(m_Map[curr_x][curr_y].tile_type==ttRoom){
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
void CDungeonLevel::CreateRoom(sroom *room, unsigned int grid_x, unsigned int grid_y){
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

void CDungeonLevel::NewLevel(){
    unsigned int x,y,i;
    // fill level to wall block
    for(y=0;y<m_height;y++){
        for(x=0;x<m_width;x++){
            AddMapTile(ttWall,x,y);
        }
    }

    // clear grid array
    for(y=0;y<m_grid_h;y++){
        for(x=0;x<m_grid_w;x++){
            m_grid[x][y].filled=false;
        }
    }

    // create rooms
    for(i=0;i<m_rooms_num;i++){
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
        AddLightSource(m_rooms[i].left+(int)m_rooms[i].width/2,m_rooms[i].top+(int)m_rooms[i].height/2,5);
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
};

void CDungeonLevel::AddLightSource(unsigned int x, unsigned int y, unsigned char strength){
    sMapLightSource light;
    light.position.x=x;
    light.position.y=y;
    light.strength=strength;
    LightSourcesList.push_back(light);
    AddMapTile(ttFire,x,y);
};

// Bresenhams line algo
bool CDungeonLevel::LineOfSight(int x1, int y1, int x2, int y2){

    int i, deltax, deltay, numtiles;
    int d, dinc1, dinc2;
    int x, xinc1, xinc2;
    int y, yinc1, yinc2;
    bool isseen=true;

    deltax = abs(x2 - x1);
    deltay = abs(y2 - y1);

    if(deltax >= deltay){
        numtiles = deltax + 1;
        d = (2 * deltay) - deltax;
        dinc1 = deltay*2;
        dinc2 = (deltay - deltax)*2;
        xinc1 = 1;
        xinc2 = 1;
        yinc1 = 0;
        yinc2 = 1;
    }else{
        numtiles = deltay + 1;
        d = (2 * deltax) - deltay;
        dinc1 = deltax*2;
        dinc2 = (deltax - deltay)*2;
        xinc1 = 0;
        xinc2 = 1;
        yinc1 = 1;
        yinc2 = 1;
    }

    if( x1 > x2 ){
        xinc1 = - xinc1;
        xinc2 = - xinc2;
    }

    if(y1 > y2){
        yinc1 = - yinc1;
        yinc2 = - yinc2;
    }

    x = x1;
    y = y1;

    for(i = 2;i<=numtiles;i++){
      if(!m_Map[x][y].skip_light){
        isseen = false;
        break;
      }
      if( d < 0 ){
          d = d + dinc1;
          x = x + xinc1;
          y = y + yinc1;
      }else{
          d = d + dinc2;
          x = x + xinc2;
          y = y + yinc2;
      }
    }
    return isseen;
}

void CDungeonLevel::CalculateLOS(int x_pos, int y_pos, int distance){
    //
    int i,j;
    //unsigned int map_x, map_y;   // los array to map array coords
    // clear LOS data
    if(!distance) distance=m_ViewPort.width/2;
        // clear old LOS
/*        for(i=m_ViewPort.left;i<(m_ViewPort.left+m_ViewPort.width);i++){
            for(j=m_ViewPort.top;j<(m_ViewPort.top+m_ViewPort.height);j++){
                m_Map[i][j].visible=false;
            }
        }
*/
        // clear full map, TODO: remember and clean last filled LOS field
    for(i=0;i<(int)m_width;i++){
        for(j=0;j<(int)m_height;j++){
            m_Map[i][j].visible=false;
        }
    }

    // check all tiles in viewport
        //distance*=distance; // distance^2 - for skip sqrt calculation
        for(i=m_ViewPort.left;i<(int)(m_ViewPort.left+m_ViewPort.width);i++){    // x
            for(j=m_ViewPort.top;j<(int)(m_ViewPort.top+m_ViewPort.height);j++){   // y
            // skip visible
                if(m_Map[i][j].visible) continue;
            // skip far tiles
                int distance_to_tile = sqrt(((i - x_pos) * (i - x_pos)) + ((j - y_pos) * (j - y_pos)));
                // skip far tiles
                if(distance < distance_to_tile){ continue;}
                if(LineOfSight(i,j,x_pos,y_pos)){
                    m_Map[i][j].visible=true;
                    m_Map[i][j].viewed=true;
                }
            }
        }
        int x,y;
        // post processing
        for(i=m_ViewPort.left;i<(int)(m_ViewPort.left+m_ViewPort.width);i++){
            for(j=m_ViewPort.top;j<(int)(m_ViewPort.top+m_ViewPort.height);j++){
                //
                if(!m_Map[i][j].skip_light && !m_Map[i][j].visible){ // invisible block light tile
                    x = i;
                    y = j - 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
                    x = i;
                    y = j + 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }

                    x = i - 1;
                    y = j;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }

                    x = i + 1;
                    y = j;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
//
                    x = i - 1;
                    y = j - 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
                    x = i + 1;
                    y = j - 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
                    x = i + 1;
                    y = j + 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
                    x = i - 1;
                    y = j + 1;
                    if(x>0 && x<(int)m_width){
                        if(y>0 && y<(int)m_height){
                            if(m_Map[x][y].skip_light && m_Map[x][y].visible){  // if near viewed floor
                                m_Map[i][j].visible=true;
                                m_Map[i][j].viewed=true;
                            }
                        }
                    }
                }
            }
        }
        // end post processing fixes
    Log->puts("Calculate los from %d,%d coords\n",x,y);
}

// set global light to ViewPort,TODO: add normal light calculation
void CDungeonLevel::CalculateLight(){
    // check viewport and set global light
    if(m_light_changed){
        unsigned int dx, dy;
        for(dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++){
            for(dx=m_ViewPort.left;dx<m_ViewPort.left+m_ViewPort.width;dx++){
                m_Map[dx][dy].light=m_global_light;
            }
        }
        m_light_changed=false;
    }
}

void CDungeonLevel::Render(){
    unsigned int dy,dx;
    int pos_x, pos_y;
    pos_y=0, pos_x=0;
    CalculateLight();
    for(dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++, pos_y+=32){
        glTranslatef(-pos_x,0,0);
        for(dx=m_ViewPort.left, pos_x=0;dx<m_ViewPort.left+m_ViewPort.width;dx++,pos_x+=32){
            //if(m_Map[dx][dy].visible){
            if(m_Map[dx][dy].visible || m_Map[dx][dy].viewed){
            //if(m_Map[dx][dy].layer[0]!=255){
                //m_tileset->RenderAt(pos_x,pos_y,m_Map[dx][dy].layer[0],&LightMaterials[m_Map[dx][dy].light]);
                if(m_Map[dx][dy].visible){
                    m_tileset->Tile(m_Map[dx][dy].layer[0])->Render(&LightMaterials[m_Map[dx][dy].light]);
                    if(m_Map[dx][dy].layer[1]!=255){
                        m_tileset->Tile(m_Map[dx][dy].layer[1])->Render(&LightMaterials[m_Map[dx][dy].light]);
                    }
                    if(m_Map[dx][dy].layer[2]!=255){
                        m_tileset->Tile(m_Map[dx][dy].layer[2])->Render(&LightMaterials[m_Map[dx][dy].light]);
                    }
                    // TODO, show dropped items
                    // show monsters
                    //if(dx==Herro->GetPosX() && dy==Herro->GetPosY()){
                    //    m_tileset->RenderAt(0,0,tnHerro);
                    //}
                }else{  // viewed
                    m_tileset->Tile(m_Map[dx][dy].layer[0])->Render(&LightMaterials[9]);
                    if(m_Map[dx][dy].layer[1]!=255){
                        m_tileset->Tile(m_Map[dx][dy].layer[1])->Render(&LightMaterials[9]);
                    }
                    if(m_Map[dx][dy].layer[2]!=255){
                        m_tileset->Tile(m_Map[dx][dy].layer[2])->Render(&LightMaterials[9]);
                    }
                }
            }
            glTranslatef(32, 0,0);
        }
        glTranslatef(0,32,0);
    }
    glTranslatef(-pos_x,-pos_y,0);
};
