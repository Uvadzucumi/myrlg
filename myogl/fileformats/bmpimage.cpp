
#include "bmpimage.h"
#include <string.h> // strlen, strcpy
#include "../log.h"
#include <stdlib.h> // for abs()

using namespace MyOGL;

// calculate & return number of enabled bits in byte
short CBitMapImage::CountBits(unsigned char byte){
    short bits=0;
    for(int i=0;i<8;i++){
        if(byte%2!=0){
            bits++;
        }
        byte>>=1;
    }
    return bits;
}

// calculate & return Shift value from Mask Value
short CBitMapImage::ShiftCount(long int Mask){
    if(!Mask) return 0;
    int tmp=0;
    while(Mask%2 == 0){
        tmp++;
        Mask>>=1;
    }
    return tmp - (8 - CountBits(Mask && 0xFF));
}

// load bitmap image from file
bool CBitMapImage::LoadFromFile(const char *file_name){
    m_file_name=new char[strlen(file_name)];
    strcpy(m_file_name,file_name);

    FILE *fp = fopen(file_name, "rb");
    if (fp==NULL) { Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Can't open %s\n", file_name); return false; }

    // set the file pointer to end of file
    fseek(fp, 0, SEEK_END);
    // get file size
    int file_size=ftell(fp);
    // set the file pointer to start of file
    //fseek( fp, 0, SEEK_SET );
    rewind(fp);

    // read file header
    if(fread(&m_file_header,1,sizeof(m_file_header),fp)!=sizeof(m_file_header)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Rread bitmap File header error!\n",m_file_name);
        fclose(fp);
        return false;
    };
    // check bitmap and file size
    if(m_file_header.magic[0]!='B' || m_file_header.magic[1]!='M'){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Wrong BMP File Format!\n",m_file_name);
        fclose(fp);
        return false;
    }
    // check bmp file size
    // check file size & readed from header
    if(m_file_header.file_size!=file_size){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Wrong BMP File Header Size! header.size:%d != filesystem.size:%d\n",file_name, m_file_header.file_size,file_size);
        fclose(fp);
        return false;
    }

    // read bitmap info header
    if(fread(&m_info_header,1,sizeof(m_info_header),fp)!=sizeof(m_info_header)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Rread bitmap Info header error!\n",file_name);
        fclose(fp);
        return false;
    }
    fseek( fp, sizeof(m_file_header)+m_info_header.size, SEEK_SET ); // if info header not 40 bytes
// check compression
    if(
       (m_info_header.compression==BI_RLE4 && m_info_header.bpp!=4) ||
       (m_info_header.compression==BI_RLE8 && m_info_header.bpp!=8) ||
       (m_info_header.compression==BI_BITFIELDS && ( m_info_header.bpp!=16 && m_info_header.bpp!=32)) ||
       (m_info_header.compression!=BI_RGB && m_info_header.compression!=BI_RLE4 && m_info_header.compression!=BI_RLE8 && m_info_header.compression!=BI_BITFIELDS)
       ){ // bad compression
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Bad BMP Compression method %d!\n", file_name, m_info_header.compression);
        fclose(fp);
        return false;
    }
    m_top_down=(m_info_header.height < 0);
    m_info_header.height=abs(m_info_header.height);
    if(m_top_down && (m_info_header.compression != BI_RGB || m_info_header.compression != BI_BITFIELDS)){
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") top down bmp cannot be compressed!\n",m_file_name);
        fclose(fp);
        return false;
    }

    if(m_info_header.compression == BI_RGB && m_info_header.bpp==16){ // 5 bits per channel, fixed mask
        Log->puts("DEBUG: Fixed map BMP image (5 bits)\n");
        RedMask = 0x7C00;
        RedShift = 7;
        GreenMask = 0x03E0;
        GreenShift = 2;
        BlueMask = 0x001F;
        BlueShift = -3;
    }else if(m_info_header.compression == BI_BITFIELDS && (m_info_header.bpp == 16 || m_info_header.bpp == 32)){ // arbitrary mask
        Log->puts("DEBUG: Arbitrary map BMP image\n");
        // read mask from file
        fread(&RedMask,1,4,fp);
        fread(&GreenMask,1,4,fp);
        fread(&BlueMask,1,4,fp);
        RedShift = ShiftCount(RedMask);
        GreenShift = ShiftCount(GreenMask);
        BlueShift = ShiftCount(BlueMask);
    }else if(m_info_header.bpp==1 || m_info_header.bpp==4 || m_info_header.bpp==8){
        Log->puts("DEBUG: BMP with palette\n");
        m_palette_size=1 << m_info_header.bpp;
        Log->puts("DEBUG: Palette seize=%d\n", m_palette_size);
        m_palette=new GLPixel32 [m_palette_size];
        // read palette
        if(m_info_header.colors_used){
            // read colors
            fread(&m_palette,m_info_header.colors_used,sizeof(GLPixel32),fp);
        }else{ // read colors (numbers - calculated)
            Log->printf("Warning: used calculated palette size!");
            fread(&m_palette,m_palette_size,sizeof(GLPixel32),fp);
        }
    }else if(m_info_header.colors_used){ // skip palette
        Log->printf("Warning: skip palette from BMP file. old pos: %d new pos: %d\n", ftell(fp), ftell(fp)+m_info_header.colors_used*3);
        fseek(fp, ftell(fp)+m_info_header.colors_used*3, SEEK_SET);
    }

// file header debug
    Log->printf("DEBUG: file_name=%s file_size=%d\n", m_file_name, m_file_header.file_size);
    Log->printf("DEBUG: magic=%c%c offset=%d\n",m_file_header.magic[0],m_file_header.magic[1],m_file_header.offset);
// info header debug
    Log->printf("DEBUG: if.size=%d img.w=%d img.h=%d img.planes=%d img.bpp=%d\n",m_info_header.size,
              m_info_header.width, m_info_header.height, m_info_header.planes, m_info_header.bpp);
    Log->printf("DEBUG: if.compression=%d if.image_size=%d if.x_ppm=%d if.y_ppm=%d\n",
                m_info_header.compression, m_info_header.image_size, m_info_header.x_pixs_per_meter, m_info_header.y_pixs_per_meter);
    Log->printf("DEBUG: if.colors_used=%d if.colors_important=%d\n", m_info_header.colors_used, m_info_header.colors_important);

    // read data (now only BMP32)
    if(m_info_header.bpp==32){
        m_pixel_format=GL_BGRA;
        m_bytes_prer_pixel=4;
        int scanline_size=m_bytes_prer_pixel*m_info_header.width;
        int pixel_data_size=scanline_size*m_info_header.height;
    // allocate mem
        if(m_pixel_data){ // free mem from old data
            delete m_pixel_data;
        }
        m_pixel_data=new unsigned char [pixel_data_size];
        //unsigned char* read_buffer=new unsigned char [scanline_size];
        for(int row=m_info_header.height-1; row >= 0; row--){
            if(fread(&m_pixel_data[row*scanline_size], 1, scanline_size, fp) != scanline_size){
                Log->printf("Error read scan line # %d\n", m_info_header.height-row);
            };
            //memcpy(&m_pixel_data[row*scanline_size], read_buffer, scanline_size);
        }
        //memcpy(dest,src,read_bytes);
        //delete read_buffer; // free line buffer
    }else{
        Log->printf("ERROR: CBitMapImage::LoadFromFile(\"%s\") Unsupported bitmap Format!\n",m_file_name);
        fclose(fp);
        return false;
    }
    fclose(fp);
    Log->printf("DEBUG: BMP FILE READED!\n");
    return true;
}

// copy pixel data to data pointer and return address
unsigned char* CBitMapImage::GetPixelData(unsigned char *data){
    if(data!=NULL){
        Log->puts("Warning: CBitMapImage::GetPixelData() pinter not NULL, previous data deleted\n");
        delete data;
    }
    int pixel_data_size=m_bytes_prer_pixel*m_info_header.width*m_info_header.height;
    // allocate mem
    data=new unsigned char [pixel_data_size];
    memcpy(data,m_pixel_data,pixel_data_size);
    return data;
}
