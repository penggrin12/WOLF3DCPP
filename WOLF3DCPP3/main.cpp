#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )

// libs
#include <GL/freeglut.h>
#include <cmath>
#include <vector>

// .h
#include "utils.h"
#include "enums.h"

// textures
#include "Textures/AllTextures.ppm"
#include "Textures/sky.ppm"
#include "Textures/title.ppm"
#include "Textures/won.ppm"
#include "Textures/lost.ppm"
#include "Textures/sprites.ppm"

using namespace std;

void makeSprite(int type, int state, int texture, double x, double y, double z);
void makeSpriteNPC(int type, int state, int texture, int health, bool dead, double x, double y, double z);

int frame1, frame2, fps;
int timer = 0;
double fade = 0;

#define mapSS 8
#define mapS 64

int mapW[mapSS * mapSS] =
{
 1,1,1,1,2,2,2,2,
 5,0,0,1,0,0,0,2,
 1,0,0,4,0,1,0,2,
 1,1,4,1,0,0,0,2,
 2,0,0,0,0,0,0,1,
 2,0,0,0,0,1,0,1,
 2,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1,
};

int mapF[mapSS * mapSS] =
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,1,1,1,0,
 0,0,0,0,1,0,1,0,
 0,0,1,0,1,2,1,0,
 0,0,1,0,0,0,0,0,
 0,0,1,0,1,0,0,0,
 0,1,1,1,1,0,2,0,
 0,0,0,0,0,0,0,0,
};

int mapC[mapSS * mapSS] =
{
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
 0,1,1,1,0,0,0,0,
 0,0,1,0,0,0,0,0,
 0,0,1,0,0,0,0,0,
 0,0,0,0,0,0,0,0,
};

class GameManager
{
public:
    int gameState = 0;
} gameManager;

class PlayerKeys
{
public:
    bool white;
    bool red, blue, yellow, green;

    void reset()
    {
        this->white = false;
        this->red = false, this->blue = false, this->yellow = false, this->green = false;
    }
} MyKeys;

class Player
{
private:
    int health = 100;
    bool dead = false;
public:
    double x, y, dx, dy, a;

    int damage(int dmg, DMG dmgType)
    {
        if (this->health > 0)
        {
            this->health -= dmg;
        }

        if ((this->health <= 0) && (!this->dead))
        {
            this->dead = true;
        }
    }

    bool isDead()
    {
        return this->dead;
    }
} player;

class ButtonKeys
{
public:
    bool w, a, d, s;
    bool space, lmb;
    bool moving(void) { return (w || s || a || d); }
    bool shooting(void) { return (space || lmb); }

    void ButtonDown(unsigned char key, int x, int y)
    {
        if (key == 'a') { this->a = true; }
        if (key == 'd') { this->d = true; }
        if (key == 'w') { this->w = true; }
        if (key == 's') { this->s = true; }
        if (key == 'e' && MyKeys.white == true)
        {
            int xo = 0; if (player.dx < 0) { xo = -25; }
            else { xo = 25; }
            int yo = 0; if (player.dy < 0) { yo = -25; }
            else { yo = 25; }
            int ipx = player.x / 64.0, ipx_add_xo = (player.x + xo) / 64.0;
            int ipy = player.y / 64.0, ipy_add_yo = (player.y + yo) / 64.0;
            if (mapW[ipy_add_yo * mapSS + ipx_add_xo] == 4) { mapW[ipy_add_yo * mapSS + ipx_add_xo] = -4; }
        }
        if (key == ' ') { this->space = true; }

        glutPostRedisplay();
    }

    void ButtonUp(unsigned char key, int x, int y)
    {
        if (key == 'a') { this->a = false; }
        if (key == 'd') { this->d = false; }
        if (key == 'w') { this->w = false; }
        if (key == 's') { this->s = false; }
        if (key == ' ') { this->space = false; }
        glutPostRedisplay();
    }
} Keys;

class Sprite
{
public:
    int type;
    int state;
    int texture;
    double speed;
    double x, y, z, a;

    void remove()
    {
        this->type = 0; this->state = 0;
    }

    bool think();
};

class SpriteNPC : public Sprite
{
public:
    int health;
    bool dead;

    int damage(int dmg, DMG dmgType)
    {
        if (this->health > 0)
        {
            this->health -= dmg;
        }

        if ((this->health <= 0) && (!this->dead))
        {
            this->dead = true;
        }
    }

