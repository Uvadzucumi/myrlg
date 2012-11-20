#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <vector>

#include "animation.h"
#include "sprites.h"
#include "material.h"

namespace MyOGL{

    class CEntity {

        protected:
            CAnimation *m_animation;
            CTexture  *m_texture;
            CMaterial *m_material;
            std::vector<CHudSprite*> SpritesList;

        public:
            float           X;
            float           Y;

            int             Width;
            int             Height;

            int             AnimState;

        public:
            CEntity();
            virtual ~CEntity();

        public:
            //virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);
            virtual bool OnCreate(CTexture* texture, int Width, int Height, int MaxFrames,CMaterial *material=NULL);
            virtual void OnLoop(double DeltaTime);
            virtual void OnRender();
            virtual void Free();
            void SetMaterail(CMaterial *material, int animation=-1);
    };

    extern std::vector<CEntity*> EntityList;

}

#endif // ENTITY_H_INCLUDED
