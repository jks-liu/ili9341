# ILI9341 DRIVER
a-Si TFT LCD Single Chip Driver, 240RGBx320 Resolution and 262K color.

## Config
In this section, you will define some operation which can be MACRO, function declaraton or static inline function in .h file I will point out.
MACRO and static inline function in .h file will be fasert than function declaration. If possible, static inline function I think is the best choice.

Every config should in a head file whose filename is hardware-config.h and that can see by ili9341.h.
TIP: If you use gcc, command option -Ipath-to-hardware-config.h is helpful.

### Config items

* 

