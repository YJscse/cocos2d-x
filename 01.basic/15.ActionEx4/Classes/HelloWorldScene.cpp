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

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}

	auto pMenuItem = MenuItemFont::create(
		"Action",
		CC_CALLBACK_1(HelloWorld::doAction, this));
	pMenuItem->setColor(Color3B(0, 0, 0));

	auto pMenu = Menu::create(pMenuItem, nullptr);

	pMenu->setPosition(Vec2(240, 50));

	this->addChild(pMenu);

	// 동그란 버튼 스프라이트 추가
	pBall = Sprite::create("Images/r1.png");
	pBall->setPosition(Vec2(50, 100));
	pBall->setScale(0.7f);

	this->addChild(pBall);

	// Grossini 스프라이트 추가
	pMan = Sprite::create("Images/grossini.png");
	pMan->setPosition(Vec2(50, 150));
	pMan->setScale(0.5f);

	this->addChild(pMan);

	// Grossini 의 sister1 스프라이트 추가
	pWomen1 = Sprite::create("Images/grossinis_sister1.png");
	pWomen1->setPosition(Vec2(50, 220));
	pWomen1->setScale(0.5f);

	this->addChild(pWomen1);

	// Grossini 의 sister2 스프라이트 추가
	pWomen2 = Sprite::create("Images/grossinis_sister2.png");
	pWomen2->setPosition(Vec2(50, 280));
	pWomen2->setScale(0.5f);

	this->addChild(pWomen2);


	return true;
}

void HelloWorld::doAction(Ref* pSender)
{
	doActionReset();

	// 정상 속도
	auto move = MoveBy::create(3.0f, Vec2(400, 0));

	// 속도 ------------------------------------
	// 빨라지기 : 시작이 늦고 나중에 빠름
	//	auto ease_in = EaseIn::create(move, 4.0f);
	//	auto ease_in = EaseIn::create(move->clone(), 4.0f);

	// 느려지기 : 시작이 빠르고 나중이 늦음
	//	auto ease_out = EaseOut::create(move, 4.0f);
	//	auto ease_out = EaseOut::create(move->clone(), 4.0f);

	// 빨라졌다가 느려지기: 시작과 끝이 느리고 중간이 빠름
	//	auto ease_inout1 = EaseInOut::create(move->clone(), 4.0f);
	//	auto ease_inout2 = EaseInOut::create(move->clone(), 3.0f);
	//	auto ease_inout3 = EaseInOut::create(move->clone(), 1.0f);

	// ----------------- 탄 성 -------------------------------------------------

	//	auto ease_in = EaseElasticIn::create(move->clone(), 0.4f);

	//	auto ease_out = EaseElasticOut::create(move->clone(), 0.4f);

	//	auto ease_inout1 = EaseElasticInOut::create(move->clone(), 0.4f);

	// ---------------- 바운스 --------------------------------------------------

	//	auto ease_in = EaseBounceIn::create(move->clone());

	//	auto ease_out = EaseBounceOut::create(move->clone());

	//	auto ease_inout1 = EaseBounceInOut::create(move);

	// ---------------- 스피드 ---------------------------------------------------
	auto ease_in = Speed::create(move->clone(), 1.0f);

	auto ease_out = Speed::create(move->clone(), 2.0f);

	auto ease_inout1 = Speed::create(move->clone(), 3.0f);

	pBall->runAction(move);
	pMan->runAction(ease_in);
	pWomen1->runAction(ease_out);
	pWomen2->runAction(ease_inout1);
}

void HelloWorld::doActionReset()
{
	pBall->setPosition(Vec2(50, 100));
	pBall->setScale(0.7f);

	pMan->setPosition(Vec2(50, 150));
	pMan->setScale(0.5f);

	pWomen1->setPosition(Vec2(50, 220));
	pWomen1->setScale(0.5f);

	pWomen2->setPosition(Vec2(50, 280));
	pWomen2->setScale(0.5f);

}