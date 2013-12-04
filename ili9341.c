#include "ili9341.h"
#include "delay.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"

#include "utils/uartstdio.h"


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


inline static unsigned long spi_swap(unsigned long c) {
    //
    // Read any residual data from the SSI port.  This makes sure the receive
    // FIFOs are empty, so we don't read any unwanted junk.  This is done here
    // because the SPI SSI mode is full-duplex, which allows you to send and
    // receive at the same time.  The SSIDataGetNonBlocking function returns
    // "true" when data was returned, and "false" when no data was returned.
    // The "non-blocking" function checks if there is any data in the receive
    // FIFO and does not "hang" if there isn't.
    //
    unsigned long tmp;
    while(SSIDataGetNonBlocking(SSI0_BASE, &tmp))
    {
    }

    SSIDataPut(SSI0_BASE, c);
    SSIDataGet(SSI0_BASE, &tmp);
    return tmp;
}

void ili9341_init(void) {
    //
    // The SSI0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    //
    // For this example SSI0 is used with PortA[5:2].  The actual port and pins
    // used may be different on your part, consult the data sheet for more
    // information.  GPIO port A needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    //
    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.  Consult the data sheet to
    // see which functions are allocated per pin.
    // The pins are assigned as follows:
    //      PA5 - SSI0Tx
    //      PA4 - SSI0Rx
    //      PA3 - SSI0Fss
    //      PA2 - SSI0CLK
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 |  GPIO_PIN_3 |
                   GPIO_PIN_2);

    //
    // Configure and enable the SSI port for SPI master mode.  Use SSI0,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 1000000, 8);

    //
    // Enable the SSI0 module.
    //
    SSIEnable(SSI0_BASE);
    
    GPIOPinTypeGPIOOutput(DC_BASE, DC_PIN);
    GPIOPinTypeGPIOOutput(RESET_BASE, RESET_PIN);

    GPIOPinWrite(RESET_BASE, RESET_PIN, 0x00);
    delay_ms(10);    
    GPIOPinWrite(RESET_BASE, RESET_PIN, RESET_PIN);
    delay_ms(10);
}

#define COMMAND_MODE() GPIOPinWrite(DC_BASE, DC_PIN, 0)
#define DATA_MODE()    GPIOPinWrite(DC_BASE, DC_PIN, DC_PIN)
#define spi_write(c)                                    \
    do {                                                \
        unsigned long tmp_;                             \
        SSIDataPut(SSI0_BASE, c);                       \
        SSIDataGet(SSI0_BASE, &tmp_);                   \
    } while (0)

inline static void write_command(unsigned long c) {
    GPIOPinWrite(DC_BASE, DC_PIN, 0);
    spi_swap(c);
}

inline static void write_data(unsigned long d) {
    GPIOPinWrite(DC_BASE, DC_PIN, DC_PIN);
    spi_swap(d);
}

void wr_com16(unsigned long i,unsigned long j) {
    write_data(i);
    write_data(j);
}

void lcd_initial(void)
{
    ili9341_init();
    
    write_command(0xCB);  
    write_data(0x39); 
    write_data(0x2C); 
    write_data(0x00); 
    write_data(0x34); 
    write_data(0x02); 
    
    write_command(0xCF);  
    write_data(0x00); 
    write_data(0XC1); 
    write_data(0X30); 
    
    write_command(0xE8);  
    write_data(0x85); 
    write_data(0x00); 
    write_data(0x78); 
    
    write_command(0xEA);  
    write_data(0x00); 
    write_data(0x00); 
        
    write_command(0xED);  
    write_data(0x64); 
    write_data(0x03); 
    write_data(0X12); 
    write_data(0X81); 
    
    write_command(0xF7);  
    write_data(0x20); 
        
    write_command(0xC0);    //Power control 
    write_data(0x23);   //VRH[5:0] 
 
    write_command(0xC1);    //Power control 
    write_data(0x10);   //SAP[2:0];BT[3:0] 
 
    write_command(0xC5);    //VCM control 
    write_data(0x3e); //对比度调节
    write_data(0x28); 
 
    write_command(0xC7);    //VCM control2 
    write_data(0x86);  //--
 
    write_command(0x36);    // Memory Access Control 
    write_data(0x48); //C8	   //48 68竖屏//28 E8 横屏
    //write_data(0x40);
    
    write_command(0x3A);    
    write_data(0x55); 
    
    write_command(0xB1);    
    write_data(0x00);  
    write_data(0x18); 
 
    write_command(0xB6);    // Display Function Control 
    write_data(0x08); 
    write_data(0x82);
    write_data(0x27);  
    
    write_command(0xF2);    // 3Gamma Function Disable 
    write_data(0x00); 
    
    write_command(0x26);    //Gamma curve selected 
    write_data(0x01); 
    
    write_command(0xE0);    //Set Gamma 
    write_data(0x0F); 
    write_data(0x31); 
    write_data(0x2B); 
    write_data(0x0C); 
    write_data(0x0E); 
    write_data(0x08); 
    write_data(0x4E); 
    write_data(0xF1); 
    write_data(0x37); 
    write_data(0x07); 
    write_data(0x10); 
    write_data(0x03); 
    write_data(0x0E); 
    write_data(0x09); 
    write_data(0x00); 
    
    write_command(0XE1);    //Set Gamma 
    write_data(0x00); 
    write_data(0x0E); 
    write_data(0x14); 
    write_data(0x03); 
    write_data(0x11); 
    write_data(0x07); 
    write_data(0x31); 
    write_data(0xC1); 
    write_data(0x48); 
    write_data(0x08); 
    write_data(0x0F); 
    write_data(0x0C); 
    write_data(0x31); 
    write_data(0x36); 
    write_data(0x0F); 
    
    write_command(0x11);    //Exit Sleep 
    delay_ms(120); 
				
    write_command(0x29);    //Display on 
    write_command(0x2c); 

}

