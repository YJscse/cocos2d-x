#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

//#define PTM_RATIO 32

using namespace cocos2d;

class VRope;

class HelloWorld : public cocos2d::Layer
	, public b2ContactListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	bool createBox2dWorld(bool debug);
	~HelloWorld();
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch *touch, Event* event);
	virtual void onTouchMoved(Touch *touch, Event* event);

	void tick(float dt);
	
	b2Body* createCandyAt(Vec2 pt, bool userData);
	b2Body* createRopeTipBody();
	
	bool checkLineIntersection(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4);
	void createRope(b2Body* bodyA, b2Vec2 anchorA, b2Body* bodyB, b2Vec2 anchorB, float32 sag);
	
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type);
	b2Body* addNewSpriteAt(Vec2 point);

	virtual void BeginContact(b2Contact *contact);

	//	b2Body* getBodyAtTab(Vec2 p);
//	virtual void onTouchEnded(Touch *touch, Event* event);

private:
	Size winSize;
	Texture2D* texture;
	b2World* _world;
	GLESDebugDraw* m_debugDraw;

	bool bDrag;
	b2Body* dragBody;
	b2MouseJoint* mouseJoint;
	b2Body* gbody;

	b2Body* groundBody;
	std::vector<VRope*>* ropes;
	std::vector<b2Body*>* candies;
	SpriteBatchNode* ropeSpriteSheet;

	Sprite* sprite;
	Sprite* croc_;
	b2Body* crocMouth_;
	b2Fixture* crocMouthBottom_;
	bool crocMouthOpened;

protected:
	void onDraw(const Mat4& transform, uint32_t flags);

	CustomCommand _customCmd;

};

#endif // __HELLOWORLD_SCENE_H__
