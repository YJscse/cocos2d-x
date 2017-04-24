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

	// 레이블 생성 및 초기화
	auto pLabel1 = LabelTTF::create("Hello World", "Arial", 34);

	// 레이블의 위치 지정
	pLabel1->setPosition(Vec2(240, 250));

	// 레이블의 색 지정
	pLabel1->setColor(Color3B(0, 0, 0));

	// 레이어에 레이블 객체 추가
	this->addChild(pLabel1);

	//비트맵 폰트 파일 사용법

	//사용하려면 같은 이름의 .fnt 파일과 png 파일이 필요하다.
	//해당하는 글자는 반드시 *.fnt, *png에 있어야 한다.

	// 레이블 생성 및 초기화
	auto pLabel2 = LabelBMFont::create("Hello", "futura-48.fnt");

	// 레이블의 위치 지정
	pLabel2->setPosition(Vec2(240, 150));

	// 레이어에 레이블 객체 추가
	this->addChild(pLabel2);


	// 캐릭터맵 폰트 파일 사용법

	// itemWidth와 itemHeight는 같은 크기의 이미지로 된 sprite sheet를 이용해
	//SpriteShhet를 만들 때와 마찬가지로 글자 하나에 해당하는 크기를 나타낸다.
	//starCharMap은  첫 번째 이미지에 해당하는 ascii 값을 말한다.
	//char 형태의 값을 사용하면 된다.

	// 레이블 생성 및 초기화
	auto pLabel3 = LabelAtlas::create("1234", "fps_images.png", 12, 32, '.');

	// 레이블의 위치 지정
	pLabel3->setPosition(Vec2(240, 50));

	// 레이어에 레이블 객체 추가
	this->addChild(pLabel3);


	return true;
}