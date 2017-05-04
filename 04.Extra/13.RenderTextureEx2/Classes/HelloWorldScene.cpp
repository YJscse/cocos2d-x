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

	// Render Texture 만들기
	m_pTarget = RenderTexture::create(winSize.width,
		winSize.height,
		Texture2D::PixelFormat::RGBA8888);
	m_pTarget->retain();  // 화면에 아무것도 없으면 이 함수는 쓰지않는구나라고 생각해서 지워버리기때문에 강제로 retain을 걸어줘야함
	m_pTarget->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

	this->addChild(m_pTarget, 0);

	// 선물상자 만들기
	auto box = Sprite::create("Images/image01.png");
	box->setPosition(Vec2(240, 190));
	box->getContentSize();
	this->addChild(box, 2);

	checkRect = box->getBoundingBox();

	// clear 만들기
	auto item2 = MenuItemFont::create("Clear", CC_CALLBACK_1(HelloWorld::clearImage, this));
	item2->setColor(Color3B::BLACK);
	auto menu = Menu::create(item2, nullptr);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(winSize.width - 80, winSize.height - 30));

	this->addChild(menu, 2);
	
	TableView* tableView1 = TableView::create(this, Size(240, 90));
	tableView1->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView1->setPosition(Vec2(120, 20));
	tableView1->setDelegate(this);
	this->addChild(tableView1);
	tableView1->reloadData();

    return true;
}

HelloWorld::~HelloWorld()
{
	m_pTarget->release();  // 위에 retain을 했기에 해줘야함
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::onExit()
{
	_eventDispatcher->removeEventListener(listener);

	Layer::onExit();
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	auto touch = touches[0];
	Vec2 start = touch->getLocation();
	Vec2 end = touch->getPreviousLocation();

	if (!checkRect.containsPoint(start))   
	{

		return;
	}

	if (!checkRect.containsPoint(end))
	{
		return;
	}

	m_pTarget->begin();

	// for extra points, we`ll draw this smootly from the last position and vary the sprite`s
	// scale/rotation/offset
	float distance = start.getDistance(end);
	if (distance > 1)
	{
		int d = (int)distance;
		m_pBrush.clear();
		for (int i = 0; i < d; ++i)
		{
			Sprite* sprite = Sprite::create("Images/brush.png");
			sprite->setColor(mycolor);
			//sprite->setOpacity(20);
			//sprite->setOpacity(150);
			m_pBrush.pushBack(sprite);
		}

		for (int i = 0; i < d; i++)
		{
			float difx = end.x - start.x;
			float dify = end.y - start.y;
			float delta = (float)i / distance;
			m_pBrush.at(i)->setPosition(Vec2(start.x + (difx * delta), start.y + (dify * delta)));
			m_pBrush.at(i)->setRotation(rand() % 360);
			float r = (float)(rand() % 50 / 50.f) + 0.25f;
			m_pBrush.at(i)->setScale(r);

			// m_pBrush.at(i)->setColor(Color3B(rand() % 127 + 1298, 255, 255));

			// Call visit to draw the brush, don`t call draw..
			m_pBrush.at(i)->visit();
		}
	}

	m_pTarget->end();
}


void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell)
{

	for (int i = 0; i < 13; i++)
	{
		TableViewCell* tcell = table->cellAtIndex(i);
		if (tcell)
		{
			Sprite* tsprite = (Sprite*)(tcell->getChildByTag(100));
			tsprite->setPosition(Vec2(0, 0));
		}
	}

	// 선택한 크레용이 위로 올라온다.
	auto sprite1 = (Sprite*)(cell->getChildByTag(100));
	sprite1->setPosition(Vec2(0, 17));

	// 색지정
	switch (cell->getIdx())
	{
	case 0:
		mycolor = Color3B(255, 0, 0);
		break;
	case 1:
		mycolor = Color3B(255, 112, 1);
		break;
	case 2:
		mycolor = Color3B(255, 255, 0);
		break;
	case 3:
		mycolor = Color3B(32, 215, 0);
		break;
	case 4:
		mycolor = Color3B(0, 154, 255);
		break;
	case 5:
		mycolor = Color3B(241, 95, 210);
		break;
	case 6:
		mycolor = Color3B(181, 0, 255);
		break;
	case 7:
		mycolor = Color3B(239, 239, 239);
		break;
	case 8:
		mycolor = Color3B(0, 0, 0);
		break;
	case 9:
		mycolor = Color3B(156, 156, 156);
		break;
	case 10:
		mycolor = Color3B(114, 64, 0);
		break;
	case 11:
		mycolor = Color3B(255, 207, 156);
		break;
	case 12:
		mycolor = Color3B(255, 231, 214);
		break;
	default:
		mycolor = Color3B::BLACK;
	}
}

