#include "HelloWorldScene.h"


USING_NS_CC;

static std::string fontList[] =
{

	"fonts/A Damn Mess.ttf",
	"fonts/Abberancy.ttf",
	"fonts/Abduction.ttf",
	"fontsPaint Boy.ttf",
	"fonts/Schwarzwald Regular.ttf",
	"fonts/Scissor Cuts.ttf",

};

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
	auto pLabel1 = LabelTTF::create("Hello Gikimi", "Arial", 34);

	//레이블의 위치 지정
	pLabel1->setPosition(Vec2(240, 200));

	//레이블의 색 지정
	pLabel1->setColor(Color3B(0, 0, 0));

	//레이어에 레이블 객체 추가
	this->addChild(pLabel1);

	//True Type Font

	//ttf 파일을 프로젝트의 resources에 디렉터리로 추가
	//iPhone : Info.plist에 리소스 추가 / 리소스에 지정된 이름으로 호출
	//Android : 디렉터리를 포함한 이름으로 호출

	//레이블 생성 및 초기화
	auto pLabel2 = LabelTTF::create("Hello Gikimi", fontList[5].c_str(), 34);

	//레이블의 위치 지정
	pLabel2->setPosition(Vec2(240, 100));

	//레이블의 색 지정
	pLabel2->setColor(Color3B(0, 0, 0));

	//레이어에 레이블 객체 추가
	this->addChild(pLabel2);

    return true;
}