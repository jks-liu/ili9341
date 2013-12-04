#include "ili9341.h"
#include "delay.h"

int main(void) {
    lcd_initial();
    tContext context;
    horizontal_line(0, 0, 239, 0, 0x07e0);

    GrContextInit(&context, &kILI9341);
    //
    // Fill the top 12 rows of the screen with blue to create the banner.
    //
    tRectangle sRect;
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&context) - 1;
    sRect.i16YMax = 11;
    GrContextForegroundSet(&context, ClrDarkBlue);
    GrRectFill(&context, &sRect);
    GrContextForegroundSet(&context, ClrWhite);
    GrContextBackgroundSet(&context, ClrBlack);
    GrContextFontSet(&context, &g_sFontCm20);
    GrStringDraw(&context, "Hello Xiao Zhang", -1, 0, 100, 1);
    GrContextFontSet(&context, g_psFontFixed6x8);
    GrStringDrawCentered(&context, "grlib_demo", -1,
                         GrContextDpyWidthGet(&context) / 2, 5, 0);
    delay_ms(2000);

    for (;;) {
        //        horizontal_line(0, 0, 239, 1, 0x07e0);
        GrCircleDraw(&context, 32, 24, 10);
        GrCircleDraw(&context, 280, 200, 10);
        GrCircleDraw(&context, 200, 40, 10);
        pixel(0, 0, 0, 0x07e0);

        delay_ms(1000);
    }
}

