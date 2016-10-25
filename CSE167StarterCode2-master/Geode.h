#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"

//Classes for Scene Graph

class Geode : public Node {

protected:
	glm::mat4 modelview;

public:
	//set modelview matrix to C somehow

	//Draw
	void draw(glm::mat4 C);

	//Render
	virtual void render() = 0;
};

#endif

