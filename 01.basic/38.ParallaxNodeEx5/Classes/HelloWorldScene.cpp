#include "HelloWorldScene.h"


USING_NS_CC;

#define IMG_WIDTH 1024

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

	////////////////////////////////

	layer1 = Layer::create();
	this->addChild(layer1);

	layer2 = Layer::create();
	this->addChild(layer2);

	// 화면의 픽셀 크기를 구한다.
	winSize = Director::getInstance()->getWinSize();

	// 배경을 만든다
	this->createBackgroundParallax();

	// 드래곤을 만든다
	this->createDragon();

	// 메튜 버튼을 만든다.
	this->createArrowButtons();

    return true;
}

void HelloWorld::createBackgroundParallax()
{
	// 배경 레이어1
	auto background1 = Sprite::create("Images/background1.png");
	background1->setAnchorPoint(Vec2(0, 0));

	// 배경 레이어2
	auto background2 = Sprite::create("Images/background2.png");
	background2->setAnchorPoint(Vec2(0, 0));

	// 위에서 만든 스프라이트를 담을 부모로 패럴랙스노드를 만든다.
	auto voidNode = ParallaxNode::create();

	// 배경 스프라이트를 패럴랙스노드에 넣는다.
	// 배경이미지의 사이즈 : 512 x 320
	voidNode->addChild(background1, 1, Vec2(1.0f, 0.0f), Vec2(0, 0));
	voidNode->addChild(background2, 1, Vec2(1.0f, 0.0f), Vec2(512, 0));

	voidNode->setTag(1);

	layer1->addChild(voidNode, 0);
}

void HelloWorld::createDragon()
{
	// 움직이는 드래곤 넣기 시작
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/dragon_animation.png");

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int i = 0; i < 6; i++)
	{
		// 첫째 줄에 4개의 프레임이 있고, 5번째부터는 두 번째 줄에 있으므로
		// 5번째(idx==4)부터는 y 좌표의 값을 증가시켜야 한다.
		int index = i % 4;
		int rowIndex = i / 4;

		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	// 스프라이트 생성 및 초기화
	dragon = Sprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	dragon->setPosition(Vec2(240, 160));
	layer1->addChild(dragon);
	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	dragon->runAction(rep);
}

void HelloWorld::createArrowButtons()
{
	// 왼쪽 화살표
	leftSprite = Sprite::create("Images/b1.png");
	leftSprite->setPosition(Vec2(180, 30));

	layer2->addChild(leftSprite, 2);

	// 눌렸을 때 쓰일 왼쪽 화살표
	leftPressedSprite = Sprite::create("Images/b2.png");
	// self.leftSprite와 똑같은 위치에 표시한다.
	leftPressedSprite->setPosition(leftSprite->getPosition());

	layer2->addChild(leftPressedSprite, 1);

	// 오른쪽 화살표
	rightSprite = Sprite::create("Images/f1.png");
	rightSprite->setPosition(Vec2(300, 30));

	layer2->addChild(rightSprite, 2);

	// 눌렸을 때 쓰일 왼쪽 화살표
	rightPressedSprite = Sprite::create("Images/f2.png");
	// self.leftSprite와 똑같은 위치에 표시한다.
	rightPressedSprite->setPosition(rightSprite->getPosition());

	layer2->addChild(rightPressedSprite, 1);
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();

	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (스프라이트가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

#pragma mark -
#pragma mark Touch Event Handling

// 손가락이 닿는 순간 호출된다.
bool HelloWorld::onTouchBegan(Touch* touch, Event *event)
{
	// 아래 Boolean 변수 대신 leftSprite와 rightSprite의 visible의 값을 직접 사용해도 무방하다.
	isLeftPressed = false;
	isRightPressed = false;

	// 터치가 왼쪽 또는 오른쪽 화살표 안에 들어왔는지 확인한다.
	if (this->isTouchInside(leftSprite, touch) == true)
	{
		// 왼쪽 화살표를 안 보이게 한다.
		// 그럼 그 아래에 있던 눈릴 때 보여지는 이미지가 나타날 것이다.
		leftSprite->setVisible(false);
		isLeftPressed = true;
	}
	else if (this->isTouchInside(rightSprite, touch) == true)
	{

		// 오른쪽 화살표를 안 보이게 한다.
		rightSprite->setVisible(false);
		isRightPressed = true;
	}

	// 버튼이 눌려 졌으면 화면을 움직인다.
	if (isLeftPressed == true || isRightPressed == true)
	{
		this->startMovingBackground();
	}

	return true;
}

// 손가락을 화면에서 떼지 않고 이리저리 움직일 때 계속해서 호출된다.
void HelloWorld::onTouchMoved(Touch* touch, Event *event)
{

	// 손가락이 버튼을 벗어나면 움직임을 중단한다.
	if (isLeftPressed == true && this->isTouchInside(leftSprite, touch) == false)
	{

		leftSprite->setVisible(true);
		this->stopMovingBackground();
	}
	else if (isRightPressed == true && this->isTouchInside(rightSprite, touch) == false)
	{

		// 오른쪽 화살표를 안 보이게 한다.
		rightSprite->setVisible(true);
		this->stopMovingBackground();
	}

}

void HelloWorld::onTouchEnded(Touch* touch, Event *event)
{
	// 배경화면을 멈춘다.
	if (isLeftPressed == true || isRightPressed == true)
	{
		this->stopMovingBackground();
	}
	// 감춰졌던 버튼 이미지를 다시 보이게 한다.
	if (isLeftPressed == true)
	{
		leftSprite->setVisible(true);
	}

	if (isRightPressed == true)
	{
		rightSprite->setVisible(true);
	}

}

#pragma mark -
#pragma mark Game Play

// 터치가 버튼 Sprite 안에서 이뤄졋는지 확인한다.
bool HelloWorld::isTouchInside(Sprite* sprite, Touch* touch)
{
	// Cocoa 좌표
	auto touchPoint = touch->getLocation();

	bool bTouch = sprite->getBoundingBox().containsPoint(touchPoint);

	return bTouch;
}

void HelloWorld::startMovingBackground()
{
	// 만약 버튼 두 개가 다 눌려졌으면 화면을 이동시키지 않는다.
	if (isLeftPressed == true && isRightPressed == true)
		return;

	log("start moving");

	// 스케쥴을 이용해 매 프레임마다 배경화면을 움직인다.
	this->schedule(schedule_selector(HelloWorld::moveBackground));

}

void HelloWorld::stopMovingBackground()
{
	log("stop moving");
	this->unschedule(schedule_selector(HelloWorld::moveBackground));
}
void HelloWorld::moveBackground(float f)
{
	// 매 프레임마다 움직일 거리
	auto moveStep = 3;

	if (isLeftPressed)
	{
		moveStep = -3;
		dragon->setFlippedX(false);
	}
	else
	{
		moveStep = 3;
		dragon->setFlippedX(true);
	}

	auto newPos = Vec2(dragon->getPosition().x + moveStep, dragon->getPosition().y);

	if (newPos.x < 0)
	{
		newPos.x = 0;
	}
	else if (newPos.x > 512 * 2)
	{
		newPos.x = 512 * 2;
	}

	dragon->setPosition(newPos);

	layer1->runAction(Follow::create(dragon, Rect(0, 0, 512 * 2, 320)));

	log("moving background...");
}