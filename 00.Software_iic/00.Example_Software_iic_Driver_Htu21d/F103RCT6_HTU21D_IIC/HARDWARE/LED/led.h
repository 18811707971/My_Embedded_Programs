#ifndef __LED__H__
#define __LED__H__ 

#include "sys.h"

#define LED0 PAout(8)
#define LED1 PDout(2)

void LED_Configure(void);

#endif
