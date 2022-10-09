#pragma once

#include "textures.h"

class Sprite
{
public:
    int type;
    int state;
    int texture;
    float x, y, z;
};

class Player
{
public:
    int health;
};

class ButtonKeys
{
public:
    bool w, a, d, s;
    bool space, lmb;
    bool moving(void) { return ((w != 0) || (s != 0) || (a != 0) || (d != 0)); }
    bool shooting(void) { return ((space) || (lmb)); }
};

class Gun
{
public:
    float anim;
    float animSpeed;
    int type;
    int state;
    int texture;
    int x, y;
    bool ready;
    bool ammo;

    void drawGun();
    void animateGun(ButtonKeys *Keys);
    void shakeGun();
};

class PlayerKeys
{
public:
    bool white;
    bool red, blue, yellow, green;
};