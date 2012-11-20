#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "vector_types.h"
#include "render.h"

#include <GL/gl.h>

namespace MyOGL{

    enum  MyMaterialType{
        mColorMaterial,
        mNormal,
        mShader
    };

    class CMaterial{
        protected:
            MyMaterialType m_MaterialType;
            Vector4f m_Ambient, m_Diffuse, m_Specular, m_Emission;
            float m_Shininess;

        public:
            CMaterial(){
                m_MaterialType=mColorMaterial;
                //The color of the object (Diffuse and Ambient)
                m_Diffuse.r=1; m_Diffuse.g=1; m_Diffuse.b=1, m_Diffuse.a=1;
                m_Ambient.r=1; m_Ambient.g=1; m_Ambient.b=1, m_Ambient.a=1;
                m_Specular.r=1; m_Specular.g=1; m_Specular.b=1,m_Specular.a=1; // The specular (shiny) component of the material
                m_Emission.r=0; m_Emission.g=0; m_Emission.b=0, m_Emission.a=1; // The color emitted by the material
                m_Shininess=50;
            };

            void Apply(void);
            void SetDiffuseColor(float r, float g, float b, float a=1){
                m_Diffuse.r=r; m_Diffuse.g=g; m_Diffuse.b=b, m_Diffuse.a=a;
            };
            void SetAmbientColor(float r, float g, float b, float a=1){
                m_Ambient.r=r; m_Ambient.g=g; m_Ambient.b=b, m_Ambient.a=a;
            };
            void SetSpecularColor(float r, float g, float b, float a=1){
                m_Specular.r=r; m_Specular.g=g; m_Specular.b=b,m_Specular.a=a;
            };
            void SetEmissionColor(float r, float g, float b, float a=1){
                m_Emission.r=r; m_Emission.g=g; m_Emission.b=b, m_Emission.a=a;
            };
            void SetShininess(float shininess){ m_Shininess=shininess; };

    };

}

#endif // MATERIAL_H_INCLUDED
