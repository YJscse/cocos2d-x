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

	///////////////////

	auto bg = LayerColor::create(Color4B(125, 125, 125, 255));
	this->addChild(bg);

	//LabelTTF
	TTFConfig ttfConfig1("fonts/Scissor Cuts.ttf", 40);
	TTFConfig ttfConfig2("fonts/Scissor Cuts.ttf", 40, GlyphCollection::DYNAMIC, nullptr, true);

	auto label1 = Label::createWithTTF(
		ttfConfig1,
		"Normal",
		TextHAlignment::CENTER,
		480);
	label1->setPosition(Vec2(240, 160));
	label1->setColor(Color3B::WHITE);
	this->addChild(label1);

	auto label2 = Label::createWithTTF(
		ttfConfig2,
		"Glow",
		TextHAlignment::CENTER,
		480);
	label2->setPosition(Vec2(240, 160 + 50));
	label2->setColor(Color3B::GREEN);
	label2->enableGlow(Color4B::YELLOW);
	this->addChild(label2);

	ttfConfig1.outlineSize = 2;

	auto label3 = Label::createWithTTF(
		ttfConfig1,
		"Outline",
		TextHAlignment::LEFT,
		480);
	label3->setPosition(Vec2(240, 160 - 50));
	label3->setColor(Color3B::WHITE);
	label3->enableOutline(Color4B::BLUE);
	this->addChild(label3);

	auto label4 = Label::createWithTTF(
		ttfConfig1,
		"Shadow",
		TextHAlignment::RIGHT,
		480);
	label4->setPosition(Vec2(240, 160 - 100));
	label4->setColor(Color3B::RED);
	label4->enableShadow(Color4B::BLACK);
	this->addChild(label4);

	return true;
}