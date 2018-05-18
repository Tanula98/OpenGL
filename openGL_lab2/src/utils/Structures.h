#pragma once
#include <GLFW/glfw3.h>
#include <string>

using std::string;

struct Point {
	GLint x;
	GLint y;
	int calcOffset() const;
	string toString() const;
	static int width;
	static int floatsInVertex;
};
struct Line {
	Point beg;
	Point end;
	Point center() const;
	string toString() const;
};

struct RGB {
	GLfloat r;
	GLfloat g;
	GLfloat b;
};
