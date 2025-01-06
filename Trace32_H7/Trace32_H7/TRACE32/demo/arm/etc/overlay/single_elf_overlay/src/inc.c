/* Function inc() only available if overlay segment "page2" is active */
__attribute__ ((section (".page2")))
void inc(int* x)
{
	(*x)++;
}

