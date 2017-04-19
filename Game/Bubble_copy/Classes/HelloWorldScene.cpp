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

	auto pBackGround = Sprite::create("Images/Background.png");

	pBackGround->setPosition(Vec2(320, 480));

	this->addChild(pBackGround);

	///////////////////////////////////////////////////// 클리어

	auto pMenuItem1 = MenuItemImage::create(
		"Images/Play_Button_01.png",
		"Images/Play_Button_02.png",
		CC_CALLBACK_1(HelloWorld::doClick1, this));

	auto pMenuItem2 = MenuItemImage::create(
		"Images/Create_Button_01.png",
		"Images/Create_Button_02.png",
		CC_CALLBACK_1(HelloWorld::doClick2, this));

	auto mode = Sprite::create("Images/Vibration_ON.png");
	auto play = Menu::create(pMenuItem1, nullptr);
	auto Create = Menu::create(pMenuItem2, nullptr);
	
	TitleLight = Sprite::create("Images/Logo_Light.png");
	TitleLight->setOpacity(100);

	MainLight = Sprite::create("Images/Lighting_Background.png");
	MainLight->setPosition(Vec2(320, 810));

	Light1 = Sprite::create("Images/All_Light.png");
	Light1->setPosition(Vec2(320, 600));

	Light2 = Sprite::create("Images/Flash_001.png");
	Light2->setPosition(Vec2(320, 600));

	Light3 = Sprite::create("Images/Flash_002.png");
	Light3->setPosition(Vec2(320, 600));

	Base = Sprite::create("Images/Base.png");
	Base->setPosition(Vec2(350, 275));

	Fish1 = Sprite::create("Images/Fish_001.png");
	Fish1->setPosition(Vec2(-30, 480));

	Fish2 = Sprite::create("Images/Fish_002.png");
	Fish2->setPosition(Vec2(800, 430));

	Bubble1 = Sprite::create("Images/Title_Bubble_001.png");
	Bubble1->setPosition(Vec2(320, -300));

	Bubble2 = Sprite::create("Images/Title_Bubble_002.png");
	Bubble2->setPosition(Vec2(320, -300));

	Bubble3 = Sprite::create("Images/Title_Bubble_003.png");
	Bubble3->setPosition(Vec2(320, -300));

	Bubble4 = Sprite::create("Images/Title_Bubble_004.png");
	Bubble4->setPosition(Vec2(320, -300));

	Star1 = Sprite::create("Images/Gold_Star.png");
	Star1->setPosition(Vec2(320, 35));

	Star2 = Sprite::create("Images/Purple_Star.png");
	Star2->setPosition(Vec2(600, 100));

	Star3 = Sprite::create("Images/Blue_Star.png");
	Star3->setPosition(Vec2(100, 100));

	Title = Sprite::create("Images/Bubble_Logo.png");
	Title->setPosition(Vec2(320, 700));
	Size TitleSize;
	TitleSize = Title->getContentSize();
	TitleLight->setPosition(Vec2(300, 150));

	CreateBase = Sprite::create("Images/Create_base.png");
	CreateBase->setPosition(Vec2(600, 40));
	play->setPosition(Vec2(320, 300));
	Create->setPosition(Vec2(600, 40));

	// 핸드폰 그림 재생버튼에 종속  클리어
	Size pMenuItem1Size;
	pMenuItem1Size = pMenuItem1->getContentSize();
	mode->setPosition(Vec2(pMenuItem1Size.width + 7, 3));

	this->addChild(play, 20);
	this->addChild(Create);
	this->addChild(Base, 20);
	this->addChild(CreateBase);
	this->addChild(Title, 20);
	Title->addChild(TitleLight);
	this->addChild(MainLight, 1);
	this->addChild(Light1, 1);
	this->addChild(Light2, 1);
	this->addChild(Light3, 1);
	this->addChild(Fish1, 2);
	this->addChild(Fish2, 2);
	this->addChild(Bubble1);
	this->addChild(Bubble2);
	this->addChild(Bubble3);
	this->addChild(Bubble4);
	this->addChild(Star1);
	this->addChild(Star2);
	this->addChild(Star3);
	this->ActionRepeatForever(this);
	pMenuItem1->addChild(mode);

    return true;
}

