
#include "level_map.h"

const char* GetTileNameByType(eTileTypes tile){
    switch(tile){
        case ttNone: return "Непонятный тип тайла"; break;
        case ttWall: return "Стена"; break;
        case ttCoridor: return "Коридор подземелья"; break;
        case ttRoom: return "Комната"; break;
        case ttDoor: return "Дверь"; break;
        default: return "Ошибочный тип тайла"; break;
    }

}

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
        Log->printf("CLevelMap::AddMapTile() Error. Wrong coordinates [%d,%d]. Skipped\n",x,y);
    }
    m_Map[x][y].tile_type=TileType;
    m_Map[x][y].layer[1]=255;
    m_Map[x][y].layer[2]=255;
    if(m_Map[x][y].p_tile_data!=NULL){
        Log->printf("Error: CLevelMap::AddMapTile() tile_data!=NULL\n",x,y);
    }
    m_Map[x][y].p_tile_data=NULL;
    switch(TileType){
        case ttDoor: // create door tile
            // check door data
            if(tile_data==NULL){
                Log->puts("Error! Wrong door tile_data!\n");
                return;
            }
            // door need add only if (left & right) or (top & bottom) - wall
            // check walls and sec coridor if not need add door
            if( ( !IsWall(x-1,y,false) || !IsWall(x+1,y,false) ) && ( !IsWall(x,y-1,false) || !IsWall(x,y+1,false) )){
                // set coridor
                m_Map[x][y].tile_type=ttCoridor;
                m_Map[x][y].layer[0]=tnFloorDungeonCoridor;
                m_Map[x][y].skip_light=true;
                m_Map[x][y].can_move=true;
                break;
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
            m_Map[x][y].layer[1]=tnBonfire;
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
            MyOGL::Log->printf("Warning: unknow AddMapTile(TileType) = [%d]\n",(int)TileType);
    }
}


void CLevelMap::AddBonfire(unsigned int x, unsigned int y, unsigned char strength){
    CMapLightSource *light;
    // add light source
    light=new CMapLightSource(x, y, strength);
    LightSourcesList.push_back(light);
    // add dynamic tile
    CMapDynamicTile *dynamic_tile=new CMapDynamicTile();
    dynamic_tile->AddTiles(tnBonfire+1,4);
    dynamic_tile->Update((rand()%200)/100);
    dynamic_tile->SetAnimationRate(.15);
    dynamic_tile->SetAnimationOscillate(true);
    dynamic_tile->position.x=x;
    dynamic_tile->position.y=y;
    dynamic_tile->layer=1;
    //AddMapTile(ttFire,x,y);
    m_Map[x][y].layer[dynamic_tile->layer]=dynamic_tile->GetCurrentTile();
    DynamicTilesList.push_back(dynamic_tile);
};

void CLevelMap::Update(CFOV *fov, double DeltaTime){
    // update animated sprites
    for(unsigned i=0;i<DynamicTilesList.size();i++){
        // if in visible field, may be need change animated tile
        if( fov->IsInArea(DynamicTilesList[i]->position.x, DynamicTilesList[i]->position.y) &&
            fov->IsVisible(DynamicTilesList[i]->position.x,DynamicTilesList[i]->position.y)){
            // update animations
            if(DynamicTilesList[i]->Update(DeltaTime)){
//            if(m_Map[DynamicTilesList[i]->position.x][DynamicTilesList[i]->position.y].layer[DynamicTilesList[i]->layer]!=DynamicTilesList[i]->GetCurrentTile()){
                m_Map[DynamicTilesList[i]->position.x][DynamicTilesList[i]->position.y].layer[DynamicTilesList[i]->layer]=DynamicTilesList[i]->GetCurrentTile();
//            };
            }
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
                // recalculate lighs
                //CalculateNearesLights();
                CalculateAllLights();
                return true;
        }else{
            return false;
        }
    }
    return false;
}

// need call after map generation
void CLevelMap::CalculateAllLights(){
    for(unsigned int i=0; i < LightSourcesList.size(); i++){
        LightSourcesList[i]->Calculate(this);
    }
}

// update light near FOV
void CLevelMap::CalculateNearesLights(){
// update saved lights
    Log->printf("Nearest lights: %d\n",LightsNearFOV.size());
    for(unsigned int i=0; i<LightsNearFOV.size(); i++){
        MyOGL::Vector2i light_pos=LightSourcesList[LightsNearFOV[i]]->GetPosition();
        Log->printf("recalculate light in pos: %d, %d\n", light_pos.x, light_pos.y );
        LightSourcesList[LightsNearFOV[i]]->Calculate(this);
    }
}


