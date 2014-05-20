/*!
 * @file error.c
 *
 * @brief 错误处理
 * @author hikari <anderain.develop@gmail.com>
 * @date 2014-5-11
 */
#ifdef WIN32
#include "windows.h"
#endif

#include "kuro.h"
#include "stdarg.h"
#include "error.h"

void Error_Exit(const char * format,...)
{
	char		buf[512];
	va_list		arg_list;

	va_start	(arg_list,format);
	vsprintf	((char*)buf,(char*)format,arg_list);
	va_end		(arg_list);

#ifdef ___NDLESS___
    puts(buf);
#else
#ifdef WIN32
    MessageBox(NULL,buf,"ERROR!",MB_OK);
#else
    puts("ERROR:");
    puts(buf);
#endif
#endif
	exit(0);
}
