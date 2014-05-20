/*!
 * @file tar_reader.c
 *
 * @brief 简易的tar档读取器
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-11
 */
#include "kuro.h"
#include "tar_reader.h"

#define BLOCK_SIZE 512

typedef unsigned char byte;

typedef struct tag_TAR_HEADER {
    char fileName[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char fileSize[12];
    char lastModification[12];
    char checksum[8];
    char typeFlag;
    char linkedFileName[100];
    char ustarIndicator[6];
    char ustarVersion[2];
    char ownerUserName[32];
    char ownerGroupName[32];
    char deviceMajorNumber[8];
    char deviceMinorNumber[8];
    char filenamePrefix[155];
    char padding[12];
}
TAR_HEADER;

/* 八进制字符串转整数 */
static size_t oatol (const char * str) {
    size_t t = 0,mul = 0;
    char * p = strchr(str,'\0') - 1;
    while (p >= str) {
        t += (*p - '0') << mul;
        mul += 3; // *=8
        p--;
    }
    return t;
}

static size_t getHeaderSize (const TAR_HEADER * header) {
    char szNum[12],*ptr = szNum;
    strncpy(szNum,header->fileSize,12);
    szNum[11] = '\0';
    while (!isdigit(*ptr)) {
        ++ptr;
    }
    return oatol(ptr);
}
/*!
 * @biref 从tar档中获取指定文件的数据
 * @param file TAR文件的文件指针
 * @param fileName tar中文件的文件名
 * @param pSize 获取的文件大小
 * @param result 读取结果
 *
 * @return 读取的数据
 * @retval NULL 读取失败时返回NULL
 */
void * Tar_GetInTarFile (TAR_FILE * file,const char * fileName,size_t * pSize,int * result) {
    TAR_HEADER  header;
    fseek(file,0,SEEK_SET);
    if (file == NULL) {
        /*无法打开 */
        if (result != NULL)
            *result = TAR_CANNOTOPENTAR;
        return NULL;
    }
    /* 开始搜索文件 */
    while (!feof(file)) {
        int     bytesToRead;
        /* 读取头部 */
        fread(&header,sizeof(header),1,file);
        /* 空的文件名（尾部空block）读取结束 */
        if (strcmp(header.fileName,"") == 0) {
            break;
        }
        bytesToRead = getHeaderSize(&header);
        /* 跳过文件夹 */
        if (bytesToRead == 0) {
            continue;
        }
        /* 找到指定文件 */
        if (strcmp(header.fileName,fileName) == 0) {
            byte *  data = (byte*)malloc(bytesToRead);
            byte *  pdata = data;
            if (pSize != NULL) {
                *pSize = bytesToRead;
            }
            while (!feof(file) && bytesToRead > 0) {
                byte    buffer[BLOCK_SIZE];
                size_t  thisBlockRead = bytesToRead > BLOCK_SIZE ? BLOCK_SIZE : bytesToRead;
                fread(buffer,BLOCK_SIZE,sizeof(byte),file);
                memcpy(pdata,buffer,thisBlockRead);
                pdata += thisBlockRead;
                bytesToRead -= BLOCK_SIZE;
            }
            if (result != NULL)
                *result = TAR_SUCCESS;
            return data;
        }
        /* 非指定文件，跳过 */
        else {
            int totalSize;
            if (bytesToRead % BLOCK_SIZE == 0) {
                totalSize = bytesToRead;
            }
            else {
                totalSize = (bytesToRead / BLOCK_SIZE + 1) * BLOCK_SIZE;
            }
            fseek(file,totalSize,SEEK_CUR);
        }
    }
    if (result != NULL)
        *result = TAR_FILENOTFOUND;
    return NULL;
}