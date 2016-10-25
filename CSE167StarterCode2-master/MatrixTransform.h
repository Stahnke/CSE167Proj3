#ifndef _MatrixTransform_H
#define _MatrixTransform_H_

#include "Group.h"

class MatrixTransform : public Group {

protected:
	glm::mat4 M;

public:

	MatrixTransform(glm::mat4 m);
	~MatrixTransform();

	//Draw
	void draw(glm::mat4 C);
	void update();

	//Extra debug methods
	void printMatrix(glm::mat4 mat);
};

#endif

