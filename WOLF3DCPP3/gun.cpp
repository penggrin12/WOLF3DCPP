#pragma once

#include "structs.h"
#include <GL/glut.h>

void Gun::drawGun()
{
    glPointSize(8);
    for (int y = 0;y < 32;y++)
    {
        for (int x = 0;x < 32;x++)
        {
            //int pixel = (y * 32 + x) * 3;
            int pixel = ((int)y * 32 + (int)x) * 3 + (this->texture * 32 * 32 * 3);
            int red = spritesTextures[pixel + 0];
            int green = spritesTextures[pixel + 1];
            int blue = spritesTextures[pixel + 2];

            if (red != 255, green != 0, blue != 255)
            {
                glColor3ub(red, green, blue);
                glBegin(GL_POINTS);
                glVertex2i(((this->x + this->anim) + x) * 8, ((this->y + this->anim / 16.0) + y) * 8);
                glEnd();
            }
        }
    }
}

void Gun::animateGun(ButtonKeys *Keys)
{
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

    if (Keys->moving() || (Keys->shooting() && ((this->animSpeed > 0.25) || (this->animSpeed < -0.25))))
    {
        this->anim += this->animSpeed;
    }
    else
    {
        this->anim = 0;
        //this->animSpeed = 0;
    }
}

void Gun::shakeGun()
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