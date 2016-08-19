//
//  VerticalCrossLineNode.hpp
//  QuadTree
//
//  Created by macuser on 16/8/18.
//
//

#ifndef CrossLineNode_hpp
#define CrossLineNode_hpp
#include "cocos2d.h"
USING_NS_CC;

class CrossLineNode : public Node{
protected:
    CrossLineNode();
    ~CrossLineNode();
public:
    inline DrawNode *getDrawNode(){
        return this->drawNode;
    }
    
    static CrossLineNode* createWithRect(const Rect& rect);
    bool initWithRect(const Rect& rect);
    
    void clear();
    void redrawWithRect(const Rect& rect);
private:
    
private:
    DrawNode* drawNode;
};

#endif /* CrossLineNode_hpp */
