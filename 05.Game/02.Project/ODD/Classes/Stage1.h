#pragma once
#ifndef __STAGE1__
#define __STAGE1__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // �ѱ۳����� �ϴ� ���
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class Stage1 
	: public Layer
	, public b2ContactListener
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Stage1);

	bool createBox2dWorld(bool debug);
	~Stage1();
	virtual void draw(Renderer* renderer, const Mat4& transform,
					  uint32_t flags) override;

	virtual void onEnter();
	virtual void onExit();
	void tick(float dt);
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type);

	b2Body* getBodyAtTab(Vec2 p);
	virtual bool onTouchBegan(Touch *touch, Event* event);
//	virtual void onTouchMoved(Touch *touch, Event* event);
	virtual void onTouchEnded(Touch *touch, Event* event);

	Size winSize;
	Texture2D* texture;
	Texture2D* texture2;
	Texture2D* texture3;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	bool bDrag;
	b2Body* dragBody;
	b2MouseJoint* mouseJoint;
	b2Body* gbody;

	/////////////////////////////

	Layer* layer;
	Sprite* bg;
	Sprite* pMan;
	Sprite* replay;
	Sprite* home;
	Sprite* barrier;
	Sprite* sItem;

	b2Body* pManBody;
	b2Body* shieldItem;

	RenderTexture* miniMap;

	float playerVelocity;
	bool playerIsFlying;

	bool jumpBool = false;
	bool rBool = true;
	bool uBool = false;
	bool lBool = false;
	bool dBool = false;
	bool gBool = false;
	bool overBool = false;
	bool shield = false;

	void movePlayer(float f);
	void createPlayer();
	void createFire();
	void waySwich();
	void createWall();
	void BeginContact(b2Contact *contact);
	void gameOver();
	void createItem();
	void createBarrier(float f);
	void createReplay(Ref* pSender);
	void createHome(Ref* pSender);

	
	int num = 0;
	int swichNum = 0;
	int sum = 0;
	int shieldNum = 0;
	int sum2 = 0;
	std::vector<b2Body*> delVec;

	


protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;

};

#endif // __STAGE1__
