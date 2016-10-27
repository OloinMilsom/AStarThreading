#include "BasicTypes.h"

#pragma region Vector

Vector2::Vector2(float _x, float _y) 
	:x(_x), 
	 y(_y) {
};

float Vector2::length() { 
	return (float)sqrt(x*x + y*y); 
};

Vector2 Vector2::operator+ (const Vector2 & other) {
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator- (const Vector2 & other) {
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator* (const float &scalar) {
	return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/ (const float &scalar) {
	return Vector2(x / scalar, y / scalar);
}

void Vector2::operator+= (const Vector2 & other) {
	x += other.x;
	y += other.y;
}

void Vector2::operator-= (const Vector2 & other) {
	x -= other.x;
	y -= other.y;
}

void Vector2::operator*= (const float & scalar) {
	x *= scalar;
	y *= scalar;
}

void Vector2::operator/= (const float & scalar) {
	x /= scalar;
	y /= scalar;
}

#pragma endregion Vector

#pragma region Size

Size::Size(float _w, float  _h) 
	:w(_w), 
	 h(_h) {
};

Size::Size(Vector2 v)
	:w(v.x),
	 h(v.y) {
}

#pragma endregion Size