    bool think()
    {
        bool toRender = true;

        switch (this->type) {
        case 3:
            int spx = (int)this->x >> 6, spy = (int)this->y >> 6;
            int spx_add = ((int)this->x + 15) >> 6, spy_add = ((int)this->y + 15) >> 6;
            int spx_sub = ((int)this->x - 15) >> 6, spy_sub = ((int)this->y - 15) >> 6;

            if (this->x > player.x && mapW[spy * 8 + spx_sub] <= 0) { this->x -= 0.04 * fps; }
            if (this->x < player.x && mapW[spy * 8 + spx_add] <= 0) { this->x += 0.04 * fps; }
            if (this->y > player.y && mapW[spy_sub * 8 + spx] <= 0) { this->y -= 0.04 * fps; }
            if (this->y < player.y && mapW[spy_add * 8 + spx] <= 0) { this->y += 0.04 * fps; }

            if (player.x<this->x + 30 && player.x>this->x - 30 && player.y<this->y + 30 && player.y>this->y - 30)
                player.damage(3, DMG_MELEE);
                //gameManager.gameState = 4;

            if (this->dead)
                this->remove();

            break;
        }

        return toRender;
    }
};

class Bullet : public Sprite
{
public:
    double a;
    double dx, dy;
    DMG dmgType;
    TEAM owner;
};

//class Monster : public SpriteNPC
//{
//
//};
vector < Sprite > sprites(64);
int depth[120];

bool Sprite::think()
{
    bool toRender = true;

    switch (this->type) {
    case 1: // key
        if (player.x<this->x + 30 && player.x>this->x - 30 && player.y<this->y + 30 && player.y>this->y - 30)
        {
            this->state = 0;
            MyKeys.white = true;
        }

        if (this->state == 0)
            toRender = false;

        break;

    case 2: // light
        break;
    case 3: // enemy
        break;
    case 4: // bullet
    {
        this->x += cos(degToRad(this->a)) * this->speed * fps;
        this->y += -sin(degToRad(this->a)) * this->speed * fps;
        int sw = mapW[xyToMap(this->x, this->y, mapSS)];

        toRender = false;

        if (sw > 0)
        {
            printf("bullet hit a wall\n");

            this->remove();
            break;
        }

        for (int spr = 0;spr < sprites.size();spr++)
        {
            Sprite& sp = sprites.at(spr);

            if ((sp.type == 3) && (sp.state > 0))
            {
                if (this->x<sp.x + 20 && this->x>sp.x - 20 && this->y<sp.y + 20 && this->y>sp.y - 20)
                {
                    printf("bullet hit an enemy\n");
                    this->remove();
                    
                    sp.remove(); //FIXME: somehow use sp.damage() instead

                    makeSprite(5, 1, 2, sp.x, sp.y, sp.z);
                    makeSprite(5, 1, 5, sp.x, sp.y, sp.z);
                    break;
                }
            }
        }

        break;
    }
    case 5: // gib
        break;
    default:
        break;
    }

    return toRender;
}

class PlayerGun
{
public:
    double anim;
    double animSpeed;
    int type;
    int state;
    int texture;
    int x, y;
    int shootAnimTimer;
    bool ready;
    int ammo;

    void draw()
    {
        glPointSize(4);
        for (int y = 0;y < 64;y++)
        {
            for (int x = 0;x < 64;x++)
            {
                //int pixel = (y * 64 + x) * 3;
                int pixel = ((int)y * 64 + (int)x) * 3 + (this->texture * 64 * 64 * 3);
                int red = spritesTextures[pixel + 0];
                int green = spritesTextures[pixel + 1];
                int blue = spritesTextures[pixel + 2];

                if ((red != 255) || (green != 0) || (blue != 255))
                {
                    glColor3ub(red, green, blue);
                    glBegin(GL_POINTS);
                    glVertex2i(((this->x + this->anim) + x) * 4, ((this->y + this->anim / 16.0) + y) * 4);
                    glEnd();
                }
            }
        }
    }

    void animate()
    {
        if (this->shootAnimTimer > 0)
        {
            this->shootAnimTimer -= 1;
        }
        else
        {
            this->texture = 3;
            this->ready = true;
        }

        if (this->anim > 0.0)
        {
            if (this->anim > 3.0)
            {
                this->animSpeed = -0.25;
            }
        }
        else
        {
            if (this->anim < -3.0)
            {
                this->animSpeed = 0.25;
            }
        }

        if (Keys.moving() || (Keys.shooting() && ((this->animSpeed > 0.25) || (this->animSpeed < -0.25))))
        {
            this->anim += this->animSpeed;
        }
        else
        {
            this->anim = 0;
            //this->animSpeed = 0;
        }
    }