Size HelloWorld::tableCellSizeForIndex(TableView * table, ssize_t idx)
{
	
	return Size(38, 100);
}

TableViewCell* HelloWorld::tableCellAtIndex(TableView* table, ssize_t idx)
{	
	auto sprite1 = Sprite::create("Images/crayon_01.png");
	auto sprite2 = Sprite::create("Images/crayon_02.png");
	auto sprite3 = Sprite::create("Images/crayon_03.png");
	auto sprite4 = Sprite::create("Images/crayon_04.png");
	auto sprite5 = Sprite::create("Images/crayon_05.png");
	auto sprite6 = Sprite::create("Images/crayon_06.png");
	auto sprite7 = Sprite::create("Images/crayon_07.png");
	auto sprite8 = Sprite::create("Images/crayon_08.png");
	auto sprite9 = Sprite::create("Images/crayon_09.png");
	auto sprite10 = Sprite::create("Images/crayon_10.png");
	auto sprite11 = Sprite::create("Images/crayon_11.png");
	auto sprite12 = Sprite::create("Images/crayon_12.png");
	auto sprite13 = Sprite::create("Images/crayon_13.png");

	cocos2d::Vector<cocos2d::Sprite*> crayons;

	crayons.pushBack(sprite1);
	crayons.pushBack(sprite2);
	crayons.pushBack(sprite3);
	crayons.pushBack(sprite4);
	crayons.pushBack(sprite5);
	crayons.pushBack(sprite6);
	crayons.pushBack(sprite7);
	crayons.pushBack(sprite8);
	crayons.pushBack(sprite9);
	crayons.pushBack(sprite10);
	crayons.pushBack(sprite11);
	crayons.pushBack(sprite12);
	crayons.pushBack(sprite13);

	for (int i = 0; i < 13; i ++)
	{
		TableViewCell* tcell = table->cellAtIndex(i);
		if (tcell)
		{
			auto tsprite = (Sprite*)(tcell->getChildByTag(100));
			tsprite->setPosition(Vec2(0, 0));
		}
	}

	auto string = String::createWithFormat("%ld", idx);

	TableViewCell* cell = table->dequeueCell();
	if (!cell)
	{
		cell = new CustomTableViewCell();
		cell->autorelease();

		log("idx 1 .. %d", idx);

		auto str = String::createWithFormat("Images/crayon_%02d.png", idx + 1);
		auto sprite = Sprite::create(str->getCString());
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(Vec2(0, 0));
		sprite->setTag(100);
		sprite->setScale(0.5f);
		cell->addChild(sprite, 7);

		auto label = LabelTTF::create(string->getCString(), "Helvetica", 20.0f);
		label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
		label->setTag(123);
		label->setColor(Color3B(0, 0, 0));
		cell->addChild(label, 7);
	}
	else
	{
		log("idx 2 .. %d", idx);

		auto sprite111 = (Sprite*)(cell->getChildByTag(100));
		auto sprite222 = (Sprite*)(crayons.at(idx));
		sprite111->setTexture(sprite222->getTexture());

		auto label = (LabelTTF*)cell->getChildByTag(123);
		label->setString(string->getCString());
	}

	return cell;
}

ssize_t HelloWorld::numberOfCellsInTableView(TableView* table)
{
	return 13;
}


void HelloWorld::clearImage(Ref* sender)
{
	m_pTarget->clear(255, 255, 255, 255);
}

