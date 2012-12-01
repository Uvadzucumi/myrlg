#include "dungeon_map.h"

#include <math.h>
#include "game.h"

#include<SDL/SDL.h>

void CDungeonLevel::NewGridDungeon(int grid_w, int grid_h, int rooms){
    CDungeonGeneration dlg(grid_w, grid_h, rooms);   // grid_w, grid_h, rooms
    dlg.Generate(m_Map);
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
      if(!m_Map->IsSkipLight(x,y)){
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
//    unsigned int start=SDL_GetTicks();
    //Log->puts("calc LOS ");

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
            m_Map->GetMap()[i][j].visible=false;
        }
    }

    // check all tiles in viewport
        //distance*=distance; // distance^2 - for skip sqrt calculation
    for(i=m_ViewPort.left;i<(int)(m_ViewPort.left+m_ViewPort.width);i++){    // x
        for(j=m_ViewPort.top;j<(int)(m_ViewPort.top+m_ViewPort.height);j++){   // y
        // skip visible
            if(m_Map->IsVisible(i,j)) continue;
            // skip far tiles
            int distance_to_tile = sqrt(((i - x_pos) * (i - x_pos)) + ((j - y_pos) * (j - y_pos)));
                // skip far tiles
            if(distance < distance_to_tile){ continue;}
            if(LineOfSight(i,j,x_pos,y_pos)){
                    m_Map->GetMap()[i][j].visible=true;
                    m_Map->GetMap()[i][j].viewed=true;
            }
        }
    }

    int x,y;
    // post processing

    for(i=m_ViewPort.left;i<(int)(m_ViewPort.left+m_ViewPort.width);i++){
        for(j=m_ViewPort.top;j<(int)(m_ViewPort.top+m_ViewPort.height);j++){
                //
            if(!m_Map->IsSkipLight(i,j) && !m_Map->IsVisible(i,j)){ // invisible block light tile
                // north
                x = i;
                y = j - 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y)){  // if near viewed floor
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }
                // south
                x = i;
                y = j + 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y)){  // if near viewed floor
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }
                // west
                x = i - 1;
                y = j;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y)){  // if near viewed floor
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }
                // east
                x = i + 1;
                y = j;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y)){  // if near viewed floor
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }

                // nort-west
                x = i - 1;
                y = j - 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y) &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,j) && !m_Map->IsSkipLight(i,y)){    // and 2 block
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }
                // north-east
                x = i + 1;
                y = j - 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y)  &&         // if near viewed floor
                            !m_Map->IsSkipLight(x, j) && !m_Map->IsSkipLight(i,y)){    // and 2 block
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }
                // south-east
                x = i + 1;
                y = j + 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y) &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,j) && !m_Map->IsSkipLight(i,y)){    // and 2 block
                            m_Map->GetMap()[i][j].visible=true;
                            m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }

                //south-west
                x = i - 1;
                y = j + 1;
                if(x>0 && x<(int)m_width){
                    if(y>0 && y<(int)m_height){
                        if(m_Map->IsSkipLight(x,y) && m_Map->IsVisible(x,y) &&         // if near viewed floor
                            !m_Map->IsSkipLight(x,j) && !m_Map->IsSkipLight(i,y)){    // and near 2 block
                                m_Map->GetMap()[i][j].visible=true;
                                m_Map->GetMap()[i][j].viewed=true;
                        }
                    }
                }

            } // end checking invisible tile

        }
    }  // end post processing fixes

// post processing 2
/*
    for(i=m_ViewPort.left;i<(int)(m_ViewPort.left+m_ViewPort.width);i++){
        for(j=m_ViewPort.top;j<(int)(m_ViewPort.top+m_ViewPort.height);j++){

            if(m_Map[i][j].skip_light && !m_Map[i][j].visible){ // skip light and invisible
                //if up & down blocked and visible - set visible
                // if left and right blocked and visible - set visible
                if(j>1 && j<m_height-1){
                    if(!m_Map[i][j-1].skip_light && m_Map[i][j-1].visible){
                        if(!m_Map[i][j+1].skip_light && m_Map[i][j+1].visible){
                            m_Map[i][j].visible=true;
                            m_Map[i][j].viewed=true;
                        }
                    }
                }

                //if left & right blocked and visible - set visible
                // if left and right blocked and visible - set visible
                if(i>1 && i<m_width-1){
                    if(!m_Map[i-1][j].skip_light && m_Map[i-1][j].visible &&
                       !m_Map[i+1][j].skip_light && m_Map[i+1][j].visible){
                            m_Map[i][j].visible=true;
                            m_Map[i][j].viewed=true;
                    }
                }

            }
        }
    } // end postprocessing 2
*/
    //Log->puts("%d ticks\n",SDL_GetTicks()-start);
}

// set global light to ViewPort,TODO: add normal light calculation
void CDungeonLevel::CalculateLight(){
    // check viewport and set global light
    if(m_light_changed){
        unsigned int dx, dy;
        for(dy=m_ViewPort.top; dy<m_ViewPort.top+m_ViewPort.height; dy++){
            for(dx=m_ViewPort.left;dx<m_ViewPort.left+m_ViewPort.width;dx++){
                m_Map->GetMap()[dx][dy].light=m_global_light;
            }
        }
        m_light_changed=false;
    }
}

// Update map. change animated tiles and etc.
void CDungeonLevel::Update(double DeltaTime){
    m_Map->Update(DeltaTime);
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
            if(m_Map->IsVisible(dx,dy) || m_Map->IsViewed(dx,dy)){
            //if(m_Map[dx][dy].layer[0]!=255){
                //m_tileset->RenderAt(pos_x,pos_y,m_Map[dx][dy].layer[0],&LightMaterials[m_Map[dx][dy].light]);
                if(m_Map->IsVisible(dx,dy)){
                    m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[0])->Render(&LightMaterials[m_Map->GetMap()[dx][dy].light]);
                    if(m_Map->GetMap()[dx][dy].layer[1]!=255){
                        m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[1])->Render(&LightMaterials[m_Map->GetMap()[dx][dy].light]);
                    }
                    if(m_Map->GetMap()[dx][dy].layer[2]!=255){
                        m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[2])->Render(&LightMaterials[m_Map->GetMap()[dx][dy].light]);
                    }
                    // TODO, show dropped items
                    // show monsters
                    //if(dx==Herro->GetPosX() && dy==Herro->GetPosY()){
                    //    m_tileset->RenderAt(0,0,tnHerro);
                    //}
                }else{  // viewed
                    m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[0])->Render(&LightMaterials[9]);
                    if(m_Map->GetMap()[dx][dy].layer[1]!=255){
                        m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[1])->Render(&LightMaterials[9]);
                    }
                    if(m_Map->GetMap()[dx][dy].layer[2]!=255){
                        m_tileset->Tile(m_Map->GetMap()[dx][dy].layer[2])->Render(&LightMaterials[9]);
                    }
                }
            }
            glTranslatef(32, 0,0);
        }
        glTranslatef(0,32,0);
    }
    glTranslatef(-pos_x,-pos_y,0);
};
