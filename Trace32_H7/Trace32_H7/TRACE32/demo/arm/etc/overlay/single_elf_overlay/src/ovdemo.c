extern char* memcopy(char *dst, const char *src, int n);
extern void  SetContextID(const int id);

typedef struct { char* execaddr; char* loadstartaddr; char* loadstoptaddr; } ovpage;

#ifdef __GNUC__
	extern char __PAGE[];
	extern char __load_start_page1[];
	extern char __load_start_page2[];
	extern char __load_stop_page1[];
	extern char __load_stop_page2[];
	const ovpage Page1 = { __PAGE, __load_start_page1, __load_stop_page1 };
	const ovpage Page2 = { __PAGE, __load_start_page2, __load_stop_page2 };
#else
	extern const ovpage Page1;
	extern const ovpage Page2;
	/* Page1 and Page2 are defined in crt0.asm */
#endif


static void load_page( const ovpage *p )
{
	int size = p->loadstoptaddr - p->loadstartaddr;
	memcopy( p->execaddr, p->loadstartaddr, size );
	/* If your CPU has cache, flush you data cache and invalidate your instruction cache here. */
	SetContextID(*(int*)(p->execaddr));
}


extern void main1(void);
extern void main2(void);

int main()
{
	SetContextID(0);
	while(1)
	{
		load_page(&Page1);
		main1();

		load_page(&Page2);
		main2();
	}
}

