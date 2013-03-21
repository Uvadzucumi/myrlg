#ifndef CELL_INFO_H_INCLUDED
#define CELL_INFO_H_INCLUDED

#include "../myogl/ui/panel.h"
#include "../map/level_map.h"

class CUICellInfo : public MyOGL::CUIPanel{
        MyOGL::CFont *m_font;
        sMapField m_field;
        int m_map_tile_x, m_map_tile_y;
    public:
        CUICellInfo(CFont *font):CUIPanel(){
            m_font=font;
        };
        CUICellInfo(int x, int y, int w, int h, CFont *font):CUIPanel(x,y,w,h){
            m_font=font;
        };

        void SetMapTileCoords(int map_tile_x, int map_tile_y){ m_map_tile_x=map_tile_x; m_map_tile_y=map_tile_y;};
        void Render();
};

#endif // CELL_INFO_H_INCLUDED
