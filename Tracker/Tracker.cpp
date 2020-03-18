
#include "Tracker.h"
#include "Arrays.h"

mycookie trCookie;

void Tracker::SetColorPalette() {

    switch( mode ) {

    case Modes::edit:
        pok.display.paletteptr[0] = pok.display.RGBto565(176,46,245); // violet
        break;

    case Modes::play:
        pok.display.paletteptr[0] = pok.display.RGBto565(18,46,129);  // blue
        break;

    case Modes::travel:
        if(screen==0)
            pok.display.paletteptr[0] = COLOR_BLACK;
        else
            pok.display.paletteptr[0] = pok.display.RGBto565(100,100,100);  // grey , Settings screen

        break;

    case Modes::settings:
        pok.display.paletteptr[0] = pok.display.RGBto565(100,100,100);  // grey
        break;
    }
}

int16_t Tracker::getBPM(){
    return bpm;
}

void Tracker::emptyPatches() {
    for (int i=0;i<16;i++) {
    char pname[10];
    for (int j=0;j<10;j++) patchnames[i][j]=0;
    patch[i].wave = 1;
    patch[i].vol =  127;
    patch[i].bendrate =  0;
    patch[i].pitchbend =  0;
    patch[i].maxbend =  0;
    patch[i].vibrate =  0;
    patch[i].arpmode =  0;
    patch[i].adsr =  1;
    patch[i].attack =  0;
    patch[i].decay =  50;
    patch[i].sustain =  0;
    patch[i].release =  0;
    patch[i].loop =  0;
    patch[i].echo =  0;
    patch[i].overdrive =  0;
    patch[i].kick =  0;
    }
}

void Tracker::playNote() {
    // listed like this for easier debugging
    uint8_t w = patch[_currpatch].wave;
    uint8_t l = patch[_currpatch].loop;
    uint8_t e = patch[_currpatch].echo;
    uint8_t a = patch[_currpatch].adsr;
    uint8_t n = _currpitch;
    uint16_t v = patch[_currpatch].vol;
    uint16_t att = patch[_currpatch].attack;
    uint16_t dec = patch[_currpatch].decay;
    uint16_t sus = patch[_currpatch].sustain;
    uint16_t rel = patch[_currpatch].release;
    int16_t bend = patch[_currpatch].maxbend;
    int16_t brate = patch[_currpatch].bendrate;
    uint8_t arp = patch[_currpatch].arpmode;
    uint8_t ovr = patch[_currpatch].overdrive;
    uint8_t kick = patch[_currpatch].kick;

    if (colPointer == 0)setOSC(&osc1,1,w,l,e,a,n,v,att,dec,sus,rel,bend,brate,arp,ovr,kick);
    else if (colPointer == 1)setOSC(&osc2,1,w,l,e,a,n,v,att,dec,sus,rel,bend,brate,arp,ovr,kick);
    else setOSC(&osc3,1,w,l,e,a,n,v,att,dec,sus,rel,bend,brate,arp,ovr,kick);
}


