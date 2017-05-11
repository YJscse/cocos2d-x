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

	srand((int)time(NULL));

	// 터치 활성화 : 앞의 예제들처럼 onEnter, onExit에서 처리

	// 벽돌 개수 지정
	BRICKS_HEIGHT = 4;
	BRICKS_WIDTH = 5;

	// 벽돌에 사욜될 텍스쳐 로딩
	texture = Director::getInstance()->getTextureCache()->addImage("white-512x512.png");

	// 벽돌 초기화
	this->initializeBricks();

	// 공초기화
	this->initializeBall();

	// 패들 초기화
	this->initializePaddle();

	// 2초후 게임 시작
	this->scheduleOnce(schedule_selector(HelloWorld::startGame), 2.0f);

    return true;
}


void HelloWorld::onEnter()
{
	Layer::onEnter();

	// 싱글 터치 모드로 터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	// Swallow touches only available in OneByOne mode.
	// 핸들링된 터치 이벤트 array에서 지우겠다는 의미이다.
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);

	// The priority of the touch listener is based on the draw order of sprite
	// 터치 리스너의 우선순위를 (노드가) 화면에 그려진 순서대로 한다.
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);                 // 실제로 게임을 만들때는 onEnter에 넣지말고 init에 넣어라 onExit도 마찬가지!!!
}

void HelloWorld::onExit()
{
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);

	Layer::onExit();
}
void HelloWorld::initializeBricks()
{
	int count = 0;

	for (int y = 0; y < BRICKS_HEIGHT; y++)
	{
		for (int x = 0; x < BRICKS_WIDTH; x++)
		{
			Sprite* bricks = Sprite::createWithTexture(texture, Rect(0, 0, 64, 40));

			// 색 지정
			switch(count++ % 4)
			{
				case 0:
					bricks->setColor(Color3B(255, 255, 255));
					break;

				case 1:
					bricks->setColor(Color3B(255, 0, 0));
					break;

				case 2:
					bricks->setColor(Color3B(255, 255, 0));
					break;

				case 3:
					bricks->setColor(Color3B(75, 255, 0));
					break;

				default:
					break;
			}

			// 좌표 지정
			bricks->setPosition(Vec2(x * 64 + 32, (y * 40) + 280));

			// 화면에 추가
			this->addChild(bricks);

			// 벡터에 추가
			targets.pushBack(bricks);
		}
	}
}

void HelloWorld::initializeBall()
{
	ball = Sprite::createWithTexture(texture, Rect(0, 0, 16, 16));

	ball->setColor(Color3B(0, 255, 255));
	ball->setPosition(Vec2(160, 240));
	this->addChild(ball);
}

void HelloWorld::initializePaddle()
{
	paddle = Sprite::createWithTexture(texture, Rect(0, 0, 80, 10));
	paddle->setColor(Color3B(255, 255, 0));
	paddle->setPosition(Vec2(160, 50));
	this->addChild(paddle);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
	if (isPlaying)
	{
		return true;
	}

	// 하나의 터치이벤트만 가져온다.
	auto touchPoint = touch->getLocation();

	// 패들을 터치했는지 체크한다.
	Rect rect = paddle->getBoundingBox();
	if (rect.containsPoint(touchPoint))
	{
		// 패들이 터치됏음을 체크
		isPaddleTouched = true;
	}
	else
	{
		isPaddleTouched = false;
	}

	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
	if (isPaddleTouched)
	{
		auto touchPoint = touch->getLocation();

		// 패들이 좌우로만 움직이게 y값은 바꾸지 않는다.
		// 또한 패들이 화면 바깥으로 나가지 않게 한다.
		if (touchPoint.x < 40)
		{
			touchPoint.x = 40;
		}

		if (touchPoint.x > 280)
		{
			touchPoint.x = 280;
		}

		Vec2 newLocation = Vec2(touchPoint.x, paddle->getPosition().y);
		paddle->setPosition(newLocation);
	}
}

