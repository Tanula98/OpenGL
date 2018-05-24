#include "Spiral.h"
#include <math.h>
#include <vector>
#include <tuple>
#include <GLFW/glfw3.h>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/kupihleba.h>
#include <utils/Structures.h>
# define PI 3.14159265358979323846 

using namespace obj;

Spiral::Spiral(GLuint shader):Object(shader)
{
	glUseProgram(_shader);
	//_shader = shader;
	
	_precision = 50;
	_build(_precision, 0.4f, 1.0f, 2.0f);

	glGenBuffers(1, &_VBufObj);

	glBindBuffer(GL_ARRAY_BUFFER, _VBufObj);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

obj::Spiral::Spiral(std::shared_ptr<Shader> shader)
	:Object(shader)
{
	//_newShader->enable(); ??
	_precision = 50;
	_build(_precision, 0.4f, 1.0f, 2.0f);

	glBindBuffer(GL_ARRAY_BUFFER, _newShader->getBufAllocation());
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//_newShader->disable(); ??
}



void obj::Spiral::_draw()
{
#if 0
	GLint myColor = glGetUniformLocation(_shader, "myColor");
	glUniform4f(myColor, 1.0f, 0.3f, 0.3f, 0.9f);

	
	GLint position = glGetAttribLocation(_shader, "position");
	glEnableVertexAttribArray(position);
	
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	GLint color = glGetAttribLocation(_shader, "color");

	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)sizeof(3 * GL_FLOAT));

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
#endif
	
	if (_newShader.get() != nullptr) {
		_newShader->color(1.0f, 0.3f, 0.3f, 0.9f);
		glBindBuffer(GL_ARRAY_BUFFER, _newShader->getBufAllocation());
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / 3);
	}
	else {
		glUseProgram(_shader);
		glPolygonMode(GL_FRONT_AND_BACK, _view);

		GLint position = glGetAttribLocation(_shader, "position");
		glEnableVertexAttribArray(position);

		// Because of this, it works correct:
		glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, _VBufObj);
		glDrawArrays(GL_TRIANGLES, 0, _vertices.size() / 3);
		//GLint color = glGetAttribLocation(_shader, "color");

		//glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)sizeof(3 * GL_FLOAT));
		glUseProgram(0);
	}
	//glEnableVertexAttribArray(0);
	//glBindVertexArray(0);
}


Spiral::~Spiral()
{
	if (_newShader.get() == nullptr)
		glDeleteBuffers(1, &_VBufObj);
}

void obj::Spiral::_build(int precision, float r, float R, float len)
{
	using std::get;
	const int X = 0;
	const int Y = 1;
	const int Z = 2;

	auto _f = [r, R](float u, float v) -> Point3D {
		//const float k = 0.4f;
		//const float m = 1.0f;

		return Point3D {
			r*cos(u) + R*cos(v),
			r*v,
			r*sin(u) + R*sin(v)
		};
	};
	auto _n = [r, R, f = _f](float u, float v) -> Point3D {
		Point3D tmp(f(u, v));
		/*float x = get<X>(tmp),
			y = get<Y>(tmp),
			z = get<Z>(tmp);
			*/

		glm::cross(glm::vec3(0, 0, 0),
			glm::vec3(0, 0, 0));

		return Point3D{};
	};

	//const float len = 2.0f;
	float delta = static_cast<float>(2 * PI / precision);
	//Point3D coords;
	float u = 0, v = 0;
	_vertices.clear();

	using std::move;
	for (v = 0; v <= 2 * PI * len + delta; v += delta) {
		for (u = 0; u <= 2 * PI + delta; u += delta) {
			Point3D a = _f(u, v),
				b = _f(u, v + delta),
				c = _f(u + delta, v);
			pushBack(a);
			pushNormal(a, b, c);
			pushBack(b);
			pushNormal(a, b, c);
			pushBack(c);
			pushNormal(a, b, c);
			a = _f(u + delta, v + delta);
			b = _f(u, v + delta);
			c = _f(u + delta, v);
			pushBack(a);
			pushNormal(a, b, c);
			pushBack(b);
			pushNormal(a, b, c);
			pushBack(c);
			pushNormal(a, b, c);
		}
	}
	/*for (u = 0; u <= 2 * PI + delta; u += delta) { 
		// TODO: Wrong vertices inserted!
		coords = _f(u, v);
		_vertices.push_back(get<0>(coords));
		_vertices.push_back(get<1>(coords));
		_vertices.push_back(get<2>(coords));
	}*/
}

void obj::Spiral::pushBack(Point3D point)
{
	_vertices.push_back(std::move(point.x));
	_vertices.push_back(std::move(point.y));
	_vertices.push_back(std::move(point.z));
}

void obj::Spiral::pushNormal(const Point3D & a, const Point3D & b, const Point3D & c)
{
	auto n = glm::triangleNormal(a.toVec(), b.toVec(), c.toVec());
	_vertices.push_back(std::move(n.x));
	_vertices.push_back(std::move(n.y));
	_vertices.push_back(std::move(n.z));
}

/// ATTENTION! RACE CONDITIONS HERE?
void obj::Spiral::reconstruct(int precision) 
{
	_precision = precision;
	_build(precision, 0.4f, 1.0f, 2.0f);
	_newShader->regenerateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, _newShader->getBufAllocation());
	
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int obj::Spiral::getBuildPrecision()
{
	return _precision;
}

string obj::Spiral::toString() const
{
	return "Spiral";
}

