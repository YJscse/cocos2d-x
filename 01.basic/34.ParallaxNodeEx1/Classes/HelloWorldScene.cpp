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

	/////////////////////////////////////
	
	// 스프라이트
	auto cocosImage = Sprite::create("Images/powered.png");
	cocosImage->setAnchorPoint(Vec2(0, 0));

	// 배경 레이어
	auto background = Sprite::create("Images/background.png");
	background->setAnchorPoint(Vec2(0, 0));

	// 레이블 : 위치 구분용
	auto gubun1 = LabelTTF::create("Start", "Arial", 64);
	gubun1->setPosition(Vec2(0, 240));
	gubun1->setAnchorPoint(Vec2(0.0f, 0.5f));

	auto gubun2 = LabelTTF::create("End", "Arial", 64);
	gubun2->setPosition(Vec2(640, 240));
	gubun2->setAnchorPoint(Vec2(1.0f, 0.5f));

	auto gubun3 = LabelTTF::create("80 Pixel", "Arial", 24);
	gubun3->setPosition(Vec2(80, 210));
	gubun3->setAnchorPoint(Vec2(0.0f, 0.5f));

	// 시뮬레이터 해상도 : 480 x 320
	// 배경 이미지       : 640 x 480

	// 패럴랙스노드 생성
	auto voidNode = ParallaxNode::create();

	// 패럴랙스노드에 배경 레이어 추가
	// 다음은 배경이미지가 패럴랙스노드와 같은 비율로 x축으로만 움직인다.
	voidNode->addChild(background, 1, Vec2(1.0f, 1.0f), Vec2(0, -160));

	// 패럴랙스노드에 스프라이트 추가
	// 패럴랙스노드보다 2배의 비율로 x축 이동을 한다.
	// 240 - (160 x 2) = -80
	// 4초 동안 x축 이동을 한 후의 위치는 cpp(-80, 0)이 된다.
	voidNode->addChild(cocosImage, 2, Vec2(2.0f, 0.0f), Vec2(240, 0)); 

	auto go = MoveBy::create(4, Vec2(-160, 0));
	auto goBack = go->reverse();
	auto seq = Sequence::create(go, goBack, nullptr);
	auto act = RepeatForever::create(seq);

	// 다음 줄을 리마크하면 패럴랙스노드에 배치된 스프라이트 및 이미지의
	// 초기 위치를 볼 수 있다.
	voidNode->runAction(act);

	// 위치 구분용 레이블 1, 2 는 백그라운드 이미지와 함께 움직이게 한다.
	background->addChild(gubun1);
	background->addChild(gubun2);

	// 위치 구분용 레이블 3은 백그라운드 이미지와 함께 움직이게 한다.
	cocosImage->addChild(gubun3);

	this->addChild(voidNode);

    return true;
}