bool Tracker::checkButtons(){
    bool changed=false;
    if(Tracker::mode == Modes::travel ){ // travel mode
        if (pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame))
            {colPointer = minMax(colPointer-1, 0, maxColumn - 1); changed=true;}
    	if (pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame))
            {colPointer = minMax(colPointer+1, 0, maxColumn - 1); changed=true;}
		if (pok.buttons.repeat(BTN_UP, buttonRepeatFrame)){
    		rowPointer--;
    		screenPointer--;
            rowPointer = minMax(rowPointer, 0, maxRow - 1);
            if ((screenPointer + screenMin) < screenMin){
                screenMin--;
                screenMax--;
                screenPointer++;
            }
            if(screenMin < 0){
                screenMin = maxRow - screenMaxInit; // maxRow - 13
                screenMax = maxRow - 1; // maxRow - 1
                screenPointer = screenMaxInit - 1;
            }
            changed=true;
    	}
    	if (pok.buttons.repeat(BTN_DOWN, buttonRepeatFrame)){
    		rowPointer++;
    		screenPointer++;
            rowPointer = minMax(rowPointer, 0, maxRow - 1);
            if ((screenPointer + screenMin) > screenMax){
                screenMin++;
                screenMax++;
                screenPointer--;
            }
            if(screenMax > (maxRow - 1)){
                screenMin = 0;
                screenMax = screenMaxInit;
                screenPointer = 0;
            }
            changed=true;
    	}
    	if (pok.buttons.pressed(BTN_A)){
            mode = Modes::edit; changed=true;
    	}
    	if (pok.buttons.pressed(BTN_C)){
            mode = Modes::play; changed=true;
    	}
    	if (pok.buttons.repeat(BTN_B,buttonRepeatFrame+1)){
            // delete note
            _pitch[_songPos[songPos][colPointer]][rowPointer] = -1;
            _patch[_songPos[songPos][colPointer]][rowPointer] = 0;
            // Zero all oscillators
            setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            changed=true;
    	}
    }
    else if (mode == Modes::edit){ // edit pitches and patches
        if (pok.buttons.pressed(BTN_A)){
            mode = Modes::travel;
            edit = 0;
            changed=true;
        }
        if (pok.buttons.pressed(BTN_LEFT))
            {edit = minMax(edit-1, 0, 1); changed=true;}
    	if (pok.buttons.pressed(BTN_RIGHT))
            {edit = minMax(edit+1, 0, 1); changed=true;}
    	if (pok.buttons.repeat(BTN_UP, buttonRepeatFrame)){
            if (edit == 0) {
                // editing pitch
                if (_pitch[_songPos[songPos][colPointer]][rowPointer] == -1) {
                    //auto-fill an empty cell. We should not have cells with only pitch or patch!
                    _pitch[_songPos[songPos][colPointer]][rowPointer] = _currpitch;
                    _patch[_songPos[songPos][colPointer]][rowPointer] = _currpatch;
                } else {
                    // not an empty cell, so increment values
                    _currpitch = _pitch[_songPos[songPos][colPointer]][rowPointer] = minMax(_pitch[_songPos[songPos][colPointer]][rowPointer]+1, 0, 99);
                    // automatically pick up the instrument if available, for faster editing
                    _currpatch = _patch[_songPos[songPos][colPointer]][rowPointer];
                }
                changed=true;
            } else if (edit == 1) {
                // editing patch
                if (_pitch[_songPos[songPos][colPointer]][rowPointer] == -1) {
                        //auto-fill an empty cell. We should not have cells with only pitch or patch!
                        _pitch[_songPos[songPos][colPointer]][rowPointer] = _currpitch;
                } else {
                        //auto pick-up the pitch for faster editing
                        _currpitch = _pitch[_songPos[songPos][colPointer]][rowPointer];
                }
                _currpatch = _patch[_songPos[songPos][colPointer]][rowPointer] = minMax(_patch[_songPos[songPos][colPointer]][rowPointer]+1, 0, 14);
                changed=true;
            }
            playNote();
        }
        if (pok.buttons.repeat(BTN_DOWN, buttonRepeatFrame)){
            changed=true;
            if (edit == 0) {
                // editing pitch
                if (_pitch[_songPos[songPos][colPointer]][rowPointer] == -1) {
                    //auto-fill an empty cell. We should not have cells with only pitch or patch!
                    _pitch[_songPos[songPos][colPointer]][rowPointer] = _currpitch;
                    _patch[_songPos[songPos][colPointer]][rowPointer] = _currpatch;
                } else {
                    // not an empty cell, so increment values
                    _currpitch = _pitch[_songPos[songPos][colPointer]][rowPointer] = minMax(_pitch[_songPos[songPos][colPointer]][rowPointer]-1, 0, 99);
                    // automatically pick up the instrument if available, for faster editing
                    _currpatch = _patch[_songPos[songPos][colPointer]][rowPointer];
                }
            } else if (edit == 1) {
                // editing patch
                if (_pitch[_songPos[songPos][colPointer]][rowPointer] == -1) {
                        //auto-fill an empty cell. We should not have cells with only pitch or patch!
                        _pitch[_songPos[songPos][colPointer]][rowPointer] = _currpitch;
                } else {
                        //auto pick-up the pitch for faster editing
                        _currpitch = _pitch[_songPos[songPos][colPointer]][rowPointer];
                }
                _currpatch = _patch[_songPos[songPos][colPointer]][rowPointer] = minMax(_patch[_songPos[songPos][colPointer]][rowPointer]-1, 0, 14);
            }
            playNote();
        }
    }
    else if(mode == Modes::settings){ // this screen settings
        if (pok.buttons.pressed(BTN_C)){
            screen = 1;
            mode = Modes::travel;
            edit = 0;
            changed=true;
            return changed; // do not process more buttons if C has already been pressed!
        }
        if (pok.buttons.pressed(BTN_UP))
            {edit = minMax(edit-1, 0, 9);changed=true;}
    	if (pok.buttons.pressed(BTN_DOWN))
            {edit = minMax(edit+1, 0, 9);changed=true;}
    	if (pok.buttons.repeat(BTN_LEFT, buttonRepeatFrame)){
            changed=true;
            if (edit == 0){
                bpm = minMax(bpm-1, 1, 300);
                _tempo = (60000 / bpm) / 4;
            }
            if (edit == 1) songPos = minMax(songPos-1, 0, 9);
            else if (edit == 2) lastPattern = minMax(lastPattern-1, 0, 9);
            else if (edit == 3) loopTo = minMax(loopTo-1, 0, 9);
            else if (edit == 4) _songPos[songPos][0] = minMax(_songPos[songPos][0]-1, 0, 29);
            else if (edit == 5) _songPos[songPos][1] = minMax(_songPos[songPos][1]-1, 0, 29);
            else if (edit == 6) _songPos[songPos][2] = minMax(_songPos[songPos][2]-1, 0, 29);
            else if (edit == 7) {
                // Mute or unmute channel 1.
                if( track1on ) { track1on=0; setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track1on = 1;
            }
            else if (edit == 8) {
                // Mute or unmute channel 2.
               if( track2on ) { track2on=0; setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track2on = 1;
            }
            else if (edit == 9) {
                // Mute or unmute channel 3.
               if( track3on ) { track3on=0; setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track3on = 1;
            }
        }
        if (pok.buttons.repeat(BTN_RIGHT, buttonRepeatFrame)){
            changed=true;
            if (edit == 0){
                bpm = minMax(bpm+1, 1, 300);
                _tempo = (60000 / bpm) / 4;
            }
            if (edit == 1) songPos = minMax(songPos+1, 0, 9);
            if (edit == 2) lastPattern = minMax(lastPattern+1, 0, 9);
            if (edit == 3) loopTo = minMax(loopTo+1, 0, 9);
            if (edit == 4) _songPos[songPos][0] = minMax(_songPos[songPos][0]+1, 0, 29);
            if (edit == 5) _songPos[songPos][1] = minMax(_songPos[songPos][1]+1, 0, 29);
            if (edit == 6) _songPos[songPos][2] = minMax(_songPos[songPos][2]+1, 0, 29);
            else if (edit == 7) {
                // Mute or unmute channel 1.
                if( track1on ) { track1on=0; setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track1on = 1;
            }
            else if (edit == 8) {
                // Mute or unmute channel 2.
               if( track2on ) { track2on=0; setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track2on = 1;
            }
            else if (edit == 9) {
                // Mute or unmute channel 3.
               if( track3on ) { track3on=0; setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0); }
                else track3on = 1;
            }
        }
    }
    else if(mode == Modes::play){ // play, pause, stop
        if (pok.buttons.pressed(BTN_A)){
            changed=true;
            stop = 0;
            play = !play;
            if (play) {
                playTime = pok.getTime();
                rowPointer = 0;
                screenPointer = 0;
                screenMin = 0;
                screenMax = screenMaxInit;
                playPtn(); // for legacy reasons
            }
    	}
    	if (pok.buttons.pressed(BTN_LEFT)) {track1on = !track1on; setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);}
    	if (pok.buttons.pressed(BTN_UP)) {track2on = !track2on; setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);}
    	if (pok.buttons.pressed(BTN_RIGHT)) {track3on = !track3on; setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);}
    	if (pok.buttons.pressed(BTN_DOWN)) {track1on=track2on=track3on=true;}
    	if (pok.buttons.pressed(BTN_B)){

            // If already stopped go to travel mode
            if(!playing) {
                mode = Modes::travel;
            }

            changed=true;
            // Zero all oscillators
            setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            playing = false;
            stop = 1;
            play = 0;
            songPos = 0;
            rowPointer = 0;
            screenPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
    	}
    	if (pok.buttons.pressed(BTN_C)){
            changed=true;
            // Zero all oscillators
            setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
            playing = false;
            play = 0;
            mode = Modes::settings;
    	}
    }
    return changed; //return bool true if buttons were pressed
}

