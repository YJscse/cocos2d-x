#include "HelloWorldScene.h"
#include "ctime"
#include "cstdlib"

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
	
		auto pLabel1 = LabelTTF::create("Your Score: ", "Arial", 15);

		pLabel1->setPosition(Vec2(100, 280));

		pLabel1->setColor(Color3B::BLUE);

		this->addChild(pLabel1);

		auto pLabel2 = LabelTTF::create("Coumputer Score: ", "Arial", 15);

		pLabel2->setPosition(Vec2(320, 280));

		pLabel2->setColor(Color3B::BLUE);

		this->addChild(pLabel2);

		// 스코어 (레이블)
		////////////////////////////////////////////////

		auto pMenuItem1 = MenuItemFont::create(
			"[ 가위 ] ",
			CC_CALLBACK_1(HelloWorld::doClick, this));
		pMenuItem1->setColor(Color3B::BLACK);

		auto pMenuItem2 = MenuItemFont::create(
			"[ 바위 ] ",
			CC_CALLBACK_1(HelloWorld::doClick, this));
		pMenuItem2->setColor(Color3B::BLACK);

		auto pMenuItem3 = MenuItemFont::create(
			"[ 보 ]",
			CC_CALLBACK_1(HelloWorld::doClick, this));
		pMenuItem3->setColor(Color3B::BLACK);

		auto pMenu1Item4 = MenuItemFont::create(
			"[ 다시하기 ]",
			CC_CALLBACK_1(HelloWorld::doClick, this));
		pMenu1Item4->setColor(Color3B::BLACK);

		pMenuItem1->setTag(1);
		pMenuItem2->setTag(2);
		pMenuItem3->setTag(3);
		pMenu1Item4->setTag(4);

		auto pMenu = Menu::create(pMenuItem1, pMenuItem2, pMenuItem3, nullptr);
		auto pMenu1 = Menu::create(pMenu1Item4, nullptr);

		pMenu->alignItemsHorizontally();

		pMenu->setPosition(Vec2(220, 100));
		pMenu1->setPosition(Vec2(240, 50));

		this->addChild(pMenu);
		this->addChild(pMenu1);

		

		// 사용자 클릭 (메뉴)
		////////////////////////////////////////////////////////////
	
	
	return true;

}

