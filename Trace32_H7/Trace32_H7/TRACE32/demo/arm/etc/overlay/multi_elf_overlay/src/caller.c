
typedef void (*tFuncPtr)(void);

tFuncPtr page1 = (tFuncPtr)0x1400;
tFuncPtr page2 = (tFuncPtr)0x2400;

int main()
{
	while(1)
	{
		page1();
		page2();
	}
}

