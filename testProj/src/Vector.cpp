#include "Vector.h"


// Data structures --------------------------------

// Vector 

Vector::Vector(float x, float y) : x(x), y(y) {}

Vector& Vector::operator-(const Vector& p) {
	x -= p.x;
	y -= p.y;
	return *this;
}

Vector& Vector::operator-(const float& p) {
	x -= p;
	y -= p;
	return *this;
}

Vector& Vector::operator+(const float& p) {
	x += p;
	y += p;
	return *this;
}

Vector& Vector::operator+(const Vector& p) {
	Vector* vec = new Vector(x + p.x, y + p.y);
	return *vec;
}

Vector* Vector::operator+(const Vector* p) {
	x += p->x;
	y += p->y;
	return this;
}

Vector& Vector::operator*(const int& value) {
	x *= value;
	y *= value;
	return *this;
}

Vector* Vector::operator*(const int* value) {
	x *= *value;
	y *= *value;
	return this;
}

Vector* Vector::operator*(const Vector* vec) {
	x *= vec->x;
	y *= vec->y;
	return this;
}

Vector& Vector::multiplyWithoutEffect(const Vector& vec) {
	Vector* newVec = new Vector(x*vec.x, y*vec.y);
	return *newVec;
}

Vector& Vector::operator/(const float& value) {
	x /= value;
	y /= value;
	return *this;
}

Vector& Vector::divideWithoutEffect(const float& value) {
	Vector* vec = new Vector(x / value, y / value);
	return *vec;
}

float Vector::getMagnitude() {
	return get_magnitude_from_vector(x, y);
}

void Vector::set(const float x1, const float y1) {
	x = x1;
	y = y1;
}

// Point

Point::Point(int x, int y) : x(x), y(y) {}

Vector& Point::operator-(const Point& p) {
	Vector* vec = new Vector(float(x - p.x), float(y - p.y));
	return *vec;
}

Vector& Point::operator+(const Point& p) {
	Vector* vec = new Vector(x + (float)p.x, y + (float)p.y);
	return *vec;
}

bool Point::isEqual(const Point* p)const {
	if (p->x == x && p->y == y) return true;
	return false;
}

bool Point::isEqual(const Point& p)const {
	if (p.x == x && p.y == y) return true;
	return false;
}




// Drawing functions ------------------------------

void draw_vector_from_coordinates(SDL_Renderer* ren, const int x, const int y, const Vector vec) {
	dwl(ren, x, y, x + (int)vec.x, y + (int)vec.y);
}

void draw_vector_as_triangle(SDL_Renderer* ren, const int x, const int y, const Vector vec) {

	// draw hypotenuse
	draw_vector_from_coordinates(ren, x, y, vec);

	// draw y line
	dwl(ren, x, y, x, y + (int)vec.y);

	// draw x line
	dwl(ren, x, y + (int)vec.y, x + (int)vec.x, y + (int)vec.y);
}

void draw_vector_as_rectangle(SDL_Renderer* ren, const int x, const int y, const Vector vec, bool draw_hypotenuse = false) {
	//draw first triangle

	if (draw_hypotenuse) {
		// draw hypotenuse
		draw_vector_from_coordinates(ren, x, y, vec);
	}

	// draw y line
	dwl(ren, x, y, x, y + (int)vec.y);

	// draw x line
	dwl(ren, x, y + (int)vec.y, x + (int)vec.x, y + (int)vec.y);


	// draw second triangle without hypotenuse

	// draw second y line
	dwl(ren, x + (int)vec.x, y, x + (int)vec.x, y + (int)vec.y);

	// draw second x line
	dwl(ren, x, y, x + (int)vec.x, y);
}

void dwl(SDL_Renderer* ren, int x1, int y1, int x2, int y2) {
	// draw white line
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}



// Conversion functions ---------------------------

void set_vec_to_point(Vector& vec, const Point& p) {
	vec.x = float(p.x);
	vec.y = float(p.y);
}

float deg_to_rad(const float deg) {
	// PI*2/360 * degrees = radians
	float rad = (float)PI / 180 * deg;
	return rad;
}

Vector* convert_point_to_vec(const Point& p) {
	return new Vector((float)p.x, (float)p.y);
}

Point* convert_vec_to_point(const Vector& vec) {
	return new Point((int)vec.x, (int)vec.y);
}

Vector* rad_to_unit_vector(const float& rad) {
	return new Vector(std::cos(rad), std::sin(rad));
}

float get_magnitude_from_vector(const float& x, const float& y) {
	return (float)std::sqrt(x * x + y * y);
}

float get_magnitude_from_coords(const int x1, const int y1, int x2, int y2) {
	// get difference as vector in form of two coords
	x2 -= x1;
	y2 -= y1;

	x2 = abs(x2);
	y2 = abs(y2);

	// get hypotenuse length
	//float hypotenuse = std::sqrt(x2*x2 + y2*y2);

	//return value
	return get_magnitude_from_vector((float)x2, (float)y2);
}