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
	: public cocos2d::Layer
	, public b2ContactListener
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Stage1);

	bool createBox2dWorld(bool debug);
	~Stage1();
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;

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
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	bool bDrag;
	b2Body* dragBody;
	b2MouseJoint* mouseJoint;
	b2Body* gbody;

	/////////////////////////////

	Sprite* bg;
	Sprite* pMan;

	b2Body* pManBody;

	float playerVelocity;
	bool playerIsFlying;

	bool rBool = true;
	bool uBool = false;
	bool lBool = false;
	bool dBool = false;

	void moveBackGround(float f);
	void createPlayer();
	void createSpine();
	void waySwich();
	//void createWall();
	void BeginContact(b2Contact *contact);
	

protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;

};

#endif // __STAGE1__
