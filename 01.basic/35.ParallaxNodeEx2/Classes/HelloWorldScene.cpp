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

	/////////////////////////////////////////

	// 배경 레이어1
	auto background1 = Sprite::create("Images/background1.png");
	background1->setAnchorPoint(Vec2(0, 0));

	// 배경 레이어2
	auto background2 = Sprite::create("Images/background2.png");
	background2->setAnchorPoint(Vec2(0, 0));

	// 이미지가 만나는 가장자리(edge)에 검은 선이 생기는 현상을 방지하기 위해
	// 안티앨리어싱(Anti-Aliasing)을 비활성화한다.
	background1->getTexture()->setAliasTexParameters();
	background2->getTexture()->setAliasTexParameters();

	// 위에서 만든 스프라이트를 담을 부모로 패럴랙스노드를 만든다.
	auto voidNode = ParallaxNode::create();

	// 배경 스프라이트를 패럴랙스노드에 넣는다.
	// 배경이미지의 사이즈 : 512 x 320
	voidNode->addChild(background1, 1, Vec2(1.0f, 0.0f), Vec2(0, 0));
	voidNode->addChild(background2, 1, Vec2(1.0f, 0.0f), Vec2(512, 0));

	auto go = MoveBy::create(4, Vec2(-512, 0));
	auto goBack = go->reverse();
	auto seq = Sequence::create(go, goBack, nullptr);
	auto act = RepeatForever::create(seq);

	voidNode->runAction(act);

	this->addChild(voidNode);

    return true;
}