
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "Keyboard.h"
using namespace std;

void RGBtoHSV(float r, float g, float b, float *h, float *s, float *v)
{
	float minval, maxval, delta;
	minval = min(r, min(g, b));
	maxval = max(r, max(g, b));
	*v = maxval;
	delta = maxval - minval;
	if (maxval != 0)
		*s = delta / maxval;
	else
    {
		*s = 0;
		*h = -1;
		return;
	}
	if (r == maxval)
		*h = (g - b) / delta;
	else if (g == maxval)
		*h = 2 + (b - r) / delta;
	else
		*h = 4 + (r - g) / delta;
	*h *= 60;
	if (*h < 0)
		*h += 360;
}
void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
{
	int i;
	float f, p, q, t;
	if (s == 0) {
		*r = *g = *b = v;
		return;
	}
	h /= 60;
	i = (int)floor(h);
	f = h - i;
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}
Keyboard kbd;
void keyboardUp(int keyCode, int x, int y) {
	kbd.keyboardUp(keyCode);
}
void keyboardDown(int keyCode, int x, int y) {
	//printf("%d ", keyCode);
	kbd.keyboardDown(keyCode);
}
