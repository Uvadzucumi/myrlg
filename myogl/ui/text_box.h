#ifndef TEXT_BOX_H_INCLUDED
#define TEXT_BOX_H_INCLUDED

#include "../render.h"
#include "bitmap_font.h"
#include <vector>

namespace MyOGL{

typedef struct{
    Vector2i str_size;
    CText* string;
} sTextBoxString;

class CTextBox{
        std::vector <sTextBoxString> TextStrings;
        Vector4i m_viewport;
        CFont *m_font;
        unsigned int m_max_strings;
        int m_text_height;
        bool m_vAlignTop;
    public:
        CTextBox(CFont *font){
            m_font=font;
            m_viewport.left=0;
            m_viewport.top=0;
            m_viewport.width=MyOGL::Render->GetWidth();
            m_viewport.height=MyOGL::Render->GetHeight();
            m_max_strings=50;
            m_text_height=0;
            m_vAlignTop=false;
            Log->puts("CTextBox Viewport x: %d, y: %d",m_viewport.left, m_viewport.top);
                        Log->puts("w: %d, h: %d\n",m_viewport.width, m_viewport.height);
        };
        ~CTextBox(){
            for(unsigned int i=0;i<TextStrings.size();i++){
                delete TextStrings[i].string;
            }
            TextStrings.clear();
        }
        void AddString(const char *text_string){
            sTextBoxString str;
            if(TextStrings.size()==m_max_strings){
                m_text_height-=TextStrings[0].str_size.height;
                delete TextStrings[0].string;
                TextStrings.erase(TextStrings.begin());
            }
            str.string=new CText(m_font, text_string, m_viewport.width);
            str.str_size=str.string->GetTextSizes();
            m_text_height+=str.str_size.height;
            TextStrings.push_back(str);
            printf("full text height: %d strings: %d\n",m_text_height,TextStrings.size());
        };
        void SetViewPort(int left, int top, int width, int height){
            m_viewport.left=left;
            m_viewport.top=top;
            m_viewport.width=width;
            m_viewport.height=height;
            Log->puts("CTextBox Viewport x: %d, y: %d",m_viewport.left, m_viewport.top);
                        Log->puts(" w: %d, h: %d\n",m_viewport.width, m_viewport.height);
        };
        void Render(Vector4f color){
// TODO: for optimize need search first display string index
// set scissor - for cut not displayed text
            glEnable(GL_SCISSOR_TEST);
            // left, bottom, width, height
            int bottom=MyOGL::Render->GetHeight()-(m_viewport.top+m_viewport.height);
            glScissor(m_viewport.left, bottom, m_viewport.width, m_viewport.height);
            /*
            Log->puts("CTextBox Scissor left: %d, bottom: %d",m_viewport.left, bottom);
                        Log->puts("width: %d, height: %d\n",m_viewport.width, m_viewport.height);
*/
            if(m_text_height>m_viewport.height){
                glTranslatef(m_viewport.left, m_viewport.top+(m_viewport.height-(int)m_text_height),0);
            }else{
                // vAlign=top
                if(m_vAlignTop){
                    glTranslatef(m_viewport.left,m_viewport.top,0);
                }else{
                    //glTranslatef(m_viewport.left,m_viewport.height-(int)m_text_height,0);
                    glTranslatef(m_viewport.left, m_viewport.top+(m_viewport.height-(int)m_text_height),0);
                }
            }
            int dx=0,dy=0;
            for(unsigned int i=0;i<TextStrings.size();i++){
                MyOGL::Render->SetColor(color.r, color.g, color.b, color.a);
                TextStrings[i].string->PrintAt(dx,dy);
                dy+=TextStrings[i].str_size.height;
            }
            glDisable(GL_SCISSOR_TEST);
// disable scissor
        }
    };

}

#endif // TEXT_BOX_H_INCLUDED
