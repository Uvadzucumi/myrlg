#ifndef BITMAP_FONT_H_INCLUDED
#define BITMAP_FONT_H_INCLUDED

#include "vector_types.h"
#include "texture.h"
#include <vector>

namespace MyOGL{

    struct Glyph{
        unsigned int code;
        Vector2f text_coords[2];
        int width, height, orig_width;
        int x_offset, y_offset;
    };

    class CFont{
            CTexture *m_texture;
            std::vector <Glyph> m_chars;
            int m_font_height;
        public:
            CFont(){m_font_height=-1;}
            ~CFont();
            int RenderGlyph(int x, int y, Glyph *glyph);
            bool LoadGlyphData(const char *file_name, CTexture *fexture);
            void debug(void);
            int PrintAt(int x, int y, const char *string, Vector4f color);
            int GetGlyphId(unsigned int code);
            int RenderText(const char *string);
    };

    class CText{
            GLuint m_list_id;
            CFont *m_font;
            char *m_text;
            int m_width;
        public:
            CText(CFont *font, const char *text=NULL);
            ~CText();
            void SetText(const char *text);
            int PrintAt(int x, int y, Vector4f color);
            void Free(void);
    };

}

#endif // BITMAP_FONT_H_INCLUDED
