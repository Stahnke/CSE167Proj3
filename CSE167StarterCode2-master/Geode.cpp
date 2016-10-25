#include "Geode.h"

void Geode::draw(glm::mat4 C) {
	//set modelview matrix to C
	modelview = C;

	//call sphere or cube's render func
	render();
}


