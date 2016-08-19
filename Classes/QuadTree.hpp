//
//  QuadTree.hpp
//  QuadTree
//
//  Created by macuser on 16/8/17.
//
//


/*
 四叉树原理:四叉树就是每个父节点都具有四个子节点的树状数据结构
 减少检测数量:将屏幕上可能和不可能发生碰撞的物体区分开来
 
 2YELLOW | 1BLUE
 ---------------
 3MAGENTA | 4RED
 
 将完全处于某一个象限的物体存储在该象限对应的子节点下
 跨越多个象限的物体，我们将它们存在父节点中
 如果某个象限内的物体的数量过多，它会同样会分裂成四个子象限，以此类推
 */
#ifndef QuadTree_hpp
#define QuadTree_hpp

#include "cocos2d.h"
USING_NS_CC;

class CrossLineNode;

//调用时需retain
class QuadTree : public Ref{
protected:
    QuadTree();
    ~QuadTree();
public:
    static QuadTree* createWithBound(int level, const Rect& rect);
    bool initWithBound(int level, const Rect& rect);
    
    void clear();
    void addObjectRect(const Rect& objectRect);
    //返回与指定对象可能发生碰撞的所有对象
    std::vector<Rect> retrieveCollision(std::vector<Rect> &objects, const Rect& targetRect);

    
    //use for node
    void clearNodeObject();
    void addNodeObject(Node *node);
    std::vector<Node *> retrieveNodeCollision(std::vector<Node *> &objects, Node* targetNode);
    
    void update(std::vector<Node *> &nodes);
    
private:
    
    void splitToFourQuadrants();
    int getQuadrantIndex(const Rect& objectRect);
    //use for node
    int getNodeQuadrantIndex(Node* node);
    
    Color3B getColorByQuadrantIndex(int index);
    
private:
    int _level;
    Rect _bound;
    std::vector<Rect> objectRects;//对象Rect集合  
    QuadTree* quadNode[4];
    //Vector<QuadTree *> quadNodes;
    
    
    //use for node
    std::vector<Node *> nodeObjects;
    
    CrossLineNode* crossLineNode;
    
    bool isUpdate;
    bool isSplit;//（还需要前次QuadTree的Objects数量等来判断）drawcall优化 CrossLineNode 存在的有些不用clear再redraw 而是隐藏和显示
};

#endif /* QuadTree_hpp */
