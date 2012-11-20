
#include "camera.h"

using namespace MyOGL;


void CCamera::Apply(){
// First, transform the camera (viewing matrix) from world space to eye space
// Notice all values are negated, because we move the whole scene with the
// inverse of camera transform
//    glRotatef(-Angle[2], 0, 0, 1); // roll
//    glRotatef(-Angle[1], 0, 1, 0); // heading
//    glRotatef(-Angle[0], 1, 0, 0); // pitch
    glTranslatef(-(int)m_position.x, -(int)m_position.y, -(int)m_position.z);
}