void HelloWorld::ActionRepeatForever(Ref* pSender)
{
	auto myActionForward = FadeOut::create(1.0f);
	auto myActionBack = myActionForward->reverse();
	auto myAction = Sequence::create(myActionForward, myActionBack, nullptr);

	auto TitleActionForward = MoveBy::create(3, Vec2(0, 20));
	auto TitleActionBack = TitleActionForward->reverse();
	auto TitleAction = Sequence::create(TitleActionForward, TitleActionBack , nullptr);

	auto TLActionForward = FadeOut::create(1.0f);
	auto TLActionBack = TLActionForward->reverse();
	auto TLAction = Sequence::create(TLActionForward, TLActionBack, nullptr);

	auto MLActionForward = FadeOut::create(2.0f);
	auto MLActionBack = MLActionForward->reverse();
	auto MLAction = Sequence::create(MLActionForward, MLActionBack, nullptr);

	auto LAction1Forward = FadeOut::create(3.0f);
	auto LAction1Back = LAction1Forward->reverse();
	auto LAction1 = Sequence::create(LAction1Forward, LAction1Back, nullptr);

	auto LAction2Forward = FadeOut::create(3.0f);
	auto LAction2Back = LAction1Forward->reverse();
	auto LAction2 = Sequence::create(LAction2Forward, LAction2Back, nullptr);

	auto LAction3Forward = FadeOut::create(3.0f);
	auto LAction3Back = LAction1Forward->reverse();
	auto LAction3 = Sequence::create(LAction3Forward, LAction3Back, nullptr);

	auto FAction1Forward = MoveBy::create(12, Vec2(800, 0));
	auto FAction1Back = Place::create(Vec2(-200, 480));
	auto FAction1 = Sequence::create(FAction1Forward, FAction1Back, nullptr);

	auto FAction2Forward = MoveBy::create(8, Vec2(-800, 0));
	auto FAction2Back = Place::create(Vec2(800, 400));
	auto FAction2 = Sequence::create(FAction2Forward, FAction2Back, nullptr);

	auto BAction1Forward = MoveBy::create(2, Vec2(0, 2000));
	auto BAction1Back = Place::create(Vec2(320, -300));
	auto BAction1 = Sequence::create(BAction1Forward , BAction1Back, nullptr);

	auto BAction2Forward = MoveBy::create(7, Vec2(0, 2000));
	auto BAction2Back = Place::create(Vec2(320, -300));
	auto BAction2 = Sequence::create(BAction2Forward, BAction2Back, nullptr);

	auto BAction3Forward = MoveBy::create(8, Vec2(0, 2000));
	auto BAction3Back = Place::create(Vec2(320, -300));
	auto BAction3 = Sequence::create(BAction3Forward, BAction3Back, nullptr);

	auto BAction4Forward = MoveBy::create(5, Vec2(0, 2000));
	auto BAction4Back = Place::create(Vec2(320, -300));
	auto BAction4 = Sequence::create(BAction4Forward, BAction4Back, nullptr);

	auto SAction1Forward = RotateBy::create(0.5, 360);
	auto SAction1Back = Place::create(Vec2(320, 35));
	auto SAction1 = Sequence::create(SAction1Forward, SAction1Back, nullptr);

	auto SAction2Forward = RotateBy::create(0.5, 360);
	auto SAction2Back = Place::create(Vec2(600, 100));
	auto SAction2 = Sequence::create(SAction2Forward, SAction2Back, nullptr);

	auto SAction3Forward = RotateBy::create(1.5, 360);
	auto SAction3Back = Place::create(Vec2(100, 100));
	auto SAction3 = Sequence::create(SAction3Forward, SAction3Back, nullptr);

	auto CBActionForward = RotateBy::create(1, 360);
	auto CBActionBack = Place::create(Vec2(600, 40));
	auto CBAction = Sequence::create(CBActionForward, CBActionBack, nullptr);

	auto rep1 = RepeatForever::create(myAction);
	auto rep2 = RepeatForever::create(TitleAction);
	auto rep3 = RepeatForever::create(TLAction);
	auto rep4 = RepeatForever::create(MLAction);
	auto rep5 = RepeatForever::create(LAction1);
	auto rep6 = RepeatForever::create(FAction1);
	auto rep7 = RepeatForever::create(FAction2);
	auto rep8 = RepeatForever::create(BAction1);
	auto rep9 = RepeatForever::create(BAction2);
	auto rep10 = RepeatForever::create(BAction3);
	auto rep11 = RepeatForever::create(BAction4);
	auto rep12 = RepeatForever::create(SAction1);
	auto rep13 = RepeatForever::create(SAction2);
	auto rep14 = RepeatForever::create(SAction3);
	auto rep15 = RepeatForever::create(CBAction);

	Base->runAction(rep1);
	Title->runAction(rep2);
	TitleLight->runAction(rep3);
	MainLight->runAction(rep4);
	Light1->runAction(rep5);
	Fish1->runAction(rep6);
	Fish2->runAction(rep7);
	Bubble1->runAction(rep8);
	Bubble2->runAction(rep9);
	Bubble3->runAction(rep10);
	Bubble4->runAction(rep11);
	Star1->runAction(rep12);
	Star2->runAction(rep13);
	Star3->runAction(rep14);
	CreateBase->runAction(rep15);
}

void HelloWorld::doClick1(Ref* pSender)
{
	log("첫 번째 메뉴가 선택되었습니다.");
}

void HelloWorld::doClick2(Ref* pSender)
{
	log("두 번째 메뉴가 선택되었습니다.");
}