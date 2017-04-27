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

	if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
	{
		return false;
	}
////////////////////////////////////////////////////////////////

	// 타일 맵 읽어오기
	auto map = TMXTiledMap::create("TileMaps/orthogonal-test-zorder.tmx");
	this->addChild(map, 0, 1);

	Size s = map->getContentSize();
	log("ContentSize: %f, %f", s.width, s.height);

	m_tamara = Sprite::create("TileMaps/grossinis_sister1.png");

	// 맵에 m_tamara를 z-order 4로 배치해라 *z-order가 4인 이유는 map에 layer가 4개있기때문
	map->addChild(m_tamara, map->getChildren().size());

	// map->getMapSize().width    <- 30(Width)    map->getTileSize().width  <- 64(Tile Width)
	m_tamara->setAnchorPoint(Vec2(0.5f, 0));

	auto move = MoveBy::create(10, Vec2(400, 320));
	auto back = move->reverse();
	auto seq = Sequence::create(move, back, nullptr);
	m_tamara->runAction(RepeatForever::create(seq));

	schedule(schedule_selector(HelloWorld::repositionSprite));


	return true;
}

void HelloWorld::repositionSprite(float dt)
{
	Vec2 p = m_tamara->getPosition();
	p = CC_POINT_POINTS_TO_PIXELS(p);     // 아이폰의 경우 화면을 압축하기 때문에 이것이 필요, 지금은 필요 없음
	Node* map = getChildByTag(1);

	//there are only 4 layers. (grass and 3 trees layers)
	//if tamara < 81, z = 4
	//if tamara < 162, z = 3
	//if tamara < 243,z = 2

	int newZ = 4 - ((p.y - 10) / 81);
	newZ = MAX(newZ, 0);

	map->reorderChild(m_tamara, newZ);
}