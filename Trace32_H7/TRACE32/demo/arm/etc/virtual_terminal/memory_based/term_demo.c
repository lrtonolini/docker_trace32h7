/**************************************************************************

	TRACE32 Semihosting demo

	origin file:
	  files/demo/etc/terminal/t32term/term_demo.c

	interface and possible defines in t32term.h
	
	15/10/12		initial release
		
**************************************************************************/

#include <string.h>
#include "t32term.h"

int main()
{
	static char hmessage[] = "\r\nHello World!\r\n";
	static char mmessage[] = "\r\nr read file\r\nw write file\r\np performance test\r\n";
	static char pmessage[] = "Performance Test Text         \r\n";
#if defined(T32_TERM_BLOCKED)
	char buffer[4096];
#endif
	int i, ch, handle;

	T32_Term_Puts( hmessage );

	while (1)
	{
		T32_Term_Puts( mmessage );

		ch = T32_Term_Getchar();

		if (ch == 'r' || ch == 'R')
		{
#if defined(T32_TERM_BLOCKED)
			handle = T32_Term_OpenFileANSI("test1", T32_TERM_OA_R);
			if (handle == -1) {
				T32_Term_Puts( "failed to open file\r\n" );
				continue;
			}
			i = T32_Term_ReadFile(handle,buffer,4096);
			if (i == -1) {
				T32_Term_Puts( "failed to read file\r\n" );
				continue;
			}
			buffer[i] = '\0';
			T32_Term_Puts(buffer);

			i = T32_Term_CloseFile(handle);
			if (i == -1) {
				T32_Term_Puts( "failed to close file\r\n" );
				continue;
			}
#else /*defined(T32_TERM_BLOCKED)*/
			T32_Term_Puts( "only supported with BLOCK/BUFFER mode\r\n" );
#endif /*defined(T32_TERM_BLOCKED)*/
			continue;
		}

		if (ch == 'w' || ch == 'W')
		{
#if defined(T32_TERM_BLOCKED)
			handle = T32_Term_OpenFileANSI("test2",T32_TERM_OA_W);
			if (handle == -1) {
				T32_Term_Puts( "failed to open file\r\n" );
				continue;
			}
			i = T32_Term_WriteFile(handle,hmessage,strlen(hmessage));
			if (i == -1) {
				T32_Term_Puts( "failed to write file\r\n" );
				continue;
			}

			i = T32_Term_CloseFile(handle);
			if (i == -1) {
				T32_Term_Puts( "failed to close file\r\n" );
				continue;
			}
			continue;
#else /*defined(T32_TERM_BLOCKED)*/
			T32_Term_Puts( "only supported with BLOCK/BUFFER mode\r\n" );
#endif /*defined(T32_TERM_BLOCKED)*/
		}

		if (ch == 'p' || ch == 'P')
		{
			for ( i = 0 ; i < 32 ; i++ )
				T32_Term_PutBlock( (unsigned char *) pmessage, 32 );
			continue;
		}
	}

	return 0;
}

