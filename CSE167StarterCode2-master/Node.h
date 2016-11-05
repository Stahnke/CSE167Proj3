#ifndef _NODE_H_
#define _NODE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <iostream>
#include <list>

using namespace std;

class Node
{
public:

	virtual void draw(glm::mat4 C, int mode, glm::vec3 color) = 0;
	virtual void update(bool on, glm::vec3 points[6], glm::vec3 normals[6]) = 0;
	void printAlive();
};

#endif

