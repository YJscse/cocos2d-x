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

	//////////////////////////////////////////

	// 메뉴 아이템 생성 및 초기화
	auto pMenuItem1 = MenuItemFont::create(
		"Menu-1 ",
		CC_CALLBACK_1(HelloWorld::doClick, this));

	pMenuItem1->setColor(Color3B(0, 0, 0));

	auto pMenuItem2 = MenuItemFont::create(
		"Menu-2 ",
		CC_CALLBACK_1(HelloWorld::doClick, this));

	pMenuItem2->setColor(Color3B(0, 0, 0));

	auto pMenuItem3 = MenuItemFont::create(
		"Menu-3",
		CC_CALLBACK_1(HelloWorld::doClick, this));

	pMenuItem3->setColor(Color3B(0, 0, 0));

	pMenuItem1->setTag(1);
	pMenuItem2->setTag(2);
	pMenuItem3->setTag(3);

	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, nullptr);

	// 메뉴 배치
	pMenu->alignItemsHorizontally();

	// 레이어에 메뉴 객체 추가
	this->addChild(pMenu);

    return true;
}

void HelloWorld::doClick(Ref* pSender)
{

	auto tItem = (MenuItem *)pSender;

	int i = tItem->getTag();

	log("%d번째 메뉴가 선택되었습니다.", i);
}