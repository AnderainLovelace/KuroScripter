#ifndef _TAR_READER_H_
#define _TAR_READER_H_

#include "stdio.h"

#define TAR_SUCCESS         0
#define TAR_CANNOTOPENTAR   1
#define TAR_FILENOTFOUND    2

#define TAR_FILE            FILE
#define Tar_Open(fileName)  (fopen(fileName,"rb"))
#define Tar_Close           fclose

void * Tar_GetInTarFile (TAR_FILE * file,const char * fileName,size_t * pSize,int * result);

#endif