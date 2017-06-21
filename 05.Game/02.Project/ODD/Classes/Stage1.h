#pragma once
#ifndef __STAGE1__
#define __STAGE1__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
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

	virtual bool onTouchBegan(Touch *touch, Event* event);
//	virtual void onTouchMoved(Touch *touch, Event* event);
	virtual void onTouchEnded(Touch *touch, Event* event);

	Size winSize;
	Texture2D* texture;
	Texture2D* texture2;
	Texture2D* texture3;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	/////////////////////////////

	Layer* layer;
	Sprite* bg;
	Sprite* pMan;
	Sprite* replay;
	Sprite* home;
	Sprite* barrier;
	Sprite* sItem;
	Sprite* over;
	Sprite* clear;
	Sprite* jumpjump;
	Sprite* cover;
	Sprite* ready;
	Sprite* go;
	Sprite* bord;
	Sprite* score;
	Sprite* best;
	Sprite* howTo;

	b2Body* pManBody;
	b2Body* shieldItem;
	b2Body* jumpItem;

	LabelAtlas* Score;
	LabelAtlas* NowScore;
	LabelAtlas* BestScore;

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
	bool jump = false;
	bool dJump = false;

	void movePlayer(float f);
	void waySwich();
	void BeginContact(b2Contact *contact);
	void gameOver(float f);
	void overScore(float f);
	void overBestScore(float f);

	void createPlayer();
	void createFire();
	void createWall();
	void createItem();
	void createReplay(Ref* pSender);
	void createHome(Ref* pSender);
	void createNext(Ref* pSender);
	void createStar();
	void createScore();
	void soundOnoff(int i);

	int num = 0;
	int swichNum = 0;
	int sum = 0;
	int shieldNum = 0;
	int sum2 = 0;
	int count = 0;
	int gameClear = 0;
	int doubleJump = 0;
	int jSum = 0;
	int stageNum = 0;
	int nowScore = 0;
	int nScore = 0;

	int sendJump = 0;
	int sendShield = 0;

	char str[500] = { 0 };

	std::vector<b2Body*> delVec;

	// 사운드
	float _soundVolume;

	int audioId1;
	int audioId2;
	int audioId3;
	int audioId4;
	int audioId5;
	int audioId6;
	int audioId7;
	int audioId8;

protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;

};

#endif // __STAGE1__
