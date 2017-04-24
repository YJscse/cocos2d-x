#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

//ansroid dffect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE    "Sounds/effect2.ogg"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE    "Sounds/effect1.raw"
#else
#define EFFECT_FILE    "Sounds/effect1.wav"
#endif // CC_PLATFORM_ANDROID

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define MUSIC_FILE    "Sounds/music.mid"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
#define MUSIC_FILE    "Sounds/background.ogg"
#else
#define MUSIC_FILE    "Sounds/background.mp3"
#endif // CC_PLATFORM_WIN32

std::string menuItems[] =
{
	"play background music",
	"pause background music",
	"resume background music",
	"stop background music",
	"play effect",
	"play effect repeatly",
	"pause effect",
	"resume effect",
	"stop effect",
};

//USING_NS_CC;
using namespace cocos2d;
using namespace CocosDenshion; // 추가

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
	/////////////////////////////////

	// 메뉴 아이템 생성 및 초기화

	auto pMenu = CCMenu::create();

	int nMenuCount = sizeof(menuItems) / sizeof(menuItems[0]);

	for (int i = 0; i < nMenuCount; ++i)
	{
		auto label = LabelTTF::create(menuItems[i].c_str(), "Arial", 24);
		label->setColor(Color3B(0, 0, 0));
		auto pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(HelloWorld::doSoundAction, this));
		pMenu->addChild(pMenuItem, i + 10000);
	}

	// 메뉴 정렬
	pMenu->alignItemsVertically();

	// 레이어에 메뉴 객체 추가
	this->addChild(pMenu);

	// preload background music and effect
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_FILE);

	// set default volume
	SimpleAudioEngine::getInstance()->setEffectsVolume(0.5);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);

    return true;
}

void HelloWorld::onExit()
{

	SimpleAudioEngine::getInstance()->unloadEffect(EFFECT_FILE);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);

	SimpleAudioEngine::getInstance()->end();

	Layer::onExit();
}

void HelloWorld::doSoundAction(Ref* pSender)
{
	auto pMenuItem = (MenuItem *)(pSender);
	int nIdx = pMenuItem->getZOrder() - 10000;

	switch (nIdx)
	{
		// play background music
		case 0:
			SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);
			break;
			// pause background music : 윈도우에선 동작 안함
		case 1:
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
			break;
			// resume background music : 윈도우에선 동작 안함
		case 2:
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
			break;
			//stop background music
		case 3:
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			break;
			// play effect
		case 4:
			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE);
			break;
			// play effect
		case 5:
			m_nSoundId = SimpleAudioEngine::getInstance()->playEffect(EFFECT_FILE, true);
			break;
			// pause effect
		case 6:
			SimpleAudioEngine::getInstance()->pauseEffect(m_nSoundId);
			break;
			// resume effect
		case 7:
			SimpleAudioEngine::getInstance()->resumeEffect(m_nSoundId);
			break;
			// stop effect
		case 8:
			SimpleAudioEngine::getInstance()->stopEffect(m_nSoundId);
			break;

	}
}