    void shake()
    {
        if (this->animSpeed > 0)
        {
            this->animSpeed += 0.25;
        }
        else
        {
            this->animSpeed -= 0.25;
        }
    }

    void shoot()
    {
        // FIXME: я нифига не понимаю
        //        оно не хочет отображаться (или даже в sprites помещаться)

        // FIXME: я понял в чем прикол...
        //        пуля была слишком высоко, её небыло видно.....
        //        теперь нужно почистить код
        printf("shoot\n");

        Sprite tBullet;
        
        tBullet.type = 4; tBullet.state = 1; tBullet.texture = 3;

        tBullet.speed = 1.5;
        tBullet.x = player.x; tBullet.y = player.y; tBullet.a = player.a;
        tBullet.z = 20 + (int)(getRnd() / 900);
        /*tBullet.a = player.a;
        tBullet.owner = TEAM_PLAYER;
        tBullet.dmgType = DMG_BULLET;*/
        
        //tBullet.make(player.x, player.y, player.a, DMG_BULLET, TEAM_PLAYER);
        sprites.push_back(tBullet);

        //printf("%s \n", tBullet.type);

        shake();

        this->shootAnimTimer = 10 + (int)(getRnd() / 900);
        this->texture = 4;
        this->ready = false;
    }
} gun;

void makeSprite(int type, int state, int texture, double x, double y, double z)
{
    Sprite tSprite;
    tSprite.type = type;
    tSprite.state = state;
    tSprite.texture = texture;
    tSprite.x = x;
    tSprite.y = y;
    tSprite.z = z;

    sprites.push_back(tSprite);
}

void makeSpriteNPC(int type, int state, int texture, int health, bool dead, double x, double y, double z)
{
    SpriteNPC tSprite;
    tSprite.type = type;
    tSprite.state = state;
    tSprite.texture = texture;
    tSprite.health = health;
    tSprite.dead = dead;
    tSprite.x = x;
    tSprite.y = y;
    tSprite.z = z;

    sprites.push_back(tSprite);
}

bool spriteLogic(Sprite &sprite)
{
    bool toRender = true;

    toRender = sprite.think();

    return toRender;
}

void drawSprites()
{
    for (int spr = 0;spr < sprites.size();spr++)
    {
        Sprite &sp = sprites.at(spr);
        int x, y, s;

        if ((sp.type == 0) || (sp.state == 0))
            continue;

        bool toRender = true;

        if (sp.state > 0)
            toRender = spriteLogic(sp);

        //cout << sp.type << endl;

        if (toRender)
        {
            for (s = 0;s < 4;s++)
            {
                double sx = sp.x - player.x;
                double sy = sp.y - player.y;
                double sz = sp.z;

                double CS = cos(degToRad(player.a)), SN = sin(degToRad(player.a));
                double a = sy * CS + sx * SN;
                double b = sx * CS - sy * SN;
                sx = a; sy = b;

                sx = (sx * 108.0 / sy) + (120 / 2);
                sy = (sz * 108.0 / sy) + (80 / 2);

                int scale = 32 * 80 / b;
                if (scale < 0) { scale = 0; } if (scale > 120) { scale = 120; }

                //texture
                double t_x = 0, t_y = 63, t_x_step = 63.5 / (double)scale, t_y_step = 64 / (double)scale;

                for (x = sx - scale / 2;x < sx + scale / 2;x++)
                {
                    t_y = 63;
                    for (y = 0;y < scale;y++)
                    {
                        if (sp.state > 0 && x > 0 && x < 120 && b < depth[x])
                        {
                            int pixel = ((int)t_y * 64 + (int)t_x) * 3 + (sp.texture * 64 * 64 * 3);
                            int red = spritesTextures[pixel + 0];
                            int green = spritesTextures[pixel + 1];
                            int blue = spritesTextures[pixel + 2];
                            if ((red != 255) || (green != 0) || (blue != 255)) //dont draw if purple
                            {
                                glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(x * 8, sy * 8 - y * 8); glEnd(); //draw point 
                            }
                            t_y -= t_y_step; if (t_y < 0) { t_y = 0; }
                        }
                    }
                    t_x += t_x_step;
                }
            }
        }
    }
}

