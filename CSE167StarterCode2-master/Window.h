#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "Sphere.h"
#include "SphereSolid.h"
//#include "RenderWith.h"
#include "MatrixTransform.h"
#include "Skybox.h"
#include "shader.h"

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

//for proj3
#include <chrono>
#define ANG2RAD 3.14159265358979323846/180.0

using namespace std::chrono;
using namespace std;

class Window
{

private:
	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

public:

	//frustum
	static glm::vec3 planePos[6];
	static glm::vec3 planeNorm[6];

	static bool on;


	static glm::mat4 camTransform;
	static glm::mat4 camRotate;
	static glm::mat4 camRotatePos;

	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(glm::vec2 point);

	static void translateCamera(glm::vec3 transVec);
	static void rotateCamera(float rot_angle, glm::vec3 transVec);
	static void resetCamera();
	static void transformCamera();

	static void translateArmy(glm::vec3 transVec);
	static void scaleArmy(glm::vec3 transVec);
	static void rotateArmy(float rot_angle, glm::vec3 transVec);
	static void resetArmy();

	static MatrixTransform * createRobot(glm::mat4 startMat);

};

#endif
