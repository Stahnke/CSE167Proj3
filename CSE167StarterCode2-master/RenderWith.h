#ifndef _RENDERWITH_H
#define _RENDERWITH_H

#include "Group.h"

class RenderWith : public Group {

public:

	int myMode;

	RenderWith(int mode);
	~RenderWith();

	//Draw
	void draw(glm::mat4 C, int mode, glm::vec3 color);
	void update();
};

#endif

