//
//  BlockSprite.hpp
//  QuadTree
//
//  Created by macuser on 16/8/19.
//
//

#ifndef BlockSprite_hpp
#define BlockSprite_hpp

#include "cocos2d.h"
USING_NS_CC;

class BlockSprite : public Sprite{
public:
    static BlockSprite* createWithFile(const std::string& filename);
    virtual bool initWithFile(const std::string& filename);
public:
    CC_SYNTHESIZE(int, speedX, SpeedX);
    CC_SYNTHESIZE(int, speedY, SpeedY);
};

#endif /* BlockSprite_hpp */
