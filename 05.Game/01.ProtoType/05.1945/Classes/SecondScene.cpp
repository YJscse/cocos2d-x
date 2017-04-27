#include "SecondScene.h"
#include "HelloWorldScene.h"
//#include "SimpleAudioEngine.h"




using namespace cocos2d;




USING_NS_CC;




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
	auto item1 = MenuItemFont::create(
		"ReStart",
		CC_CALLBACK_1(SecondScene::doClose, this));

	item1->setColor(Color3B(0, 0, 0));

	auto pMenu = Menu::create(item1, NULL);

	pMenu->setPosition(Vec2(160, 240));

	this->addChild(pMenu);

	log("SecondScene::init");

	auto pCloseItem1 = MenuItemImage::create(
		"Images/CloseNormal.png",
		"Images/CloseSelected.png",
		CC_CALLBACK_1(SecondScene::doClose, this));

	auto pMenu1 = Menu::create(pCloseItem1, NULL);
	pMenu1->setPosition(Vec2(100, 100));
	this->addChild(pMenu1, 1);

	return true;
}

void SecondScene::onEnter()
{
	Layer::onEnter();

	log("SecondScene ::onEnter");
}

void SecondScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	log("SecondScene::onEnterTransitionDidFinish");
}

void SecondScene::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();

	log("SecondScene::onExitTransitionDisStart");
}

void SecondScene::onExit()
{
	Layer::onExit();

	log("SecondScene::onExit");
}

SecondScene::~SecondScene()
{
	log("SecondScene::dealloc");
}

void SecondScene::doClose(Ref* pSender)
{
	log("doClose");

	auto pScene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(pScene);
}

void SecondScene::doClose2(Ref* pSender)
{
	log("doClose2");
}
