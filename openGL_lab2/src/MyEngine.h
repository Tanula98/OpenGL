#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Cube.h"
#include "ShaderFactory.h"

#define use(ns) using ns;
use(std::string)

class MyEngine
{
public:
	MyEngine();
	~MyEngine();

private:
	GLFWwindow *_activity;
	int _activityWidth = 720;
	int _activityHeight = 480;

	GLuint _basicShader;
	GLuint _staticShader;
	GLuint _buffer;

	GLint _type = GL_LINE;

	void _initObjects();
	void _draw();
	void _destroyObjects();

	ShaderFactory _shaderFactory;

	obj::Cube *_cube;
	obj::Cube *_miniCube;
	obj::Cube *_extraCube;

	void _mainLoop();

	static void _keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