void addset(unsigned int x,unsigned int y)
{
    write_command(0x2a);
    wr_com16(x>>8,x);
    write_command(0x2b);
    wr_com16(y>>8,y);
    write_command(0x2c);
}

void dsp_single_colour(unsigned char DH, unsigned char DL)
{
    unsigned int i,j;
    addset(0,0);
    for (i=0;i<320;i++)
        for (j=0;j<240;j++)
            wr_com16(DH,DL);    

}

unsigned long rgb888_to_565(void *t, unsigned long v888) {
    return ((v888 & 0xf80000ul) >> 8) |
           ((v888 & 0x00fc00ul) >> 5) |
           ((v888 & 0x0000f8ul) >> 3);
}

void flush(void *t) {
    /* Empty */
}

inline static void set_area(uint16_t x0, uint16_t y0,
                            uint16_t x1, uint16_t y1) {
    COMMAND_MODE();
    spi_write(COLUMN_ADDRESS_SET);
    DATA_MODE();
    spi_write(x0 >> 8);
    spi_write(x0);
    spi_write(x1 >> 8);
    spi_write(x1);
    COMMAND_MODE();
    spi_write(PAGE_ADDRESS_SET);
    DATA_MODE();
    spi_write(y0 >> 8);
    spi_write(y0);
    spi_write(y1 >> 8);
    spi_write(y1);
}
                            
    

void horizontal_line(void *t,
                     long x1, long x2, long y1,
                     unsigned long color) {
    set_area(x1, y1, x2, y1);
    COMMAND_MODE();
    spi_write(MEMORY_WRITE);
    DATA_MODE();
    while(x1++ <= x2) {
        spi_write(color >> 8);
        spi_write(color);
    }
}

void vertical_line(void *t,
                   long x1, long y1, long y2,
                   unsigned long color) {
    set_area(x1, y1, x1, y2);
    COMMAND_MODE();
    spi_write(MEMORY_WRITE);
    DATA_MODE();
    while(y1++ <= y2) {
        spi_write(color >> 8);
        spi_write(color);
    }
}

void pixel(void *t, long x, long y, unsigned long color) {
    set_area(x, y, x, y);
    COMMAND_MODE();
    spi_write(MEMORY_WRITE);
    DATA_MODE();
    spi_write(color >> 8);
    spi_write(color);
}

void multiple_pixels(void *t, long x, long y, long x0, long count, long bpp,
                     const unsigned char *data,
                     const unsigned char *palette) {
    set_area(x, y, x + count - 1, y);
    COMMAND_MODE();
    spi_write(MEMORY_WRITE);
    DATA_MODE();
    unsigned long color;
    unsigned int index;

    for (long i = 0; i < count; ++i) {
        switch (bpp) {
            case 1:
                index = *data >> (7 - x0);
                color = palette[(index & 0x01) * sizeof(unsigned long)];
                if (++x0 == 8) {
                    ++data;
                    x0 = 0;
                }
                break;
            case 4:
                index = *data >> (x0 ? 0 : 4);
                /* FIXME: Endian MAYBE error */
                color = rgb888_to_565(0, *(unsigned long *)&palette[(index & 0x0f) * 3]);
                if (++x0 == 2) {
                    ++data;
                    x0 = 0;
                }
                break;
            case 8:
                color = rgb888_to_565(0, *(unsigned long *)&palette[*data++ * 3]);
                break;
            default:
                /* TODO(chinatianma@gmail.com): Handle error */
                return;
                break;
        }
        spi_write(color >> 8);
        spi_write(color);
    }
}

void rectangle(void *t, const tRectangle *r, unsigned long color) {
    set_area(r->i16XMin, r->i16YMin, r->i16XMax, r->i16YMax);
    COMMAND_MODE();
    spi_write(MEMORY_WRITE);
    DATA_MODE();
    long count = (r->i16XMax - r->i16XMin + 1) * (r->i16YMax - r->i16YMin + 1);
    for (int i = 0; i < count; ++i) {
        spi_write(color >> 8);
        spi_write(color);
    }
}

void lcd_clear(unsigned long color) {
    set_area(0, 0, 239, 319);
    COMMAND_MODE();
    spi_swap(MEMORY_WRITE);
    DATA_MODE();
    for (int i = 0; i < 320 * 240; ++i) {
        spi_write(color >> 8);
        spi_write(color);
    }
}

const tDisplay kILI9341 = {
    sizeof(tDisplay),
    0,
    240,
    320,
    pixel,
    multiple_pixels,
    horizontal_line,
    vertical_line,
    rectangle,
    rgb888_to_565,
    flush
};
    





