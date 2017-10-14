#include "JumpingHippo.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* JumpingHippo::createScene()
{
    return JumpingHippo::create();
}

bool JumpingHippo::init()
{
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }

    getPhysicsWorld()->setGravity(Vec2::ZERO);
    
    initHippoSprite();
    initSpawner();
    initClickListener();
    
    return true;
}

void JumpingHippo::initHippoSprite()
{
    hippoSprite = Sprite::create("Hippo.png");
    
    hippoSprite->setPosition(Director::getInstance()->getVisibleSize()/2);
    hippoSprite->setScale(0.1f);
    
    // Set object collision
    //hippoSprite->getPhysicsBody()->setCategoryBitmask(0x02);
    //hippoSprite->getPhysicsBody()->setCollisionBitmask(0x01);
    
    addChild(hippoSprite, 1);
    
}

void JumpingHippo::initSpawner()
{
    auto size = Director::getInstance()->getVisibleSize();
    schedule([=](float dt){
        
        Sprite* spikesSprite = Sprite::create("SpikeBall.png");
        
        // Set collision to the object
        //spikesSprite->getPhysicsBody()->setCategoryBitmask(0x01);
        //spikesSprite->getPhysicsBody()->setCollisionBitmask(0x02);
        
        context->addChild(spikesSprite);
        
        float random = std::rand() % 10;
        float scale = 0.25f;
        
        if(random > 9) {
            scale = 0.5f;
        }
        
        spikesSprite->setScale(scale);
        spikesSprite->setPosition(Vec2(size.width, size.height/2));
        
        // Movement to the left part. Add a -50 to completely disappear it
        auto moveToLeft = MoveTo::create(2.f, Vec2(-50, size.height/2));
        spikesSprite->runAction(moveToLeft);
        
    }, 1.f, 99999, 1.f, "spawnerCallBack");
    // Is unlimited... I don't believe someone can achieve that number
}

void JumpingHippo::initClickListener()
{
    auto mouseListener = EventListenerTouchOneByOne::create();
    
    mouseListener->onTouchBegan = [=](Touch* touch, Event* event){
        auto jump = JumpBy::create(0.8, Vec2::ZERO, 150, 1);
        hippoSprite->runAction(jump);
        
        // 360 flip when is jumping to be cool
        auto rotate = RotateBy::create(0.5f, 360.0f);
        hippoSprite->runAction(rotate);
        
        // Play audio when jumping
        auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
        audio->playEffect("hippo_jump.wav", false, 1.0f, 1.0f, 1.0f);
        
        return true;
    };
    mouseListener->onTouchMoved = [=](Touch* touch, Event* event){};
    
    mouseListener->onTouchEnded = [=](Touch* touch, Event* event){};
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

