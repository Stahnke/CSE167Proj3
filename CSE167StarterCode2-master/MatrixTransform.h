#ifndef _MatrixTransform_H
#define _MatrixTransform_H_

#include "Group.h"

class MatrixTransform : public Group {


public:

	glm::mat4 M;
	float deg;
	glm::vec3 axis;
	float angleMax;
	float count = 0;

	MatrixTransform(glm::mat4 m);
	MatrixTransform(glm::mat4 m, float deg, glm::vec3 axis, float angleMax);
	~MatrixTransform();

	//Draw
	void draw(glm::mat4 C);
	void update();

	//Extra debug methods
	void printMatrix(glm::mat4 mat);
};

#endif

