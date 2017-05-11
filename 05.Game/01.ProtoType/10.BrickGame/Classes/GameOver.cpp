#include "GameOver.h"
#include "HelloWorldScene.h"

Scene* GameOver::createScene()
{
	auto scene = Scene::create();

	auto layer = GameOver::create();

	scene->addChild(layer);

	return scene;
}

bool GameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// 메뉴 아이템 생성 및 초기화

	auto item1 = MenuItemFont::create("New Game", CC_CALLBACK_1(GameOver::doClose, this));

	item1->setColor(Color3B(255, 255, 255));

	// 메뉴 생성
	auto pMenu = Menu::create(item1, nullptr);

	// 메뉴 위치
	pMenu->setPosition(Vec2(160, 240));

	// 레이어에 메뉴 객체 추가
	this->addChild(pMenu);

	return true;
}

void GameOver::doClose(Ref* pSender)
{
	// 새로운 게임을 시작하도록 처음 신으로 이동
	Scene* pScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(pScene);
}