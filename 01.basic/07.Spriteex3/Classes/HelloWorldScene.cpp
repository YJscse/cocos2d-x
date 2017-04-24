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

	auto parent = Sprite::create("Images/grossini.png");

	parent->setPosition(Vec2(240, 160));

	this->addChild(parent);

	// 부모 스프라이트에 자식 스프라이트 추가
	auto child = Sprite::create("Images/white-512x512.png");

	child->setTextureRect(Rect(0, 0, 50, 5));

	child->setColor(Color3B::RED);

	Size parentSize;
	parentSize = parent->getContentSize();
	child->setPosition(Vec2(parentSize.width / 2.0, parentSize.height + 10));

	parent->addChild(child);

    return true;
}