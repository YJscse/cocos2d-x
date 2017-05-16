#pragma once
#ifndef __GAMEPLAY__
#define __GAMEPLAY__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class GamePlay : public cocos2d::Layer
			   , public b2ContactListener
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GamePlay);

	bool createBox2dWorld(bool debug);
	~GamePlay();
	virtual void draw(Renderer* renderer, const Mat4& transform, 
					  uint32_t flags) override;
	b2Body* addNewSprite(Vec2 point, Size size,
						 b2BodyType bodytype, 
						 const char* spriteName, 
						 int type);
	//b2Body* getBodyAtTab(Vec2 p);

	//void createRope(b2Body* bodyA, b2Vec2 anchorA, b2Body* bodyB, b2Vec2 anchorB, float32 sag);

	Size winSize;
	Texture2D* texture;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;
	b2BodyDef bodyDef1;

	bool playerIsFlying;
	bool bDrag;
	bool bBallTouch;
	b2Body* dragBody;

	b2MouseJoint* mouseJoint;
	b2Body* gbody;

	float playerVelocity;
	void tick(float dt);

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	void BeginContact(b2Contact *contact);

//	void SceneTrans(float dt);
	void moveBird();
	void createGround();
	void createPipe(float df);
	void gameOver();
	void baseScore(float f);
	void baseNowScore(float f);
	void createMenu(float f);

	int bRotate = 0;
	int nowScore = 0;
	int nScore = 0;
	bool wallCheck;
	bool bBool = true;
	char str[50] = { 0 };
	int gameover = 0;

	Sprite* ready;
	Sprite* howto;
	Sprite* bird;
	Sprite* ground;
	Sprite* startButton;
	Sprite* gradeButton;

	b2Body* upPipe;
	b2Body* downPipe;
	b2Body* body;
	b2Body* myBird;

	LabelAtlas* Score;
	LabelAtlas* smallScore;
	LabelAtlas* bestScore;

	Vector<Sprite*> highPipe;
	Vector<Sprite*> lowPipe;
	Vector<LabelAtlas*> scoreVector;
protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;
};
#endif // __GAMEPLAY__
