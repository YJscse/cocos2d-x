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

	////////////////////////

	// 배경을 만든다
	this->createBackgroundParallax();

	// 드래곤을 만든다
	this->createDragon();

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

	this->addChild(voidNode, 0);
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
	this->addChild(dragon);
	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	dragon->runAction(rep);
}