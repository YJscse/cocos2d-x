#include "HelloWorldScene.h"


//USING_NS_CC;
using namespace cocos2d;
using namespace cocos2d::ui;
//using namespace cocos2d::extension;

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

	Size editBoxSize = Size(300, 60);

	// top
	m_pEditName = EditBox::create(editBoxSize,
		Scale9Sprite::create("extensions/orange_edit.png"));
	m_pEditName->setPosition(Vec2(240, 250));
	m_pEditName->setFontColor(Color3B::GREEN);
	m_pEditName->setPlaceHolder("Name:");
	m_pEditName->setMaxLength(8);
	m_pEditName->setReturnType(EditBox::KeyboardReturnType::DONE);
	m_pEditName->setDelegate(this);
	addChild(m_pEditName);

	// middle
	m_pEditPassword = EditBox::create(editBoxSize,
		Scale9Sprite::create("extensions/green_edit.png"));
	m_pEditPassword->setPosition(Vec2(240, 150));
	m_pEditPassword->setFontColor(Color3B::RED);
	m_pEditPassword->setPlaceHolder("Password:");
	m_pEditPassword->setMaxLength(6);
	m_pEditPassword->setInputFlag(EditBox::InputFlag::PASSWORD);
	m_pEditPassword->setInputMode(EditBox::InputMode::SINGLE_LINE);
	m_pEditPassword->setDelegate(this);
	addChild(m_pEditPassword);

	// bottom
	m_pEditEmail = EditBox::create(Size(editBoxSize.width, editBoxSize.height),
		Scale9Sprite::create("extensions/yellow_edit.png"));
	m_pEditEmail->setPosition(Vec2(240, 50));
	m_pEditEmail->setPlaceHolder("Email:");
	m_pEditEmail->setInputMode(EditBox::InputMode::EMAIL_ADDRESS);
	m_pEditEmail->setDelegate(this);
	addChild(m_pEditEmail);

    return true;
}

void HelloWorld::editBoxEditingDidBegin(EditBox* editBox)
{
	log("editBox %p DidBegin !", editBox);
}

void HelloWorld::editBoxEditingDidEnd(EditBox* editBox)
{
	log("editBox %p DidEnd !", editBox);
}

void HelloWorld::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
	log("editBox %p TextureChanged, text: %s !", editBox, text.c_str());
}

void HelloWorld::editBoxReturn(EditBox* editBox)
{
	log("editBox %p was returned !", editBox);
}