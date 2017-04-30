#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"

#define RANDOM_INT(MIN, MAX) ((MIN) + rand() % ((MAX + 1) - (MIN)))

//MIN~MAX까지 할당되게 해주는 함수

#define MAX_MISSILE  2

class HelloWorld : public cocos2d::LayerColor
{
public:
	HelloWorld();
	static cocos2d::Scene* createScene();

	virtual bool init();
	CREATE_FUNC(HelloWorld);

	void menuCloseCallback(Ref* pSender);
	int lifeNum = 0;
	int score = 0;
	char c[10];
	float regenCheckTime_;
private:
	cocos2d::Sprite *player_;//내 비행기
	cocos2d::Vector<cocos2d::Sprite*> missile_[MAX_MISSILE]; //미사일 배열 2개
	cocos2d::Vector<cocos2d::Sprite*> enemy_; //적 비행기
	cocos2d::Vector<cocos2d::Sprite*> life_[30];
	cocos2d::Vec2 distance_;//거리 잴 때 쓰는거
	cocos2d::Size size_; //사이즈 잴 때 쓰는거


	void Update(float time);
	void RegenEnemy(float time);
	void Shooting(float time);
	void AddEnemy(const cocos2d::Vec2& position);
	void PutCrashEffect(const cocos2d::Vec2 &pos);

	//장면 바꿔주기
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExitTransitionDidStart();
	virtual void onExit();
	~HelloWorld(void);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

};

#endif // __HELLOWORLD_SCENE_H__



