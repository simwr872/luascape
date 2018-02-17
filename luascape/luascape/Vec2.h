#pragma once
#include <math.h>
#include <string>

// Defines how a 2D vector behaves in space. Methods
// implemented are for basic manipulation and comparing.
struct vec2 {
	float x;
	float y;
	vec2(float _x, float _y) {
		x = _x;
		y = _y;
	}
	vec2() {
		x = 0;
		y = 0;
	}
	float length() {
		return hypot(x, y);
	}
	float dot(vec2 v) {
		return v.x*x + v.y*y;
	}
	vec2 normal() {
		return vec2(y, -x);
	}
	vec2 normalize() {
		if (this->length() == 0) {
			return vec2();
		} else {
			return *this / this->length();
		}
	}
	vec2 operator * (float s) {
		return vec2(x*s, y*s);
	}
	vec2 operator *= (float s) {
		return *this * s;
	}
	vec2 operator / (float s) {
		return vec2(x/s, y/s);
	}
	vec2 operator /= (float s) {
		return *this / s;
	}
	vec2 operator - (vec2 v) {
		return vec2(x - v.x, y - v.y);
	}
	vec2 operator -= (vec2 v) {
		return *this - v;
	}
	vec2 operator + (vec2 v) {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator += (vec2 v) {
		return *this + v;
	}
	bool operator == (vec2 v) {
		return x == v.x && y == v.y;
	}
	std::string to_string()  {
		return std::to_string((*this).x) + ", " + std::to_string((*this).y);
	}
};

// Operator defined outside struct definition to
// handle the case where a scalar is multiplied
// with a vector (s * vec2) as opposed to (vec2 * s)
/*vec2 operator * (float s, vec2 v) {
	return v * s;
}*/