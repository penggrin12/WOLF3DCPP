#pragma once

#include <list>
#include "Textures/numbers.ppm"
#include <GL/freeglut.h>

using namespace std;

class FontManager
{
public:
    void renderNumber(int number, int sx, int sy)
    {
        glPointSize(4);
        for (int y = 0;y < 8;y++)
        {
            for (int x = 0;x < 8;x++)
            {
                //int pixel = (x * 8 + y) * 3;
                int pixel = (y * 8 + x) * 3 + (number * 8 * 8 * 3);
                int red = numbers[pixel + 0];
                int green = numbers[pixel + 1];
                int blue = numbers[pixel + 2];

                if ((red != 255) || (green != 0) || (blue != 255))
                {
                    glColor3ub(red, green, blue);
                    glBegin(GL_POINTS);
                    glVertex2i((x + sx) * 4, (y + sy) * 4);
                    glEnd();
                }
            }
        }
    }
	
    void renderValue(int value, int space, int x, int y)
    {
        list<int> digits;

        if (0 == value) {
            digits.push_back(0);
        }
        else
        {
            while (value != 0) {
                int last = value % 10;
                digits.push_front(last);
                value = (value - last) / 10;
            }
        }

        int off = 0;

        for ( int const &i : digits) {
            this->renderNumber(i, x + off, y);
            off += space;
        }
    }
};