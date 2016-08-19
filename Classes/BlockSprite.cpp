//
//  BlockSprite.cpp
//  QuadTree
//
//  Created by macuser on 16/8/19.
//
//

#include "BlockSprite.hpp"

BlockSprite* BlockSprite::createWithFile(const std::string& filename){
    auto ret = new (std::nothrow) BlockSprite();
    if (ret && ret->initWithFile(filename)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool BlockSprite::initWithFile(const std::string& filename){
    if (!Sprite::initWithFile(filename)) {
        return false;
    }
    //log("BlockSprite initWithFile!!!!!!!!!!!");
    
    this->speedX = (rand() % 11) -5;
    this->speedY = (rand() % 11) -5;
    
    return true;
}