void HelloWorld::doClick(Ref* pSender)
{
	auto tItem = (MenuItem *)pSender;
	int i = tItem->getTag();
	int com;


		srand((int)time(NULL));
		com = rand() % 3;
		
		

		if (com == 0)   
		{
			if (i == 1)
			{

				auto pScissors = Sprite::create("Images/img1.png");
				auto comScissors = Sprite::create("images/img1.png");
				auto pResult1 = LabelTTF::create("비겼습니다.", "Arial", 30);
			
				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comScissors->setPosition(Vec2(320, 180));
				comScissors->setTag(13);

				pScissors->setPosition(Vec2(100, 180));
				pScissors->setTag(11);
				pScissors->setFlippedX(11);

				pResult1->setPosition(Vec2(240, 250));
				pResult1->setColor(Color3B::RED);
				pResult1->setTag(12);

				this->addChild(pScissors);
				this->addChild(comScissors);
				this->addChild(pResult1);

			}
			else if (i == 2)
			{
				auto pRock = Sprite::create("Images/img2.png");
				auto comScissors = Sprite::create("images/img1.png");
				auto pResult2 = LabelTTF::create("이겼습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comScissors->setPosition(Vec2(320, 180));
				comScissors->setTag(13);

				pRock->setPosition(Vec2(100, 180));
				pRock->setTag(11);
				pRock->setFlippedX(11);

				pResult2->setPosition(Vec2(240, 250));
				pResult2->setColor(Color3B::RED);
				pResult2->setTag(12);

				this->addChild(pRock);
				this->addChild(comScissors);
				this->addChild(pResult2);
			}
			else if (i == 3)
			{
				auto pPaper = Sprite::create("Images/img3.png");
				auto comScissors = Sprite::create("images/img1.png");
				auto pResult3 = LabelTTF::create("졌습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comScissors->setPosition(Vec2(320, 180));
				comScissors->setTag(13);

				pPaper->setPosition(Vec2(100, 180));
				pPaper->setTag(11);
				pPaper->setFlippedX(11);

				pResult3->setPosition(Vec2(240, 250));
				pResult3->setColor(Color3B::RED);
				pResult3->setTag(12);

				this->addChild(pPaper);
				this->addChild(comScissors);
				this->addChild(pResult3);
			}
			else if(i == 4)
			{
				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);
			}
		}

		else if (com == 1)
		{
			if (i == 1)
			{

				auto pScissors = Sprite::create("Images/img1.png");
				auto comRock = Sprite::create("Images/img2.png");
				auto pResult1 = LabelTTF::create("졌습니다.", "Arial", 30);
				
				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comRock->setPosition(Vec2(320, 180));
				comRock->setTag(13);

				pScissors->setPosition(Vec2(100, 180));
				pScissors->setTag(11);
				pScissors->setFlippedX(11);

				pResult1->setPosition(Vec2(240, 250));
				pResult1->setColor(Color3B::RED);
				pResult1->setTag(12);


			
				this->addChild(pScissors);
				this->addChild(comRock);
				this->addChild(pResult1);
			}
			else if (i == 2)
			{
				auto pRock = Sprite::create("Images/img2.png");
				auto comRock = Sprite::create("Images/img2.png");
				auto pResult2 = LabelTTF::create("비겼습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comRock->setPosition(Vec2(320, 180));
				comRock->setTag(13);

				pRock->setPosition(Vec2(100, 180));
				pRock->setTag(11);
				pRock->setFlippedX(11);

				pResult2->setPosition(Vec2(240, 250));
				pResult2->setColor(Color3B::RED);
				pResult2->setTag(12);

			
				this->addChild(pRock);
				this->addChild(comRock);
				this->addChild(pResult2);
			}
			else if (i == 3)
			{
				auto pPaper = Sprite::create("Images/img3.png");
				auto comRock = Sprite::create("Images/img2.png");
				auto pResult3 = LabelTTF::create("이겼습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comRock->setPosition(Vec2(320, 180));
				comRock->setTag(13);

				pPaper->setPosition(Vec2(100, 180));
				pPaper->setTag(11);
				pPaper->setFlippedX(11);

				pResult3->setPosition(Vec2(240, 250));
				pResult3->setColor(Color3B::RED);
				pResult3->setTag(12);


				this->addChild(pPaper);
				this->addChild(comRock);
				this->addChild(pResult3);
			}
			else if (i == 4)
			{
				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);
			}
		}

		else if (com == 2)
		{
			if (i == 1)
			{

				auto pScissors = Sprite::create("Images/img1.png");
				auto comPaper = Sprite::create("Images/img3.png");
				auto pResult1 = LabelTTF::create("이겼습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comPaper->setPosition(Vec2(320, 180));
				comPaper->setTag(13);

				pScissors->setPosition(Vec2(100, 180));
				pScissors->setTag(11);
				pScissors->setFlippedX(11);

				pResult1->setPosition(Vec2(240, 250));
				pResult1->setColor(Color3B::RED);
				pResult1->setTag(12);

				this->addChild(pScissors);
				this->addChild(comPaper);
				this->addChild(pResult1);
			}
			else if (i == 2)
			{
				auto pRock = Sprite::create("Images/img2.png");
				auto comPaper = Sprite::create("Images/img3.png");
				auto pResult2 = LabelTTF::create("졌습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comPaper->setPosition(Vec2(320, 180));
				comPaper->setTag(13);

				pRock->setPosition(Vec2(100, 180));
				pRock->setTag(11);
				pRock->setFlippedX(11);

				pResult2->setPosition(Vec2(240, 250));
				pResult2->setColor(Color3B::RED);
				pResult2->setTag(12);
				
				this->addChild(pRock);
				this->addChild(comPaper);
				this->addChild(pResult2);
			}
			else if (i == 3)
			{
				auto pPaper = Sprite::create("Images/img3.png");
				auto comPaper = Sprite::create("Images/img3.png");
				auto pResult3 = LabelTTF::create("비겼습니다.", "Arial", 30);

				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);

				comPaper->setPosition(Vec2(320, 180));
				comPaper->setTag(13);

				pPaper->setPosition(Vec2(100, 180));
				pPaper->setTag(11);
				pPaper->setFlippedX(11);

				pResult3->setPosition(Vec2(240, 250));
				pResult3->setColor(Color3B::RED);
				pResult3->setTag(12);


				this->addChild(pPaper);
				this->addChild(comPaper);
				this->addChild(pResult3);
				
			}
			else if (i == 4)
			{
				this->removeChildByTag(11, true);
				this->removeChildByTag(12, true);
				this->removeChildByTag(13, true);
			}
		}
		
	
}