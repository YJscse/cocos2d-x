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

	Sister = Sprite::create("Images/grossinis_sister1.png");
	Sister->setPosition(120, 140);
	Sister->setScale(0.7f);

	this->addChild(Sister);
	this->schedule(schedule_selector(HelloWorld::moveBackground), 0.01f);

	xValue = -2;

    return true;
}

//void HelloWorld::setPlayerPosition(Vec2 position)
//{
//	Vec2 tileCoord = this->(position);
//
//	// 현재 위치의 Tile GID 구하기
//	int tileGid = this->Ground->getTileGIDAt(tileCoord);
//
//	if (tileGid)
//	{
//		Value properties = tmap->getPropertiesForGID(tileGid);
//
//		if (!properties.isNull())
//		{
//			std::string cliff = properties.asValueMap()["Cliff"].asString();
//			if (cliff == "YES")
//			{
//				log("낭떠러지 입니다...");
//				return;
//			}
//		}
//	}
//
//}

void HelloWorld::moveBackground(float f)
{
	if (tmap->getPosition().x < -800)
	{
		tmap->setPosition(Vec2(0, 0));
	}

	tmap->setPosition(Vec2(tmap->getPosition().x + xValue, tmap->getPosition().y));

}