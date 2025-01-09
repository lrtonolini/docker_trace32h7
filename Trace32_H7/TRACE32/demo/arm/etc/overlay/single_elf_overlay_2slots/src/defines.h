#include <string.h>
#include <stddef.h>
#include <stdint.h>

typedef struct { char* execaddr; char* loadstartaddr; char* loadstoptaddr; uint32_t id; uint32_t mask;} ovlpage_t;
extern const ovlpage_t PageA0;
extern const ovlpage_t PageA1;
extern const ovlpage_t PageB0;
extern const ovlpage_t PageB1;


extern     void SetContextID(const uint32_t id);
extern uint32_t GetContextID(void);

extern     void LoadOverlayPage(const ovlpage_t *p);
extern     void LoadSlotA(int ovl);
extern     void LoadSlotB(int ovl);

extern      int checkUniv(void);
extern    char* encode(char *str);
extern     void func2(int *var);
extern      int func13( int a, int c, int e );
extern      int func42(char *txt1, char *txt2);
extern     void funcDT(char *txt1, char *txt2);
extern      int getFinalAnswer(int l, int u);
extern      int getLfeInfo(void);
extern      int getMagic(int x);
extern     void inc(int * intptr);
extern      int sieve(void);
extern     char subst(char c);
