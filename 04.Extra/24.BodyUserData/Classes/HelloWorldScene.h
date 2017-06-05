#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include <GLES-Render.h>

#define PTM_RATIO 32

using namespace cocos2d;

class HelloWorld
	: public cocos2d::Layer
	, public b2ContactListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(HelloWorld);


    bool createBox2dWorld(bool debug);
    ~HelloWorld();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

    virtual void onEnter();
    virtual void onExit();
    void tick(float dt);
    b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type);

    virtual bool onTouchBegan(Touch *touch, Event * event);

    Size winSize;
    Texture2D* texture;
    b2World* _world;
    GLESDebugDraw* m_debugDraw;

//	virtual void BeginContact(b2Contact *contact);
//	virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
//	virtual void EndContact(b2Contact *contact);

	std::vector<b2Body*> delVec;

protected:
    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
    cocos2d::CustomCommand _customCmd;
};

#endif // __HELLOWORLD_SCENE_H__
