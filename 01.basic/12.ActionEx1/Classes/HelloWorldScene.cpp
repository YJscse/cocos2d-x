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

	// 메뉴 아이템 생성 및 초기화

	MenuItemFont::setFontSize(28);

	//auto pMenuItem = MenuItemFont::create(
	//	"Action ",
	//	CC_CALLBACK_1(HelloWorld::doAction, this));
	//pMenuItem->setColor(Color3B(0, 0, 0));

	//auto pMenu = Menu::create(pMenuItem, nullptr);

	//pMenu->alignItemsHorizontally();

	//pMenu->setPosition(Vec2(240, 50));

	//this->addChild(pMenu);

	// Grossini 스프라이트 추가
	pMan = Sprite::create("Images/grossini.png");

	pMan->setPosition(Vec2(50, 200));

	this->addChild(pMan);

    return true;
}

void HelloWorld::doAction(Ref* pSender)
{
//	auto myAction = MoveBy::create(2, Vec2(400, 0));   //지금 위치로부터 2초동안 (400, 0) 만큼 이동해라     By
//	auto myAction = MoveTo::create(2, Vec2(400, 0));   //2초동안 좌표 (400, 0)로 이동해라				 To

//	auto myAction = JumpBy::create(2, Vec2(400, 0), 50, 3);  // 지금으로부터 2초동안 (400, 0) 만큼 이동하면서 높이는 50만큼 3번 점프하면서 이동해라         By
//	auto myAction = JumpTo::create(2, Vec2(400, 0), 50, 3);  // 2초동안 좌표 (400, 0)로 높이 50만큼 3번 점프하면서  이동해라				                To

//	pMan->runAction(Place::create(Vec2(450, 200)));         // (450, 200) 으로 바로 이동해라   
//	pMan->setPosition(Vec2(450, 200));                      // (450, 200) 으로 바로 이동해라 
	 
//	auto myAction = ScaleBy::create(2, 2.0f);                  // 현재 크기로 부터 2초동안 2배 커져라     By    
//	auto myAction = ScaleTo::create(2, 2.0f);                  // 처음 크기로 부터 2초동안 2배 커져라     To

//	auto myAction = RotateBy::create(2, 270);                 // 현재로부터 2초동안 270도 회전해라
//	auto myAction = RotateTo::create(2, 270);                 // 해당위치로 2초동안 회전해라

	auto myAction = Blink::create(2, 5);                      // 2초동안 5번 깜빡여라

//	auto myAction = FadeOut::create(1.5f);                    // 1.5초 동안 점점 사라져라
//	auto myAction = FadeIn::create(1.5f);                     // 1.5초 동안 점점 나타나라 그전에 투명도값을 0으로 지정하여 안 보이게 해야함

//	auto myAction = TintTo::create(2, 128, 128, 128);
//	auto myAction = TintBy::create(2, 0, -30, 30);


	pMan->runAction(myAction);
}