#include "Group.h"

void Group::draw(glm::mat4 C) {
	for each(Node* node in childList) {
		node->draw(C);
	}
}

//Add child
void Group::addChild(Node * node){
	childList.push_back(node);
}

//Remove child
void Group::removeChild(Node * node){
	childList.remove(node);
}


