#include "kuro.h"


double myatof(const char* sptr)
{
    double  temp = 10;
    BOOL    ispnum = TRUE;
    double  ans = 0;
    
    if(*sptr=='-') {
        ispnum=FALSE;
        sptr++;
    }
    else if(*sptr=='+') {
        sptr++;
    }
    while(*sptr!='\0') {
        if(*sptr=='.') {
            sptr++;
            break;
        }
        ans = ans * 10 + (*sptr - '0');
        sptr++;
    }
    while(*sptr!='\0') {
        ans = ans + (*sptr - '0') / temp;
        temp *= 10;
        sptr++;
    }
    if(ispnum)
        return ans;
    else
        return -ans;
}

char * mystrdup(const char * src) {
    char * str = (char*)malloc(strlen(src)+1);
    strcpy(str,src);
    return str;
}

#ifdef ___NDLESS___

static unsigned int* p_contrast = (unsigned int*) 0x900F0020;

void ContrastInc () {
    if( is_cx && (*p_contrast)< 225)
        (*p_contrast)++;
    else if( (!is_cx) && (*p_contrast) < 0xc0 )
    (*p_contrast)++;
}

void ContrastDec () 
{
    if( is_cx && (*p_contrast)> 1 )
        (*p_contrast)--;
        else if( (!is_cx) && (*p_contrast) >50 )
    (*p_contrast)--;
}

#endif