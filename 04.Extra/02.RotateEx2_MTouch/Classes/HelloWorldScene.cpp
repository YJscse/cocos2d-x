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

	pMan = Sprite::create("Images/grossini.png");
	pMan->setPosition(Vec2(240, 160));             // 나중에 안드로이드에 시험할때는 480, 320으로 바꿔서 ㄱㄱ
	pMan->setScale(2.0);
	this->addChild(pMan);

	pLabel1 = LabelTTF::create("0", "Arial", 64);
	pLabel1->setPosition(Vec2(100, 100));
	pLabel1->setColor(Color3B::BLACK);
	this->addChild(pLabel1);

	pLabel2 = LabelTTF::create("0", "Arial", 64);
	pLabel2->setPosition(Vec2(200, 100));
	pLabel2->setColor(Color3B::BLACK);
	this->addChild(pLabel2);

	bSelect = false;
	i = 0;
    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 멀티터치모드로 터치 리스너 등록
	auto listener = EventListenerTouchAllAtOnce::create();

	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void HelloWorld::onExit()
{
	_eventDispatcher->removeAllEventListeners();

	Layer::onExit();
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *event)
{
	bSelect = false;

	char myNum1[20] = { 0 };
	sprintf(myNum1, "%lu", touches.size());
	pLabel1->setString(myNum1);

	//if ((int)touches.size() < 2)
	//{
	//	return;
	//}

	for (auto &item : touches)
	{
		auto touch = item;
		auto touchPoint = touch->getLocation();
		// log("Touch x : %d y : %d", (int)touchPoint.x, (int)touchPoint.y);

		Rect rect = pMan->getBoundingBox();
		if (rect.containsPoint(touchPoint))
		{
			log("Touch x: %d y: %d", (int)touchPoint.x, (int)touchPoint.y);
			i++;
		}
	}

	char myNum2[20] = { 0 };
	sprintf(myNum2, "%d", i);
	pLabel2->setString(myNum2);

	if (i > 1)
	{
		bSelect = true;
	}
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	if (bSelect)
	{
		auto touch = touches[0];

		Vec2 oldPoint = touch->getPreviousLocation();
		Vec2 nowPoint = touch->getLocation();

		Vec2 firstVector = oldPoint - pMan->getPosition();
		float firstRotateAngle = -firstVector.getAngle();
		float previousTouch = CC_RADIANS_TO_DEGREES(firstRotateAngle);

		Vec2 secondVector = nowPoint - pMan->getPosition();
		float secondRotateAngle = -secondVector.getAngle();
		float currentTouch = CC_RADIANS_TO_DEGREES(secondRotateAngle);

		gRotation = gRotation + currentTouch - previousTouch;
		gRotation = fmod(gRotation, 360.0f);

		pMan->setRotation(gRotation);
	}
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *event)
{
	pLabel1->setString("0");
	pLabel2->setString("0");

	i = 0;
	bSelect = false;
}