void HelloWorld::startGame(float dt)
{
	ball->setPosition(Vec2(160, 240));

	ballMovement = Vec2(4, 4);

	if (rand() & 100 < 50)
	{
		ballMovement.x = -ballMovement.x;
	}
	ballMovement.y = -ballMovement.y;

	isPlaying = true;

	this->schedule(schedule_selector(HelloWorld::gameLogic), 2.0f / 60.0f);
}

void HelloWorld::gameLogic(float dt)
{
	// ballMovement.y 가 음수이면 볼이 내려오고 있는것.
	// ballMovement.y 가 양수이면 볼이 올라가고 있는것.
	// log("tick..%f", ballMovement.y);

	// 볼의 현재 위치
	ball->setPosition(Vec2(ball->getPosition().x + ballMovement.x,
						   ball->getPosition().y + ballMovement.y));

	// 볼과 패들 충돌 여부
	bool paddleCollision =
		ball->getPosition().y <= paddle->getPosition().y + 13 &&
		ball->getPosition().x >= paddle->getPosition().x - 48 &&
		ball->getPosition().x <= paddle->getPosition().x + 48;

	// 패들과 충돌 시 처리
	if (paddleCollision)
	{
		if (ball->getPosition().y <= paddle->getPosition().y + 13 && ballMovement.y < 0)
		{
			ball->setPosition(Vec2(ball->getPosition().x, paddle->getPosition().y + 13));
		}

		// 내려오던 것을 위로 올라가도록 공의 상하 진행 방향 바꾸기
		ballMovement.y = -ballMovement.y;
	}

	// 블록과 충돌 파악
	bool there_are_solid_bricks = false;
	
	for (auto &item : targets)
	{
		Sprite* brick = item;

		if (255 == brick->getOpacity())
		{
			there_are_solid_bricks = true;

			Rect rectA = ball->getBoundingBox();
			Rect rectB = brick->getBoundingBox();
			
			if (rectA.intersectsRect(rectB))
			{
				// 블록과 충돌 처리
				this->processCollision(brick);
			}
		}
	}

	// 블록이 없을 대 - 게임 종료 상태
	if (!there_are_solid_bricks)
	{
		isPlaying = false;
		ball->setOpacity(0);

		// 모든 스케쥴 제거
		this->unscheduleAllSelectors();

		log("You Win !!!");

		// 게임을 이겼다. 새로운 게임 대기화면...
		Scene* pScene = GameOver::createScene();
		Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, pScene));
	}
	
	// 벽면 충돌 체크
	if (ball->getPosition().x > 312 || ball->getPosition().x < 8)
		ballMovement.x = -ballMovement.x;

	if(ball->getPosition().y > 450)
		ballMovement.y = -ballMovement.y;

	//if (ball.position.y < 10)
	//{
	//	ballMovement.y = -ballMovement.y;
	//}

	// 패들을 빠져 나갈 때
	if (ball->getPosition().y < (50 + 5 + 8))
	{
		isPlaying = false;
		ball->setOpacity(0);

		// 모든 스케줄 제거
		this->unscheduleAllSelectors();

		log("You lose..");

		// 게임에서 졌다. 새로운 게임 대기화면은
		Scene* pScene = GameOver::createScene();
		Director::getInstance()->replaceScene(TransitionProgressRadialCCW::create(1, pScene));
	}
}

void HelloWorld::processCollision(Sprite* brick)
{
	Vec2 brickPos = brick->getPosition();
	Vec2 ballPos = ball->getPosition();

	if (ballMovement.x > 0 && brick->getPosition().x < ball->getPosition().x)
	{
		ballMovement.x = -ballMovement.x;
	}

	else if (ballMovement.x < 0 && brick->getPosition().x < ball->getPosition().x)
	{
		ballMovement.x = -ballMovement.x;
	}

	if (ballMovement.y > 0 && brick->getPosition().y > ball->getPosition().y)
	{
		ballMovement.y = -ballMovement.y;
	}

	else if (ballMovement.y < 0 && brick->getPosition().y < ball->getPosition().y)
	{
		ballMovement.y = -ballMovement.y;
	}

	brick->setOpacity(0);
}