
#include "level_map.h"

// Bresenhams line algo
bool CLevelMap::LineOfSight(int x1, int y1, int x2, int y2){
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


void CLevelMap::AddMapTile(eTileTypes TileType, int x, int y, void *tile_data){
    //void *data;
    // default values
    if(x>=m_width || y>=m_height){
        Log->puts("CLevelMap::AddMapTile() Error. Wrong coordinates [%d,%d]. Skipped\n",x,y);
    }
    m_Map[x][y].tile_type=TileType;
    m_Map[x][y].layer[1]=255;
    m_Map[x][y].layer[2]=255;
    if(m_Map[x][y].p_tile_data!=NULL){
        Log->puts("Error: CLevelMap::AddMapTile() tile_data!=NULL\n",x,y);
    }
    m_Map[x][y].p_tile_data=NULL;
    switch(TileType){
        case ttDoor: // create door tile
            // check door data
            if(tile_data==NULL){
                Log->puts("Error! Wrong door tile_data!\n");
                return;
            }

            m_Map[x][y].skip_light=false;
            m_Map[x][y].can_move=false;

            if(((sTileDataDoor *)tile_data)->hidden){
                m_Map[x][y].layer[0]=tnWall;
                m_Map[x][y].can_move=false;
            }else{
                m_Map[x][y].layer[0]=tnFloorDungeonCoridor;
                if(((sTileDataDoor *)tile_data)->broken){
                    m_Map[x][y].can_move=true;
                    m_Map[x][y].skip_light=true;
                    m_Map[x][y].layer[1]=tnDoorBrokenDungeon;
                }else if(((sTileDataDoor *)tile_data)->opened){
                    m_Map[x][y].can_move=true;
                    m_Map[x][y].skip_light=true;
                    m_Map[x][y].layer[1]=tnDoorOpenedDungeon;
                }else{
                    m_Map[x][y].layer[1]=tnDoorClosedDungeon;
                }
            }
            m_Map[x][y].p_tile_data=tile_data;
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


void CLevelMap::AddLightSource(unsigned int x, unsigned int y, unsigned char strength){
    sMapLightSource light;
    light.position.x=x;
    light.position.y=y;
    light.strength=strength;
    light.dynamic_tile=new CMapDynamicTile();
    light.dynamic_tile->AddTiles(tnFire+1,4);
    light.dynamic_tile->Update((rand()%200)/100);
    light.dynamic_tile->SetAnimationRate(.15);
    light.dynamic_tile->SetAnimationOscillate(true);
    LightSourcesList.push_back(light);
    //AddMapTile(ttFire,x,y);
    m_Map[x][y].layer[1]=light.dynamic_tile->GetCurrentTile();
};

void CLevelMap::Update(double DeltaTime){
    bool update_light_field=false;
    // update fire animation
    for(unsigned i=0;i<LightSourcesList.size();i++){
        // update animations
        LightSourcesList[i].dynamic_tile->Update(DeltaTime);
        // if in visible field, set animation tile
        if(m_Map[LightSourcesList[i].position.x][LightSourcesList[i].position.y].visible){
            if(m_Map[LightSourcesList[i].position.x][LightSourcesList[i].position.y].layer[1]!=LightSourcesList[i].dynamic_tile->GetCurrentTile()){
                update_light_field=true;
                m_Map[LightSourcesList[i].position.x][LightSourcesList[i].position.y].layer[1]=LightSourcesList[i].dynamic_tile->GetCurrentTile();
            };
        }
    }
    if(update_light_field){

    }
}

// check doorin coords x,y
bool CLevelMap::IsDoor(int x, int y){
    if(m_Map[x][y].p_tile_data==NULL){
        return false;
    }
    return m_Map[x][y].tile_type==ttDoor;
}

// return true if in coords x,y closed door
bool CLevelMap::IsDoorClosed(int x, int y){
    sTileDataDoor* door_data;
    if(IsDoor(x, y)){
        door_data=(sTileDataDoor *)m_Map[x][y].p_tile_data;
        if(!door_data->opened && !door_data->hidden && !door_data->broken){
            return true;
        }
    }
    return false;
}

// open door in coords x, y
bool CLevelMap::OpenDoor(int x, int y){
    sTileDataDoor* door_data;
    if(IsDoor(x, y)){
        door_data=(sTileDataDoor *)m_Map[x][y].p_tile_data;
        if( !door_data->opened && !door_data->broken && !door_data->hidden ){// all ok - open door
                door_data->opened=true;
                m_Map[x][y].can_move=true;
                m_Map[x][y].skip_light=true;
                m_Map[x][y].layer[1]=tnDoorOpenedDungeon;
                return true;
        }else{
            return false;
        }
    }
    return false;
}

// calculate light in map part, light_array - result array, left,top, width, height - map part coords
void CLevelMap::CalculateMapLight(/*int *light_array,*/ int left, int top, int width, int height){
    //printf("calculate light: x:%d, y:%d\n",left,top);
    int dx, dy;
    int max_x=left+width, max_y=top+height;
    if(max_x>=m_width) max_x=m_width-1;
    if(max_y>=m_height) max_y=m_height-1;
    // clear light map
    /*
    for(int i=0;i<width*height;i++){
        light_array[i]=0;
    }
    */
    for(dy=top;dy<=max_y;dy++){
        for(dx=left;dx<=max_x;dx++){
            m_Map[dx][dy].light=0;
        }
    }
    // create light sorce list in current coords
    for(unsigned int i=0; i < LightSourcesList.size(); i++){

        if( LightSourcesList[i].position.x>left-LightSourcesList[i].strength &&
            LightSourcesList[i].position.x<left+width+LightSourcesList[i].strength &&
            LightSourcesList[i].position.y>top-LightSourcesList[i].strength &&
            LightSourcesList[i].position.y<top+height+LightSourcesList[i].strength
        ){

            for(dy=top; dy<=max_y; dy++){
                for(dx=left; dx<max_x; dx++){
                    if( this->IsVisible(dx,dy) &&
                        LineOfSight(LightSourcesList[i].position.x, LightSourcesList[i].position.y,dx,dy)
                       ){
                        // add light intensivity
                        //int vp_index=dx+dy*width;   // view port coordinates index
                        int new_light=LightSourcesList[i].GetIntnsivity(dx,dy);
                        if(m_Map[dx][dy].light < new_light) m_Map[dx][dy].light=new_light;
/*                        if(light_array[vp_index] < new_light){
                            light_array[vp_index]=new_light;
                        }*/
                    }
                }
                //printf("\n");
            }
        }
    }
    //printf("---\n");
}

bool CLevelMap::CloseDoor(int x, int y){

    return false;
}
