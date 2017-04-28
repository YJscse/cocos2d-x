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

    if ( !LayerColor::initWithColor(Color4B(255, 255,255,255) ))
    {
        return false;
    }

	/////////////////////////////////////////
	// 타일맵 읽어 오기
	tmap = TMXTiledMap::create("TileMaps/TestDesert.tmx");
	this->addChild(tmap, 0, 11); // z-order를 -1로 하면 화면이 나오지 않는다.

	// 타일맵에서 objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects");

	// 오브젝트 레이어에서 SpawnPoint라고 지정한 속성값 읽어오기
	ValueMap spawnPoint = objects->getObject("SpawnPoint");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	// SpawnPoint의 위치로 생성할 드래곤의 위치 지정하기
	dragonPosition = Vec2(x, y);

	// 드래곤을 만든다.
	this->createDragon();

	return true;
}

void HelloWorld::createDragon()
{
	// 움직이는 드래곤 넣기 시작
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/dragon_animation.png");

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.1f);

	for (int i = 0; i < 6; i++)
	{
		// 첫째 줄에 4개의 프레임이 있고, 5번째부터는 두 번째 줄에 있으므로
		// 5번째(idx==4)부터는 y 좌표의 값을 증가시켜야 한다.
		int index = i % 4;
		int rowIndex = i / 4;

		animation->addSpriteFrameWithTexture(texture, Rect(index * 130, rowIndex * 140, 130, 140));
	}

	// 스프라이트 생성 및 초기화
	dragon = Sprite::createWithTexture(texture, Rect(0, 0, 130, 140));
	dragon->setPosition(dragonPosition);
	this->addChild(dragon);

	// 드래곤의 방향을 바꿔준다.
	dragon->setFlippedX(true);

	// 드래곤의 크기를 줄여준다. 맵에 비해 너무 크다.
	dragon->setScale(0.5);

	// 드래곤 애니메이션
	auto animate = Animate::create(animation);
	auto rep = RepeatForever::create(animate);
	dragon->runAction(rep);
}