#ifndef _GROUP_H
#define _GROUP_H_

#include "Node.h"

class Group : public Node {
protected:
	std::list<Node*> childList;	//Child list

public:

	//Add child
	void addChild(Node * node);

	//Remove child
	void removeChild(Node * node);

	//Draw
	virtual void draw(glm::mat4 C);
};

#endif

