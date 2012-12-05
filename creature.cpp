#include "creature.h"
#include "myogl/vector_types.h"

// creature movement
// dx, dy - in range [-1,1]
// dungeon - current dungeon level
bool CCreature::Move(int dx, int dy, CDungeonLevel *dungeon){

    if(dx > 1 || dx < -1 || dy > 1 || dy < -1){
        MyOGL::Log->puts("Wrong creature movement dx=%d dy=%d\n",dx,dy);
        return false;
    }
    if(dungeon->Map()[m_x+dx][m_y+dy].can_move){
        SetPosition(m_x+dx, m_y+dy);
        dungeon->SetViewportToTarget(m_x,m_y);
        dungeon->CalculateLOS(m_x,m_y);
        return true;
    }
    return false;
};

Vector2i CCreature::GetMapCoords(eDirections direction){
    Vector2i coords;
    switch(direction){
        case dNorth:
            coords.x=m_x; coords.y=m_y-1; break;
        case dSouth:
            coords.x=m_x; coords.y=m_y+1; break;
        case dEast:
            coords.x=m_x+1; coords.y=m_y; break;
        case dWest:
            coords.x=m_x-1; coords.y=m_y; break;
        case dNone:
        default:
            // search door position
            Log->puts("Wrong CCreature::GetMapCoords(direction) direction parameter! Using creature coords\n");
            coords.x=m_x;
            coords.y=m_y;
    }
    return coords;
}

bool CCreature::OpenDoor(CDungeonLevel *dungeon, eDirections direction){
    // search door position
    Vector2i map_coord;
    sTileDataDoor *data=NULL;
    map_coord.x=-1;
    switch(direction){
        case dNorth:
        case dSouth:
        case dEast:
        case dWest:
            map_coord=GetMapCoords(direction);
        break;
        default:
            // search door position
            for(int i=dNorth; i<dDirectionsCount; i++){
                Vector2i tmp_coord;
                tmp_coord=GetMapCoords((eDirections)i);
                Log->puts("Check [%d][%d]\n",tmp_coord.x,tmp_coord.y);
                if(dungeon->Map()[tmp_coord.x][tmp_coord.y].tile_type==ttDoor){
                    Log->puts("Founded door in coords [%d][%d]\n",tmp_coord.x,tmp_coord.y);
                    // founded door
                    if((sTileDataDoor *)dungeon->Map()[tmp_coord.x][tmp_coord.y].p_tile_data!=NULL){
                        data=(sTileDataDoor *)dungeon->Map()[tmp_coord.x][tmp_coord.y].p_tile_data;
                        if(!data->opened && !data->hidden && !data->broken){
                        // founded closed door
                            map_coord=tmp_coord;
                            Log->puts("Founded closed door in map coords [%d][%d]\n",map_coord.x, map_coord.y);
                            break;
                        }
                    }else{
                        Log->puts("Error: Door p_tile_data=NULL\n");
                    }
                }
            }
            if(map_coord.x==-1){
                Log->puts("Not founded closed Door near [%d][%d]\n",m_x,m_y);
                return false;
            }
    }
    if(dungeon->Map()[map_coord.x][map_coord.y].tile_type==ttDoor &&
                    ! ((sTileDataDoor *)(dungeon->Map()[map_coord.x][map_coord.y].p_tile_data))->opened &&
                    ! ((sTileDataDoor *)(dungeon->Map()[map_coord.x][map_coord.y].p_tile_data))->broken &&
                    ! ((sTileDataDoor *)(dungeon->Map()[map_coord.x][map_coord.y].p_tile_data))->hidden
    ){
    // Open door
        ((sTileDataDoor *)(dungeon->Map()[map_coord.x][map_coord.y].p_tile_data))->opened=true;
        dungeon->Map()[map_coord.x][map_coord.y].can_move=true;
        dungeon->Map()[map_coord.x][map_coord.y].skip_light=true;
        dungeon->Map()[map_coord.x][map_coord.y].layer[1]=tnDoorOpenedDungeon;
        Log->puts("Door opened!\n");
        dungeon->CalculateLOS(m_x,m_y);
        return true;
    }else{
        Log->puts("Not founded closed door in this direction\n");
        return false;
    }
}

bool CHerro::Move(int dx, int dy, CDungeonLevel *dungeon){
// movement delay
    unsigned int tick=SDL_GetTicks();
    if(m_last_mov_tick && (tick-m_last_mov_tick)<movement_delay){
        return false;
    }
    m_last_mov_tick=tick;

    return CCreature::Move(dx, dy, dungeon);

}

void CHerro::Render(){
    CCreature::Render(); // rener sprite
    // render equipped items
    inventory->RenderEquipped();
}
