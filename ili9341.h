/* **********************************************************************
 * Author: Jks Liu(chinatianma@gmail.com)
 * Date:
 * ***********************************************************************/

#ifndef ILI9341_ILI9341_H_
#define ILI9341_ILI9341_H_

#ifdef __SDCC
  /* http://sdcc.sourceforge.net/ */
  #ifndef code
    #define code __code
  #endif
  #include <mcs51/at89x52.h>
  #include <stdint.h>
  #include <stdbool.h>
#elif defined(__C51__) 
  /* Keil C51 */
  #include <atmel/at89x52.h>
  /* Keil C51 does not support c99 */
  #ifndef STDINT_AND_STDBOOL_DEFINED_
  #define STDINT_AND_STDBOOL_DEFINED_
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned long uint32_t;
    typedef bit bool;
    #define true 1
    #define false 0
  #endif
#else
  #include <stdint.h>
  #include <stdbool.h>
#endif

#include "grlib/grlib.h"

#define DC_BASE GPIO_PORTA_BASE
#define DC_PIN  GPIO_PIN_6
#define RESET_BASE GPIO_PORTA_BASE
#define RESET_PIN  GPIO_PIN_7

#define COLUMN_ADDRESS_SET 0x2a
#define PAGE_ADDRESS_SET   0x2b
#define MEMORY_WRITE       0x2c
#define NO_OPERATION       0x00

extern const tDisplay kILI9341;

#ifdef __cplusplus
extern "C"
{
#endif
#if 0
}
#endif 

void lcd_initial(void);

void horizontal_line(void *t,
                     long x1, long x2, long y1,
                     unsigned long color);
void vertical_line(void *t,
                   long x1, long y1, long y2,
                   unsigned long color);
void pixel(void *t, long x, long y, unsigned long color);


#if 0
{
#endif
#ifdef __cplusplus
}
#endif

#endif  

