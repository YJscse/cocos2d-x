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

	///////////////////////// 

	// 메뉴 아이템 생성
	auto pMenuItem1 = MenuItemFont::create(
		"Pause",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	pMenuItem1->setColor(Color3B(0, 0, 0));

	auto pMenuItem2 = MenuItemFont::create(
		"Resume",
		CC_CALLBACK_1(HelloWorld::doClick, this));
	pMenuItem2->setColor(Color3B(0, 0, 0));

	pMenuItem1->setTag(1);
	pMenuItem2->setTag(2);

	// 메뉴 생성
	auto pMenu = Menu::create(pMenuItem1, pMenuItem2, nullptr);
	pMenu->setPosition(Vec2(240, 80));

	// 메뉴 배치
	pMenu->alignItemsHorizontally();

	// 레이어에 메뉴 객체 추가
	this->addChild(pMenu);

	//////////////////////////////

	auto pMan = Sprite::create("Images/grossini.png");
	pMan->setPosition(Vec2(140, 200));
	this->addChild(pMan);

	auto myActionForward = MoveBy::create(2, Vec2(200, 0));
	auto myActionBack = myActionForward->reverse();
	auto myAction = Sequence::create(myActionForward, myActionBack, nullptr);
	auto rep = RepeatForever::create(myAction);

	pMan->runAction(rep);

    return true;
}

void HelloWorld::doClick(Ref* pSender)
{
	auto tItem = (MenuItem*)pSender;

	int i = tItem->getTag();

	if (i == 1)
	{
		Director::getInstance()->pause();         // 작동하고있는 모든 애니메이션 액션 등이 멈춘다
	}
	else
	{
		Director::getInstance()->resume();        // 다시 작동한다.
	}
}