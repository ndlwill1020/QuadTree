#include "HelloWorldScene.h"
#include "QuadTree.hpp"
#include "BlockSprite.hpp"

#define OBJECT_WIDTH 20
#define OBJECT_HEIGHT 20



Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();

    //Director::getInstance()->getScheduler()->setTimeScale(0.5f);

    /*
    targetNode = DrawNode::create();
    Vec2 verts[4];
    verts[0] = Vec2::ZERO;
    verts[1] = Vec2(OBJECT_WIDTH, 0.0f);
    verts[2] = Vec2(OBJECT_WIDTH, OBJECT_HEIGHT);
    verts[3] = Vec2(0.0f, OBJECT_HEIGHT);
    targetNode->drawPolygon(verts, 4, Color4F::GREEN, 0.0f, Color4F::WHITE);
    targetNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    targetNode->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    targetNode->setContentSize(Size(OBJECT_WIDTH, OBJECT_HEIGHT));
    this->addChild(targetNode);
    

    speedX = (rand() % 11) -5;
    speedY = (rand() % 11) -5;
    for (int i = 0; i < 100; i++) {
        drawNode = DrawNode::create();
        Vec2 verts[4];
        verts[0] = Vec2::ZERO;
        verts[1] = Vec2(OBJECT_WIDTH, 0.0f);
        verts[2] = Vec2(OBJECT_WIDTH, OBJECT_HEIGHT);
        verts[3] = Vec2(0.0f, OBJECT_HEIGHT);
        drawNode->setContentSize(Size(OBJECT_WIDTH, OBJECT_HEIGHT));
        drawNode->drawPolygon(verts, 4, Color4F::RED, 0.0f, Color4F::WHITE);
        drawNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        //drawNode->setPosition(visibleSize.width / 2, visibleSize.height / 2);
        drawNode->setPosition(CCRANDOM_0_1() * (visibleSize.width - OBJECT_WIDTH / 2 - OBJECT_WIDTH / 2 + 1) + OBJECT_WIDTH / 2, \
                              CCRANDOM_0_1() * (visibleSize.height - OBJECT_HEIGHT / 2 - OBJECT_HEIGHT / 2 + 1) + OBJECT_HEIGHT / 2);
        
        this->addChild(drawNode);
        objects.push_back(drawNode);
    }
    */
    targetSprite = Sprite::create("block.png");
    targetSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    targetSprite->setColor(Color3B::GREEN);
    this->addChild(targetSprite, 8);
    
    speedX = (rand() % 11) -5;
    speedY = (rand() % 11) -5;
    //100
    for (int i = 0; i < 100; i++) {
        //sprite = Sprite::create("block.png");
        sprite = BlockSprite::createWithFile("block.png");
        sprite->setPosition(CCRANDOM_0_1() * (visibleSize.width - OBJECT_WIDTH / 2 - OBJECT_WIDTH / 2 + 1) + OBJECT_WIDTH / 2, \
                              CCRANDOM_0_1() * (visibleSize.height - OBJECT_HEIGHT / 2 - OBJECT_HEIGHT / 2 + 1) + OBJECT_HEIGHT / 2);
        
        this->addChild(sprite);
        objects.push_back(sprite);
    }
    
    
    

//    auto listener = EventListenerTouchOneByOne::create();
//    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    //这边结束前获取Director::getInstance()->getRunningScene()是空
    
    
    return true;
}

void HelloWorld::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    //到这边才能获取Director::getInstance()->getRunningScene()
    
    
    
    
    //0表示当前层这个节点
    quadTree = QuadTree::createWithBound(0, this->getBoundingBox());
    quadTree->retain();
    //quadTree->clear();
    quadTree->clearNodeObject();
    
    for (std::vector<Node *>::iterator it = objects.begin(); it != objects.end(); it++) {
        //quadTree->addObjectRect((*it)->getBoundingBox());
        quadTree->addNodeObject((*it));
    }
    
    scheduleUpdate();
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event){
    auto beginPoint = touch->getLocation();
    targetSprite->setPosition(beginPoint);
    
    returnNodes.clear();
    quadTree->retrieveNodeCollision(returnNodes, targetSprite);
    
    
    for (auto node : returnNodes) {
        if (node->getBoundingBox().intersectsRect(targetSprite->getBoundingBox())) {
            node->runAction(Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5, 1.0f), NULL));
        }
    }
    
    return false;
}

