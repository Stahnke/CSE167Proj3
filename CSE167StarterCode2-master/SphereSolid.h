#ifndef _SphereSolid_H
#define _SphereSolid_H

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

#include "Geode.h"
#include <vector>
#include "Window.h"

class SphereSolid : public Geode
{
public:
	SphereSolid(GLuint shaderProgram, glm::vec3 color, int mode);
	~SphereSolid();

	glm::mat4 toWorld;
	int SphereMode;
	int drawingindicesSolid;

	GLuint shaderProgram;
	glm::vec3 color;

	void render();
	void update(bool on, glm::vec3 points[6], glm::vec3 normals[6]);

	// These variables are needed for the shader program
	GLuint WVBO, WVAO, WEBO;
	GLuint SVBO, SVAO, SEBO;
	GLuint uProjection, uModelview;

	// Define the coordinates and indices needed to draw the Sphere. Note that it is not necessary
	// to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
	// This just looks nicer since it's easy to tell what coordinates/indices belong where.
};

#endif

