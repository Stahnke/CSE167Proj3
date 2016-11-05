#ifndef _GROUP_H
#define _GROUP_H_

#include "Node.h"

class Group : public Node {
protected:
	std::list<Node*> childList;	//Child list

public:

	Group();
	~Group();

	//Add child
	void addChild(Node * node);

	//Remove child
	void removeChild(Node * node);

	//Draw
	virtual void draw(glm::mat4 C, int mode, glm::vec3 color);

	//Update
	virtual void update(bool on, glm::vec3 points[6], glm::vec3 normals[6]);
};

#endif

