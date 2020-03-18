/* Pokitto drawBitmap example - draws a Pokitto icon by @trelemar */

#include "Pokitto.h" // include Pokitto library
#include "pokitto_icon.h" // include the Pokitto icon graphics file

/* 16-bit (565 rgb) palette data for "PokiDude" */
static const uint16_t pokidude_pal[] = {
0xf81f, 0x7e4, 0xffff, 0x7d5, 0x0,
};

/* Piskel data for "PokiDude" */
static const uint8_t pokidude[258] = {
16,16,
//frame 0
0x0,0x0,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x1,0x4,0x0,0x0,0x0,
0x4,0x4,0x4,0x1,0x2,0x4,0x2,0x2,0x2,0x4,0x2,0x1,0x4,0x4,0x4,0x0,
0x4,0x1,0x1,0x1,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x1,0x1,0x1,0x4,0x0,
0x4,0x4,0x4,0x1,0x2,0x4,0x2,0x2,0x2,0x4,0x2,0x1,0x4,0x4,0x4,0x0,
0x0,0x0,0x4,0x1,0x2,0x2,0x4,0x4,0x4,0x2,0x2,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x1,0x2,0x1,0x1,0x1,0x1,0x2,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x2,0x2,0x2,0x1,0x2,0x1,0x1,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x1,0x2,0x1,0x1,0x1,0x1,0x1,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x1,0x4,0x0,0x0,0x0,0x0,0x0,0x4,0x1,0x4,0x0,0x0,0x0,
0x0,0x0,0x4,0x4,0x4,0x0,0x0,0x0,0x0,0x0,0x4,0x4,0x4,0x0,0x0,0x0,
};



Pokitto::Core mygame; //create Pokitto application instance

int main () {
    mygame.begin(); // start the application
    mygame.display.load565Palette(pokidude_pal); //load the palette for the image
    mygame.display.bgcolor=1;
    /* the "while" loop runs as long as the program is running */
    while (mygame.isRunning()) {
        /* mygame.update() is processed whenever it is time to update the screen */
        if (mygame.update()) {
            mygame.display.drawBitmap(0,0,pokidude); // draw the pokitto_icon graphic
            }
        }

    return 0; // this is "good programming manners". Program informs it ended without errors
}
