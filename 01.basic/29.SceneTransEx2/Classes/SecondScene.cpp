#include "SecondScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

Scene* SecondScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SecondScene::create();
	scene->addChild(layer);

	return scene;
}

bool SecondScene::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 255, 0, 255)))
	{
		return false;
	}

	auto item1 = MenuItemFont::create("Close Scene 2", CC_CALLBACK_1(SecondScene::doClose, this));
	item1->setColor(Color3B(0, 0, 0));

	auto pMenu = Menu::create(item1, nullptr);

	pMenu->setPosition(Vec2(240, 50));

	this->addChild(pMenu);

	log("SecondScene :: init");

	return true;
}

void SecondScene::onEnter()
{
	Layer::onEnter();

	log("SecondScene :: onEnter");
}

void SecondScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	log("SecondScene :: onEnterTransitionDidFinish");
}

void SecondScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();

	log("SecondScene :: onExitTransitionDidStart");
}

void SecondScene::onExit()
{
	Layer::onExit();

	log("SecondScene :: onExit");
}

SecondScene::~SecondScene()
{
	log("SecondScene :: dealloc");
}

void SecondScene::doClose(Ref* pSender)
{
	// Director:: getInstance()->popScene();

	auto pScene = HelloWorld::createScene();

	Director::getInstance()->replaceScene(pScene);
}