void HelloWorld::update(float dt){
    log("1 frame!!!!!!!!!!!!!!!!!!!!!!!!!!  %f", dt);
    
    /*
    targetNode->setPosition(targetNode->getPositionX() + speedX, targetNode->getPositionY() + speedY);
    
    if ((targetNode->getPositionX() < targetNode->getContentSize().width / 2) || (targetNode->getPositionX() > visibleSize.width - targetNode->getContentSize().width / 2)) {
        speedX *= -1;
    }
    
    if ((targetNode->getPositionY() < targetNode->getContentSize().height / 2) || (targetNode->getPositionY() > visibleSize.height - targetNode->getContentSize().height / 2)) {
        speedY *= -1;
    }
    
    returnObjects.clear();
    log("target origin:%f %f",targetNode->getBoundingBox().origin.x, targetNode->getBoundingBox().origin.y);
    quadTree->retrieveCollision(returnObjects, targetNode->getBoundingBox());
    
    log("size:%ld",returnObjects.size());
    for (auto rect : returnObjects) {
        log("return:%f %f target%f %f",rect.origin.x, rect.origin.y, targetNode->getBoundingBox().origin.x, targetNode->getBoundingBox().origin.y);
        targetNode->setScale(1.0f);
        if (rect.intersectsRect(targetNode->getBoundingBox())) {
            log("collision!!!!!!!!!!!!!!!!!!!!!");
            targetNode->setScale(1.5f);
        }
    }
     */
    
    //target sprite
    targetSprite->setPosition(targetSprite->getPositionX() + speedX, targetSprite->getPositionY() + speedY);
    
    if ((targetSprite->getPositionX() < targetSprite->getContentSize().width / 2) || (targetSprite->getPositionX() > visibleSize.width - targetSprite->getContentSize().width / 2)) {
        speedX *= -1;
    }
    
    if ((targetSprite->getPositionY() < targetSprite->getContentSize().height / 2) || (targetSprite->getPositionY() > visibleSize.height - targetSprite->getContentSize().height / 2)) {
        speedY *= -1;
    }
    
    //100 sprite 动态
    int offsetX = 0, offsetY = 0;
    for (int i = 0; i < objects.size(); i++) {
        
        auto sp = (BlockSprite *)objects.at(i);
        offsetX = sp->getSpeedX();
        offsetY = sp->getSpeedY();
        
        
        sp->setPosition(sp->getPositionX() + offsetX, sp->getPositionY() + offsetY);
        if ((sp->getPositionX() < sp->getContentSize().width / 2) || (sp->getPositionX() > visibleSize.width - sp->getContentSize().width / 2)) {
            offsetX *= -1;
            sp->setSpeedX(offsetX);
        }
        
        if ((sp->getPositionY() < sp->getContentSize().height / 2) || (sp->getPositionY() > visibleSize.height - sp->getContentSize().height / 2)) {
            offsetY *= -1;
            sp->setSpeedY(offsetY);
        }
    }
    

    //update
    quadTree->update(objects);
    
    returnNodes.clear();
    quadTree->retrieveNodeCollision(returnNodes, targetSprite);
    log("return objects count:%ld", returnNodes.size());
    //C++ 11
    for (auto node : returnNodes) {
        
        if (node->getBoundingBox().intersectsRect(targetSprite->getBoundingBox())) {
            node->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5, 1.0f), NULL));
        }
    }
    

    //common retrieve
//    for (int i = 0; i < objects.size(); i++) {
//        if (objects.at(i)->getBoundingBox().intersectsRect(targetSprite->getBoundingBox())) {
//            objects.at(i)->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5, 1.0f), NULL));
//        }
//    }
    
    
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