void Tracker::fillArrays(){
    for(uint8_t i = 0; i < 30; i++){
        for(uint8_t j = 0; j < 64; j++){
            _patch[i][j] = 0; //0 means inactive
            _pitch[i][j] = -1; //255 means invalid note
        }
    }
    lastPattern=1;
    loopTo=0;
    numPatches=15;
    bpm=60;
    track1on=true;
    track2on=true;
    track3on=true;
}

void Tracker::drawLines(){ // draw lines in tracker mode
    pok.display.setColor(3);
    pok.display.drawFastVLine(vLines[0], 0, screenH);
    pok.display.drawFastVLine(vLines[1], 0, screenH);
    pok.display.drawFastVLine(vLines[2], 0, screenH);
    pok.display.drawFastVLine(vLines[3], 0, screenH);
    pok.display.drawFastHLine(0, fontH + 1, vLines[3]);
    pok.display.setColor(1);
    pok.display.setCursor(vLines[0] + 1, 0);
    pok.display.print("Trk 1 (");
    pok.display.print(_songPos[songPos][0], 10);
    pok.display.print(")");
    pok.display.setCursor(vLines[1] + 1, 0);
    pok.display.print("Trk 2 (");
    pok.display.print(_songPos[songPos][1], 10);
    pok.display.print(")");
    pok.display.setCursor(vLines[2] + 1, 0);
    pok.display.print("Trk 3 (");
    pok.display.print(_songPos[songPos][2], 10);
    pok.display.print(")");
}

