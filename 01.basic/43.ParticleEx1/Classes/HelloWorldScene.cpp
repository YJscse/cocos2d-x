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

	// 파티클 시스템 적용
	this->doParticles();

    return true;
}

void HelloWorld::doParticles()
{
	//ParticleSystem* particleTest = ParticleFire::create();    // Images/fire.png
	//ParticleSystem* particleTest = ParticleSun::create();    // Images/fire.png
	//ParticleSystem* particleTest = ParticleGalaxy::create();    // Images/fire.png
	//ParticleSystem* particleTest = ParticleSmoke::create();    // Images/fire.png
	//ParticleSystem* particleTest = ParticleMeteor::create();    // Images/fire.png
	//ParticleSystem* particleTest = ParticleFlower::create();    // Images/stars.png
	//ParticleSystem* particleTest = ParticleFireworks::create();    // Images/stars.png
	ParticleSystem* particleTest = ParticleExplosion::create();    // Images/stars.png

	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/fire.png");
	particleTest->setTexture(texture);

	if (particleTest != NULL)
	{

		// 파티클의 크기 조정
		particleTest->setScale(1.0);

		// 파티클의 지속 시간 조정 : -1 means 'forever'
		// particleTest->setDuration(1.0);

		// 파티클의 위치 조정
		particleTest->setPosition(Vec2(240, 160));

		this->addChild(particleTest);
	}
}