#pragma once


//#ifdef NT_PLATFORM_WINDOWS
//#ifdef NT_OPENGL

	#define UNI_KEY_UNKNOWN            -1
	#define UNI_KEY_SPACE              32
	#define UNI_KEY_APOSTROPHE         39  /* ' */
	#define UNI_KEY_COMMA              44  /* , */
	#define UNI_KEY_MINUS              45  /* - */
	#define UNI_KEY_PERIOD             46  /* . */
	#define UNI_KEY_SLASH              47  /* / */
	#define UNI_KEY_0                  48
	#define UNI_KEY_1                  49
	#define UNI_KEY_2                  50
	#define UNI_KEY_3                  51
	#define UNI_KEY_4                  52
	#define UNI_KEY_5                  53
	#define UNI_KEY_6                  54
	#define UNI_KEY_7                  55
	#define UNI_KEY_8                  56
	#define UNI_KEY_9                  57
	#define UNI_KEY_SEMICOLON          59  /* ; */
	#define UNI_KEY_EQUAL              61  /* = */
	#define UNI_KEY_A                  65
	#define UNI_KEY_B                  66
	#define UNI_KEY_C                  67
	#define UNI_KEY_D                  68
	#define UNI_KEY_E                  69
	#define UNI_KEY_F                  70
	#define UNI_KEY_G                  71
	#define UNI_KEY_H                  72
	#define UNI_KEY_I                  73
	#define UNI_KEY_J                  74
	#define UNI_KEY_K                  75
	#define UNI_KEY_L                  76
	#define UNI_KEY_M                  77
	#define UNI_KEY_N                  78
	#define UNI_KEY_O                  79
	#define UNI_KEY_P                  80
	#define UNI_KEY_Q                  81
	#define UNI_KEY_R                  82
	#define UNI_KEY_S                  83
	#define UNI_KEY_T                  84
	#define UNI_KEY_U                  85
	#define UNI_KEY_V                  86
	#define UNI_KEY_W                  87
	#define UNI_KEY_X                  88
	#define UNI_KEY_Y                  89
	#define UNI_KEY_Z                  90
	#define UNI_KEY_LEFT_BRACKET       91  /* [ */
	#define UNI_KEY_BACKSLASH          92  /* \ */
	#define UNI_KEY_RIGHT_BRACKET      93  /* ] */
	#define UNI_KEY_GRAVE_ACCENT       96  /* ` */
	#define UNI_KEY_WORLD_1            161 /* non-US #1 */
	#define UNI_KEY_WORLD_2            162 /* non-US #2 */

	/* Function keys */
	#define UNI_KEY_ESCAPE             256
	#define UNI_KEY_ENTER              257
	#define UNI_KEY_TAB                258
	#define UNI_KEY_BACKSPACE          259
	#define UNI_KEY_INSERT             260
	#define UNI_KEY_DELETE             261
	#define UNI_KEY_RIGHT              262
	#define UNI_KEY_LEFT               263
	#define UNI_KEY_DOWN               264
	#define UNI_KEY_UP                 265
	#define UNI_KEY_PAGE_UP            266
	#define UNI_KEY_PAGE_DOWN          267
	#define UNI_KEY_HOME               268
	#define UNI_KEY_END                269
	#define UNI_KEY_CAPS_LOCK          280
	#define UNI_KEY_SCROLL_LOCK        281
	#define UNI_KEY_NUM_LOCK           282
	#define UNI_KEY_PRINT_SCREEN       283
	#define UNI_KEY_PAUSE              284
	#define UNI_KEY_F1                 290
	#define UNI_KEY_F2                 291
	#define UNI_KEY_F3                 292
	#define UNI_KEY_F4                 293
	#define UNI_KEY_F5                 294
	#define UNI_KEY_F6                 295
	#define UNI_KEY_F7                 296
	#define UNI_KEY_F8                 297
	#define UNI_KEY_F9                 298
	#define UNI_KEY_F10                299
	#define UNI_KEY_F11                300
	#define UNI_KEY_F12                301
	#define UNI_KEY_F13                302
	#define UNI_KEY_F14                303
	#define UNI_KEY_F15                304
	#define UNI_KEY_F16                305
	#define UNI_KEY_F17                306
	#define UNI_KEY_F18                307
	#define UNI_KEY_F19                308
	#define UNI_KEY_F20                309
	#define UNI_KEY_F21                310
	#define UNI_KEY_F22                311
	#define UNI_KEY_F23                312
	#define UNI_KEY_F24                313
	#define UNI_KEY_F25                314
	#define UNI_KEY_KP_0               320
	#define UNI_KEY_KP_1               321
	#define UNI_KEY_KP_2               322
	#define UNI_KEY_KP_3               323
	#define UNI_KEY_KP_4               324
	#define UNI_KEY_KP_5               325
	#define UNI_KEY_KP_6               326
	#define UNI_KEY_KP_7               327
	#define UNI_KEY_KP_8               328
	#define UNI_KEY_KP_9               329
	#define UNI_KEY_KP_DECIMAL         330
	#define UNI_KEY_KP_DIVIDE          331
	#define UNI_KEY_KP_MULTIPLY        332
	#define UNI_KEY_KP_SUBTRACT        333
	#define UNI_KEY_KP_ADD             334
	#define UNI_KEY_KP_ENTER           335
	#define UNI_KEY_KP_EQUAL           336
	#define UNI_KEY_LEFT_SHIFT         340
	#define UNI_KEY_LEFT_CONTROL       341
	#define UNI_KEY_LEFT_ALT           342
	#define UNI_KEY_LEFT_SUPER         343
	#define UNI_KEY_RIGHT_SHIFT        344
	#define UNI_KEY_RIGHT_CONTROL      345
	#define UNI_KEY_RIGHT_ALT          346
	#define UNI_KEY_RIGHT_SUPER        347
	#define UNI_KEY_MENU               348
	#define UNI_KEY_LAST               NT_KEY_MENU

