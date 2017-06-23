#include "HelloWorldScene.h"

// KMJ begin
#include "soundManager.h"
#include "textButton.h"
#include <string>

// for test
const std::string BGM_1_PATH = "sounds/bgm/bgm1.mp3";
const std::string BGM_2_PATH = "sounds/bgm/bgm2.mp3";
const std::string EFFECT_1_PATH = "sounds/effect/effect1.mp3";
const std::string EFFECT_2_PATH = "sounds/effect/effect2.mp3";
// KMJ end

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	// KMJ begin
	// 여기 초기화 부분은 단순히 테스트용 UI 생성하는 부분이므로 전혀 볼필요 없습니다.
	// 다음 KMJ begin 으로 이동하세요.
	_bgmVolume = 1.0f;
	_effectVolume = 1.0f;

	auto & layerSize = this->getContentSize();
	Vec2 pos;

	pos = Vec2(10.0f, layerSize.height - 30.0f);
	addTextButton(pos, "Preload all audio files",	CC_CALLBACK_1(HelloWorld::preloadAll, this));
	addTextButton(pos, "Uncache all audio files",	CC_CALLBACK_1(HelloWorld::uncacheAll, this));
	addTextButton(pos, "BGM on",					CC_CALLBACK_1(HelloWorld::onBgm, this));
	addTextButton(pos, "BGM off",					CC_CALLBACK_1(HelloWorld::offBgm, this));
	addTextButton(pos, "Effect on",					CC_CALLBACK_1(HelloWorld::onEffect, this));
	addTextButton(pos, "Effect off",				CC_CALLBACK_1(HelloWorld::offEffect, this));
	addTextButton(pos, "BGM Volume up",				CC_CALLBACK_1(HelloWorld::upBgmVolume, this));
	addTextButton(pos, "BGM Volume down",			CC_CALLBACK_1(HelloWorld::downBgmVolume, this));
	addTextButton(pos, "Effect Volume up",			CC_CALLBACK_1(HelloWorld::upEffectVolume, this));
	addTextButton(pos, "Effect Volume down",		CC_CALLBACK_1(HelloWorld::downEffectVolume, this));

	pos = Vec2(layerSize.width * 0.28f, layerSize.height - 30.0f);
	addTextButton(pos, "Play bgm1.mp3 in addition",		CC_CALLBACK_1(HelloWorld::addBgm1Play, this));
	addTextButton(pos, "Play bgm2.mp3 in addition",		CC_CALLBACK_1(HelloWorld::addBgm2Play, this));
	addTextButton(pos, "Play bgm1.mp3 alone",			CC_CALLBACK_1(HelloWorld::playBgm1Alone, this));
	addTextButton(pos, "Play bgm2.mp3 alone",			CC_CALLBACK_1(HelloWorld::playBgm2Alone, this));
	addTextButton(pos, "Stop all bgm1.mp3 playing",		CC_CALLBACK_1(HelloWorld::stopAllBgm1, this));
	addTextButton(pos, "Stop all bgm2.mp3 playing",		CC_CALLBACK_1(HelloWorld::stopAllBgm2, this));
	addTextButton(pos, "Pause all bgm1.mp3 playing",	CC_CALLBACK_1(HelloWorld::pauseAllBgm1, this));
	addTextButton(pos, "Pause all bgm2.mp3 playing",	CC_CALLBACK_1(HelloWorld::pauseAllBgm2, this));
	addTextButton(pos, "Resume all bgm1.mp3 playing",	CC_CALLBACK_1(HelloWorld::resumeAllBgm1, this));
	addTextButton(pos, "Resume all bgm2.mp3 playing",	CC_CALLBACK_1(HelloWorld::resumeAllBgm2, this));

	pos = Vec2(layerSize.width * 0.64f, layerSize.height - 30.0f);
	addTextButton(pos, "Play effect1.mp3 in addition",		CC_CALLBACK_1(HelloWorld::addEffect1Play, this));
	addTextButton(pos, "Play effect2.mp3 in addition",		CC_CALLBACK_1(HelloWorld::addEffect2Play, this));
	addTextButton(pos, "",									nullptr);
	addTextButton(pos, "",									nullptr);
	addTextButton(pos, "Stop all effect1.mp3 playing",		CC_CALLBACK_1(HelloWorld::stopAllEffect1, this));
	addTextButton(pos, "Stop all effect2.mp3 playing",		CC_CALLBACK_1(HelloWorld::stopAllEffect2, this));
	addTextButton(pos, "Pause all effect1.mp3 playing",		CC_CALLBACK_1(HelloWorld::pauseAllEffect1, this));
	addTextButton(pos, "Pause all effect2.mp3 playing",		CC_CALLBACK_1(HelloWorld::pauseAllEffect2, this));
	addTextButton(pos, "Resume all effect1.mp3 playing",	CC_CALLBACK_1(HelloWorld::resumeAllEffect1, this));
	addTextButton(pos, "Resume all effect2.mp3 playing",	CC_CALLBACK_1(HelloWorld::resumeAllEffect2, this));
	// KMJ end
    
    return true;
}

