/**************************************************************************/
/*!
    @file     My_settings.h
    @author   XX

    @section HOW TO USE My_settings

   My_settings can be used to set project settings inside the mbed online IDE
*/
/**************************************************************************/

#ifndef MY_SETTINGS_H
#define MY_SETTINGS_H

#define PROJ_HIRES 0            //1 = high resolution (220x176) , 0 = low resolution fast mode (110x88)
#define PROJ_ENABLE_SOUND 1     // 0 = all sound functions disabled
#define PROJ_ENABLE_SYNTH 1
#define PROJ_STREAMING_MUSIC 1
#define PROJ_AUD_FREQ 44100
#define PRODUCTIONTESTING 1

#ifndef POK_SIM
#define PROJ_HIGH_RAM HIGH_RAM_MUSIC
#endif


#endif
