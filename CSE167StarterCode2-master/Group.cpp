#include "Group.h"

Group::Group() {

}

Group::~Group() {

}

void Group::draw(glm::mat4 C, int mode, glm::vec3 color) {
	for each(Node* node in childList) {
		node->draw(C, mode, color);
	}
}

void Group::update(bool on, glm::vec3 points[6], glm::vec3 normals[6]) {
	for each(Node* node in childList) {
		node->update(on, points, normals);
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


