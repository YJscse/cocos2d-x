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
	/////////////////////////////////////////////

	// 화면 사이즈 구하기
	winSize = Director::getInstance()->getVisibleSize();

	// 타일맵 읽어 오기 1
	tmap = TMXTiledMap::create("TileMaps/astar1.tmx");
	metainfo = tmap->getLayer("MetaInfo");
	tmap->setPosition(Vec2(winSize.width / 2 - tmap->getContentSize().width / 2, winSize.height / 2 - tmap->getContentSize().height / 2));

	// 타일맵 읽어 오기 2
	/*tmap = TMXTiledMap::create("TileMaps/TestDesert.tmx");
	metainfo = tmap->getLayer("MetaInfo");

	metainfo->setVisible(false);*/
	this->addChild(tmap, 0, 11);   // z-order를 -1로 하면 화면이 나오지 않는다.

								   // 타일맵에서 Objects라고 지정한 오브젝트 레이어의 객체들 가져오기
	TMXObjectGroup* objects = tmap->getObjectGroup("Objects");

	// 오브젝트 레이어에서 SpawnPoint라고 지정한 속성값 읽어 오기
	ValueMap spawnPoint = objects->getObject("SpawnPoint");

	int x = spawnPoint["x"].asInt();
	int y = spawnPoint["y"].asInt();

	// 캐릭터 초기화
	tamara = Sprite::create("grossinis_sister1.png");
	tmap->addChild(tamara, (int)tmap->getChildren().size());
	tamara->setPosition(Vec2(x, y));
	tamara->setScale(0.3f);

	openPath = new std::vector<AstarItem*>();
	closePath = new std::vector<AstarItem*>();
	myPath = new std::vector<Vec2>();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HelloWorld::setViewpointCenter(Vec2 position)
{
	if (tmap->getMapSize().width * tmap->getTileSize().width < winSize.width ||
		tmap->getMapSize().height * tmap->getTileSize().height < winSize.height)
	{
		return;
	}

	// 파라미터로 들어오는 위치에 맞춰 화면을 움직인다.
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);

	x = MIN(x, (tmap->getMapSize().width * tmap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (tmap->getMapSize().height * tmap->getTileSize().height) - winSize.height / 2);

	Vec2 actualPosition = Vec2(x, y);
	Vec2 centerOfView = Vec2(winSize.width / 2, winSize.height / 2);
	Vec2 viewPoint = centerOfView - actualPosition;

	this->setPosition(viewPoint);
}

// 현재 탭으로 선택된 타일의 위치를 가져온다.
Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
	int x = position.x / tmap->getTileSize().width;
	int y = ((tmap->getMapSize().height * tmap->getTileSize().height) - position.y) / tmap->getTileSize().height;
	return Vec2(x, y);
}

// 현재 타일의 위치로 화면 좌표를 가져온다.
Vec2 HelloWorld::positionForTileCoord(Vec2 position)
{
	int x = position.x * tmap->getTileSize().width + tmap->getTileSize().width / 2;
	int y = (tmap->getMapSize().height * tmap->getTileSize().height) - position.y * tmap->getTileSize().height - tmap->getTileSize().height / 2;
	return Vec2(x, y);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (tamara->getNumberOfRunningActions())             // 액션중에 다른액션이 실행되도록 하지못하게한다.(연타 방지)
	{
		log("광클 금지");
		return false;
	}
	return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
	auto touchPoint = touch->getLocation();
	touchPoint = tmap->convertToNodeSpace(touchPoint);

	if (touchPoint.x >= (tmap->getMapSize().width * tmap->getTileSize().width) ||
		touchPoint.y >= (tmap->getMapSize().height* tmap->getTileSize().height) ||
		touchPoint.y <= 0 ||
		touchPoint.x <= 0)
	{
		//log("맵의 범위를 벗어난 곳을 클릭하였습니다.");
		return;
	}

	// 탭된 위치 구하기
	Vec2 tileCoord = this->tileCoordForPosition(touchPoint);
	log("%f..%f", tileCoord.x, tileCoord.y);

	// 현재 위치의 Tile GID 구하기
	int tileGid = this->metainfo->getTileGIDAt(tileCoord);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value& properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			int wall = properties.asValueMap()["Wall"].asInt();
			if (wall == 1)
			{
				log("1. 이동 불가능한 지역입니다.");
				return;
			}
		}
	}
	log("tileaGid : %d", tileGid);

	// A* 검색
	Vec2 tamaraPos = tamara->getPosition();
	pCoordS = this->tileCoordForPosition(tamaraPos);
	pCoordE = this->tileCoordForPosition(touchPoint);
	this->AStarPathFind();
}