// KMJ begin
void HelloWorld::addTextButton(Vec2 & pos, const std::string & btnText, const std::function<void(TextButton*)> & btnCallback)
{
	if (btnCallback)
	{
		auto textButton = TextButton::create(btnText, btnCallback);
		textButton->setPosition(pos);
		textButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
		addChild(textButton);
	}	

	pos.y -= 25.0f;
}

void HelloWorld::preloadAll(TextButton * button)
{
	// bool 값을 바꿔서 테스트 해보면 됩니다. 효과는 동일.
	bool preloadOneByOne = true;

	if (preloadOneByOne)
	{
		CSoundManager::getInstance()->preloadAudioFile(BGM_1_PATH);
		CSoundManager::getInstance()->preloadAudioFile(BGM_2_PATH);
		CSoundManager::getInstance()->preloadAudioFile(EFFECT_1_PATH);
		CSoundManager::getInstance()->preloadAudioFile(EFFECT_2_PATH);
	}
	else
	{
		std::vector<std::string> filePathVec;
		filePathVec.reserve(4);
		filePathVec.push_back(BGM_1_PATH);
		filePathVec.push_back(BGM_2_PATH);
		filePathVec.push_back(EFFECT_1_PATH);
		filePathVec.push_back(EFFECT_2_PATH);
		CSoundManager::getInstance()->preloadAudioFiles(filePathVec);
	}
}

void HelloWorld::uncacheAll(TextButton * button)
{
	// bool 값을 바꿔서 테스트 해보면 됩니다. 효과는 동일.
	bool uncacheOneByOne = true;

	if (uncacheOneByOne)
	{
		CSoundManager::getInstance()->uncacheAudioFile(BGM_1_PATH);
		CSoundManager::getInstance()->uncacheAudioFile(BGM_2_PATH);
		CSoundManager::getInstance()->uncacheAudioFile(EFFECT_1_PATH);
		CSoundManager::getInstance()->uncacheAudioFile(EFFECT_2_PATH);
	}
	else
	{
		CSoundManager::getInstance()->uncacheAllAudioFiles();
	}
}

void HelloWorld::addBgm1Play(TextButton * button)
{
	// 세번 째 인자는 이전에 재생 중이던 모든 BGM을 STOP 시키는 옵션입니다.
	// 현재 재생 중인 BGM에 추가적으로 BGM을 동시 재생 시키는 것이므로 false를 넘김.
	
	// 단, 주의해야 할 점은 BGM의 경우 같은 파일의 중첩 재생을 허용하지 않게 해두었습니다.
	// (굳이 중첩 재생을 하고 싶다면 파일 경로를 다르게 해서 두개를 가지고 있으면 됩니다.)
	
	// 씬 넘어 갈 때 마다 이전에 재생 중이던 BGM을 체크할 필요가 없게 하기 위함 입니다. 
	// (같은 BGM 인데 씬이 바뀌었다고 처음부터 재생되면 이상하니까요...)

	CSoundManager::getInstance()->playBgm(BGM_1_PATH, true, false);
}

void HelloWorld::addBgm2Play(TextButton * button)
{
	CSoundManager::getInstance()->playBgm(BGM_2_PATH, true, false);
}

void HelloWorld::playBgm1Alone(TextButton * button)
{
	// 이전 까지 재생 중이던 BGM들을 모두 STOP 시키고, 아래 함수의 파라미터로 넘긴 것 하나만 재생
	// 단, 위와 마찬가지로 이미 재생 중인 파일 중에 BGM_1_PATH가 있다면 그대로 두고, 나머지 BGM만 STOP 시킴.
	CSoundManager::getInstance()->playBgm(BGM_1_PATH);
}

