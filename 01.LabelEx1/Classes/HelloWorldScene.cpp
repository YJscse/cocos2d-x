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

    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255) ))
    {
        return false;
    }

	//레이블 생성 및 초기화 1
//	auto pLabel = LabelTTF::create("Hello World", "Arial", 64);    //(나타내고싶은 글자, 폰트, 크기)

	//레이블 생성 및 초기화 2
	//내용이 길면 자동개행이 되지 않고 중앙정렬만 된다
	// 32 vs 64
//	auto pLabel = LabelTTF::create("동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라 만세.", "Arial", 32);

	//레이블 생성 및 초기화 3
	//폰트 크기는 너무 크고 레이블의 크기가 상대적으로 작을 때 글자가 잘리는 현상이
	//발생할 수 있다. 이럴 때는 레이블의 크기를 더 늘리거나 폰트 크기를 줄여 본다.
	auto pLabel = LabelTTF::create("동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라 만세.",
		"Arial",
		34,
	Size(300, 200),
		TextHAlignment::CENTER,
		TextVAlignment::CENTER);

	//레이블의 위치 지정
	pLabel->setPosition(Vec2(240, 160));

	//레이블의 색 지정
	pLabel->setColor(Color3B(0, 0, 0)); 

	//레이블의 투명도 지정
	pLabel->setOpacity(100.0f);

	//레이어에 레이블 객체 추가
	this->addChild(pLabel);

    return true;
}