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

#define PROJ_HIRES 1            //1 = high resolution (220x176) , 0 = low resolution fast mode (110x88)
#define PROJ_ENABLE_SOUND 1     // 0 = all sound functions disabled
#define PROJ_ENABLE_SYNTH 1
#define PROJ_STREAMING_MUSIC 0
#define PROJ_SOUND_BUFFERED  0 // use buffered IRQ
#define PROJ_STREAM_TO_DAC   0 // synth to DAC
#define PROJ_AUD_FREQ 44100 //22050 //11025//22050
#define PROJ_FPS 20
//#define PROJ_SHOW_FPS_COUNTER
//#define PROJ_DEVELOPER_MODE 1

#define ENABLE_GUI

#define TRACKER_EXAMPLE

#endif