void Tracker::drawPointer(){
    pok.display.setColor(2);
    if (mode == Modes::travel){
        pok.display.drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (9 * fontW) + 10, fontH + 1);
    }
    if (mode == Modes::edit){
        if (edit == 0){
            pok.display.drawRect(vLines[colPointer], (fontH + 1) * (screenPointer + 1), (3 * fontW) + 5, fontH + 1);
        }
        if (edit == 1){
            pok.display.drawRect(vLines[colPointer] + ((fontH * 3) + 1), (fontH + 1) * (screenPointer + 1), (2 * fontW) + 5, fontH + 1);
        }
    }
    if (mode == Modes::settings){
        pok.display.drawRect(vLines[3] + 1, (fontH + 1) * ((edit * 2) + 1), (6 * fontW) + 7, fontH + 1);
    }
    if (mode == Modes::play){
        pok.display.drawRect(vLines[0], (fontH + 1) * (screenPointer + 1), vLines[3] - vLines[0], fontH + 1);
    }
}

void Tracker::drawHelpBar(){

    pok.display.setColor(3);
    uint16_t bgcolor_org = pok.display.bgcolor;
    pok.display.fillRect(0, screenH-10+3, screenW+1, 10);
    pok.display.setColor(0,3);
    pok.display.setFont(font3x5);
    if (mode == Modes::travel){
        if(screen==0)
            pok.display.print(2, screenH-6,         "C: Play mode   B: Delete   A: Edit mode");
        else
            pok.display.print(2, screenH-6,         "C: Travel mode");
    }
    if (mode == Modes::edit){
        pok.display.print(2, screenH-6,             "A: Travel mode");
    }
    if (mode == Modes::settings){
        pok.display.print(2, screenH-6,             "C: More settings");
    }
    if (mode == Modes::play){
        if(playing) {
            if(!play)
                pok.display.print(2, screenH-6,     "C: Settings     B: Stop    A: Play");
            else
                pok.display.print(2, screenH-6,     "C: Settings     B: Stop    A: Pause");
        }
        else {
            if(!play)
                pok.display.print(2, screenH-6,     "C: Settings  B: Travel mode  A: Play");
            else
                pok.display.print(2, screenH-6,     "C: Settings  B: Travel mode  A: Pause");
        }
    }
    pok.display.bgcolor = bgcolor_org;
    pok.display.setFont(font5x7);
}