//#endif NT_OPENGL
//#endif NT_PLATFORM_WINDOWS


#define NT_KEY_UNKNOWN            UNI_KEY_UNKNOWN       
#define NT_KEY_SPACE              UNI_KEY_SPACE         
#define NT_KEY_APOSTROPHE         UNI_KEY_APOSTROPHE    
#define NT_KEY_COMMA              UNI_KEY_COMMA         
#define NT_KEY_MINUS              UNI_KEY_MINUS         
#define NT_KEY_PERIOD             UNI_KEY_PERIOD        
#define NT_KEY_SLASH              UNI_KEY_SLASH         
#define NT_KEY_0                  UNI_KEY_0             
#define NT_KEY_1                  UNI_KEY_1             
#define NT_KEY_2                  UNI_KEY_2             
#define NT_KEY_3                  UNI_KEY_3             
#define NT_KEY_4                  UNI_KEY_4             
#define NT_KEY_5                  UNI_KEY_5             
#define NT_KEY_6                  UNI_KEY_6             
#define NT_KEY_7                  UNI_KEY_7             
#define NT_KEY_8                  UNI_KEY_8             
#define NT_KEY_9                  UNI_KEY_9             
#define NT_KEY_SEMICOLON          UNI_KEY_SEMICOLON     
#define NT_KEY_EQUAL              UNI_KEY_EQUAL         
#define NT_KEY_A                  UNI_KEY_A             
#define NT_KEY_B                  UNI_KEY_B             
#define NT_KEY_C                  UNI_KEY_C             
#define NT_KEY_D                  UNI_KEY_D             
#define NT_KEY_E                  UNI_KEY_E             
#define NT_KEY_F                  UNI_KEY_F             
#define NT_KEY_G                  UNI_KEY_G             
#define NT_KEY_H                  UNI_KEY_H             
#define NT_KEY_I                  UNI_KEY_I             
#define NT_KEY_J                  UNI_KEY_J             
#define NT_KEY_K                  UNI_KEY_K             
#define NT_KEY_L                  UNI_KEY_L             
#define NT_KEY_M                  UNI_KEY_M             
#define NT_KEY_N                  UNI_KEY_N             
#define NT_KEY_O                  UNI_KEY_O             
#define NT_KEY_P                  UNI_KEY_P             
#define NT_KEY_Q                  UNI_KEY_Q             
#define NT_KEY_R                  UNI_KEY_R             
#define NT_KEY_S                  UNI_KEY_S             
#define NT_KEY_T                  UNI_KEY_T             
#define NT_KEY_U                  UNI_KEY_U             
#define NT_KEY_V                  UNI_KEY_V             
#define NT_KEY_W                  UNI_KEY_W             
#define NT_KEY_X                  UNI_KEY_X             
#define NT_KEY_Y                  UNI_KEY_Y             
#define NT_KEY_Z                  UNI_KEY_Z             
#define NT_KEY_LEFT_BRACKET       UNI_KEY_LEFT_BRACKET  
#define NT_KEY_BACKSLASH          UNI_KEY_BACKSLASH     
#define NT_KEY_RIGHT_BRACKET      UNI_KEY_RIGHT_BRACKET 
#define NT_KEY_GRAVE_ACCENT       UNI_KEY_GRAVE_ACCENT  
#define NT_KEY_WORLD_1            UNI_KEY_WORLD_1       
#define NT_KEY_WORLD_2            UNI_KEY_WORLD_2       

