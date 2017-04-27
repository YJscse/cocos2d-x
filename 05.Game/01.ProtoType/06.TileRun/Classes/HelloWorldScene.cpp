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
	
	winSize = Director::getInstance()->getWinSize();

	tmap = TMXTiledMap::create("TileMaps/TileRunMap.tmx");
	background = tmap->getLayer("Background");
	Ground = tmap->getLayer("Ground");
	Ground->setVisible(false);
	this->addChild(tmap, 0, 11);

	auto objects = tmap->getObjectGroup("Objects");

	ValueMap& spawnPoint = objects->getObject("SpawnPoint");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	SisterPosition = Vec2(x, y);
	this->createSister();

	xValue = -2;

	this->schedule(schedule_selector(HelloWorld::moveBackground), 0.01f);  // moveBackground를 0.01초마다 소환
	this->schedule(schedule_selector(HelloWorld::setSisterPosition), 0.01f);
	

    return true;
}

void HelloWorld::createSister()
{
	auto texture = Director::getInstance()->getTextureCache()->addImage("Images/grossinis_sister1.png");
	Sister = Sprite::createWithTexture(texture, Rect(0, 0, 52, 139));
	Sister->setAnchorPoint(Vec2(0.5f, 0.02f));
	Sister->setPosition(SisterPosition);
	Sister->setScale(0.5f);
	this->addChild(Sister);
}


Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
	position.x = position.x + count;

	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;

	return Vec2(x, y);
}

void HelloWorld::setSisterPosition(float f)
{
	Vec2 tileCoord = this->tileCoordForPosition(SisterPosition);

	// 현재 위치의 Tile GID 구하기
	int tileGid = this->Ground->getTileGIDAt(tileCoord);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string cliff = properties.asValueMap()["Cliff"].asString();
			if (cliff == "YES")
			{
				log("낭떠러지 입니다... %d", num);
				num++;
				return;
			}
		}
	}

}

void HelloWorld::moveBackground(float f)
{
	if (tmap->getPosition().x < -800)
	{
		tmap->setPosition(Vec2(0, 0));
	}
	
	tmap->setPosition(Vec2(tmap->getPosition().x + xValue, tmap->getPosition().y));

	count = count + 2;

	if (count == 960)
	{
		count = 0;
	}

}