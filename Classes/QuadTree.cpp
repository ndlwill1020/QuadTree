//
//  QuadTree.cpp
//  QuadTree
//
//  Created by macuser on 16/8/17.
//
//

#include "QuadTree.hpp"
#include "Constants.h"
#include "CrossLineNode.hpp"

#define NDL_DEBUG 1

QuadTree::QuadTree()
:_level(-1)
,_bound(Rect::ZERO)
,crossLineNode(nullptr)
,isUpdate(false)
,isSplit(false)
{

}

QuadTree::~QuadTree(){

}

QuadTree* QuadTree::createWithBound(int level, const Rect& rect){
    auto ret = new (std::nothrow) QuadTree();
    if (ret && ret->initWithBound(level, rect)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool QuadTree::initWithBound(int level, const Rect& rect){
    memset(quadNode, 0, sizeof(QuadTree *) * 4);
    this->_level = level;
    this->_bound = rect;
    
    
    return true;
}

void QuadTree::clear(){
    objectRects.clear();
    
//    for (Vector<QuadTree *>::iterator it = quadNodes.begin(); it != quadNodes.end(); it++) {
//        if ((*it) != nullptr) {
//            (*it)->clear();
//            (*it) = nullptr;
//        }
//    }
    
    if (quadNode[0] != nullptr) {
        log("clear QuadNode is not nullptr!!!!!!!!!!");
        for (int i = 0; i < (sizeof(quadNode) / sizeof(quadNode[0])); i++) {
            if (quadNode[i] != nullptr) {
                quadNode[i]->clear();
                quadNode[i] = nullptr;
            }
        }
    }
    
}

void QuadTree::splitToFourQuadrants(){
    float subQuadWidth = this->_bound.size.width / 2;
    float subQuadHeight = this->_bound.size.height / 2;
    
    //父节点原点
    auto origin = _bound.origin;
    
    
    //1
    quadNode[0] = QuadTree::createWithBound(_level + 1, Rect(origin.x + subQuadWidth, origin.y + subQuadHeight, subQuadWidth, subQuadHeight));
    quadNode[0]->retain();
    
    //2
    quadNode[1] = QuadTree::createWithBound(_level + 1, Rect(origin.x, origin.y + subQuadHeight, subQuadWidth, subQuadHeight));
    quadNode[1]->retain();
    
    //3
    quadNode[2] = QuadTree::createWithBound(_level + 1, Rect(origin.x, origin.y, subQuadWidth, subQuadHeight));
    quadNode[2]->retain();
    
    //4
    quadNode[3] = QuadTree::createWithBound(_level + 1, Rect(origin.x + subQuadWidth, origin.y, subQuadWidth, subQuadHeight));
    quadNode[3]->retain();
    
//    QuadTree *tempNode = nullptr;
//    tempNode = QuadTree::createWithBound(_level + 1, Rect(origin.x + subQuadWidth, origin.y + subQuadHeight, subQuadWidth, subQuadHeight));
//    quadNodes.pushBack(tempNode);
//    
//    tempNode = QuadTree::createWithBound(_level + 1, Rect(origin.x, origin.y + subQuadHeight, subQuadWidth, subQuadHeight));
//    quadNodes.pushBack(tempNode);
//    
//    tempNode = QuadTree::createWithBound(_level + 1, Rect(origin.x, origin.y, subQuadWidth, subQuadHeight));
//    quadNodes.pushBack(tempNode);
//    
//    tempNode = QuadTree::createWithBound(_level + 1, Rect(origin.x + subQuadWidth, origin.y, subQuadWidth, subQuadHeight));
//    quadNodes.pushBack(tempNode);
    
auto curScene = Director::getInstance()->getRunningScene();
#if (NDL_DEBUG == 1)
    if (isUpdate == false) {
        if (curScene) {
            crossLineNode = CrossLineNode::createWithRect(_bound);
            curScene->addChild(crossLineNode, -1);
        }
    } else {
        if (crossLineNode != nullptr) {
            crossLineNode->redrawWithRect(_bound);
        } else {
            if (curScene) {
                crossLineNode = CrossLineNode::createWithRect(_bound);
                curScene->addChild(crossLineNode, -1);
            }
        }
    }
    
    
#endif
    //isSplit = true;
}

//objectRect.origin 世界坐标
int QuadTree::getQuadrantIndex(const Rect& objectRect){
    int index = -1;
    //世界坐标
    Vec2 midPoint = Vec2(_bound.origin.x + _bound.size.width / 2, _bound.origin.y + _bound.size.height / 2);
    
    //top
    if (objectRect.origin.y > midPoint.y) {
        if (objectRect.origin.x > midPoint.x) {
            //log("object in quadrant 1");
            index = 1;
        } else if (objectRect.origin.x + objectRect.size.width < midPoint.x) {
            //log("object in quadrant 2");
            index = 2;
        }
    } else if (objectRect.origin.y < midPoint.y && (objectRect.origin.y + objectRect.size.height < midPoint.y)) {//bottom
        if (objectRect.origin.x > midPoint.x) {
            //log("object in quadrant 4");
            index = 4;
        } else if (objectRect.origin.x + objectRect.size.width < midPoint.x) {
            //log("object in quadrant 3");
            index = 3;
        }
    }
    return index;
}



void QuadTree::addObjectRect(const Rect& objectRect){
    //已经splitToFourQuadrants 将objectRects分到各个象限
    int quadrantIndex = -1;
    if (quadNode[0] != nullptr) {
    //if (quadNodes.size() == 4) {
        quadrantIndex = getQuadrantIndex(objectRect);
        
        
        if (quadrantIndex != -1) {
            quadNode[quadrantIndex - 1]->addObjectRect(objectRect);
            //quadNodes.at(quadrantIndex - 1)->addObjectRect(objectRect);
            return;
        }
    }
    
    //push_back
    objectRects.push_back(objectRect);
    //splitToFourQuadrants
    if (objectRects.size() > max_objects && _level < max_levels) {
        splitToFourQuadrants();
        
        std::vector<Rect>::iterator it = objectRects.begin();
        Rect tempRect = Rect::ZERO;
        while (it != objectRects.end()) {
            tempRect = (*it);
            quadrantIndex = getQuadrantIndex(tempRect);
            //在4个象限中
            if (quadrantIndex != -1) {
                quadNode[quadrantIndex - 1]->addObjectRect(tempRect);
                //quadNodes.at(quadrantIndex - 1)->addObjectRect(tempRect);//递归
                objectRects.erase(it);
            } else {
                ++it;//不在4个象限中的 不删除 就存在objectRects里(即父节点)
            }
        }
    }
}



std::vector<Rect> QuadTree::retrieveCollision(std::vector<Rect> &objects, const Rect& targetRect){
    int quadrantIndex = getQuadrantIndex(targetRect);
    if (quadrantIndex != -1 && quadNode[0] != nullptr) {
        quadNode[quadrantIndex - 1]->retrieveCollision(objects, targetRect);//递归寻找与所给对象在同样象限的对象 objects里面数量会越来越少
    }
    
    for (int i = 0; i < objectRects.size(); i++) {
        objects.push_back(objectRects.at(i));
    }
    
    //log("!!!!!!!!!!!!!!!!!");
    return objects;
}



/**************************************************use for node**************************************************/
void QuadTree::update(std::vector<Node *> &nodes){
    if (isUpdate == false) {
        isUpdate = true;
    }
    this->clearNodeObject();
    
    for (int i = 0; i < nodes.size(); i++) {
        this->addNodeObject(nodes.at(i));
    }
    
}


//use for node
Color3B QuadTree::getColorByQuadrantIndex(int index){
    Color3B color;
    switch (index) {
        case 1:
            color = Color3B::BLUE;
            break;
        case 2:
            color = Color3B::YELLOW;
            break;
        case 3:
            color = Color3B::MAGENTA;
            break;
        case 4:
            color = Color3B::RED;
            break;
        default:
            break;
    }
    return color;
}
//use for node
void QuadTree::clearNodeObject(){//递归清空
    nodeObjects.clear();
    
    //debug
    if (crossLineNode != nullptr) {
        crossLineNode->clear();
    }
    
    if (quadNode[0] != nullptr) {
        for (int i = 0; i < (sizeof(quadNode) / sizeof(quadNode[0])); i++) {
            if (quadNode[i] != nullptr) {
                quadNode[i]->clearNodeObject();
                quadNode[i] = nullptr;
            }
        }
    }

}
//use for node
int QuadTree::getNodeQuadrantIndex(Node* node){
    auto objectRect = node->getBoundingBox();
    int index = -1;
    Vec2 midPoint = Vec2(_bound.origin.x + _bound.size.width / 2, _bound.origin.y + _bound.size.height / 2);
    
    if (objectRect.origin.y > midPoint.y) {
        if (objectRect.origin.x > midPoint.x) {
            index = 1;
        } else if (objectRect.origin.x + objectRect.size.width < midPoint.x) {
            index = 2;
        }
    } else if (objectRect.origin.y < midPoint.y && (objectRect.origin.y + objectRect.size.height < midPoint.y)) {
        if (objectRect.origin.x > midPoint.x) {
            index = 4;
        } else if (objectRect.origin.x + objectRect.size.width < midPoint.x) {
            index = 3;
        }
    }
    return index;
}

//use for node
void QuadTree::addNodeObject(Node *node){
    //auto objectRect = node->getBoundingBox();
    int quadrantIndex = -1;
    if (quadNode[0] != nullptr) {
        quadrantIndex = getNodeQuadrantIndex(node);
        if (quadrantIndex != -1) {
            node->setColor(getColorByQuadrantIndex(quadrantIndex));//setColor
            quadNode[quadrantIndex - 1]->addNodeObject(node);
            return;
        } else {
            node->setColor(Color3B::WHITE);
        }
    }
    
    nodeObjects.push_back(node);//白色
    if (nodeObjects.size() > max_objects && _level < max_levels) {
        splitToFourQuadrants();//add CrossLine
        
        std::vector<Node *>::iterator it = nodeObjects.begin();
        Node* tempNode = nullptr;
        while (it != nodeObjects.end()) {
            tempNode = (*it);
            quadrantIndex = getNodeQuadrantIndex(tempNode);
            
            if (quadrantIndex != -1) {
                tempNode->setColor(getColorByQuadrantIndex(quadrantIndex));
                quadNode[quadrantIndex - 1]->addNodeObject(tempNode);
                nodeObjects.erase(it);
            } else {
                tempNode->setColor(Color3B::WHITE);
                ++it;
            }
        }
    }
}
//use for node
std::vector<Node *> QuadTree::retrieveNodeCollision(std::vector<Node *> &objects, Node* targetNode){
    int quadrantIndex = getNodeQuadrantIndex(targetNode);
    if (quadrantIndex != -1 && quadNode[0] != nullptr) {
        quadNode[quadrantIndex - 1]->retrieveNodeCollision(objects, targetNode);
    }
    
    for (int i = 0; i < nodeObjects.size(); i++) {
        objects.push_back(nodeObjects.at(i));
    }
    return objects;
}