/*
void drawSprites2D()
{
    for (int spr2d = 0;spr2d < sprites2d.size();spr2d++)
    {
        Sprite2D &sp = sprites2d.at(spr2d);
        bool toRender = true;

        if (toRender)
        {
            for (int y = 0;y < 32;y++)
            {
                for (int x = 0;x < 32;x++)
                {
                    int pixel = (y * 32 + x) * 3;
                    int red   = spritesTextures[pixel + 0];
                    int green = spritesTextures[pixel + 1];
                    int blue  = spritesTextures[pixel + 2];

                    glPointSize(8);
                    glColor3ub(red, green, blue);
                    glBegin(GL_POINTS);
                    glVertex2i(((int)(sp.x) + x) * 8, ((int)(sp.y) + y) * 8);
                    glEnd();
                }
            }
        }
    }
}
*/

void shooting()
{
    if ((Keys.shooting()) && (gun.ready) && (gun.ammo > 0))
    {
        gun.shoot();
    }
}

void drawRays2D()
{
    int r, mx, my, mp, dof, side;
    double vx, vy, rx, ry, ra, xo, yo, disV, disH;

    ra = FixAng(player.a + 30);

    for (r = 0;r < 120;r++)
    {
        int vmt = 0, hmt = 0;
        //---Vertical--- 
        dof = 0; side = 0; disV = 100000;
        double Tan = tan(degToRad(ra));
        if (cos(degToRad(ra)) > 0.001) { rx = (((int)player.x >> 6) << 6) + 64;      ry = (player.x - rx) * Tan + player.y; xo = 64; yo = -xo * Tan; }
        else if (cos(degToRad(ra)) < -0.001) { rx = (((int)player.x >> 6) << 6) - 0.0001; ry = (player.x - rx) * Tan + player.y; xo = -64; yo = -xo * Tan; }
        else { rx = player.x; ry = player.y; dof = 8; }

        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapSS + mx;
            if (mp > 0 && mp < mapSS * mapSS && mapW[mp]>0) { vmt = mapW[mp] - 1; dof = 8; disV = cos(degToRad(ra)) * (rx - player.x) - sin(degToRad(ra)) * (ry - player.y); }//hit         
            else { rx += xo; ry += yo; dof += 1; }
        }
        vx = rx; vy = ry;

        //---Horizontal---
        dof = 0; disH = 100000;
        Tan = 1.0 / Tan;
        if (sin(degToRad(ra)) > 0.001) { ry = (((int)player.y >> 6) << 6) - 0.0001; rx = (player.y - ry) * Tan + player.x; yo = -64; xo = -yo * Tan; }
        else if (sin(degToRad(ra)) < -0.001) { ry = (((int)player.y >> 6) << 6) + 64;      rx = (player.y - ry) * Tan + player.x; yo = 64; xo = -yo * Tan; }
        else { rx = player.x; ry = player.y; dof = 8; }

        while (dof < 8)
        {
            mx = (int)(rx) >> 6; my = (int)(ry) >> 6; mp = my * mapSS + mx;
            if (mp > 0 && mp < mapSS * mapSS && mapW[mp]>0) { hmt = mapW[mp] - 1; dof = 8; disH = cos(degToRad(ra)) * (rx - player.x) - sin(degToRad(ra)) * (ry - player.y); }//hit         
            else { rx += xo; ry += yo; dof += 1; }
        }

        double shade = 1;
        glColor3f(0, 0.8, 0);
        if (disV < disH) { hmt = vmt; shade = 0.5; rx = vx; ry = vy; disH = disV; glColor3f(0, 0.6, 0); }

        int ca = FixAng(player.a - ra); disH = disH * cos(degToRad(ca));
        int lineH = (mapS * 640) / (disH);
        double ty_step = 32.0 / (double)lineH;
        double ty_off = 0;
        if (lineH > 640) { ty_off = (lineH - 640) / 2.0; lineH = 640; }
        int lineOff = 320 - (lineH >> 1);

        depth[r] = disH;

        if (mapW[mp] < 0)
            continue;

        //---draw walls---
        int y;
        double ty = ty_off * ty_step;
        double tx;
        if (shade == 1) { tx = (int)(rx / 2.0) % 32; if (ra > 180) { tx = 31 - tx; } }
        else { tx = (int)(ry / 2.0) % 32; if (ra > 90 && ra < 270) { tx = 31 - tx; } }
        for (y = 0;y < lineH;y++)
        {
            int pixel = ((int)ty * 32 + (int)tx) * 3 + (hmt * 32 * 32 * 3);
            int red = AllTextures[pixel + 0] * shade;
            int green = AllTextures[pixel + 1] * shade;
            int blue = AllTextures[pixel + 2] * shade;

            if ((AllTextures[pixel + 0] != 255) || (AllTextures[pixel + 1] != 0) || (AllTextures[pixel + 2] != 255))
            {
                glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(r * 8, y + lineOff); glEnd();
            }
            ty += ty_step;
        }

        //---draw floors---
        for (y = lineOff + lineH;y < 640;y++)
        {
            double dy = y - (640 / 2.0), deg = degToRad(ra), raFix = cos(degToRad(FixAng(player.a - ra)));
            tx = player.x / 2 + cos(deg) * 158 * 2 * 32 / dy / raFix;
            ty = player.y / 2 - sin(deg) * 158 * 2 * 32 / dy / raFix;
            int mp = mapF[(int)(ty / 32.0) * mapSS + (int)(tx / 32.0)] * 32 * 32;
            int pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mp * 3;
            int red = AllTextures[pixel + 0] * 0.7;
            int green = AllTextures[pixel + 1] * 0.7;
            int blue = AllTextures[pixel + 2] * 0.7;
            glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(r * 8, y); glEnd();

            //---draw ceiling--- 
            mp = mapC[(int)(ty / 32.0) * mapSS + (int)(tx / 32.0)] * 32 * 32;

            if (mp > 0)
            {
                pixel = (((int)(ty) & 31) * 32 + ((int)(tx) & 31)) * 3 + mp * 3;
                red = AllTextures[pixel + 0];
                green = AllTextures[pixel + 1];
                blue = AllTextures[pixel + 2];

                if ((red != 255) || (green != 0) || (blue != 255))
                {
                    glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(r * 8, 640 - y); glEnd();
                }
            }
        }

        ra = FixAng(ra - 0.5);
    }
}//-----------------------------------------------------------------------------

