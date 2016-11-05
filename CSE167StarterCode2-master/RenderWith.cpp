#include "RenderWith.h"

//no animation
RenderWith::RenderWith(int mode) : myMode(mode) {
}


RenderWith::~RenderWith() {
}

void RenderWith::draw(glm::mat4 C, int mode, glm::vec3 color) {

	//use our mode instead
	Group::draw(C, myMode, color);
}

void RenderWith::update() {
}


