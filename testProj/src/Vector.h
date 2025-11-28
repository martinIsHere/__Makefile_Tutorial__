#pragma once
#include "includes.h"

class Vector;

// Data structures --------------------------------

class Point {
public:

	int x, y;

	Point(int x, int y);

	bool isEqual(const Point* p)const;

	bool isEqual(const Point& p)const;

	Point() = default;

	Vector& operator-(const Point& p);

	Vector& operator+(const Point& p);

};

class Vector {
public:
	float x, y;

	Vector(float x, float y);

	Vector() = default;

	Vector& operator-(const Vector& p);

	Vector& operator-(const float& p);

	Vector& operator+(const float& p);

	Vector& operator+(const Vector& p);

	Vector* operator+(const Vector* p);

	Vector& operator*(const int& value);

	Vector* operator*(const int* value);

	Vector* operator*(const Vector* vec);

	Vector& multiplyWithoutEffect(const Vector& vec);

	Vector& operator/(const float& value);

	Vector& divideWithoutEffect(const float& value);

	float getMagnitude();

	void set(const float x1, const float y1);
};



// Drawing functions ------------------------------

void draw_vector_from_coordinates(SDL_Renderer* ren, const int x, const int y, const Vector vec);

void draw_vector_as_triangle(SDL_Renderer* ren, const int x, const int y, const Vector vec);

void draw_vector_as_rectangle(SDL_Renderer* ren, const int x, const int y, const Vector vec, bool draw_hypotenuse);

void dwl(SDL_Renderer* ren, int x1, int y1, int x2, int y2);



// Conversion functions ---------------------------

void set_vec_to_point(Vector& vec, const Point& p);

float deg_to_rad(const float deg);

Vector* convert_point_to_vec(const Point& p);

Point* convert_vec_to_point(const Vector& vec);

Vector* rad_to_unit_vector(const float& rad);

float get_magnitude_from_vector(const float& x1, const float& y1);

float get_magnitude_from_coords(int x1, int y1, int x2, int y2);
