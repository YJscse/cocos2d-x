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

	// 스프라이트 생성 및 초기화
	auto pMan = Sprite::create("Images/grossini.png");

	// 스프라이트 위치 지정
	pMan->setPosition(Vec2(240, 160));

	// 레이어에 스프라이트 객체 추가
//	this->addChild(pMan);
	this->addChild(pMan, 2);  //위치 지정 2층

	// 코드 추가
	auto pWoman = Sprite::create("Images/grossinis_sister1.png");

	//pWoman->setPosition(Vec2(300, 160));   // 나란히
	pWoman->setPosition(Vec2(260, 160));   // 겹침

	this->addChild(pWoman, 1);  // 위치 지정 1층     

	/////////////////////////////////////////////////////////////////////////////////////
	// 코드 추가

//	auto pWoman = Sprite::create("Images/grossinis_sister1.png");

//	pWoman->setPosition(Vec2(300, 160));   // 나란히

//	this->addChild(pWoman, 1);

//	auto pMan = Sprite::create("Images/grossini.png");

//	pMan->setPosition(Vec2(240, 160));

//	this->addChild(pMan);


    return true;
}