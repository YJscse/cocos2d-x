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
	//----------------------- Type 1---------------------------- From Sprite
	//auto sprite = Sprite::create("Images/grossini_dance_atlas.png");
	//auto texture1 = sprite->getTexture();

	//----------------------- Type 2----------------------------From Texture
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas.png");

	//----------------------- Type 3----------------------------From BatchNode   <- array와 비슷하다
	//auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 10);  // <- 뒤에 숫자 10은 미리 메모리를 확보하는것 
	//auto texture3 = batch->getTexture();                                          // 하지만 우리의 그림은 14개이고 정상적으로 작동하는 이유는 자동적으로
	                                                                                // 메모리가 늘어난다.
	//-------------------------------------------------------------
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.3f);

	for (int i = 0; i < 14; i++)
	{
		// 첫째 줄에 5개의 프레임이 있고, 6번째부터는 두 번째 줄이 있으므로
		// 6번째(idx==5)부터는 y 좌표의 값을 증가시켜야 한다.
		int column = i % 5;
		int row = i / 5;

		animation->addSpriteFrameWithTexture(
			texture2,
			Rect(column * 85, row * 121, 85, 121));
	}

//	auto pMan = Sprite::create("Images/Grossini_dance_atlas.png", Rect(0, 0, 85, 121));
	auto pMan = Sprite::createWithTexture(texture2, Rect(0, 0, 85, 121));
	pMan->setPosition(Vec2(240, 160));
	this->addChild(pMan);

	// 애니메이션 액션 실행
	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	pMan->runAction(rep);

    return true;
}