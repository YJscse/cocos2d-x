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
	// 메뉴 아이템 생성 및 초기화

	MenuItemFont::setFontSize(28);

	auto pMenuItem = MenuItemFont::create(
		"Reverese ",
		CC_CALLBACK_1(HelloWorld::doReverse, this));
	pMenuItem->setColor(Color3B(0, 0, 0));

	auto pMenu = Menu::create(pMenuItem, nullptr);

	pMenu->alignItemsHorizontally();

	pMenu->setPosition(Vec2(240, 50));

	this->addChild(pMenu);

	// Grossini 스프라이트 추가
	pMan = Sprite::create("Images/grossini.png");

	pMan->setPosition(Vec2(50, 200));

	this->addChild(pMan);

	return true;
}

void HelloWorld::doReverse(Ref* pSender)
{
	// MoveTo   : 리버스 액션 동작 안함
	// JumpTo   : 리버스 액션 에러남
	// RotateTo : 리버스 액션 에러남
	// ScaleTo  : 리버스 액션 에러남
	// TintTo   : 리버스 액션 에러남

//	auto myAction = MoveBy::create(2, Vec2(400, 0));   
//	auto myAction = MoveTo::create(2, Vec2(400, 0));   

	auto myAction = JumpBy::create(2, Vec2(400, 0), 50, 3);  
//	auto myAction = JumpTo::create(2, Vec2(400, 0), 50, 3);  

//	pMan->runAction(Place::create(Vec2(450, 200)));        
//	pMan->setPosition(Vec2(450, 200));                      

//	auto myAction = ScaleBy::create(2, 2.0f);                    
//	auto myAction = ScaleTo::create(2, 2.0f); 

//	auto myAction = RotateBy::create(2, 270);                
//	auto myAction = RotateTo::create(2, 270);   

//	auto myAction = Blink::create(2, 5);                      

//	auto myAction = FadeOut::create(1.5f);                    
//	auto myAction = FadeIn::create(1.5f);                    

//	auto myAction = TintTo::create(2, 128, 128, 128);
//  auto myAction = TintBy::create(2, 0, -30, 30);

	auto myActionBack = myAction->reverse();
	pMan->runAction(myActionBack);

}