#ifndef LIGHT_SOURCE_H_INCLUDED
#define LIGHT_SOURCE_H_INCLUDED

#include "level_map.h"
#include <math.h>

class CMapLightSource{
    MyOGL::Vector2i m_position;
    int m_strength;
    //CMapDynamicTile *dynamic_tile;
    public:

    CMapLightSource(int x, int y, int strength=5){
        m_strength=strength;
        m_position.x=x; m_position.y=y;
    };

    ~CMapLightSource(){};

    MyOGL::Vector2i GetPosition(){ return m_position; }
    int GetStrength(){ return m_strength; }

    int GetIntesivity(int x, int y){    // return light intensivity in coords x,y
        // calculate distance
        int distance = sqrt(((x - m_position.x) * (x - m_position.x)) + ((y - m_position.y) * (y - m_position.y)));
        //int ret=strength-distance-1+dynamic_tile->GetCurrentFrame()%2;
        //int ret=strength-distance-dynamic_tile->GetCurrentFrame();
        int ret=m_strength-distance;
        if(ret<=0){
            return 0;
        }else{
            return ret;
        }
    };
};// sMapLightSource;

#endif // LIGHT_SOURCE_H_INCLUDED