void Tracker::drawIsPlaying(){
    if (stop){
        pok.display.drawBitmap(2, 1, stopBitmap);
    }
    else{
        if (play){
            pok.display.drawBitmap(2, 1, playBitmap);
        }
        else {
            pok.display.drawBitmap(2, 1, pauseBitmap);
        }
    }
}

void Tracker::printValues(){
    uint8_t counter = 0;
    for (uint8_t i = screenMin; i <= screenMax; i++){
        /* TRACK 1 */
        if (track1on) {
        if ((i % 4) == 0) pok.display.setColor(3);
        else pok.display.setColor(1);
        } else pok.display.setColor(2);
        pok.display.setCursor(0, ((fontH + 1) * (counter + 1)) + 1);
        pok.display.print(screenMin + counter); // row numbering
        pok.display.setCursor(vLines[0] + 2, ((fontH + 1) * (counter + 1)) + 1); // first col
        if (_pitch[_songPos[songPos][0]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][0]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][0]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][0]][i] / 12 + 1);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][0]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][0]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][0]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][0]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][0]][i]);
        /* TRACK 2 */
        if (track2on) {
        if ((i % 4) == 0) pok.display.setColor(3);
        else pok.display.setColor(1);
        } else pok.display.setColor(2);
        pok.display.setCursor(vLines[1] + 2, ((fontH + 1) * (counter + 1)) + 1); // second col
        if (_pitch[_songPos[songPos][1]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][1]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][1]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][1]][i] / 12+1);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][1]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][1]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][1]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][1]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][1]][i]);
        /* TRACK 3 */
        if (track3on) {
        if ((i % 4) == 0) pok.display.setColor(3);
        else pok.display.setColor(1);
        } else pok.display.setColor(2);
        pok.display.setCursor(vLines[2] + 2, ((fontH + 1) * (counter + 1)) + 1); // third col
        if (_pitch[_songPos[songPos][2]][i] == -1) pok.display.print("---");//pitch
        else{
            pok.display.print(noteLetters[_pitch[_songPos[songPos][2]][i] % 12][0]);
            pok.display.print(noteLetters[_pitch[_songPos[songPos][2]][i] % 12][1]);
            pok.display.print(_pitch[_songPos[songPos][2]][i] / 12+1);
        }
        pok.display.print(" ");
        if (_patch[_songPos[songPos][2]][i] == -1) pok.display.print("--");//patch
        else{
            if (_patch[_songPos[songPos][2]][i] < 10) pok.display.print("0");
            pok.display.print(_patch[_songPos[songPos][2]][i]);
        }
        pok.display.print(" ");
        if (_pitch[_songPos[songPos][2]][i] == -1) pok.display.print("--");//ref number
        else pok.display.print(_pitch[_songPos[songPos][2]][i]);
        counter++;
    }
}

