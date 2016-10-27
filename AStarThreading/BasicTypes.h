#pragma once
#include <math.h>

class Vector2 {
public:
	float x, y;
	Vector2(float _x = 0, float _y = 0);
	float length();
	Vector2 operator+ (const Vector2 & other);
	Vector2 operator- (const Vector2 & other);
	Vector2 operator* (const float & scalar);
	Vector2 operator/ (const float & scalar);
	void operator+= (const Vector2 & other);
	void operator-= (const Vector2 & other);
	void operator*= (const float & scalar);
	void operator/= (const float & scalar);
};


class Size {
public:
	float w, h;
	Size(float _w = 0, float  _h = 0);
	Size(Vector2 v);
};

class Colour {
public:
	int r, g, b, a;
	Colour(int _r = 255, int  _g = 255, int _b = 255, int  _a = 255) :r(_r), g(_g), b(_b), a(_a) {};
};

class Rect {
public:
	Vector2 pos;
	Size size;
	Rect(Vector2 p, Size s) :pos(p), size(s) {};
	Rect(float x = 0, float y = 0, float w = 1, float h = 1) :pos(x, y), size(w, h) {};

};