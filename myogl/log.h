#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "config.h" // enable/disable create log file
#include "vector_types.h"

namespace MyOGL{

class CLog{
        char m_file_name[100];
        FILE *fp;
    public:
        void Init(const char *file_name);
        void puts(const char *str);
        void puts(const char *str, const char *str2);
        void puts(const char *str, int num);
        void puts(const char *str, int num, int num2);
        void puts(const char *str, int num, const char *str2);
        void puts(const char *str, Vector4f *color);
        bool open_file(void);
        void close_file(void);
};

    extern CLog *Log;

}

#endif // LOG_H_INCLUDED
