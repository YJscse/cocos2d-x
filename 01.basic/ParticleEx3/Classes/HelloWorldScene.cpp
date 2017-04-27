#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld::init()
{

    if ( !Layer::init())
    {
        return false;
    }

    return true;
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();

	this->showParticle(touchPoint);

	return true;
}

void HelloWorld::showParticle(Vec2 pPoint)
{
	const char* filename1 = "Particles/BoilingFoam.plist";
	const char* filename2 = "Particles/BurstPipe.plist";
	const char* filename3 = "Particles/Comet.plist";
	const char* filename4 = "Particles/ExplodingRing.plist";
	const char* filename5 = "Particles/Flower";
	const char* filename6 = "Particles/Galaxy.plist";
	const char* filename7 = "Particles/LavaFlow.plist";
	const char* filename8 = "Particles/Phoenix.plist";
	const char* filename9 = "Particles/SmallSun.plist";
	const char* filename10 = "Particles/SprnningPeas.plist";
	const char* filename11 = "Particles/Spiral.plist";
	const char* filename12 = "Particles/SpookyPeas.plist";
	const char* filename13 = "Particles/TestPremultipliedAlpha.plist";
	const char* filename14 = "Particles/Upsidedown.plist";

	ParticleSystem* emitter = ParticleSystemQuad::create(filename1);
	emitter->setPosition(pPoint);
	emitter->setDuration(2.0f);
	emitter->setAutoRemoveOnFinish(true);

	this->addChild(emitter);
}

//  http://particle2dx.com/  파티클 사이트