/* Function keys */
#define NT_KEY_ESCAPE             UNI_KEY_ESCAPE       
#define NT_KEY_ENTER              UNI_KEY_ENTER        
#define NT_KEY_TAB                UNI_KEY_TAB          
#define NT_KEY_BACKSPACE          UNI_KEY_BACKSPACE    
#define NT_KEY_INSERT             UNI_KEY_INSERT       
#define NT_KEY_DELETE             UNI_KEY_DELETE       
#define NT_KEY_RIGHT              UNI_KEY_RIGHT        
#define NT_KEY_LEFT               UNI_KEY_LEFT         
#define NT_KEY_DOWN               UNI_KEY_DOWN         
#define NT_KEY_UP                 UNI_KEY_UP           
#define NT_KEY_PAGE_UP            UNI_KEY_PAGE_UP      
#define NT_KEY_PAGE_DOWN          UNI_KEY_PAGE_DOWN    
#define NT_KEY_HOME               UNI_KEY_HOME         
#define NT_KEY_END                UNI_KEY_END          
#define NT_KEY_CAPS_LOCK          UNI_KEY_CAPS_LOCK    
#define NT_KEY_SCROLL_LOCK        UNI_KEY_SCROLL_LOCK  
#define NT_KEY_NUM_LOCK           UNI_KEY_NUM_LOCK     
#define NT_KEY_PRINT_SCREEN       UNI_KEY_PRINT_SCREEN 
#define NT_KEY_PAUSE              UNI_KEY_PAUSE        
#define NT_KEY_F1                 UNI_KEY_F1           
#define NT_KEY_F2                 UNI_KEY_F2           
#define NT_KEY_F3                 UNI_KEY_F3           
#define NT_KEY_F4                 UNI_KEY_F4           
#define NT_KEY_F5                 UNI_KEY_F5           
#define NT_KEY_F6                 UNI_KEY_F6           
#define NT_KEY_F7                 UNI_KEY_F7           
#define NT_KEY_F8                 UNI_KEY_F8           
#define NT_KEY_F9                 UNI_KEY_F9           
#define NT_KEY_F10                UNI_KEY_F10          
#define NT_KEY_F11                UNI_KEY_F11          
#define NT_KEY_F12                UNI_KEY_F12          
#define NT_KEY_F13                UNI_KEY_F13          
#define NT_KEY_F14                UNI_KEY_F14          
#define NT_KEY_F15                UNI_KEY_F15          
#define NT_KEY_F16                UNI_KEY_F16          
#define NT_KEY_F17                UNI_KEY_F17          
#define NT_KEY_F18                UNI_KEY_F18          
#define NT_KEY_F19                UNI_KEY_F19          
#define NT_KEY_F20                UNI_KEY_F20          
#define NT_KEY_F21                UNI_KEY_F21          
#define NT_KEY_F22                UNI_KEY_F22          
#define NT_KEY_F23                UNI_KEY_F23          
#define NT_KEY_F24                UNI_KEY_F24          
#define NT_KEY_F25                UNI_KEY_F25          
#define NT_KEY_KP_0               UNI_KEY_KP_0         
#define NT_KEY_KP_1               UNI_KEY_KP_1         
#define NT_KEY_KP_2               UNI_KEY_KP_2         
#define NT_KEY_KP_3               UNI_KEY_KP_3         
#define NT_KEY_KP_4               UNI_KEY_KP_4         
#define NT_KEY_KP_5               UNI_KEY_KP_5         
#define NT_KEY_KP_6               UNI_KEY_KP_6         
#define NT_KEY_KP_7               UNI_KEY_KP_7         
#define NT_KEY_KP_8               UNI_KEY_KP_8         
#define NT_KEY_KP_9               UNI_KEY_KP_9         
#define NT_KEY_KP_DECIMAL         UNI_KEY_KP_DECIMAL   
#define NT_KEY_KP_DIVIDE          UNI_KEY_KP_DIVIDE    
#define NT_KEY_KP_MULTIPLY        UNI_KEY_KP_MULTIPLY  
#define NT_KEY_KP_SUBTRACT        UNI_KEY_KP_SUBTRACT  
#define NT_KEY_KP_ADD             UNI_KEY_KP_ADD       
#define NT_KEY_KP_ENTER           UNI_KEY_KP_ENTER     
#define NT_KEY_KP_EQUAL           UNI_KEY_KP_EQUAL     
#define NT_KEY_LEFT_SHIFT         UNI_KEY_LEFT_SHIFT   
#define NT_KEY_LEFT_CONTROL       UNI_KEY_LEFT_CONTROL 
#define NT_KEY_LEFT_ALT           UNI_KEY_LEFT_ALT     
#define NT_KEY_LEFT_SUPER         UNI_KEY_LEFT_SUPER   
#define NT_KEY_RIGHT_SHIFT        UNI_KEY_RIGHT_SHIFT  
#define NT_KEY_RIGHT_CONTROL      UNI_KEY_RIGHT_CONTROL
#define NT_KEY_RIGHT_ALT          UNI_KEY_RIGHT_ALT    
#define NT_KEY_RIGHT_SUPER        UNI_KEY_RIGHT_SUPER  
#define NT_KEY_MENU               UNI_KEY_MENU         
#define NT_KEY_LAST               UNI_KEY_LAST         


