#ifndef _SKYBOX_H_
#define _SKYBOX_H_

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

//For loading images
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include <stdlib.h>

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//new
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

class Skybox
{
public:
	Skybox();
	~Skybox();

	glm::mat4 toWorld;

	void draw(GLuint);

	GLuint loadCubemap(std::vector<const GLchar*> faces);

	// These variables are needed for the shader program
	GLuint skyboxVAO, skyboxVBO;
	GLuint uProjection, uView;
	GLuint cubemapTexture;
};

const GLfloat skyboxVertices[] = {
// Positions
-1.0f,  1.0f, -1.0f,
-1.0f, -1.0f, -1.0f,
1.0f, -1.0f, -1.0f,
1.0f, -1.0f, -1.0f,
1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f, -1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f, -1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

1.0f, -1.0f, -1.0f,
1.0f, -1.0f,  1.0f,
1.0f,  1.0f,  1.0f,
1.0f,  1.0f,  1.0f,
1.0f,  1.0f, -1.0f,
1.0f, -1.0f, -1.0f,

-1.0f, -1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
1.0f,  1.0f,  1.0f,
1.0f,  1.0f,  1.0f,
1.0f, -1.0f,  1.0f,
-1.0f, -1.0f,  1.0f,

-1.0f,  1.0f, -1.0f,
1.0f,  1.0f, -1.0f,
1.0f,  1.0f,  1.0f,
1.0f,  1.0f,  1.0f,
-1.0f,  1.0f,  1.0f,
-1.0f,  1.0f, -1.0f,

-1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
1.0f, -1.0f, -1.0f,
1.0f, -1.0f, -1.0f,
-1.0f, -1.0f,  1.0f,
1.0f, -1.0f,  1.0f
};

#endif

