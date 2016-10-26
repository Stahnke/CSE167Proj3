#include "MatrixTransform.h"

//no animation
MatrixTransform::MatrixTransform(glm::mat4 m) : M(m) {
	deg = 0.0f;
	axis = { 0.0f, 0.0f, 0.0f };
	angleMax = 0.0f;
}

//with animation
MatrixTransform::MatrixTransform(glm::mat4 m, float deg, glm::vec3 axis, float angleMax) : M(m), deg(deg), axis(axis), angleMax(angleMax) {
	
}

MatrixTransform::~MatrixTransform() {

}

void MatrixTransform::draw(glm::mat4 C) {

	/*
	cout << "M matrix = " << endl;
	printMatrix(M);
	cout << "C matrix = " << endl;
	printMatrix(C);
	*/
	if (count < angleMax)
	{
		M = M * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), axis);
		count += glm::abs(deg);
		//cout << count + " " + angleMax << endl;
	}

	else {
		count = 0.0f;
		deg *= -1.0f;
	}

	Group::draw(C * M);
}

void MatrixTransform::update() {
}


//DEBUG METHOD
void MatrixTransform::printMatrix(glm::mat4 mat) {

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat[j][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