void HelloWorld::playBgm2Alone(TextButton * button)
{
	CSoundManager::getInstance()->playBgm(BGM_2_PATH);
}

void HelloWorld::stopAllBgm1(TextButton * button)
{
	// 일단 같은 BGM파일이 중복 재생되지 않게 막아두었지만, 파일명으로 stop이 가능하기 위해 만들었습니다.
	// 파일명을 넘기지 않고 멈추게 하려면 그냥 stopAllBgms() 함수를 호출하면 됩니다.
	CSoundManager::getInstance()->stopBgm(BGM_1_PATH);
}

void HelloWorld::stopAllBgm2(TextButton * button)
{
	CSoundManager::getInstance()->stopBgm(BGM_2_PATH);
}

void HelloWorld::pauseAllBgm1(TextButton * button)
{
	CSoundManager::getInstance()->pauseBgm(BGM_1_PATH);
}

void HelloWorld::pauseAllBgm2(TextButton * button)
{
	CSoundManager::getInstance()->pauseBgm(BGM_2_PATH);
}

void HelloWorld::resumeAllBgm1(TextButton * button)
{
	CSoundManager::getInstance()->resumeBgm(BGM_1_PATH);
}

void HelloWorld::resumeAllBgm2(TextButton * button)
{
	CSoundManager::getInstance()->resumeBgm(BGM_2_PATH);
}

void HelloWorld::addEffect1Play(TextButton * button)
{
	CSoundManager::getInstance()->playEffect(EFFECT_1_PATH);
}

void HelloWorld::addEffect2Play(TextButton * button)
{
	CSoundManager::getInstance()->playEffect(EFFECT_2_PATH);
}

void HelloWorld::stopAllEffect1(TextButton * button)
{
	CSoundManager::getInstance()->stopEffect(EFFECT_1_PATH);
}

void HelloWorld::stopAllEffect2(TextButton * button)
{
	CSoundManager::getInstance()->stopEffect(EFFECT_2_PATH);
}

void HelloWorld::pauseAllEffect1(TextButton * button)
{
	CSoundManager::getInstance()->pauseEffect(EFFECT_1_PATH);
}

void HelloWorld::pauseAllEffect2(TextButton * button)
{
	CSoundManager::getInstance()->pauseEffect(EFFECT_2_PATH);
}

void HelloWorld::resumeAllEffect1(TextButton * button)
{
	CSoundManager::getInstance()->resumeEffect(EFFECT_1_PATH);
}

void HelloWorld::resumeAllEffect2(TextButton * button)
{
	CSoundManager::getInstance()->resumeEffect(EFFECT_2_PATH);
}

void HelloWorld::onBgm(TextButton * button)
{
	CSoundManager::getInstance()->setBgmOn(true);
}

void HelloWorld::offBgm(TextButton * button)
{
	CSoundManager::getInstance()->setBgmOn(false);
}

void HelloWorld::onEffect(TextButton * button)
{
	CSoundManager::getInstance()->setEffectOn(true);
}

void HelloWorld::offEffect(TextButton * button)
{
	CSoundManager::getInstance()->setEffectOn(false);
}

void HelloWorld::upBgmVolume(TextButton * button)
{
	if (_bgmVolume < 1.0f)
		_bgmVolume += 0.1f; // 10% 씩 조절

	CSoundManager::getInstance()->setBgmVolume(_bgmVolume);
}

void HelloWorld::downBgmVolume(TextButton * button)
{
	if (_bgmVolume > 0.0f)
		_bgmVolume -= 0.1f; // 10% 씩 조절

	CSoundManager::getInstance()->setBgmVolume(_bgmVolume);
}

void HelloWorld::upEffectVolume(TextButton * button)
{
	if (_effectVolume < 1.0f)
		_effectVolume += 0.1f; // 10% 씩 조절

	CSoundManager::getInstance()->setEffectVolume(_effectVolume);
}

void HelloWorld::downEffectVolume(TextButton * button)
{
	if (_effectVolume > 0.0f)
		_effectVolume -= 0.1f; // 10% 씩 조절

	CSoundManager::getInstance()->setEffectVolume(_effectVolume);
}

// KMJ end