void drawSky()
{
    int x, y;
    for (y = 0;y < 40;y++)
    {
        for (x = 0;x < 120;x++)
        {
            int xo = (int)player.a * 2 - x; if (xo < 0) { xo += 120; } xo = xo % 120;
            int pixel = (y * 120 + xo) * 3;
            int red = sky[pixel + 0];
            int green = sky[pixel + 1];
            int blue = sky[pixel + 2];
            glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(x * 8, y * 8); glEnd();
        }
    }
}

void screen(int v)
{
    int x, y;
    int* T = title;
    if (v == 1) { T = title; }
    if (v == 2) { T = won; }
    if (v == 3) { T = lost; }
    for (y = 0;y < 80;y++)
    {
        for (x = 0;x < 120;x++)
        {
            int pixel = (y * 120 + x) * 3;
            int red = T[pixel + 0] * fade;
            int green = T[pixel + 1] * fade;
            int blue = T[pixel + 2] * fade;
            glPointSize(8); glColor3ub(red, green, blue); glBegin(GL_POINTS); glVertex2i(x * 8, y * 8); glEnd();
        }
    }
    if (fade < 1) { fade += 0.001 * fps; }
    if (fade > 1) { fade = 1; }
}

void mapReset()
{
    for (int mp = 0; mp < (mapSS * mapSS); mp++)
    {
        //int map = mapW[mp];
        if (mapW[mp] < 0)
        {
            mapW[mp] += (-mapW[mp] * 2);
            //printf("%s \n", mapW[mp]);
        }
    }
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0);
    player.x = 150; player.y = 400; player.a = 90;
    player.dx = cos(degToRad(player.a)); player.dy = -sin(degToRad(player.a));
    //mapW[19] = 4; mapW[26] = 4;
    MyKeys.reset();
    mapReset();

    //makeSprite2D(1, 1, 1, 15, 35);

    gun.state = 1; gun.texture = 3; gun.type = 1; // info
    gun.x = 125; gun.y = 97; gun.anim = 0; gun.animSpeed = 0.25; // position
    gun.ammo = 10; gun.ready = true; gun.shootAnimTimer = 0; // action

    sprites.clear();

    makeSprite(1, 1, 0, 4.5 * 64, 2 * 64, 20);
    makeSprite(2, 1, 1, 1.5 * 64, 4.5 * 64, 0);
    makeSprite(2, 1, 1, 3.5 * 64, 4.5 * 64, 0);
    makeSpriteNPC(3, 1, 2, 35, false, 2.5 * 64, 2 * 64, 20);
    makeSpriteNPC(3, 1, 2, 35, false, 4.5 * 64, 2 * 64, 20);
    //sprites[0].type = 1; sprites[0].state = 1; sprites[0].texture = 0; sprites[0].x = 1.5 * 64; sprites[0].y = 5 * 64;   sprites[0].z = 20; //key
    //sprites[1].type = 2; sprites[1].state = 1; sprites[1].texture = 1; sprites[1].x = 1.5 * 64; sprites[1].y = 4.5 * 64; sprites[1].z = 0;  //light 1
    //sprites[2].type = 2; sprites[2].state = 1; sprites[2].texture = 1; sprites[2].x = 3.5 * 64; sprites[2].y = 4.5 * 64; sprites[2].z = 0;  //light 2
    //sprites[3].type = 3; sprites[3].state = 1; sprites[3].texture = 2; sprites[3].x = 2.5 * 64; sprites[3].y = 2 * 64;   sprites[3].z = 20; //enemy
}

