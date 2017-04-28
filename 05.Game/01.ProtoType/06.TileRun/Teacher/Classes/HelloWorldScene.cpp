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
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    
    /////////////////////////////

    map = TMXTiledMap::create("TileMaps/run_background.tmx");
    background = map->getLayer("Background");

    auto forward = MoveBy::create(0.5f, Vec2(-32, 0));
    auto rep = RepeatForever::create(forward);

    background->runAction(rep);

    this->addChild(map);


	m_tamara = Sprite::create("TileMaps/grossinis_sister1.png");
    m_tamara->setScale(0.4f);
    m_tamara->setPosition(Vec2(100, 88));
    this->addChild(m_tamara);


    this->schedule(schedule_selector(HelloWorld::checkPosition), (1.0f / 20));


    return true;
}

void HelloWorld::checkPosition(float dt)
{
    // 40 x 32 = 1280
    // 480 - 1280 = -800

    int posX = background->getPosition().x;
    if (posX < -800) {
        background->setPosition(Vec2(-32, 0));
    }


    Vec2 point = this->background->convertToNodeSpace(m_tamara->getPosition());
    Vec2 tileCoord = this->tileCoordForPosition(Vec2(point.x, point.y - 32));

    // 현재 위치의 Tile GID 구하기
    int tileGid = this->background->getTileGIDAt(tileCoord);

    if (tileGid) 
    {
        // 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
        Value properties = map->getPropertiesForGID(tileGid);

        if (!properties.isNull())
        {
            std::string wall = properties.asValueMap()["Wall2"].asString();

            if (wall == "YES")
            {
                return;
            }
        }
    }
    else 
    {
        static int i = 0;
        i++;
        log("낭떠러지에요~ %d", i);
    }

}

// 현재 탭으로 선택된 타일의 위치를 가져온다..
Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
    int x = position.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - position.y) / map->getTileSize().height;
    return Vec2(x, y);
}
