#include "ili9341.h"
#include "delay.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

#include <string.h>


/* L10N */
#undef GrStringTableSet
#define GrStringTableSet
#undef GrStringLanguageSet
#define GrStringLanguageSet

#define STR_SYSTEM_MAIN_MENU "System Main Menu"
#define STR_SETTING "Setting"
#define STR_DEMARCATE "Demarcate"
#define STR_MANUAL_CHECK "Manual Check"
#define STR_AUTO_RUN "Auto Run"
#define STR_PRINT "Print"
#define STR_DISPLAY "Display"
#define STR_HISTORY "History"
#define STR_NOTEBOOK "Notebook"
#define STR_HELP "Help"
#undef GrStringGet
#define GrStringGet(x, y, z) strncpy(y, x, z)

extern tCanvasWidget g_sBackground;
extern tCanvasWidget g_sHeading;
extern tCanvasWidget g_sHello;
extern tPushButtonWidget g_sPushBtn;

void OnButtonPress(tWidget *pWidget);

#define g_sKitronix320x240x16_SSD2119 kILI9341
#define tBoolean bool



Canvas(g_sHeading, &g_sBackground, 0, &g_sPushBtn,
           &g_sKitronix320x240x16_SSD2119, 0, 0, 240, 23,
           (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |
           CANVAS_STYLE_TEXT),
           ClrDarkBlue, ClrWhite, ClrWhite, &g_sFontCm20,
           "hello-widget", 0, 0);
Canvas(g_sBackground, WIDGET_ROOT, 0, &g_sHeading,
           &g_sKitronix320x240x16_SSD2119, 0, 23, 240, (240 - 23),
           CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);
RectangularButton(g_sPushBtn, &g_sHeading, 0, 0,
                      &g_sKitronix320x240x16_SSD2119, 60, 60, 200, 40,
                      (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE |
                      PB_STYLE_TEXT | PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                      ClrDarkBlue, ClrBlue, ClrWhite, ClrWhite,
                      &g_sFontCmss22b, "Show Welcome", 0, 0, 0, 0,
                      OnButtonPress);
Canvas(g_sHello, &g_sPushBtn, 0, 0,
           &g_sKitronix320x240x16_SSD2119, 0, 150, 240, 40,
           (CANVAS_STYLE_FILL | CANVAS_STYLE_TEXT),
           ClrBlack, 0, ClrWhite, &g_sFontCm40, "Hello World!", 0, 0);
tBoolean g_bHelloVisible = false;
void OnButtonPress(tWidget *pWidget){}


/* Chinese UTF8 is (often) 3 bytes per character. */
extern tCanvasWidget g_main_menu_background;
char g_main_menu_background_text[21];
extern tPushButtonWidget g_setting_button;
char g_setting_button_text[17];

static tWidget *g_current_background = (tWidget *)&g_main_menu_background;


tContext context;
void test(tWidget* w) {
    GrStringDraw(&context, "Hello Xiao Zhang", -1, 0, 100, 1);
}

Canvas(g_main_menu_background, WIDGET_ROOT, 0, &g_setting_button,
       &kILI9341, 0, 0, 240, 20,
       (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
       ClrWhite, 0, ClrBlack, &g_sFontCm20,
       g_main_menu_background_text, 0, 0);
RectangularButton(g_setting_button, &g_main_menu_background, 0, 0,
                  &kILI9341, 0, 20, 120, 40,
                  (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
                  ClrBlue, ClrDarkBlue, 0, ClrWhite,
                  &g_sFontCm20, g_setting_button_text,
                  0, 0, 0, 0, test);

void init_button(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlDelay(1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlDelay(1);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1 | GPIO_PIN_2);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE,
                         GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void check_button(void) {
}
    
                  


int main(void) {
    static unsigned char display_buffers[64];
    lcd_initial();
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
    GrContextFontSet(&context, g_psFontFixed6x8);
    GrStringDrawCentered(&context, "grlib_demo", -1,
                         GrContextDpyWidthGet(&context) / 2, 5, 0);
#if 0
    /* l10n */
    GrStringTableSet(g_putTable_change_me);
    GrStringLanguageSet(GrLangZhPRC);
#endif
    
#if 0    
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sBackground);
    WidgetPaint(WIDGET_ROOT);
    WidgetAdd((tWidget *)&g_sPushBtn, (tWidget *)&g_sHello);
    PushButtonTextSet(&g_sPushBtn, "Hide Welcome");
    WidgetPaint((tWidget *)&g_sPushBtn);
#endif

    GrStringGet(STR_SYSTEM_MAIN_MENU, g_main_menu_background_text,
                sizeof(g_main_menu_background_text));
    GrStringGet(STR_SETTING, g_setting_button_text,
                sizeof(g_setting_button_text));
                

    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_main_menu_background);
    WidgetPaint(WIDGET_ROOT);
    
    
    for (;;) {
#if 0
        horizontal_line(0, 0, 239, 1, 0x07e0);
        GrCircleDraw(&context, 32, 24, 10);
        GrCircleDraw(&context, 280, 200, 10);
        GrCircleDraw(&context, 200, 40, 10);
        pixel(0, 0, 0, 0x07e0);
        horizontal_line(0, 0, 239, 1, 0xf800);
        delay_ms(1000);
#endif   
        delay_ms(2000);
        WidgetMessageQueueAdd((tWidget *)&g_setting_button, WIDGET_MSG_PTR_DOWN, 2, 30, 0, 0);
        WidgetMessageQueueProcess();
delay_ms(5000);
        WidgetMessageQueueAdd((tWidget *)&g_setting_button, WIDGET_MSG_PTR_UP, 2, 30, 0, 0);
        WidgetMessageQueueProcess();

        horizontal_line(0, 0, 239, 1, 0xf800);
    }
}