void Tracker::printSettings(){
    pok.display.setColor(1);
    pok.display.setCursor(s_vLines[0], 1);
    pok.display.print("Tempo");
    pok.display.setCursor(s_vLines[0], fontH + 2);
    pok.display.print(bpm, 10);
    pok.display.setCursor(s_vLines[0], (2 * fontH) + 3);
    pok.display.print("SngPos");
    pok.display.setCursor(s_vLines[0], (3 * fontH) + 4);
    pok.display.print(songPos, 10);
    pok.display.setCursor(s_vLines[0], (4 * fontH) + 5);
    pok.display.print("Pattrn");
    //pok.display.print("Seqpos");
    pok.display.setCursor(s_vLines[0], (5 * fontH) + 6);
    pok.display.print((int)lastPattern, 10);
    //pok.display.print((int)sequencepos, 10);
    pok.display.setCursor(s_vLines[0], (6 * fontH) + 7);
    pok.display.print("LoopTo");
    pok.display.setCursor(s_vLines[0], (7 * fontH) + 8);
    pok.display.print((int)loopTo, 10);
    pok.display.setCursor(s_vLines[0], (8 * fontH) + 9);
    pok.display.print("T1Blck");
    pok.display.setCursor(s_vLines[0], (9 * fontH) + 10);
    pok.display.print(_songPos[songPos][0], 10);
    pok.display.setCursor(s_vLines[0], (10 * fontH) + 11);
    pok.display.print("T2Blck");
    pok.display.setCursor(s_vLines[0], (11 * fontH) + 12);
    pok.display.print(_songPos[songPos][1], 10);
    pok.display.setCursor(s_vLines[0], (12 * fontH) + 13);
    pok.display.print("T3Blck");
    pok.display.setCursor(s_vLines[0], (13 * fontH) + 14);
    pok.display.print(_songPos[songPos][2], 10);
    pok.display.setCursor(s_vLines[0], (14 * fontH) + 15);
    pok.display.print("MuteT1");
    pok.display.setCursor(s_vLines[0], (15 * fontH) + 16);
    pok.display.print(!track1on, 10);
    pok.display.setCursor(s_vLines[0], (16 * fontH) + 17);
    pok.display.print("MuteT2");
    pok.display.setCursor(s_vLines[0], (17 * fontH) + 18);
    pok.display.print(!track2on, 10);
    pok.display.setCursor(s_vLines[0], (18 * fontH) + 19);
    pok.display.print("MuteT3");
    pok.display.setCursor(s_vLines[0], (19 * fontH) + 20);
    pok.display.print(!track3on, 10);
}

bool Tracker::playTracker(){
    bool changed=false;
    songPos = sequencepos;
    if (playerpos!=oplayerpos) { //(pok.getTime() - playTime) >= _tempo){
        changed=true;
        uint8_t a = screenMaxInit;//screenMaxInit / 2;
        uint8_t b = 42;//((maxRow - 1) - (screenMaxInit / 2)) - 1;
        rowPointer=playerpos;

        if (sequencepos != oseqpos || rowPointer > 63) { //rowPointer >= maxRow){
            songPos=sequencepos;
            oseqpos = sequencepos;
            rowPointer = 0;
            screenMin = 0;
            screenMax = screenMaxInit;
            screenPointer = 0;
        }

        if (rowPointer <= 10){
            /* rowPointer on rows up to 10, only cursor moves down*/
            //rowPointer++;
            //screenPointer++;
            screenPointer=rowPointer;
        }
        else if ((rowPointer >= 11) && (rowPointer <= 54)){
            /* rowPointer 11 to 42, 'roll screen' with cursor stationary on row 10 */
            //rowPointer++;
            //screenMin++;
            screenMin=rowPointer+9-screenMaxInit; // 11+9-19 = 1, check.
            //screenMax++;
            screenMax=screenMin+screenMaxInit; //1+19=20, check.
            screenPointer=10; //stick cursor on row 10

        }
        else if (rowPointer > 54){
            /* row pointer is 53 or above, do not scroll screen. Cursor moves down */
            screenPointer=rowPointer-44; // 63 - 44 = 19
            screenMin=44;
            screenMax=63;
            //rowPointer++;
            //screenPointer++;
        }

        //if (songPos == (lastPattern + 1)){
        //    songPos = loopTo;
        //}
        oplayerpos = playerpos; //playTime = pok.getTime();

    }
    return changed; //return true if we need to update the screen
}

int Tracker::minMax(int _val, int _min, int _max){
    if (_val < _min) return _max;
    else if (_val > _max) return _min;
    else return _val;
}

int32_t Tracker::extractNextInteger(char* str) {
    char c=1; int n=0; bool negative=false;
    char valuestr[10]; int32_t number=0;
    while (!((int)c > 47 && (int)c < 58)) {
        // loop until we get a number
        c = *str++;
        if (c == NULL) return 0; //end of string
    }
    if (*(str-2)=='-') negative=true; // detect if negative value
    while ((int)c > 47 && (int)c < 58) {
        // loop until number ends
        valuestr[n++] = c;
        c = *str++;
        if (c==NULL) break; //end of string
        if (n==10) break; // too long number! it is more than an uint32
    }
    int exp=1;
    for (int i=0; i<n; i++) {
            int t;
            t = (uint8_t)valuestr[n-i-1]-(uint8_t)('0');
            number += exp*t;
            exp *= 10; // raising to power of 10, math.h pow actually gives an error due to double
    }
    if (negative) number *= -1;
    if (c==NULL) _textptr=NULL; //no more to read
    else {
            _textptr=str; // shift remainder pointer to remaining text
    }
    return number; // send back the integer
}


