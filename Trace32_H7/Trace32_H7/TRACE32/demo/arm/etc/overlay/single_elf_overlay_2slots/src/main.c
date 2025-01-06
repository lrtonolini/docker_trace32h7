#include "defines.h"


#ifdef __GNUC__
	extern char __PAGE_SLOT_A[], __PAGE_SLOT_B[];
	extern char __load_start_pageA0[], __load_stop_pageA0[];
	extern char __load_start_pageA1[], __load_stop_pageA1[];
	extern char __load_start_pageB0[], __load_stop_pageB0[];
	extern char __load_start_pageB1[], __load_stop_pageB1[];
	const ovlpage_t PageA0 = { __PAGE_SLOT_A, __load_start_pageA0, __load_stop_pageA0, 0x01, 0x0f };
	const ovlpage_t PageA1 = { __PAGE_SLOT_A, __load_start_pageA1, __load_stop_pageA1, 0x02, 0x0f };
	const ovlpage_t PageB0 = { __PAGE_SLOT_B, __load_start_pageB0, __load_stop_pageB0, 0x10, 0xf0 };
	const ovlpage_t PageB1 = { __PAGE_SLOT_B, __load_start_pageB1, __load_stop_pageB1, 0x20, 0xf0 };
#endif


uint32_t OverlayContext = 0;


char* strcpy( char *dst, const char *src)
{
	while (*src != '\0') {
		*dst = *src;
		dst++;
		src++;
	}
	return dst;
}


char* memcopy(char *dst, const char *src, int n)
{
	for (; n; n--) {
		*dst = *src;
		dst++;
		src++;
	}
	return dst;
}


void LoadOverlayPage(const ovlpage_t *p)
{
	int      size = p->loadstoptaddr - p->loadstartaddr;
	uint32_t cid;

	memcopy( p->execaddr, p->loadstartaddr, size );
	/* If your CPU has cache, flush you data cache and invalidate your instruction cache here. */

	cid = GetContextID();
	cid = (cid & ~(p->mask)) | (p->id & p->mask);
	SetContextID(cid);

	OverlayContext = cid; // Optional: Write ContextID to a global variable	(required in case of using data trace)
}


void LoadSlotA(int ovl)
{
	LoadOverlayPage(ovl ? &PageA1 : &PageA0);
}


void LoadSlotB(int ovl)
{
	LoadOverlayPage(ovl ? &PageB1 : &PageB0);
}


static int mstatic1, mcount = 0, vint = 1;

static char gText1[16] = "";
static char gText2[32] = "";
static char gText3[32] = "";


int main(void)
{
	mstatic1 = 0;
	SetContextID(0);

	LoadSlotA(0);
	LoadSlotB(0);

	do {
		strcpy(gText3, "Total Perspective Vortex");
		funcDT(gText1, gText2);

		LoadSlotA(1);
		encode(gText3);
		inc(&mstatic1);
		LoadSlotA(0);

		func2(&mstatic1);

		LoadSlotB(1);
		sieve();
		LoadSlotB(0);

		mcount++;
	} while (vint);

	return 0;
}


