#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "vector_types.h"
#include <GL/gl.h>

namespace MyOGL{

    class CCamera{
            Vector3f m_position;

        public:
            CCamera(){
                m_position.x=0;
                m_position.y=0;
                m_position.z=0;
            };

            void Apply();
            void SetPosition(float x, float y, float z){ m_position.x=x; m_position.y=y; m_position.z=z;}
            void Move(float distance){ m_position.y+=distance;};
            void Slide(float distance){ m_position.x+=distance;};

            Vector3f Position(void){ return m_position;};
    };

}


#endif // CAMERA_H_INCLUDED
