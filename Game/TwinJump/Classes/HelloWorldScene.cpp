#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{

    if ( !LayerColor::initWithColor(Color4B(255, 255,255,255) ))
    {
        return false;
    }
	// ----------------------- 애니메이션 생성
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/player2.png");
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.02f);
	
	for (int i = 0; i < 8; i++)
	{
		int column = i % 8;
		int row = i / 8;

		animation->addSpriteFrameWithTexture(
			texture,
			Rect(column * 30, row * 30, 30, 30));
	}

	pMan = Sprite::createWithTexture(texture, Rect(0, 0, 30, 30));
	pMan->setPosition(Vec2(240, 160));
	this->addChild(pMan);

	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	pMan->setScale(1.5f);
	pMan->setPosition(Vec2(120, 80));
	pMan->runAction(rep);

	// -------------------------------------- 점프
	Layer::onEnter();

	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	
	auto touchPoint = touch->getLocation();

	auto JumpUp = MoveBy::create(0.5, Vec2(0, 130));
	auto JumpUp1 = EaseOut::create(JumpUp, 2.0f);
	auto JumpDown = JumpUp1->reverse();
	auto Jump = Sequence::create(JumpUp, JumpDown, nullptr);
	auto rep1 = Repeat::create(Jump, 1);

	pMan->runAction(rep1);

	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	
	auto action = Place::create(Vec2(120, 80));

	pMan->runAction(action);

}