void movement()
{
    if (Keys.a) { player.a += 0.1 * fps; player.a = FixAng(player.a); player.dx = cos(degToRad(player.a));player.dy = -sin(degToRad(player.a)); }
    if (Keys.d) { player.a -= 0.1 * fps; player.a = FixAng(player.a); player.dx = cos(degToRad(player.a)); player.dy = -sin(degToRad(player.a)); }

    int xo = 0; if (player.dx < 0) { xo = -20; }
    else { xo = 20; }
    int yo = 0; if (player.dy < 0) { yo = -20; }
    else { yo = 20; }
    int ipx = player.x / 64.0, ipx_add_xo = (player.x + xo) / 64.0, ipx_sub_xo = (player.x - xo) / 64.0;
    int ipy = player.y / 64.0, ipy_add_yo = (player.y + yo) / 64.0, ipy_sub_yo = (player.y - yo) / 64.0;
    if (Keys.w)
    {
        if (mapW[ipy * mapSS + ipx_add_xo] <= 0) { player.x += player.dx * 0.2 * fps; }
        if (mapW[ipy_add_yo * mapSS + ipx] <= 0) { player.y += player.dy * 0.2 * fps; }
    }
    if (Keys.s)
    {
        if (mapW[ipy * mapSS + ipx_sub_xo] <= 0) { player.x -= player.dx * 0.2 * fps; }
        if (mapW[ipy_sub_yo * mapSS + ipx] <= 0) { player.y -= player.dy * 0.2 * fps; }
    }
}

void checkWin()
{
    if ((int)player.x >> 6 == 1 && (int)player.y >> 6 == 1)
    {
        fade = 0;
        timer = 0;
        gameManager.gameState = 3;
    }
}

void display()
{
    frame2 = glutGet(GLUT_ELAPSED_TIME); fps = (frame2 - frame1); frame1 = glutGet(GLUT_ELAPSED_TIME);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (gameManager.gameState == 0) { init(); fade = 0; timer = 0; gameManager.gameState = 1; } // init
    if (gameManager.gameState == 1) { screen(1); timer += 1 * fps; if (timer > 2000) { fade = 0; timer = 0; gameManager.gameState = 2; } } // menu
    if (gameManager.gameState == 2) // In-Game
    {
        // game logic
        movement();
        checkWin();
        shooting();
        gun.animate();

        // drawing
        drawSky();
        drawRays2D();
        drawSprites();
        gun.draw();
    }

    if (gameManager.gameState == 3) { screen(2); timer += 1 * fps; if (timer > 2000) { fade = 0; timer = 0; gameManager.gameState = 0; } } // win
    if (gameManager.gameState == 4) { screen(3); timer += 1 * fps; if (timer > 2000) { fade = 0; timer = 0; gameManager.gameState = 0; } } // loose

    glutPostRedisplay();
    glutSwapBuffers();
}

void resize(int w, int h) { glutReshapeWindow(960, 640); }

// FIXME: avoid this?
void ButtonDown(unsigned char key, int x, int y) { Keys.ButtonDown(key, x, y); }

void ButtonUp(unsigned char key, int x, int y) { Keys.ButtonUp(key, x, y); }

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(960, 640);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH) / 2 - 960 / 2, glutGet(GLUT_SCREEN_HEIGHT) / 2 - 640 / 2);
    glutCreateWindow("");
    gluOrtho2D(0, 960, 640, 0);
    init();
    glutDisplayFunc(&display);
    glutReshapeFunc(&resize);
    glutKeyboardFunc(&ButtonDown);
    glutKeyboardUpFunc(&ButtonUp);
    glutMainLoop();
}

