#define SIZE 18
char flags[SIZE+1];

static int sieve(void)      /* sieve of erathostenes */
{
  register int    i, prime, k;
  int             count;

  count = 0;

  for ( i = 0 ; i <= SIZE ; flags[ i++ ] = true ) ;

  for (i = 0; i <= SIZE; i++) {
    if (flags[i]) {
      prime = i + i + 3;
      k = i + prime;
      while (k <= SIZE) {
        flags[k] = false;
        k += prime;
      }
      count++;
    }
  }

  return count;
}


unsigned char Sinewave[256];
int samples[] = { 16, 64, 128, 255 };

void func_sin(unsigned int sam)
{
  unsigned char x,y,i,upper,down;

  for(x=0; x<255; x++) Sinewave[x] = 0;

  for(x=0,i=10,upper=255,down=1; x<samples[sam]; x++) {
    if(upper)
      y = 200 - (i*i);
    else
      y = i * i;

    Sinewave[x] = y;
    
    if (down) {
      if (!i) {
        i++;
        down = 0;
      }
      else
        i--;
    }
    else {
      if (i==10) {
        i--;
        down = 1;
        upper = ~upper;
      }
      else
        i++;
    }
  }
}

static int flag_func_sin = 1;
static int sam = 0;

void sieve_main()
{
  sieve();
    
  if (flag_func_sin) {
      sam++;
      if (sam>3)
        sam=0;
      func_sin(sam);
      }

}
