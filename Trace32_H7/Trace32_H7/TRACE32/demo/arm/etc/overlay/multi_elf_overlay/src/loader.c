static void* Memset(char *s, int c, int n)
{
	for (;n;n--)
		*(s++) = (char) c;
	return s;
}


static void* Memcpy(char *s1, const char *s2, int n)
{

	for (;n>3;n-=4) {
		*(unsigned int*)s1 = *(unsigned int*)s2;
		s1+=4;
		s2+=4;
	}

	for (;n;n--) {
		*s1 = *s2;
		s1++;
		s2++;
	}
	return s1;
}


#if !defined(__GNUC__)  /* ARM RealView */
#	define _data_store  Load$$DATA$$Base
#	define _data_start  Image$$DATA$$Base
#	define _data_length Image$$DATA$$Length
#	define _bss_store   Load$$BSS$$Base
#	define _bss_start   Image$$BSS$$ZI$$Base
#	define _bss_length  Image$$BSS$$ZI$$Length
#	define _text_store  Load$$TEXT$$Base
#	define _text_start  Image$$TEXT$$Base
#	define _text_length Image$$TEXT$$Length
#endif


extern void SetContextID(const int id);
extern int  main(void);

extern volatile unsigned int id;
extern char _text_store[];
extern char _data_store[];
extern char _bss_store[];
extern char _text_start[];
extern char _data_start[];
extern char _bss_start[];
extern char _text_length[];
extern char _data_length[];
extern char _bss_length[];

volatile static unsigned int segment = 0;

__attribute__ ((section (".init"))) int init(void)
{
	int result;
	static int init = 1;

	if ( init ) {
		Memset(_bss_start, 0, (int)_bss_length); /* clear BSS Data */
		init = 0;
	} else {
		Memcpy(_bss_start, _bss_store, (int)_bss_length  ); /* Load BSS Data */
	}

	Memcpy(_data_start,   _data_store,   (int)_data_length   ); /* Load Data */
	Memcpy(_text_start+4, _text_store+4, (int)_text_length-4 ); /* Load Code */
	*(unsigned int*) _text_start = *(unsigned int*) _text_store;

	/*
	 * If your CPU has cache, FLUSH YOUR DATA CACHE AND INVALIDATE YOUR INSTRUCTION CACHE HERE.
	 */

	SetContextID(segment ? segment : id); /* Write ID of section to ContexID register (only required for flow trace) */
	result = main();
	SetContextID(1); /* clear ContexID (optional) */

	id = 0; /* set ID to zero to show that page is no longer active (optional) */
	Memcpy(_data_store, _data_start, (int)_data_length );  /* Save Data */
	Memcpy(_bss_store,  _bss_start,  (int)_bss_length );   /* Save BSS Data */

	return result;
}

