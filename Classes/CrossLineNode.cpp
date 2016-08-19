//
//  VerticalCrossLineNode.cpp
//  QuadTree
//
//  Created by macuser on 16/8/18.
//
//

#include "CrossLineNode.hpp"




CrossLineNode::CrossLineNode()
:drawNode(nullptr)
{

}

CrossLineNode::~CrossLineNode(){

}

CrossLineNode* CrossLineNode::createWithRect(const Rect& rect){
    auto ret = new (std::nothrow) CrossLineNode();
    if (ret && ret->initWithRect(rect)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CrossLineNode::initWithRect(const Rect& rect){
    if (!Node::init()) {
        return false;
    }
    
    drawNode = DrawNode::create();
    //horizontal
    drawNode->drawSegment(Vec2(rect.origin.x, rect.origin.y + rect.size.height / 2), Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height / 2), 2.0f, Color4F::WHITE);
    //vertical
    drawNode->drawSegment(Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y), Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height), 2.0f, Color4F::WHITE);
    
    
    
    this->addChild(drawNode);
    //this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    return true;
}

void CrossLineNode::redrawWithRect(const Rect& rect){
    if (drawNode != nullptr) {
        //drawNode->clear();
        //horizontal
        drawNode->drawSegment(Vec2(rect.origin.x, rect.origin.y + rect.size.height / 2), Vec2(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height / 2), 2.0f, Color4F::WHITE);
        //vertical
        drawNode->drawSegment(Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y), Vec2(rect.origin.x + rect.size.width / 2, rect.origin.y + rect.size.height), 2.0f, Color4F::WHITE);
    }
}

void CrossLineNode::clear(){
    if (drawNode != nullptr) {
        drawNode->clear();
    }
}