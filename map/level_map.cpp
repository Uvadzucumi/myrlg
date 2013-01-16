
#include "level_map.h"

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


void CLevelMap::AddLightSource(unsigned int x, unsigned int y, unsigned char strength, float diffuse){
    sMapLightSource light;
    light.position.x=x;
    light.position.y=y;
    light.strength=strength;
    light.diffuse=diffuse;
    light.dynamic_tile=new CMapDynamicTile();
    light.dynamic_tile->AddTiles(tnFire+1,4);
    light.dynamic_tile->Update((rand()%200)/100);
    light.dynamic_tile->SetAnimationRate(.18);
    light.dynamic_tile->SetAnimationOscillate(true);
    LightSourcesList.push_back(light);
    //AddMapTile(ttFire,x,y);
    m_Map[x][y].layer[1]=light.dynamic_tile->GetCurrentTile();
};

void CLevelMap::Update(double DeltaTime){
    // update fire animation
    for(unsigned i=0;i<LightSourcesList.size();i++){
        // update animations
        LightSourcesList[i].dynamic_tile->Update(DeltaTime);
        // if in visible field, set animation tile
        if(m_Map[LightSourcesList[i].position.x][LightSourcesList[i].position.y].visible){
            m_Map[LightSourcesList[i].position.x][LightSourcesList[i].position.y].layer[1]=LightSourcesList[i].dynamic_tile->GetCurrentTile();
        }
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
void CLevelMap::CalculateMapLight(int *light_array, int left, int top, int width, int height){
    int dx, dy;
    int x,y;
    // clear array
    for(int i=0;i<width*height;i++){
        light_array[i]=0;   // light intensivity
    }
    // create light sorce list in current coords
    for(unsigned int i=0; i < LightSourcesList.size(); i++){
        if( LightSourcesList[i].position.x>left-LightSourcesList[i].strength &&
            LightSourcesList[i].position.x<left+width+LightSourcesList[i].strength &&
            LightSourcesList[i].position.y>top-LightSourcesList[i].strength &&
            LightSourcesList[i].position.y<top+height+LightSourcesList[i].strength
        ){
            // fill light intensivity array
            for(dx=0; dx<width; dx++){
                for(dy=0; dy<height; dy++){
                    x=left+dx; y=top+dy;
                    // skip unvisible fields
                    if(x<0 || y< 0 || x> m_width-1 || y > m_height-1) continue;
                    if(this->IsVisible(x,y) && this->IsSkipLight(x,y) ){
                        // add light intensivity
                        light_array[dx+dy*width]+=LightSourcesList[i].GetIntnsivity(x,y);
                    }
                }
            }
        }
    }
}

bool CLevelMap::CloseDoor(int x, int y){

    return false;
}
