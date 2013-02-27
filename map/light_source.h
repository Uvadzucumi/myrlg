#ifndef LIGHT_SOURCE_H_INCLUDED
#define LIGHT_SOURCE_H_INCLUDED

#include <math.h>
#include "../myogl/vector_types.h"
#include "fov.h"

class CMapLightSource{
        MyOGL::Vector2i m_position;
        int m_strength;
        CFOV* m_light_fov;
    public:

    CMapLightSource(int x, int y, int strength=10){
        m_strength=strength;
        m_position.x=x; m_position.y=y;
        m_light_fov=new CFOV(strength);
    };

    void Calculate(CLevelMap *map){
        m_light_fov->Calculate(m_position.x, m_position.y, map, m_strength);
    };

    ~CMapLightSource(){ delete m_light_fov; };

    MyOGL::Vector2i GetPosition(){ return m_position; }
    int GetStrength(){ return m_strength; }

    int GetIntesivity(int x, int y){    // return light intensivity in coords x,y
        return m_light_fov->GetDistance(x,y);
    };
    // apply light fo FOV area
    void ApplyToFOV(CFOV *fov, CLevelMap *map);

};// sMapLightSource;

#endif // LIGHT_SOURCE_H_INCLUDED
