/* Example of code linked to different overlay pages in one source file.
 * You have to keep the relocation tables in your final executable ELF file to be able to debug this.
 * (Use linker options --emit-debug-overlay-section with ARM RealView compiler.)
 * (Use linker options --emit-relocs with GNU GCC compiler.)
 */


/* Function func5() only available if overlay segment "page1" is active */
__attribute__ ((section (".page1")))
int func5( int a, char b, long c )
{
	return a+b*c;
}


/* Function inc() only available if overlay segment "page2" is active */
__attribute__ ((section (".page2")))
void inc(int* x)
{
	(*x)++;
}


/* Function memcopy() always available */
char* memcopy(char *dst, const char *src, int n)
{
	for(;n;n--) {
		*dst = *src;
		dst++;
		src++;
	}
	return dst;
}


/* Function func11() only available if overlay segment "page1" is active */
__attribute__ ((section (".page1")))
int func11( int x )
{
	switch ( x )
	{
		case 1:
			x = x+1;
			x = x*2;
			return x*x;
		case 2:
			return x+x;
		case 3:
			return x-x;
		case 4:
			x = x+1;
			x = x*2;
			return x*x;
		case 5:
			break;
		case 6:
			return x+x;
		default:
			break;
	}
	return x;
}
