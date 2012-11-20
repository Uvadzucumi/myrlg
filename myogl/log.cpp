
#include "log.h"
#include<stdarg.h>

using namespace MyOGL;

CLog *MyOGL::Log=NULL;

void CLog::Init(const char *file_name){
    strcpy(m_file_name,file_name);
#ifdef ENABLE_LOG_FILE
    remove(m_file_name);    // remove old log file
    printf("Log file name: %s\n",m_file_name);
#endif
}

bool CLog::open_file(void){
    fp=stdout;
#ifdef ENABLE_LOG_FILE
    if(!(fp=fopen(m_file_name,"a+"))){
        printf("Error open log file: %s\n",m_file_name);
        return false;
    }
#endif
    return true;
}

void CLog::close_file(void){
#ifdef ENABLE_LOG_FILE
    fclose(fp);
#endif
}

void CLog::puts(const char *str){
    if(open_file()){
        //fprintf(fp,str);
        fputs(str,fp);
        close_file();
    }
}

void CLog::puts(const char *str, const char *str2){
    if(open_file()){
        fprintf(fp,str,str2);
        close_file();
    }
}

void CLog::puts(const char *str, int num){
    if(open_file()){
        fprintf(fp,str,num);
        close_file();
    }
}

void CLog::puts(const char *str, int num, int num2){
    if(open_file()){
        fprintf(fp,str,num, num2);
        close_file();
    }
}

void CLog::puts(const char *str, int num, const char *str2){
    if(open_file()){
        fprintf(fp,str,num, str2);
        close_file();
    }
}

void CLog::puts(const char *str, Vector4f *color){
    if(open_file()){
        fprintf(fp,str,color->r, color->g, color->b, color->a);
        close_file();
    }
}
/*
void CLog::printf(char *format, ...){
    FILE *fp;
    puts("Запись в log.txt, printf ");
    if(!(fp=fopen("log.txt","w+"))){
       puts("Ошибка открытия log.txt\n");
    }else{
        va_list ap;
        char *p, *sval;
        int ival;
        double dval;

        va_start(ap, format);
        for (p = format; *p; p++) {
            if (*p != '%') {
            putchar(*p);
            //    fputc(*p,fp);
                continue;
            }
            switch (*++p) {
                case 'd':
                    ival = va_arg(ap, int);
            printf("%d", ival);
            //        fprintf(fp,"%d",ival);
                    break;
                case 'f':
                    dval = va_arg(ap, double);
            printf("%f", dval);
            //        fprintf(fp,"%f", dval);
                    break;
                case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
                putchar(*sval);
            //        sval = va_arg(ap, char *);
            //        fputs(sval,fp);
                    break;
                default:
            putchar(*p);
            //        fputc(*p,fp);
                    break;
            }
        }
        va_end(ap);
        puts("wite success\n");
        fclose(fp);
    }
}
*/