void HelloWorld::AStarPathFind()
{
	for (int i = 0; i < openPath->size(); i++)
	{
		auto tNode = (AstarItem*)(openPath->at(i));
		delete tNode;
	}
	openPath->clear();

	for (int i = 0; i < closePath->size(); i++)
	{
		auto tNode = (AstarItem*)(closePath->at(i));
		delete tNode;
	}
	closePath->clear();

	myPath->clear();
	AstarItem* temp1 = new AstarItem();
	temp1->mPos = pCoordS;

	closePath->push_back(temp1);

	int nMax = tmap->getMapSize().width * tmap->getMapSize().height;
	bool bFind = false;
	for (int i = 0; i < nMax; i++)
	{
		Vec2 tmp1 = temp1->mPos;

		// 이동할 블럭의 위치(벡터값) 구하기
		Vec2 topLeft = Vec2(tmp1.x - 1, tmp1.y - 1);
		Vec2 topCenter = Vec2(tmp1.x, tmp1.y - 1);
		Vec2 topRight = Vec2(tmp1.x + 1, tmp1.y - 1);
		Vec2 left = Vec2(tmp1.x - 1, tmp1.y);
		Vec2 right = Vec2(tmp1.x + 1, tmp1.y);
		Vec2 bottomLeft = Vec2(tmp1.x - 1, tmp1.y + 1);
		Vec2 bottomCenter = Vec2(tmp1.x, tmp1.y + 1);
		Vec2 bottomRight = Vec2(tmp1.x + 1, tmp1.y + 1);

		// 이동 가능한 길인지 체크하고 벡터에 값 추가
		isRoad(topLeft, tmp1, temp1->g, 14);
		isRoad(topCenter, tmp1, temp1->g, 10);
		isRoad(topRight, tmp1, temp1->g, 14);
		isRoad(left, tmp1, temp1->g, 10);
		isRoad(right, tmp1, temp1->g, 10);
		isRoad(bottomLeft, tmp1, temp1->g, 14);
		isRoad(bottomCenter, tmp1, temp1->g, 10);
		isRoad(bottomRight, tmp1, temp1->g, 14);

		// log("size1 .. %ld", openPath->size());

		// 비용 계산 : F = G + H
		int cost1 = 100000000;
		Vec2 tmp2;
		int nErase = 0;
		for (int j = 0; j < openPath->size(); j++)
		{
			auto tNode = (AstarItem*)(openPath->at(j));
			int cost2 = tNode->f;
			if (cost2 < cost1)
			{
				cost1 = cost2;
				tmp2 = tNode->mPos;
				nErase = j;
			}
		}
		//log("Cost : %f .. %f", tmp2.x, tmp2.y);

		auto tNode = (AstarItem*)(openPath->at(nErase));
		closePath->push_back(tNode);

		openPath->erase(openPath->begin() + nErase);

		if (tmp2.equals(pCoordE))
		{
			//log("size3 .. %ld", closePath->size());
			bFind = true;
			this->makePass();
			break;
		}

		if (openPath->size() == 0)
		{
			break;
		}
		//log("size2 .. %ld .. (%d, %d)", openPath->size(), (int)tNode->mPos.x, (int)tNode->mPos..y);

		temp1 = new AstarItem();
		temp1 = tNode;
	}

	if (!bFind)
	{
		log("길을 찾을 수 없습니다.");
	}
}

