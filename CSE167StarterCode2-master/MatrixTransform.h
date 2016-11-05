#ifndef _MATRIXTRANSFORM_H
#define _MATRIXTRANSFORM_H

#include "Group.h"
#include <vector>

class MatrixTransform : public Group {


public:

	glm::mat4 M;
	float deg;
	glm::vec3 axis;
	float angleMax;
	float count = 0;
	bool myOn = true;
	bool robot;
	glm::mat4 myC;

	MatrixTransform(glm::mat4 m);
	MatrixTransform(glm::mat4 m, bool robot);
	MatrixTransform(glm::mat4 m, float deg, glm::vec3 axis, float angleMax);
	~MatrixTransform();

	//Draw
	void draw(glm::mat4 C, int mode, glm::vec3 color);
	void update(bool on, glm::vec3 points[6], glm::vec3 normals[6]);

	//Extra debug methods
	void printMatrix(glm::mat4 mat);
};

#endif

