#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <utils/Proto.h>

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

struct Point3D {
	float x;
	float y;
	float z;
	glm::vec3 toVec() const;
};

struct Light {
	glm::vec3 position;
	glm::vec3 intensities;
};

enum ObjectType {
	CUBE,
	SPIRAL
};

struct Chunk {
	proto::AttributeType type;
	GLsizei size;
};
