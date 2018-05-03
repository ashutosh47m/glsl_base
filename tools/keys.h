/*
GLSL_base
May 2018, Ashutosh Morwal
@Email ashutosh47m@gmail.com
@blog http://www.explicitgfx.wordpress.com/
*/

/* The unknown key */
#define _2am_KEY_UNKNOWN            -1

/* Printable keys */
#define _2am_KEY_SPACE              32
#define _2am_KEY_APOSTROPHE         39  /* ' */
#define _2am_KEY_COMMA              44  /* , */
#define _2am_KEY_MINUS              45  /* - */
#define _2am_KEY_PERIOD             46  /* . */
#define _2am_KEY_SLASH              47  /* / */
#define _2am_KEY_0                  48
#define _2am_KEY_1                  49
#define _2am_KEY_2                  50
#define _2am_KEY_3                  51
#define _2am_KEY_4                  52
#define _2am_KEY_5                  53
#define _2am_KEY_6                  54
#define _2am_KEY_7                  55
#define _2am_KEY_8                  56
#define _2am_KEY_9                  57
#define _2am_KEY_SEMICOLON          59  /* ; */
#define _2am_KEY_EQUAL              61  /* = */
#define _2am_KEY_A                  65
#define _2am_KEY_B                  66
#define _2am_KEY_C                  67
#define _2am_KEY_D                  68
#define _2am_KEY_E                  69
#define _2am_KEY_F                  70
#define _2am_KEY_G                  71
#define _2am_KEY_H                  72
#define _2am_KEY_I                  73
#define _2am_KEY_J                  74
#define _2am_KEY_K                  75
#define _2am_KEY_L                  76
#define _2am_KEY_M                  77
#define _2am_KEY_N                  78
#define _2am_KEY_O                  79
#define _2am_KEY_P                  80
#define _2am_KEY_Q                  81
#define _2am_KEY_R                  82
#define _2am_KEY_S                  83
#define _2am_KEY_T                  84
#define _2am_KEY_U                  85
#define _2am_KEY_V                  86
#define _2am_KEY_W                  87
#define _2am_KEY_X                  88
#define _2am_KEY_Y                  89
#define _2am_KEY_Z                  90
#define _2am_KEY_LEFT_BRACKET       91  /* [ */
#define _2am_KEY_BACKSLASH          92  /* \ */
#define _2am_KEY_RIGHT_BRACKET      93  /* ] */
#define _2am_KEY_GRAVE_ACCENT       96  /* ` */
#define _2am_KEY_WORLD_1            161 /* non-US #1 */
#define _2am_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define _2am_KEY_ESCAPE             256
#define _2am_KEY_ENTER              257
#define _2am_KEY_TAB                258
#define _2am_KEY_BACKSPACE          259
#define _2am_KEY_INSERT             260
#define _2am_KEY_DELETE             261
#define _2am_KEY_RIGHT              262
#define _2am_KEY_LEFT               263
#define _2am_KEY_DOWN               264
#define _2am_KEY_UP                 265
#define _2am_KEY_PAGE_UP            266
#define _2am_KEY_PAGE_DOWN          267
#define _2am_KEY_HOME               268
#define _2am_KEY_END                269
#define _2am_KEY_CAPS_LOCK          280
#define _2am_KEY_SCROLL_LOCK        281
#define _2am_KEY_NUM_LOCK           282
#define _2am_KEY_PRINT_SCREEN       283
#define _2am_KEY_PAUSE              284
#define _2am_KEY_F1                 290
#define _2am_KEY_F2                 291
#define _2am_KEY_F3                 292
#define _2am_KEY_F4                 293
#define _2am_KEY_F5                 294
#define _2am_KEY_F6                 295
#define _2am_KEY_F7                 296
#define _2am_KEY_F8                 297
#define _2am_KEY_F9                 298
#define _2am_KEY_F10                299
#define _2am_KEY_F11                300
#define _2am_KEY_F12                301
#define _2am_KEY_F13                302
#define _2am_KEY_F14                303
#define _2am_KEY_F15                304
#define _2am_KEY_F16                305
#define _2am_KEY_F17                306
#define _2am_KEY_F18                307
#define _2am_KEY_F19                308
#define _2am_KEY_F20                309
#define _2am_KEY_F21                310
#define _2am_KEY_F22                311
#define _2am_KEY_F23                312
#define _2am_KEY_F24                313
#define _2am_KEY_F25                314
#define _2am_KEY_KP_0               320
#define _2am_KEY_KP_1               321
#define _2am_KEY_KP_2               322
#define _2am_KEY_KP_3               323
#define _2am_KEY_KP_4               324
#define _2am_KEY_KP_5               325
#define _2am_KEY_KP_6               326
#define _2am_KEY_KP_7               327
#define _2am_KEY_KP_8               328
#define _2am_KEY_KP_9               329
#define _2am_KEY_KP_DECIMAL         330
#define _2am_KEY_KP_DIVIDE          331
#define _2am_KEY_KP_MULTIPLY        332
#define _2am_KEY_KP_SUBTRACT        333
#define _2am_KEY_KP_ADD             334
#define _2am_KEY_KP_ENTER           335
#define _2am_KEY_KP_EQUAL           336
#define _2am_KEY_LEFT_SHIFT         340
#define _2am_KEY_LEFT_CONTROL       341
#define _2am_KEY_LEFT_ALT           342
#define _2am_KEY_LEFT_SUPER         343
#define _2am_KEY_RIGHT_SHIFT        344
#define _2am_KEY_RIGHT_CONTROL      345
#define _2am_KEY_RIGHT_ALT          346
#define _2am_KEY_RIGHT_SUPER        347
#define _2am_KEY_MENU               348

#define _2am_KEY_LAST               _2am_KEY_MENU