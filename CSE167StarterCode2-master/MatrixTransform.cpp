#include "MatrixTransform.h"


bool toRender = true;
//no animation
MatrixTransform::MatrixTransform(glm::mat4 m) : M(m) {
	deg = 0.0f;
	axis = { 0.0f, 0.0f, 0.0f };
	angleMax = 0.0f;
	robot = false;
}

MatrixTransform::MatrixTransform(glm::mat4 m, bool robot) : M(m), robot(robot) {
	deg = 0.0f;
	axis = { 0.0f, 0.0f, 0.0f };
	angleMax = 0.0f;
}

//with animation
MatrixTransform::MatrixTransform(glm::mat4 m, float deg, glm::vec3 axis, float angleMax) : M(m), deg(deg), axis(axis), angleMax(angleMax) {
	robot = false;
}

MatrixTransform::~MatrixTransform() {

}

void MatrixTransform::draw(glm::mat4 C, int mode, glm::vec3 color) {

	myC = C;
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

	if(toRender)
		Group::draw(C * M, mode, color);
}

void MatrixTransform::update(bool on, glm::vec3 points[6], glm::vec3 normals[6]) {

	
	//cout << "UPDATING" << on << " : " << robot << endl;
	if (on == true && robot == true)
	{
		float radius = 2.75f;
		glm::vec3 center = glm::vec3(M * glm::vec4{ 0.0f,0.0f,0.0f,1.0f });
		float distCenter;
		//cout << "CULLING TEST~~~" << endl;
		int x = 0;
		for (x = 0; x < 6; x++)
		{
			distCenter = glm::dot(center - points[x], normals[x]);
			//cout << x << ": " << distCenter << " ? " << radius << endl;
			//cout << x << ": " << center.x << ":" << "x" << points[x].x << "y" << points[x].y << "z" << points[x].z << " : " << "x" << normals[x].x << "y" << normals[x].y << "z" << normals[x].z << endl;
			if (distCenter > radius)
			{
				toRender = false;
				//cout << "CULLED" << endl;
				break;
			}
		}
		if (x == 6) toRender = true;
	}
	else if (on == false) toRender = true;
	Group::update(on, points, normals);
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