// calculate light in map part, light_array - result array, left,top, width, height - map part coords
void CLevelMap::CalculateMapLight(CFOV *fov){

    Vector2i left_top=fov->GetLeftTop();
    Vector2i right_bottom=fov->GetRightBottom();

    // clear old lights list
    LightsNearFOV.clear();

    // search nearest light sources
    for(unsigned int i=0; i < LightSourcesList.size(); i++){
        MyOGL::Vector2i light_pos=LightSourcesList[i]->GetPosition();
        int strength=LightSourcesList[i]->GetStrength();
        if( light_pos.x+strength>=left_top.x &&
            light_pos.x-strength<=right_bottom.x &&
            light_pos.y+strength>=left_top.y &&
            light_pos.y-strength<=right_bottom.y ){
                // save nearest lights indexes
                LightsNearFOV.push_back(i);
                // need apply light to FOV
                LightSourcesList[i]->ApplyToFOV(fov, this);
            }
    }

}

#define NOT_VIEWED_FILD_EMPTY

// return true if wall tile tupe (walls, doors, windows etc...)
// if bool check_viewed=true - not viewed - empty field
bool CLevelMap::IsWall(int x, int y, bool check_viewed){
    if(x < 0 || x >= m_width || y < 0 || y >= m_height ){
        return false;
    }
    if(check_viewed && !m_Map[x][y].viewed){ // map not viewed
        #ifdef NOT_VIEWED_FILD_EMPTY
        return false;
        #else
        return true;
        #endif
    }
    if(m_Map[x][y].tile_type==ttWall || m_Map[x][y].tile_type==ttDoor){
        return true;
    }
    return false;
}

sTileDataDoor CLevelMap::GetDoorData(int x, int y){
    if(m_Map[x][y].tile_type==ttDoor){
        if(m_Map[x][y].p_tile_data!=NULL){
            sTileDataDoor *tmp;
            tmp=(sTileDataDoor* )m_Map[x][y].p_tile_data;
            return *tmp;
        }else{
            Log->puts("ERROR: CLevelMap::GetDoorData() - empty door data!\n");
        }
    }else{
        Log->printf("ERROR: CLevelMap::GetDoorData() not found door in %d,%d coords\n",x,y);
    }
    // return error data
    sTileDataDoor tmp;
    return tmp;
}