int Tracker::loadSong(char* songname){
    //pok.display.clear();
    //pok.display.println("Loading song...");
    char text[80]; // for reading lines of text from the file
    if (fileOpen(songname, FILE_MODE_READONLY)) {return 1;} //open failed
    int numinst=1; int numpat=1;
    // file load succeeded, save loaded file name to cookie
    for (int i=0; i<14; i++) trCookie.songname[i]=songname[i];
    trCookie.firstsave=12345678; //not first save anymore
    trCookie.saveCookie();

    // Song header
    fileReadLine(&text[0],80); // vanity text only
    fileReadLine(&text[0],80); // version number
    fileReadLine(&text[0],80);
    bpm = (int16_t)extractNextInteger(&text[0]);
    fileReadLine(&text[0],80);
    lastPattern = extractNextInteger(&text[0]);
    song.song_end=lastPattern; //legacy synth code
    fileReadLine(&text[0],80);
    loopTo = extractNextInteger(&text[0]);
    song.song_loop = loopTo; //legacy synth code
    fileReadLine(&text[0],80);
    numPatches = extractNextInteger(&text[0]);

    // Read Block Sequence from file
    uint8_t highestBlockInUse=0;
    for (int i=0; i<=lastPattern; i++) {
        int temp = 0;
        fileReadLine(&text[0],80);
        temp = extractNextInteger(&text[0]);
        for (int j=0; j<3; j++) {
                temp = extractNextInteger(_textptr); //iterate over remaining text
                if (temp>highestBlockInUse) highestBlockInUse = temp;
                _songPos[i][j] = temp;
        }
    }
    // Read blocks into pitch and patch arrays
    for (int b=0; b<30; b++) { //#of blocks, should actually be handled with dynamic memory
       int temp=0;
        if (pok.update()) {
            pok.display.setCursor(0,0);
            pok.display.println("Loading...");
            pok.display.print(b);
            pok.display.print("/30");
        }
        for (int r=0; r<64; r++) { //#of rows
            fileReadLine(&text[0],80); // read text in
            if (b>highestBlockInUse) {
                _pitch[b][r] = 255; // read in pitch
                _patch[b][r] = 0; // read in patch
            } else {
                temp = extractNextInteger(&text[0]); // dummy read block #
                temp = extractNextInteger(_textptr); // dummy read row#
                temp = extractNextInteger(_textptr); //iterate over remainder
                _pitch[b][r] = temp; // read in pitch
                temp = extractNextInteger(_textptr); //iterate over remainder
                _patch[b][r] = temp; // read in patch
            }
        }
    }
    // Read instruments
    // INSTRUMENTS GO FROM 1 TO 15 !!!!!
    //Tracker::emptyPatches(); - this causes file to close!!
    //fileReadLine(&text[0],80); // dummy read
    for (int i=1;i<=numPatches;i++) {
        char pname[10];
        fileReadLine(&pname[0],10);
        strcpy(&patchnames[i][0],&pname[0]);
        fileReadLine(&text[0],80);
        patch[i].wave = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].vol = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].bendrate = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].maxbend = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].vibrate = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].arpmode = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].adsr = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].attack = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].decay = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].sustain = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].release = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].loop = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].echo = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].overdrive = extractNextInteger(&text[0]);
        fileReadLine(&text[0],80);
        patch[i].kick = extractNextInteger(&text[0]);
    }

    fileClose();

    return 0; // no error
}

