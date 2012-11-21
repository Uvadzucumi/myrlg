#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

#include "vector_types.h"
#include "render.h"
#include "texture.h"
#include "material.h"

namespace MyOGL{

    class CHudSprite{
            MyGlBlendMode BlendMode;
            //int TextureWidth, TextureHeight;
            CTexture *m_texture;
            CMaterial *m_material;
            Vector2f TextCoords[2];
            // sprite position
            int m_left, m_top;
            //int m_width, m_height;
            int m_right, m_bottom;
            float m_angle;    // rotate angle
            // Matherial
        public:
            CHudSprite(CTexture *texture, CMaterial *material=NULL){
                // set inicial values
                m_angle=0;
                m_left=0; m_top=0;  // default sprite position
                // set material
                m_material=material;
                // dafault sprite size from texture
                m_texture=texture;
                SetSize(m_texture->GetWidth(),m_texture->GetHeight());
                // default text coords for full texture
                TextCoords[0].u=0.0; TextCoords[0].v=0.0;
                TextCoords[1].u=1.0; TextCoords[1].v=1.0;
                // set color material
                // default blend mode
                // if in texture have alfa - set Source
                if(texture->IsAlpha()){
                    BlendMode=blSource;
                }else{
                    BlendMode=blNone;
                }
                //Log->puts("Created HudSprite\n");
            };
            ~CHudSprite(){
                //Log->puts("Deleted HudSprite\n");
            }
            void Rotate(float angle){ m_angle=angle; };
            void SetPosition(int left, int top);
            void SetSize(int width, int height);
            //void SetTextureCoords(int text_left, int text_top, int text_right, int text_bottom);
            void SetTransparent(MyGlBlendMode mode){ BlendMode=mode; }
            void Render(CMaterial *material=NULL);
            void SetUVCoords(float t0_u, float t0_v, float t1_u, float t1_v){
                TextCoords[0].u=t0_u; TextCoords[0].v=t0_v;
                TextCoords[1].u=t1_u; TextCoords[1].v=t1_v;
            };
            void SetUVPixelCoords(int left, int top, int width, int height){
                SetUVCoords((float)left/m_texture->GetWidth(),(float)top/m_texture->GetHeight(),(float)(left+width)/m_texture->GetWidth(),(float)(top+height)/m_texture->GetHeight());
            }

            int GetTextureWidth(){ return m_texture->GetWidth();};
            int GetTextureHeight(){ return m_texture->GetHeight();};
            // set color material value
            //void SetColorMaterial(float r, float g, float b){ m_ColorMaterial.r=r; m_ColorMaterial.g=g; m_ColorMaterial.b=b; }
            void SetMaterial(CMaterial *material){ m_material=material;};
    };

}
#endif // SPRITES_H_INCLUDED
