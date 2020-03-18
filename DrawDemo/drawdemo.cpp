#include "Pokitto.h" // liitt�� Pokitto-kirjaston ohjelman osaksi

Pokitto::Core peli; // luodaan ohjelmaobjekti nimelt� "peli"

// ohjelmassa k�ytett�v�t muuttujat
int vari=1; // pensselin v�ri - ��kk�si� ei voi k�ytt�� muuttujissa!
int koko=2; // pensselin koko
int x=55; // pensselin x-sijainti
int y=44; // pensselin y-sijainti

int main () {
    peli.begin(); // k�ynnistet��n Pokitton n�ytt� ja muut toiminnot
    peli.display.persistence=1; // k��nnet��n n�yt�n "pyyhkiytyminen" pois p��lt�
    
    while (peli.isRunning()) { // while eli "kunnes" py�rii niin kauan kun peli.isRunning = 1 
        if (peli.update()) { // kun on tullut aika piirt�� n�ytt� uudelleen           
            
            // tarkistetaan n�pp�imet
            if (peli.buttons.upBtn()) y--;
            if (peli.buttons.downBtn()) y++;
            if (peli.buttons.leftBtn()) x--;
            if (peli.buttons.rightBtn()) x++;
            if (peli.buttons.aBtn()) vari++;
            if (peli.buttons.bBtn()) koko+=2;
            if (peli.buttons.cBtn()) peli.display.clear();
                
            // rajoitetaan pensselin liike pysym��n n�yt�n sis�puolella
            if (x<0) x=0;
            if (y<0) y=0;
            if (x>110) x=110;
            if (y>88) y=88;
            
            // rajoitetaan pensselin koko ja v�ri, huomaa "==" on vertailu C++ kieless�!!
            if (vari==16) vari=1;
            if (koko>8) koko=2;
            
            // piirret��n pensseli
            peli.display.setColor(vari); // aseta pensselin v�ri
            peli.display.fillCircle(x,y,koko); // piirr� pensseli
            
            } 
        }    
}