void CLevelMap::LandPostprocessing(CFOV *fov, int x, int y){
    // update one tile
    if(fov->IsInArea(x,y) && fov->IsVisible(x,y)){ // only visible
        // only walls
        if(IsWall(x,y,false)){
            // skip not hidden doors
            if(m_Map[x][y].tile_type==ttDoor){
                sTileDataDoor door_data;
                if(!((door_data=GetDoorData(x,y)).hidden)){ // if door hidden - same wall processing
                    // set door tile & return
                    if(IsWall(x+1,y) && IsWall(x-1,y)){ // left & right blocked
                        // horizontal door
                        if(door_data.broken){
                            m_Map[x][y].layer[1]=tnDoorHorizontalBroken;
                        }else if(door_data.opened){
                            m_Map[x][y].layer[1]=tnDoorHorizontalOpened;
                        }else{ // opened door
                            m_Map[x][y].layer[1]=tnDoorHorizontalClosed;
                        }
                    }else{ // vertical door
                        if(door_data.broken){
                            m_Map[x][y].layer[1]=tnDoorVerticalBroken;
                        }else if(door_data.opened){
                            m_Map[x][y].layer[1]=tnDoorVerticalOpened;
                        }else{ // opened door
                            m_Map[x][y].layer[1]=tnDoorVerticalClosed;
                        }
                    }

                    return; // skip normal doors
                }
            }
            // vertical wall
            if(!IsWall(x+1,y) && !IsWall(x-1,y)){ // left & right empty
                if(!IsWall(x,y+1)){ // down empty
                    if(!IsWall(x,y-1)){
                        m_Map[x][y].layer[0]=tnWallAlone;
                    }else{
                        m_Map[x][y].layer[0]=tnWallEndDown;
                    }
                }else if(!IsWall(x,y-1)){ // up empty
                    m_Map[x][y].layer[0]=tnWallEndUp;
                }else{ // ud and down tiles have walls
                    m_Map[x][y].layer[0]=tnWallVertical;
                }
            }else
            // horisontal wall
            if(!IsWall(x,y+1) && !IsWall(x,y-1)){
                if(!IsWall(x+1,y)){ // right - empty
                    m_Map[x][y].layer[0]=tnWallEndRight;
                }else if(!IsWall(x-1,y)){
                    m_Map[x][y].layer[0]=tnWallEndLeft;
                }else{ // left and right - walls
                    m_Map[x][y].layer[0]=tnWallHorizontal;
                }
            }else
            // left top corner
            if(IsWall(x+1,y) && IsWall(x,y+1)){
                if(IsWall(x,y-1)){
                    if(IsWall(x+1,y+1) && IsWall(x+1,y-1)){
                        if(IsWall(x-1,y)){
                            if(IsWall(x-1,y+1)){
                                m_Map[x][y].layer[0]=tnWallRightBottomFillOutside;
                            }else{
                                if(!IsWall(x-1,y-1)){
                                    m_Map[x][y].layer[0]=tnWallVerticalLeftFillE;
                                }else{
                                    m_Map[x][y].layer[0]=tnWallRightTopFillOutside;
                                }
                            }
                        }else{
                            m_Map[x][y].layer[0]=tnWallVerticalFillRight;
                        }
                    }else{
                        if(IsWall(x-1,y)){
                            if(IsWall(x+1,y+1)){
                                if(!IsWall(x-1,y-1)){
                                    m_Map[x][y].layer[0]=tnWallHorizontalUpFillS;
                                }else{
                                    m_Map[x][y].layer[0]=tnWallLeftBottomFillOutside;
                                }
                            }else{
                                if(!IsWall(x-1,y+1)){
                                    m_Map[x][y].layer[0]=tnWallHorizontalDownFillN;
                                }else{
                                    if(!IsWall(x+1,y-1)){
                                        m_Map[x][y].layer[0]=tnWallVerticalRightFillW;
                                    }else{
                                        m_Map[x][y].layer[0]=tnWallLeftTopFillOutside;
                                    }
                                }
                            }
                        }else{
                            if(IsWall(x+1,y-1)){
                                m_Map[x][y].layer[0]=tnWallVerticalRightFillNE;
                            }else{
                                if(IsWall(x+1,y+1)){
                                    m_Map[x][y].layer[0]=tnWallVerticalRightFillSE;
                                }else{
                                    m_Map[x][y].layer[0]=tnWallVerticalRight;
                                }
                            }
                        }
                    }
                }else{
                    if(IsWall(x-1,y)){
                        if(IsWall(x-1,y+1) && IsWall(x+1,y+1)){
                            m_Map[x][y].layer[0]=tnWallHorizontalFillDown;
                        }else{
                            if(IsWall(x-1,y+1)){
                                m_Map[x][y].layer[0]=tnWallHorizontalDownFillSW;
                            }else{
                                if(IsWall(x+1,y+1)){
                                    m_Map[x][y].layer[0]=tnWallHorizontalDownFillSE;
                                }else{
                                    m_Map[x][y].layer[0]=tnWallHorizontalDown;
                                }
                            }
                        }
                    }else{
                        if(IsWall(x+1,y+1)){
                            m_Map[x][y].layer[0]=tnWallLeftTopFillInside;
                        }else{
                            m_Map[x][y].layer[0]=tnWallLeftTop;
                        }
                    }
                }
            }else
            // right top corner
            if(IsWall(x-1,y) && IsWall(x,y+1)){
                if(IsWall(x,y-1)){
                    if(IsWall(x-1,y-1) && IsWall(x-1,y+1)){
                        m_Map[x][y].layer[0]=tnWallVerticalFillLeft;
                    }else{
                        if(IsWall(x-1,y-1)){
                            m_Map[x][y].layer[0]=tnWallVerticalLeftFillNW;
                        }else{
                            if(IsWall(x-1,y+1)){
                                m_Map[x][y].layer[0]=tnWallVerticalLeftFillSW;
                            }else{
                                m_Map[x][y].layer[0]=tnWallVerticalLeft;
                            }
                        }
                    }
                }else{
                    if(IsWall(x+1,y)){
                        m_Map[x][y].layer[0]=tnWallHorizontalDown;
                    }else{
                        if(IsWall(x-1,y+1)){
                            m_Map[x][y].layer[0]=tnWallRightTopFillInside;
                        }else{
                            m_Map[x][y].layer[0]=tnWallRightTop;
                        }
                    }
                }
            }else
            // left bottom corner
            if(IsWall(x+1,y) && IsWall(x,y-1)){
                if(IsWall(x,y+1)){
                    m_Map[x][y].layer[0]=tnWallVerticalLeft;
                }else{
                    if(IsWall(x-1,y)){
                        if(IsWall(x-1,y-1) && IsWall(x+1,y-1)){
                            m_Map[x][y].layer[0]=tnWallHorizontalFillUp;
                        }else{
                            if(IsWall(x-1,y-1)){
                                m_Map[x][y].layer[0]=tnWallHorizontalUpFillNW;
                            }else{
                                if(IsWall(x+1,y-1)){
                                    m_Map[x][y].layer[0]=tnWallHorizontalUpFillNE;
                                }else{
                                    m_Map[x][y].layer[0]=tnWallHorizontalUp;
                                }
                            }
                        }
                    }else{
                        if(IsWall(x+1,y-1)){
                            m_Map[x][y].layer[0]=tnWallLeftBottomFillInside;
                        }else{
                            m_Map[x][y].layer[0]=tnWallLeftBottom;
                        }
                    }
                }
            }else
            // right bottom
            if(/*!IsWall(x-1,y-1) &&*/ IsWall(x-1,y) && IsWall(x,y-1)){
                if(IsWall(x,y+1)){
                    m_Map[x][y].layer[0]=tnWallVerticalRight;
                }else{
                    if(IsWall(x+1,y)){
                        m_Map[x][y].layer[0]=tnWallHorizontalUp;
                    }else{
                        if(IsWall(x-1,y-1)){
                            m_Map[x][y].layer[0]=tnWallRightBottomFillInside;
                        }else{
                            m_Map[x][y].layer[0]=tnWallRightBottom;
                        }
                    }
                }
            }

        }
    }
}


bool CLevelMap::CloseDoor(int x, int y){

    return false;
}
