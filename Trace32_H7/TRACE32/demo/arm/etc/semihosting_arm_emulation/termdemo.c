
#include <stdio.h>
#include <string.h>

void _sys_exit (void)
{
	while (1);
}


int main()
{
	static char hmessage[] = "\r\nHello World!\r\n";
	static char mmessage[] = "\r\nr read file\r\nw write file\r\np performance test\r\n";
	static char pmessage[] = "Performance Test Text         \r\n";
	char buffer[4096];
	int i, ch = 0;
	
	FILE *handle;
	
	puts( hmessage );

	while (1)
	{
		if (ch != '\n')
			puts( mmessage );

		ch = getchar();

		if (ch == 'r' || ch == 'R')
		{
			handle = fopen("test1","r");
			if (handle == NULL) {
				puts( "failed to open file\r\n" );
				continue;
			}
			i = fread(buffer,1, 4096,handle);
			if (i == -1) {
				puts( "failed to read file\r\n" );
				continue;
			}
			buffer[i] = '\0';
			puts(buffer);

			i = fclose(handle);
			if (i == -1) {
				puts( "failed to close file\r\n" );
				continue;
			}
			continue;
		}

		if (ch == 'w' || ch == 'W')
		{
			handle = fopen("test2","w");
			if (handle == NULL) {
				puts( "failed to open file\r\n" );
				continue;
			}
			i = fwrite(hmessage,1, strlen(hmessage), handle);
			if (i == -1) {
				puts( "failed to write file\r\n" );
				continue;
			}

			i = fclose(handle);
			if (i == -1) {
				puts( "failed to close file\r\n" );
				continue;
			}
			continue;
		}

		if (ch == 'p' || ch == 'P')
		{
			for ( i = 0 ; i < 32 ; i++ )
				
				puts( pmessage);
			continue;
		}
	}
}