//--------------------------
//--------Buttons-----------
//--------------------------

#ifdef NT_PLATFORM_WINDOWS
#ifdef NT_OPENGL

	#define UNI_MOUSE_BUTTON_1         0
	#define UNI_MOUSE_BUTTON_2         1
	#define UNI_MOUSE_BUTTON_3         2
	#define UNI_MOUSE_BUTTON_4         3
	#define UNI_MOUSE_BUTTON_5         4
	#define UNI_MOUSE_BUTTON_6         5
	#define UNI_MOUSE_BUTTON_7         6
	#define UNI_MOUSE_BUTTON_8         7
	#define UNI_MOUSE_BUTTON_LAST      NT_MOUSE_BUTTON_8
	#define UNI_MOUSE_BUTTON_LEFT      NT_MOUSE_BUTTON_1
	#define UNI_MOUSE_BUTTON_RIGHT     NT_MOUSE_BUTTON_2
	#define UNI_MOUSE_BUTTON_MIDDLE    NT_MOUSE_BUTTON_3

#endif NT_OPENGL
#endif NT_PLATFORM_WINDOWS

#define NT_MOUSE_BUTTON_1         UNI_MOUSE_BUTTON_1     
#define NT_MOUSE_BUTTON_2         UNI_MOUSE_BUTTON_2     
#define NT_MOUSE_BUTTON_3         UNI_MOUSE_BUTTON_3     
#define NT_MOUSE_BUTTON_4         UNI_MOUSE_BUTTON_4     
#define NT_MOUSE_BUTTON_5         UNI_MOUSE_BUTTON_5     
#define NT_MOUSE_BUTTON_6         UNI_MOUSE_BUTTON_6     
#define NT_MOUSE_BUTTON_7         UNI_MOUSE_BUTTON_7     
#define NT_MOUSE_BUTTON_8         UNI_MOUSE_BUTTON_8     
#define NT_MOUSE_BUTTON_LAST      UNI_MOUSE_BUTTON_LAST  
#define NT_MOUSE_BUTTON_LEFT      UNI_MOUSE_BUTTON_LEFT  
#define NT_MOUSE_BUTTON_RIGHT     UNI_MOUSE_BUTTON_RIGHT 
#define NT_MOUSE_BUTTON_MIDDLE    UNI_MOUSE_BUTTON_MIDDLE