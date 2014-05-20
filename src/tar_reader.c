/*!
 * @file tar_reader.c
 *
 * @brief ���׵�tar����ȡ��
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

/* �˽����ַ���ת���� */
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
 * @biref ��tar���л�ȡָ���ļ�������
 * @param file TAR�ļ����ļ�ָ��
 * @param fileName tar���ļ����ļ���
 * @param pSize ��ȡ���ļ���С
 * @param result ��ȡ���
 *
 * @return ��ȡ������
 * @retval NULL ��ȡʧ��ʱ����NULL
 */
void * Tar_GetInTarFile (TAR_FILE * file,const char * fileName,size_t * pSize,int * result) {
    TAR_HEADER  header;
    fseek(file,0,SEEK_SET);
    if (file == NULL) {
        /*�޷��� */
        if (result != NULL)
            *result = TAR_CANNOTOPENTAR;
        return NULL;
    }
    /* ��ʼ�����ļ� */
    while (!feof(file)) {
        int     bytesToRead;
        /* ��ȡͷ�� */
        fread(&header,sizeof(header),1,file);
        /* �յ��ļ�����β����block����ȡ���� */
        if (strcmp(header.fileName,"") == 0) {
            break;
        }
        bytesToRead = getHeaderSize(&header);
        /* �����ļ��� */
        if (bytesToRead == 0) {
            continue;
        }
        /* �ҵ�ָ���ļ� */
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
        /* ��ָ���ļ������� */
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