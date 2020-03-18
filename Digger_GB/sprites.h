#include "Pokitto.h"


const byte empty[] PROGMEM = {8, 6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,};
const byte wall[] PROGMEM = {8, 6, 0xFC, 0x84, 0xFC, 0xFC, 0x30, 0xFC,};
const byte rock[] PROGMEM = {8, 6, 0x0, 0x70, 0x78, 0xF8, 0xF8, 0x70,};
const byte soil[] PROGMEM = {8, 6, 0x90, 0x24, 0x88, 0x50, 0x4, 0x48,};
const byte diamond[] PROGMEM = {8, 6, 0x0, 0x20, 0x50, 0x88, 0x50, 0x20,};
const byte changer[] PROGMEM = {8, 6, 0x0, 0xFC, 0x84, 0x48, 0x30, 0xFC,};
const byte door[] PROGMEM = {8, 6, 0x78, 0x48, 0x48, 0x58, 0x48, 0x48,};
const byte monster[] PROGMEM = {8, 6, 0x30, 0x48, 0x48, 0x30, 0x30, 0xCC,};
const byte player[] PROGMEM = {8, 6, 0x20, 0x50, 0x20, 0x70, 0x20, 0xD8,};
const byte death[] PROGMEM = {8, 6, 0x20, 0x70, 0x20, 0x70, 0xA8, 0x24,};
const byte diamond2[] PROGMEM = {8, 6, 0x0, 0x20, 0x50, 0xB8, 0x70, 0x20,};
const byte monster_d[] PROGMEM = {8, 6, 0x70, 0xA8, 0xF8, 0x70, 0x20, 0xD8,};
const byte monster_l[] PROGMEM = {8, 6, 0x10, 0x68, 0x90, 0x38, 0x10, 0x68,};
const byte monster_r[] PROGMEM = {8, 6, 0x20, 0x58, 0x24, 0x70, 0x20, 0x58,};
const byte monster_u[] PROGMEM = {8, 6, 0x70, 0xF8, 0xF8, 0x70, 0x20, 0xD8,};
const byte digger_d[] PROGMEM = {8, 6, 0x70, 0xA8, 0x70, 0x20, 0x70, 0xD8,};
const byte digger_u[] PROGMEM = {8, 6, 0x70, 0xF8, 0x70, 0x20, 0x70, 0xD8,};
const byte digger_l[] PROGMEM = {8, 6, 0x70, 0xB8, 0xF8, 0x30, 0x70, 0xD8,};
const byte digger_r[] PROGMEM = {8, 6, 0x70, 0xE8, 0xF8, 0x60, 0x70, 0xD8,};

// array containing the different sprites

const byte* sprites[NUM_SPRITES] = {
  empty, wall, rock, soil, diamond, changer, door,  monster_d, monster_l, monster_u, monster_r, digger_d, death, diamond2, empty, empty, digger_l, digger_u, digger_d, digger_d, digger_r,
};
//initial/preferred direction of monster-movement:    down      up     lefturn rightturn


