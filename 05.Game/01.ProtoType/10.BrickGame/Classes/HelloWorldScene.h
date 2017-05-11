#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "GameOver.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	// 스프라이트용 객체 변수들
	Texture2D* texture;
	Sprite* ball;
	Sprite* paddle;
	Vector<Sprite*> targets;

	// 벽돌 숫자 조정용
	int BRICKS_HEIGHT;
	int BRICKS_WIDTH;

	// 게임이 진행 중인지 체크하기 위한 변수
	bool isPlaying;

	// 패들이 터치됐는지 체크하기 위한 변수
	bool isPaddleTouched;

	// 공의 움직임을 저장하기 위한 변수
	Vec2 ballMovement;

	//~HelloWorld();
	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);

	void initializeBricks();
	void initializeBall();
	void initializePaddle();
	void startGame(float dt);
	void gameLogic(float dt);
	void processCollision(Sprite* brick);
};

#endif // __HELLOWORLD_SCENE_H__