void Tracker::saveSong(FILE* fp){

    // Song header
    fprintf(fp, rboyChar);
    fprintf(fp, unknownChar);
    fprintf(fp, "\n");
    fprintf(fp, BPMChar);
    fprintf(fp, "%d", bpm);
    fprintf(fp, "\n");
    fprintf(fp, lastPatternChar);
    fprintf(fp, "%d", lastPattern);
    fprintf(fp, "\n");
    fprintf(fp, loopToChar);
    fprintf(fp, "%d", loopTo);
    fprintf(fp, "\n");
    fprintf(fp, patchesChar);
    fprintf(fp, "%d", NUM_PATCHES); //always save full set of patches, ToDo hack for now
    fprintf(fp, "\n");

    // Write Block Sequence
    for(uint8_t j = 0; j < lastPattern + 1; j++){
        fprintf(fp, blockSeqChar);
        fprintf(fp, "%d ,", j);
        fprintf(fp, "%d,%d,%d\n", _songPos[j][0], _songPos[j][1], _songPos[j][2]);
    }

    // Write blocks from pitch and patch arrays
    for (uint8_t k = 0; k < 30; k++){
        for (uint8_t j = 0; j < 64; j++){ //#of rows

            fprintf(fp, blockChar);
            fprintf(fp, "%d", k);
            fprintf(fp, rowChar);
            fprintf(fp, "%d ,", j);
            if(_pitch[k][j] == -1) fprintf(fp, "255,");
            else fprintf(fp, "%d,", _pitch[k][j]);
            if(_patch[k][j] == -1) fprintf(fp, "0");
            else fprintf(fp, "%d\n", _patch[k][j]);
        }
    }
}

char Tracker::intToChar(int _int, uint8_t digit){
    return ((_int % int(pow(10, (digit + 1)))) / int(pow(10, digit)))  + '0';
}

uint8_t Tracker::digitLength(int _int){
    for (uint8_t i = 9; i > 0; i--){
        if((_int % int(pow(10, (i + 1))) / int(pow(10, i))) != 0) return i;
    }
}

void Tracker::filePutInt(int _int){
    char text[16];
    itoa(_int, text, 10);
    filePrint(text, strlen(text)+1);
}

void Tracker::filePrint(const char *_string, uint8_t _size){
    for(uint8_t i = 0; i < _size - 1; i++){
        filePutChar(_string[i]);
    }
}

void Tracker::filePrint(char * _string, uint8_t _size){
    for(uint8_t i = 0; i < _size - 1; i++){
        filePutChar(_string[i]);
    }
}

void Tracker::NL(){
    filePutChar(10);
    //filePutChar(13);
}

void Tracker::initStreams() {
    // Put stream memory pointers in place
    uint16_t blocknum;
    songPos=sequencepos; //try to fix unsyncing problem
    // retarget pointers for track 1
    if(!t1Mute)
    {
        blocknum=_songPos[songPos][0]; //read current block number for track 1
        song.instrument_stream[0]=(uint8_t*)&_patch[blocknum][0];
        song.note_stream[0]=(uint8_t*)&_pitch[blocknum][0];
    }

    // retarget pointers for track 2
    if(!t2Mute)
    {
        blocknum=_songPos[songPos][1]; //read current block number for track 2
        song.instrument_stream[1]=(uint8_t*)&_patch[blocknum][0];
        song.note_stream[1]=(uint8_t*)&_pitch[blocknum][0];
    }

    // retarget pointers for track 3
    if(!t3Mute)
    {
        blocknum=_songPos[songPos][2]; //read current block number for track 3
        song.instrument_stream[2]=(uint8_t*)&_patch[blocknum][0];
        song.note_stream[2]=(uint8_t*)&_pitch[blocknum][0];
    }
}

void Tracker::playPtn() {
    samplespertick = (float)((60.0f/(float)bpm)*POK_AUD_FREQ)/16;
    samplesperpattern = samplespertick * 64;
    playerpos=0;
    sequencepos = songPos; //Position->GetValue();_e
    notetick = samplespertick; // Initiate samples per tick counter, force first SetOsc
    tick=3;
    // Zero all oscillators
    setOSC(&osc1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    setOSC(&osc2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    setOSC(&osc3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

    Tracker::initStreams();
    streamsFunction ptr = (streamsFunction)&Tracker::initStreams;
    registerStreamsCallback(ptr);

    playing=true;
}

