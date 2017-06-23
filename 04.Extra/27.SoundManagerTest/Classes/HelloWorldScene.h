#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include <functional>

class TextButton;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);

	// KMJ begin
private:
	void addTextButton(cocos2d::Vec2 & pos, const std::string & btnText, const std::function<void(TextButton*)> & btnCallback);

	void preloadAll(TextButton * button);
	void uncacheAll(TextButton * button);

	void onBgm(TextButton * button);
	void offBgm(TextButton * button);
	void onEffect(TextButton * button);
	void offEffect(TextButton * button);
	void upBgmVolume(TextButton * button);
	void downBgmVolume(TextButton * button);
	void upEffectVolume(TextButton * button);
	void downEffectVolume(TextButton * button);

	void addBgm1Play(TextButton * button);
	void addBgm2Play(TextButton * button);
	void playBgm1Alone(TextButton * button);
	void playBgm2Alone(TextButton * button);
	void stopAllBgm1(TextButton * button);
	void stopAllBgm2(TextButton * button);
	void pauseAllBgm1(TextButton * button);
	void pauseAllBgm2(TextButton * button);
	void resumeAllBgm1(TextButton * button);
	void resumeAllBgm2(TextButton * button);
	
	void addEffect1Play(TextButton * button);
	void addEffect2Play(TextButton * button);
	void stopAllEffect1(TextButton * button);
	void stopAllEffect2(TextButton * button);
	void pauseAllEffect1(TextButton * button);
	void pauseAllEffect2(TextButton * button);
	void resumeAllEffect1(TextButton * button);
	void resumeAllEffect2(TextButton * button);

private:
	float	_bgmVolume;
	float	_effectVolume;
	// KMJ end
};

#endif // __HELLOWORLD_SCENE_H__
