#include "Geode.h"

void Geode::draw(glm::mat4 C, int mode, glm::vec3 color) {
	//set modelview matrix to C
	modelview = C;

	//call sphere or cube's render func
	render();
}


