
extern unsigned int T32_Dcc_SendStatus(void);
extern void T32_Dcc_SendWord(unsigned int data);
extern unsigned int T32_Dcc_ReceiveStatus(void);
extern unsigned int T32_Dcc_ReceiveWord(void);

void T32_Term_PutBlock(const unsigned char * data, int block_size) {
	int i = 0;
	unsigned int value;

	while (i < block_size) {
		while (T32_Dcc_SendStatus());
#if defined(T32_DCC)
		value = *data++; i++;
#elif defined(T32_DCC3)
		value = (*data++); i++;
		if (i < block_size) {value += (0x01000000|((*data++)<< 8)); i++;}
		if (i < block_size) {value += (0x01000000|((*data++)<<16)); i++;}
#elif defined(T32_DCC4A)
		value = (*data++); i++;
		if (i < block_size) {value |= (*data++)<< 8; i++;}
		if (i < block_size) {value |= (*data++)<<16; i++;}
		if (i < block_size) {value |= (*data++)<<24; i++;}
#endif
		T32_Dcc_SendWord(value);
	}
}

void T32_Term_GetBlock(unsigned char * data, int size) {
	static char pRingBuffer[4];
	static unsigned int nReadPos  = 0;
	static unsigned int nWritePos = 1;
	int i = 0;
	unsigned int value;
	
	while (i < size) {
		if ((((nWritePos-nReadPos)&0x7) > 1)) {
			nReadPos = (nReadPos+1)&0x7;
			*data++ = pRingBuffer[nReadPos&0x3];
			i++;
		}
		else {
			while (!T32_Dcc_ReceiveStatus());
			value = T32_Dcc_ReceiveWord();
			
#if defined(T32_DCC)
			pRingBuffer[nWritePos&0x3] = (char)(value&0xff);
			nWritePos = (nWritePos+1)&0x7;
#elif defined(T32_DCC3)
			switch((value>>24)&0x3) {
			case 2:
				pRingBuffer[(nWritePos+2)&0x3] = (char)((value>>16)&0xff);
			case 1:
				pRingBuffer[(nWritePos+1)&0x3] = (char)((value>> 8)&0xff);
			case 0:
				pRingBuffer[(nWritePos+0)&0x3] = (char)(value&0xff);
				nWritePos = (nWritePos+((value>>24)&0x3)+1)&0x7;
				break;
			default:
				break;
			}
#elif defined(T32_DCC4A)
			pRingBuffer[nWritePos&0x3] = (char)(value&0xff); nWritePos = (nWritePos+1)&0x7; value >>= 8;
			if (value&0xff) {pRingBuffer[nWritePos&0x3] = (char)(value&0xff); nWritePos = (nWritePos+1)&0x7; value >>= 8;}
			if (value&0xff) {pRingBuffer[nWritePos&0x3] = (char)(value&0xff); nWritePos = (nWritePos+1)&0x7; value >>= 8;}
			if (value&0xff) {pRingBuffer[nWritePos&0x3] = (char)(value&0xff); nWritePos = (nWritePos+1)&0x7; value >>= 8;}
#endif
		}
	}
}