void HelloWorld::isRoad(Vec2 mPos, Vec2 pPos, int cost1, int cost2)
{
	// log("Vec2(%d, %d) %d", (int)mPos.x, (int)mPos.y, cost1);

	// 이동 가능한 길인지 체크 1 : 맵범위 안 클릭 체크
	if (mPos.x >= tmap->getMapSize().width ||
		mPos.y >= tmap->getMapSize().height ||
		mPos.y < 0 ||
		mPos.x < 0)
	{
		return;
	}

	// 이동 가능한 길인지 체크 2 : 기존 closePath 안에 있는 노드인지 체크
	for (int i = 0; i < closePath->size(); i++)
	{
		auto tNode = (AstarItem*)(closePath->at(i));

		if (mPos == tNode->mPos)
		{
			return;
		}
	}

	// 이동 가능한 길인지 체크 3 : 기존 openPath 안에 있는 노드인지 체크
	for (int i = 0; i < openPath->size(); i++)
	{
		auto tNode = (AstarItem*)(openPath->at(i));

		if (mPos == tNode->mPos)
		{
			return;
		}
	}

	// 이동 가능한 길인지 체크 4 : 맵안의 장애물 체크

	// 타일 위치의 Tile GID 구하기
	int tileGid = this->metainfo->getTileGIDAt(mPos);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value properties = tmap->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			int wall = properties.asValueMap()["Wall"].asInt();
			if (wall == 1)
			{
				log("2.이동 불가능한 지역입니다.");
				return;
			}
		}
	}

	// 이동 가능한 길인지 체크 5 : 이동시 충돌이 있다면 못 가게
	if (cost2 == 14)
	{
		// (2, 2) --> (3, 1) : (2, 1)
		// (2, 2) --> (3, 1) : (3, 2)
		Vec2 pos1 = Vec2(pPos.x, mPos.y);
		int tileGid1 = this->metainfo->getTileGIDAt(pos1);
		if (tileGid1)
		{
			// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
			Value properties = tmap->getPropertiesForGID(tileGid1);

			if (!properties.isNull())
			{
				int wall = properties.asValueMap()["Wall"].asInt();
				if (wall == 1)
				{
					return;
				}
			}
		}
		Vec2 pos2 = Vec2(mPos.x, pPos.y);
		int tileGid2 = this->metainfo->getTileGIDAt(pos2);
		if (tileGid2)
		{
			// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
			Value properties = tmap->getPropertiesForGID(tileGid2);

			if (!properties.isNull())
			{
				int wall = properties.asValueMap()["Wall"].asInt();
				if (wall == 1)
				{
					return;
				}
			}
		}
	}

	// 벡터에 이동할 지점의 정보 추가
	AstarItem* temp2 = new AstarItem();
	temp2->mPos = mPos;
	temp2->pPos = pPos;
	temp2->g = cost1 + cost2;
	temp2->h = this->getHCost(mPos);
	temp2->f = temp2->g + temp2->h;

	//log("Vec2(%d, %d) %d .. %d", (int)temp2->mPos.x, (int)temp2->mPos.y, temp2->g, temp2->);
	openPath->push_back(temp2);

	return;
}

int HelloWorld::getHCost(Vec2 pos)
{
	return std::abs(pCoordE.x - pos.x) * 10 + std::abs(pCoordE.y - pos.y) * 10;
}

// 목적지로부터 출발지까지 거꾸로 부모를 찾아서 이동 경로에 넣어 준다.
void HelloWorld::makePass()
{
	int nLast = (int)closePath->size() - 1;

	auto tNode1 = (AstarItem*)(closePath->at(nLast));
	myPath->push_back(tNode1->mPos);
	Vec2 pos1 = tNode1->pPos;

	while (1)
	{
		for (int j = nLast; j >= 0; j--)
		{
			auto tNode2 = (AstarItem*)(closePath->at(j));
			Vec2 pos2 = tNode2->mPos;
			if (pos1 == pos2)
			{
				myPath->push_back(pos2);
				pos1 = tNode2->pPos;
				break;
			}
		}
		if (pos1 == pCoordS)
		{
			break;
		}
	}
	this->movePass();
}

// 캐릭터 이동시키기
void HelloWorld::movePass()
{
	Vector<FiniteTimeAction*>* actions = new Vector<FiniteTimeAction*>();

	int nLast2 = (int)myPath->size() - 1;

	for (int i = nLast2; i >= 0; i--)
	{
		//log("MyPath1 : Vec2(%d, %d)", (int)myPath->at(i).x, (int)myPath->at(i).y);
		Vec2 pos = positionForTileCoord(myPath->at(i));
		//log("MyPath2 : Vec2(%d, %d)", (int)pos.x, (int)pos.y);
		auto act = MoveTo::create(0.5, pos);
		actions->pushBack(act);

		auto callback = CallFunc::create(CC_CALLBACK_0(HelloWorld::moveCamera, this));
		actions->pushBack(callback);
	}

	const Vector<FiniteTimeAction*> xyz = *actions;
	auto seq = Sequence::create(xyz);
	tamara->runAction(seq);

	delete actions;
}

void HelloWorld::moveCamera()
{
	//캐릭터의 위치에 맞춰 화면 위치 조정
	log("1 : %d .. %d", (int)tamara->getPosition().x, (int)tamara->getPosition().y);
	Vec2 pos = tmap->convertToWorldSpace(tamara->getPosition());
	log("2 : %d .. %d", (int)pos.x, (int)pos.y);
	Vec2 pos2 = tamara->getPosition() + tmap->getPosition();
	log("3 : %d .. %d", (int)pos2.x, (int)pos2.y);

	this->setViewpointCenter(pos);
}

