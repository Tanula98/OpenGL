#include "MyEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define use(ns) using ns;
use(std::cout) use(std::endl)

MyEngine::MyEngine()
{
	if (!glfwInit())
		throw -1;

	_activity = glfwCreateWindow(_activityWidth, _activityHeight, "Kupihleba project", NULL, NULL);
	if (!_activity)
	{
		glfwTerminate();
		throw -1;
	}
	glfwSetKeyCallback(_activity, _keyCallback);

	glfwMakeContextCurrent(_activity);
	glfwSetWindowUserPointer(_activity, this);

	if (glewInit() != GLEW_OK) {
		throw std::exception("glewInit failed!");
	}
	else {
		cout << glGetString(GL_VERSION) << endl;
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	_basicShader = _shaderFactory.loadShader("res/shaders/basic.shader");
	_staticShader = _shaderFactory.loadShader("res/shaders/static.shader");

	glViewport(0, 0, _activityWidth, _activityHeight);
	obj::Object::setScreenDims(_activityWidth, _activityHeight);

	_initObjects();
	_mainLoop();
	_destroyObjects();
}

MyEngine::~MyEngine()
{
	glfwTerminate();
}

void MyEngine::_initObjects()
{
	_cube = shared_ptr<obj::Cube>(new obj::Cube(_shaderFactory.getBasicShader()));
	_miniCube = shared_ptr<obj::Cube>(new obj::Cube(_shaderFactory.getBasicShader()));
	_projCube = shared_ptr<obj::Cube>(new obj::Cube(_shaderFactory.getBasicShader()));
	
	_cube->setXangle(0.5f);
	_cube->setYangle(0.5f);
	_cube->setXangle(0.5f);
	_cube->setXangle(0.5f);

	_miniCube->setXangle(0.5f);
	_miniCube->setYangle(0.5f);
	_miniCube->setSize(0.2f);
	_miniCube->setPosition(0.9f, -0.7f, 0.0f);

	_projCube->setXangle(0.5f);
	_projCube->setYangle(0.5f);
	_projCube->setSize(0.2f);

	float proj[] = {
		1.0f,   0.0f,  0.0f,  0.0f,
		0.0f,   1.0f,  0.0f,  0.0f,
		0.0f,   0.0f,  1.0f,  0.0f,
	   -0.9f,  -0.9f,  0.9f,  1.0f,
	};
	_projCube->setTransformation(glm::make_mat4(proj));
	_projCube->setPosition(-0.9f, -0.7f, 0.0f);
}

void MyEngine::_destroyObjects() {}

void MyEngine::_draw()
{
	_cube->setView(_type);
	_cube->simpleDraw();
	_projCube->simpleDraw();
	_miniCube->simpleDraw();
}

void MyEngine::_mainLoop()
{
	while (!glfwWindowShouldClose(_activity))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		_draw();
		glfwSwapBuffers(_activity);
		glfwPollEvents();
	}
}

void MyEngine::_keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	MyEngine *context = static_cast<MyEngine*>(glfwGetWindowUserPointer(window));
	const float POS_STEP = 0.1f;
	const float ROT_STEP = 0.05f;
	const float SIZE_STEP = 0.05f;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		if (context->_type == GL_FILL) {
			context->_type = GL_LINE;
		} else {
			context->_type = GL_FILL;
		}
	}
	else {
		float x_angle, y_angle;
		float x, y, z;
		switch (key)
		{
		case GLFW_KEY_KP_ADD:
			context->_cube->setSize(context->_cube->getSize() + SIZE_STEP);
			break;
		case GLFW_KEY_KP_SUBTRACT:
			context->_cube->setSize(context->_cube->getSize() - SIZE_STEP);
			break;

		case GLFW_KEY_LEFT:
			x_angle = std::get<0>(context->_cube->getRotation());
			context->_cube->setXangle(x_angle - ROT_STEP);
			context->_projCube->setXangle(x_angle - ROT_STEP);
			break;
		case GLFW_KEY_RIGHT:
			x_angle = std::get<0>(context->_cube->getRotation());
			context->_cube->setXangle(x_angle + ROT_STEP);
			context->_projCube->setXangle(x_angle + ROT_STEP);
			break;

		case GLFW_KEY_UP:
			y_angle = std::get<1>(context->_cube->getRotation());
			context->_cube->setYangle(y_angle - ROT_STEP);
			context->_projCube->setYangle(y_angle - ROT_STEP);

			break;
		case GLFW_KEY_DOWN:
			y_angle = std::get<1>(context->_cube->getRotation());
			context->_cube->setYangle(y_angle + ROT_STEP);
			context->_projCube->setYangle(y_angle + ROT_STEP);
			break;

		case GLFW_KEY_W:
			y = std::get<1>(context->_cube->getPosition());
			context->_cube->setYpos(y + POS_STEP);
			break;
		case GLFW_KEY_S:
			y = std::get<1>(context->_cube->getPosition());
			context->_cube->setYpos(y - POS_STEP);
			break;

		case GLFW_KEY_A:
			x = std::get<0>(context->_cube->getPosition());
			context->_cube->setXpos(x - POS_STEP);
			break;
		case GLFW_KEY_D:
			x = std::get<0>(context->_cube->getPosition());
			context->_cube->setXpos(x + POS_STEP);
			break;
		default:
			break;
		}
	}
}
