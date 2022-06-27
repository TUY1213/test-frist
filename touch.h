#ifndef __TOUCH_H
#define __TOUCH_H

#include "stm32f4xx.h"   
#include "io_bit.h" 
#include "stdio.h" 



//T_SCK£ºPB0  T_PEN£ºPB1   T_MISO£ºPB2    T_CS£ºPC13   T_MOSI£ºPF11
#define T_SCK PBout(0) 
#define T_CS PCout(13) 
#define T_MOSI PFout(11) 

#define T_PEN PBin(1)   
#define T_MISO PBin(2)   

#define X_CMD 0xd0
#define Y_CMD 0x90



void Touch_XPT2046_Initial(void);
u16 Get_XPT2046_xvalueue(void);

#endif

