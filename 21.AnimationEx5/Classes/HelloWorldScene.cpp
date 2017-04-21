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

	// 배치노드는 별도의 TextureCache를 지정하는 일 없이 자기 것을 자기가 직접 가지고 있다.
	// 이미 자기의 텍스쳐이므로 해당 텍스처 안의 이미지는 얼마든지 사용해도
	// 배치노드 안에서는 결국 하나로 처리한다.

	auto BatchNode = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 50);
	this->addChild(BatchNode, 0, 1);

	auto texture = BatchNode->getTexture();

	for (int i = 0; i < 14; i++)
	{
		// 첫 번째 줄에 5개의 프레임이 있고, 6번째줄부터 두 번째 줄에 있으므로
		// 6번째(idx==5)부터는 y 좌표의 값을 증가시켜야 한다.
		int column = i % 5;
		int row = i / 5;

		auto pTemp = Sprite::createWithTexture(
			texture,
			Rect(column * 85, row * 121, 85, 121));
		pTemp->setPosition(Vec2(column * 85, row * 121));

		BatchNode->addChild(pTemp);
	}
    return true;
}