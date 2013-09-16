#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include "map_types.h"

#include "../myogl/frame_buffer.h"
#include "../myogl/vector_types.h"
#include "../myogl/render.h"


class CDungeonMinimap{
        MyOGL::CFBO *fbo;
        MyOGL::Vector2ui m_position;
        MyOGL::MyGlBlendMode m_blend;
        int m_width, m_height;
    public:
        CDungeonMinimap(int width, int height){
            m_position.Set(0,0);
            m_width=width;
            m_height=height;
            fbo=new MyOGL::CFBO(width, height);
            fbo->Enable();
                MyOGL::Render->Set2D(true, m_width, m_height);
                MyOGL::Render->SetClearColor(0,0,0,0);
                MyOGL::Render->ClearScreen();
            fbo->Disable();
            m_blend=MyOGL::blNone; // Without Alpha
        };

        void Position(int x, int y){
            m_position.Set(x,y);
        }
        // return color by map tile
        MyOGL::Vector3i ColorByTile(eTileTypes tile_type){
            MyOGL::Vector3i color;
            switch(tile_type){
                case ttWall:
                    color.Set(255,255,255); break;
                case ttFire:
                    color.Set(255,0,0); break;
                case ttCoridor:
                case ttRoom:
                    color.Set(100,100,100);
                    break;
                case ttDoor:
                    color.Set(201,76,0); break; // brown
                default:
                    color.Set(255,0,0xde); //Purple?
                    break;
            }
            return color;
        };

        // set tile on minimap
        void SetMapPoint(int x, int y, eTileTypes tile_type){
            // color by tile type
            MyOGL::Vector3i tile_color=ColorByTile(tile_type);
            fbo->Enable();
                MyOGL::Render->Set2D(true, m_width, m_height);
                MyOGL::Render->SetColor(tile_color);
                MyOGL::GL.Disable(GL_TEXTURE_2D);
            // Set Blend mode
                //MyOGL::Render->SetBlendMode(MyOGL::blNone);
                MyOGL::Render->DrawPoint(x,y);
            fbo->Disable();
            MyOGL::Render->Set2D(true);
            MyOGL::GL.Enable(GL_TEXTURE_2D);
        };

        void SetPosition(int x, int y){
            m_position.Set(x,y);
        };

        void Render(){
            MyOGL::GL.Enable(GL_TEXTURE_2D);
            MyOGL::GL.Disable(GL_LIGHTING);
            MyOGL::Render->BindTexture(fbo->GetTextureId());
            int m_left, m_right, m_top, m_bottom;
            m_left=m_position.x;
            m_top=m_position.y;
            m_right=m_left+m_width;
            m_bottom=m_top+m_height;
            MyOGL::Render->SetBlendMode(MyOGL::blSource);
            glBegin( GL_QUADS );
	// Top-left vertex (corner)
                glTexCoord2f( 0, 1 );
                glVertex3f( m_left, m_top, 0);
	// Top-right vertex (corner)
                glTexCoord2f( 1, 1 );
                glVertex3f( m_right, m_top, 0 );
	// Bottom-right vertex (corner)
                glTexCoord2f( 1, 0 );
                glVertex3f( m_right, m_bottom, 0 );
	// Bottom-left vertex (corner)
                glTexCoord2f( 0, 0 );
                glVertex3f( m_left, m_bottom, 0);
            glEnd();
        };

        int GetX(){return m_position.x;};

        ~CDungeonMinimap(){
            delete fbo;
        };
    };

#endif // MINIMAP_H_INCLUDED
