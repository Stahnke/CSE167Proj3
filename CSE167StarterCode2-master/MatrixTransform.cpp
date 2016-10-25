#include "MatrixTransform.h"

MatrixTransform::MatrixTransform(glm::mat4 m) : M(m) {

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


