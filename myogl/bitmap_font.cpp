
#include "bitmap_font.h"
#include <wchar.h>

using namespace MyOGL;

// Clear font class data
CFont::~CFont(){
    // clear glyphs
    m_chars.clear();
    Log->puts("Clear Font glyphs data\n");
}

// loading font glyphs data from file file_name and assign font texture *texture
bool CFont::LoadGlyphData(const char *file_name, CTexture *texture){
    char font_name[100], font_size[20];
    char tmp[100];
    FILE *fp;
    int i;
    bool font_start=false;
    Glyph glyph;

    if(!texture){
        Log->puts("CFont::LoadGlyphData() - Error: Not set font texture\n");
        return false;
    }
    m_texture=texture;

    if(!(fp=fopen(file_name,"rt"))){
        Log->puts("CFont::LoadGlyphData() - Error: Not opened font description file '%s'\n",file_name);
        return false;
    };
    int char_count=0;
    int CharCode, Xpos, Ypos, Width, Height, Xoffset, Yoffset, OrigW, OrigH;
    while(!feof(fp)){
        // read string
        //fscanf(fp,"%s\n",tmp);
        if(fgets(tmp,100,fp)!=NULL){;
            if(!font_start){
                sscanf(tmp,"%s %s\n",font_name,font_size);
                font_start=true;
            }else{ // font founded - read glyph
                i=sscanf(tmp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",&CharCode, &Xpos, &Ypos, &Width, &Height, &Xoffset, &Yoffset, &OrigW, &OrigH);
                if(i!=9){   // wrong gliph string
                    Log->puts("CFont::LoadGlyphData() - Warning: Wrong glyph string '%s'",tmp);
                    break;
                }
                glyph.code=CharCode;
                glyph.width=Width;
                glyph.height=Height;
                glyph.x_offset=Xoffset;
                glyph.y_offset=Yoffset;
                glyph.orig_width=OrigW;
                if(m_font_height==-1){ m_font_height=OrigH;}
            // left top corner
                glyph.text_coords[0].u=(float)Xpos/texture->GetWidth();
                glyph.text_coords[0].v=(float)Ypos/texture->GetHeight();
            // top right
                glyph.text_coords[1].u=(float)(Xpos+Width)/(float)texture->GetWidth();
                glyph.text_coords[1].v=(float)(Ypos+Height)/(float)texture->GetWidth();

                m_chars.push_back(glyph);

                char_count++;
            } // READ GLYPH
        } // IF READED
    } // while EOF
    Log->puts("CFont::LoadGlyphData() - loaded %d glyphs\n",char_count);
    fclose(fp);
    return true;
}

// show font glyphs data
void CFont::debug(void){
// show all glyph
    unsigned int i;
    Log->puts("crasr: %d\n",m_chars.size());
    for(i=0;i<m_chars.size();i++){
        Log->puts("code: %d x_off: %d ",m_chars[i].code, m_chars[i].x_offset);
        Log->puts("y_offs: %d width: %d ",m_chars[i].y_offset, m_chars[i].width);
        Log->puts("height: %d orig_width: %d ", m_chars[i].height, m_chars[i].orig_width);
        Log->puts("(%c%c)",(m_chars[i].code>>8 & 0xff),m_chars[i].code & 0xff);
    }
}

// get utf8 char code and return glyph ID from font glyphs list
int CFont::GetGlyphId(unsigned int code){
    unsigned int index;
    for(index=0;index<m_chars.size();index++){
        if(m_chars[index].code==code){
            break;
        }
    }
    if(index==m_chars.size()){
        Log->puts("CFont::GetGlyphId - Warning: symbol %d not found!\n",code);
    }
    return index;
}

// render glyph *glyph at coordinates x,y
int CFont::RenderGlyph(int x, int y, Glyph *glyph){

    float left_top=x+glyph->x_offset;
    float right_bottom=y+glyph->y_offset;

    glTexCoord2f(glyph->text_coords[0].u,glyph->text_coords[0].v);
    glVertex2d(left_top,right_bottom);

    glTexCoord2f(glyph->text_coords[1].u,glyph->text_coords[0].v);
    glVertex2d(left_top+glyph->width,right_bottom);

    glTexCoord2f(glyph->text_coords[1].u,glyph->text_coords[1].v);
    glVertex2d(left_top+glyph->width,right_bottom+glyph->height);

    glTexCoord2f(glyph->text_coords[0].u,glyph->text_coords[1].v);
    glVertex2d(left_top,right_bottom+glyph->height);

    return glyph->orig_width;
}

// render text string
int CFont::RenderText(const char *string){
    unsigned int code=0;
    int dx=0, dy=0, max_dx=0;
    int str_len=strlen(string);
    glBegin(GL_QUADS);
    for(int i = 0; i < str_len; i++){
        if(string[i] == 10){  // new text line
            dy += this->m_font_height;
            if(dx > max_dx) max_dx = dx;
            dx = 0;
            continue;
        }
        code=string[i];
        if(code > 127){ // 2 byte symbol
            i++;
            code <<= 8;
            code += (string[i] & 0xFF);
            code &= 0xFFFF;
        }
        dx += RenderGlyph(dx, dy, &m_chars[GetGlyphId(code)]);
    }
    glEnd();
    if(dx > max_dx) max_dx = dx;
    return max_dx;
}

// render text *string at coordinates x,y. using color color
int CFont::PrintAt(int x, int y, const char *string, Vector4f color){
    int width;
    //unsigned int time_start;
    //time_start=SDL_GetTicks();
    Render->Set2D();
    GL.Disable(GL_LIGHTING);
    Render->SetColor(color.r, color.g, color.b, color.a);
    Render->SetBlendMode(blSource);
    Render->BindTexture(m_texture->GetID());
    // move to coords
    glTranslatef(x,y,0);
    width=RenderText(string);
    // return
    glTranslatef(-x,-y,0);
    //printf("BeginEnd time: %d\n",SDL_GetTicks()-time_start);
    return width;
}

// create text onject
CText::CText(CFont *font, const char *text){
    m_list_id=0;
    m_font=font;
    m_text=NULL;
    if(text){
        SetText(text);
    }
}

void CText::Free(void){
   // Log->puts("Delete display list %d",m_list_id);
   // Log->puts(" and text string (%d bytes)\n",sizeof(m_text));
    if(m_list_id){
        Log->puts("delete Display list %d\n",m_list_id);
        glDeleteLists(m_list_id,  1);
        m_list_id=0;
    }
    if(m_text!=NULL){
        Log->puts("Free text string memory. %d bytes\n",strlen(m_text));
        free(m_text);
        m_text=NULL;
    }
}

void CText::SetText(const char *text){
    Free(); // Free previos str and display list (if needed)
    m_text=(char *)malloc(strlen(text)+1);
    strcpy(m_text,text);
    // generate display list
    m_list_id=glGenLists(1);
    Log->puts("Generate display list %d\n",m_list_id);
    glNewList(m_list_id,GL_COMPILE);
    m_width=m_font->RenderText(text);
    // render text
    glEndList();
}

int CText::PrintAt(int x, int y, Vector4f color){
    //unsigned int time_start;
    //time_start=SDL_GetTicks();
    Render->Set2D();
    GL.Disable(GL_LIGHTING);
    Render->SetColor(color.r, color.g, color.b, color.a);
    Render->SetBlendMode(blSource);
    // move to coords
    glTranslatef(x,y,0);
    glCallList(m_list_id);
    // return
    glTranslatef(-x,-y,0);
    //printf("DL time: %d\n",SDL_GetTicks()-time_start);
    return m_width;
}

CText::~CText(){
    Free();
}
