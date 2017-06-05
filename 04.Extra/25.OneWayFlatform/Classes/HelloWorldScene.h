#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class HelloWorld
	: public cocos2d::Scene
	, public b2ContactListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(HelloWorld);

	Size winSize;
	Texture2D* texture;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	bool createBox2dWorld(bool debug);
	~HelloWorld();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
	void onEnter();
	void onExit();
	void tick(float dt);

	bool onTouchBegan(Touch *touch, Event * event);
	void onTouchEnded(Touch *touch, Event * event);
	void addNewSpriteAtPosition(Vec2 location);
	void setWorld();

	b2Body* myBall;
	float  playerVelocity;
	bool   playerIsFlying;

	float height1;
	float height2;

	virtual void BeginContact(b2Contact *contact);
	virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
	virtual void EndContact(b2Contact *contact);


protected:
	cocos2d::CustomCommand _customCmd;
};

#endif // __HELLOWORLD_SCENE_H__
