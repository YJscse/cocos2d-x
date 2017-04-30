#include "HelloWorldScene.h"
#include "SecondScene.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>
#include <ctime>

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#define EFFECT_FILE    "Sounds/effect2.ogg"
//#elif(CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
//#define EFFECT_FILE    "Sounds/effect1.raw"
//#else
//#define EFFECT_FILE    "Sounds/effect1.wav"
//#endif // CC_PLATFORM_ANDROID
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//#define MUSIC_FILE    "Sounds/music.mid"
//#elif(CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY)
//#define MUSIC_FILE    "Music/mainMainMusic.ogg"
//#else
//#define MUSIC_FILE    "Music/mainMainMusic.mp3"
//#endif // CC_PLATFORM_WIN32

USING_NS_CC;

using namespace cocos2d;
using namespace CocosDenshion;

HelloWorld::HelloWorld() :
	regenCheckTime_(-1.f)
{
}
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
	///전원 버튼 만들기 - menuCloseCallback은 맨 아래에 있음
	auto pCloseItem = MenuItemImage::create(
		"Images/CloseNormal.png",
		"Images/CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	//전원 버튼 만들기 
	size_ = this->getContentSize(); //화면의 사이즈를 불러오는거
	log("x:%d , y:%d", size_.width, size_.height);

	auto pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2(size_.width - 20, 20));
	this->addChild(pMenu, 1);

	//초기 화면의 score를 표시해주는 코드
	auto myScore = LabelTTF::create(itoa(score, c, 10), "Arial", 20);

	//score 위치 설정             
	myScore->setPosition(Vec2(size_.width / 2, size_.height - 20));
	myScore->setColor(Color3B(0, 0, 200));
	this->addChild(myScore, 1);
	myScore->setTag(40);
	//ⓑ태그주기

	auto pSprite = Sprite::create("Images/bg.png");
	pSprite->setPosition(Vec2(size_.width / 2, size_.height / 2));
	this->addChild(pSprite, 0);


	player_ = Sprite::create("Images/airplain_01.png");
	player_->setPosition(Vec2(size_.width / 2, 50));
	this->addChild(player_, 1);
	player_->setTag(1);

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//스케줄러 3개 업데이트, 리젠, 미사일발사
	this->schedule(schedule_selector(HelloWorld::Update));
	this->schedule(schedule_selector(HelloWorld::RegenEnemy), 1.5f);
	this->schedule(schedule_selector(HelloWorld::Shooting), 0.5f);

	srand((int)time(NULL));

	//목숨 벡터 
	for (int a = 0; a < 3; ++a)
	{
		Size size_ = this->getContentSize();//화면의 사이즈 구하는거
		auto life = Sprite::create("Images/airplain_01.png");

		// 목숨 위치 설정
		life->setPosition(Vec2(size_.width - (a * 40) - 200, 460));
		life->setScale(0.7f);

		//화면에 뿌리고
		this->addChild(life);
		life_[a].pushBack(life);//목숨 벡터에 넣어줌
	}

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/mainMainMusic.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/mainMainMusic.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("Music/explodeEffect.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("Music/fireEffect.mp3");

	return true;
}

void HelloWorld::Update(float time)
{
	Vector<Sprite*> eraseEnemy;
	Vector<Sprite*> eraseMissile;
	Vector<Sprite*> eraseLife;

	for (auto enemyObj : enemy_)//모든 적 비행기 불러온다.
	{
		//적 비행기가 화면 아래로 사라졌을 경우 메모리에서 삭제
		if (enemyObj->getPosition().y <= 0)
		{
			//removeChild(enemyObj, false); //적 비행기 화면에서 지워주지만 여기서 지워주면 뭔가 이상함.
			eraseEnemy.pushBack(enemyObj);//에너미 오브젝트를 eraseEnemy에 넣는다.
		}

		//모든 미사일 벡터를 반복하여 충돌검사
		for (int a = 0; a < MAX_MISSILE; ++a)
		{
			for (auto missileObj : missile_[a])
			{
				//미사일이 화면 위쪽으로 사라졌을 때
				if (missileObj->getPosition().y >= size_.height)
				{
					//ⓐ  화면에서 삭제하기 .. 
					//removeChild(missileObj, false); // 여기서 지워주면 뭔가 이상함
					eraseMissile.pushBack(missileObj);
				}

				//미사일이 적에게 부딪혔을 때
				if (missileObj->boundingBox().intersectsRect(enemyObj->boundingBox()))
				{
					

					//ⓑ태그받기  //스코어
					score++;
					auto myScorel = (Sprite*)getChildByTag(40);

					//스코어 화면에서 지워주고
					this->removeChild(myScorel);

					//스코어 재생성
					auto myScoreRe = LabelTTF::create(itoa(score, c, 10), "Arial", 20);

					//score 위치 설정             
					myScoreRe->setPosition(Vec2(size_.width / 2, size_.height - 20));
					myScoreRe->setColor(Color3B(0, 0, 200));
					this->addChild(myScoreRe, 1);
					myScoreRe->setTag(40);

					//폭발 이펙트
					PutCrashEffect(enemyObj->getPosition());

					//ⓐ화면에서 삭제 - 여기서 지워주면 뭔가 이상함....

					//removeChild(missileObj, false);

					//removeChild(enemyObj, false);

					//적 비행기 오브젝트? 를 푸쉬해서 위에서 새로만든 eraseEnemy벡터에 넣어준다
					eraseEnemy.pushBack(enemyObj);

					//미사일 벡터를 새로만든 eraseMissile 벡터에 넣어준다.
					eraseMissile.pushBack(missileObj);

				
				}
			//	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/explodeEffect.mp3", true);
			}
		}

		//적과 충돌했을때
		if (player_)
		{
			//충돌했을때 처리
			//intersectsRect함수 리턴값이 충돌하면 true를 리턴??
			if (player_->boundingBox().intersectsRect(enemyObj->boundingBox()))
			{
				//1.폭파 이펙트 출력
				PutCrashEffect(player_->getPosition());
				PutCrashEffect(enemyObj->getPosition());

				//2.적 비행기를 화면에서 없애주기

				//Scene에서 적비행기 스프라이트를 Remove
				eraseEnemy.pushBack(enemyObj);

				//부딪히면 깜빡이는 액션 
				auto playerl = (Sprite*)getChildByTag(1);
				auto BlinkAction = Blink::create(1, 2);
				playerl->runAction(BlinkAction);

				log("%d", lifeNum);
				auto lifeObj = life_[lifeNum];
				eraseMissile.pushBack(lifeObj);

				lifeNum = lifeNum + 1;

				if (lifeNum == 3)
				{
					auto pScene = SecondScene::createScene();
					Director::getInstance()->replaceScene(pScene);
				}
				regenCheckTime_ = 0.f;
			}
		}
	}

	//유저 비행기가 죽었을때만 체크하여 부활처리??
	if (!player_ &&
	 regenCheckTime_ != -1.f)
	{
	 //시간을 누적시켜서 3초 이상 경과되었을 때만 부활시킨다
	 regenCheckTime_ += time;
	 if (regenCheckTime_ >= 3.f)
	 {
	  //유저 비행기 부활처리
	  player_ = Sprite::create("Images/airplain_01.png");
	  player_->setPosition(ccp(size_.width / 2, 50));
	  this->addChild(player_, 1);
	 }
	}

	//ⓐ화면을 넘어가 사라져버리거나 충돌했던  적 비행기 지워주기 

	for (auto eraseAllEnemy : eraseEnemy)// eraseAllEnemy 새로만들기
	{
		removeChild(eraseAllEnemy, false);
		enemy_.eraseObject(eraseAllEnemy);
	}

	//ⓐ화면을 넘어가 사라져버리거나 충돌했던  미사일 지워주기 

	for (auto eraseAllMissile : eraseMissile) //eraseAllMissile새로만들기
	{
		removeChild(eraseAllMissile, false);

		for (int a = 0; a < MAX_MISSILE; ++a)
		{
			missile_[a].eraseObject(eraseAllMissile);
		}
	}

	for (auto eraseAllLife : eraseLife) //eraseAllLife새로만들기
	{
		removeChild(eraseAllLife, false);

		for (int a = 0; a < 30; ++a)
		{
			life_[a].eraseObject(eraseAllLife);
		}
	}
	eraseLife.clear();
	eraseMissile.clear();//미사일에 들어간 벡터 메모리 삭제
	eraseEnemy.clear();// eraseEnemy에 들어간 것들을 삭제		
	//log("%d", enemy_.size()); //현재 배열의 크기 = 적 비행기 갯수`
}

//////////////////////////////////업데이트 끝///////////////////////////////////////////////////////

void HelloWorld::PutCrashEffect(const cocos2d::Vec2 &pos)
{
	//맨첫번째 스프라이트가 로딩되어 베이스가 된다
	auto expBase = Sprite::create("exps/exp_01.png");

	expBase->setPosition(pos);

	//애니메이션 스프라이트 5프레임을 차례로 등록해줌
	auto animation = Animation::create();

	animation->addSpriteFrameWithFileName("exps/exp_01.png");
	animation->addSpriteFrameWithFileName("exps/exp_02.png");
	animation->addSpriteFrameWithFileName("exps/exp_03.png");
	animation->addSpriteFrameWithFileName("exps/exp_04.png");
	animation->addSpriteFrameWithFileName("exps/exp_05.png");

	//0.1초 간격으로 애니메이션 처리 설정
	animation->setDelayPerUnit(0.1f);

	//애니메이션이 끝나고 시작 프레임(1번)으로 돌아간다
	animation->setRestoreOriginalFrame(true);

	//에니메이션 생성
	auto animate = Animate::create(animation);

	//애니메이션이 끝나면 removeChild를 자동으로 호출하여 메모리 삭제.. 이것도 일종의 액션이다
	auto removeAction = CallFunc::create(expBase, CC_CALLFUNC_SELECTOR(Node::removeFromParent));

	//애니메이션과 리무브액션 2개의 액션을 하나의 시퀀스로 등록후 runAction 실행
	auto seq = Sequence::create(animate, removeAction, NULL);

	this->addChild(expBase);//씬에 이펙트(베이스 스프라이트)를 추가
	expBase->runAction(seq);//베이스 스프라이트는 애니메이션 시퀀스를 실행
}

void HelloWorld::RegenEnemy(float time)
{
	int EnemyNum = (rand() % 5) + 1; // EnemyNum 은 1~5까지 랜덤으로 할당됨

									 /*log("%d", RANDOM_INT(1, 5)); RANDOM_INT(1,5) 도 1~5까지 랜덤으로 할당*/

									 /*log("%d", EnemyNum);*/
	for (int a = 0; a < EnemyNum; ++a)
	{
		Size size = this->getContentSize();

		//아래 함수 AddEnemy 있음  - 위치 변수 값을 넣어줬음
		AddEnemy(Vec2(RANDOM_INT(0, (int)size.width), 480));
	}
}

void HelloWorld::AddEnemy(const Vec2& position)
{
	Size size = this->getContentSize();

	//적비행기 하나를 Scene에 추가
	auto enemy = Sprite::create("Images/enemy_01.png");
	enemy->setPosition(position);
	this->addChild(enemy, 2);

	//화면 아래로 내려가는 액션 등록
	auto move = MoveBy::create(3.f, Vec2(0, -size.height));
	enemy->runAction(move);

	//에너미 배열에 방금 만든거를 넣는다 - 메모리상에 에너미가 생김
	enemy_.pushBack(enemy);
}

void HelloWorld::Shooting(float time) //미사일 쏘기
{
	if (!player_) 
		return;

	for (int a = 0; a < MAX_MISSILE; ++a)
	{
		auto missile = Sprite::create("Images/missile.png");

		if (a == 0) 
		{ 
			missile->setPosition(Vec2(player_->getPosition().x - 16.f, player_->getPosition().y));
		}

		else if (a == 1)
		{
			missile->setPosition(Vec2(player_->getPosition().x + 16.f, player_->getPosition().y));
		}

		this->addChild(missile);

		Size size = this->getContentSize();
		auto move = MoveBy::create(0.75f, Vec2(0, size.height)); 
		missile->runAction(move);
		missile_[a].pushBack(missile);
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/fireEffect.mp3", true);
}

void HelloWorld::onEnter()
{
	Layer::onEnter();

	//싱글 터치  모드로 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

}

void HelloWorld::onExit()
{

	//이거 지우면 다음화면에서 터치가 안된다. 

	/*_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);*/

	Layer::onExit();

	log("HelloWorld::onExit");

}

bool HelloWorld::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if (!player_)

	 return true;

	auto touchPoint = Touch->getLocationInView();
	auto touchGlPoint = Director::getInstance()->convertToGL(touchPoint);

	auto playerPos = player_->getPosition();
	distance_ = Vec2(playerPos.x - touchGlPoint.x, playerPos.y - touchGlPoint.y);

	return true;
}

//떼지않고 움직일때 호출
void HelloWorld::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	if (!player_)
		return;
	//포인트 위치 알아내는거 같음...
	Vec2 touchPoint = pTouch->getLocationInView();
	Vec2 touchGlPoint = Director::getInstance()->convertToGL(touchPoint);

	/*log("touchPoint : %d", touchPoint); 포인트 위치값 알아보려고 쓴거

	log("GlPoint : %d", touchGlPoint);*/

	Vec2 pos = Vec2(touchGlPoint.x + distance_.x, touchGlPoint.y + distance_.y);

	/*Vec2 pos = Vec2(touchGlPoint.x + distance_.x, player_->getPosition().y);*/

	if (pos.x < 0.f)
	{
		pos.x = 0.f;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);

		/*distance_ = Vec2(pos.x - touchGlPoint.x, pos.y);*/
	}

	else if (pos.x > size_.width)
	{
		pos.x = size_.width;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
	
		/*distance_ = Vec2(pos.x - touchGlPoint.x, pos.y);*/
	}

	if (pos.y < 0.f)
	{

		pos.y = 0.f;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);

		/*distance_ = Vec2(pos.x - touchGlPoint.x, pos.y);*/
	}
	else if (pos.y > size_.height)
	{
		pos.y = size_.height;
		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);

		/*distance_ = Vec2(pos.x - touchGlPoint.x, pos.y);*/
	}

	player_->setPosition(pos);
}

//손가락을 화면에서 떼었을때 호출 -여기서는 딱히 필요없음 

void HelloWorld::onTouchEnded(Touch* pTouch, Event* pEvent)
{

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void  HelloWorld::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	log("HelloWorld::onEnterTransitionDidFinish");
}

void HelloWorld::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
	log("HelloWorld::onExitTransitionDisStart");
}

HelloWorld::~HelloWorld()
{
	log("HelloWorld::dealloc");
}



//#include "SimpleAudioEngine.h"
//#include "HelloWorldScene.h"
//#include <ctime>
//#include <cstdlib>
//
//USING_NS_CC;
//
//using namespace cocos2d;
//using namespace CocosDenshion;
//
//HelloWorld::HelloWorld() :
//	regenCheckTime_(-1.f)
//{
//}
//
//Scene* HelloWorld::createScene()
//{
//	auto scene = Scene::create();
//	auto layer = HelloWorld::create();
//	scene->addChild(layer);
//	return scene;
//}
//
//bool HelloWorld::init()
//{
//	if (!Layer::init())
//	{
//		return false;
//	}
//
//	auto CloseItem = MenuItemImage::create(
//		"Images/CloseNormal.png",
//		"Images/CloseSelected.png",
//		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
//	CloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width - 20, 20));
//
//	//초기 화면의 score를 표시해주는 코드
//	auto myScore = LabelTTF::create(itoa(score, c, 10), "Arial", 20);
//
//	//score 위치 설정             
//	myScore->setPosition(Vec2(size_.width / 2, size_.height - 20));
//	myScore->setColor(Color3B(0, 0, 200));
//
//	this->addChild(myScore, 1);
//
//	myScore->setTag(40);
//	
//
//	auto pMenu = Menu::create(CloseItem, nullptr);
//	pMenu->setPosition(Vec2::ZERO);
//	this->addChild(pMenu, 1);
//
//	size_ = Director::getInstance()->getWinSize();
//
//	auto pLabel = LabelTTF::create("1945", "Thonburi", 34);
//	pLabel->setColor(Color3B::RED);
//	pLabel->setPosition(Vec2(size_.width / 2, size_.height - 20));
//	this->addChild(pLabel, 1);
//
//	auto pSprite = Sprite::create("Images/bg.png");
//	pSprite->setPosition(Vec2(size_.width / 2, size_.height / 2));
//	this->addChild(pSprite, 0);
//
//	player_ = Sprite::create("Images/airplain_01.png");
//	player_->setPosition(Vec2(size_.width / 2, 50));
//	this->addChild(player_, 1);
//
//	setTouchEnabled(true);
//
//	//auto listener = EventListenerTouchOneByOne::create();
//	//listener->setSwallowTouches(true);
//
//	//listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//	//listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//	//listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
//
//	this->schedule(schedule_selector(HelloWorld::Update));
//	this->schedule(schedule_selector(HelloWorld::RegenEnemy), 1.f);
//	this->schedule(schedule_selector(HelloWorld::Shooting), 0.2f);
//
//	srand((int)time(NULL));
//
//	//목숨 벡터 만드는거
//	for (int a = 0; a < 3; ++a)
//	{
//		Size size_ = this->getContentSize();//화면의 사이즈 구하는거
//		auto life = Sprite::create("Images/airplain_01.png");
//
//		//그냥 위치 설정해주는거
//		life->setPosition(Vec2(size_.width - (a * 50) - 190, 420));
//		life->setScale(1.0f);
//
//		//화면에 뿌리고
//		this->addChild(life);
//		life_[a].pushBack(life);//목숨 벡터에 넣어줌
//	}
//	return true;
//}
//
//void HelloWorld::RegenEnemy(float time)
//{
//	int EnemyNum = (rand() % 5) + 1; // EnemyNum 은 1~5까지 랜덤으로 할당됨
//
//	/*log("%d", RANDOM_INT(1, 5)); RANDOM_INT(1,5) 도 1~5까지 랜덤으로 할당*/
//	/*log("%d", EnemyNum);*/
//
//	for (int a = 0; a < EnemyNum; ++a)
//	{
//		Size size = this->getContentSize();
//
//		//아래 함수 AddEnemy 있음  - 위치 변수 값을 넣어줬음
//		AddEnemy(Vec2(RANDOM_INT(0, (int)size.width), 480));
//	}
//}
//
//void HelloWorld::AddEnemy(const Vec2& position)
//{
//	Size size = this->getContentSize();
//
//	//적비행기 하나를 Scene에 추가
//	auto enemy = Sprite::create("Images/enemy_01.png");
//	enemy->setPosition(position);
//	this->addChild(enemy, 2);
//
//	//화면 아래로 내려가는 액션 등록
//	auto move = MoveBy::create(3.f, Vec2(0, -size.height));
//	enemy->runAction(move);
//
//	//배열에 방금 생성된 적비행기 스프라이트 포인터를 배열 요소로 추가
//
//	enemy_.pushBack(enemy);
//}
//
//void HelloWorld::Update(float time)
//{
//	// 적, 미사일, 목숨을 지울 백터를 만든다.
//	Vector<Sprite*> eraseEnemy;
//	Vector<Sprite*> eraseMissile;
//	Vector<Sprite*> eraseLife;
//	
//	for (auto enemy : enemy_)
//	{
//		// 적 비행기가 화면 아래로 사라졋을 경우 메모리에서 삭제합니다
//		if (enemy->getPosition().y <= 0)
//		{
//			// 적을 지울 백터에 넣는다.
//			eraseEnemy.pushBack(enemy);
//		}
//
//		// 모든 미사일 백터를 반복하여 충돌검사
//		for (int a = 0; a < MAX_MISSILE; a++)
//		{
//			for (auto missile : missile_[a])
//			{
//				//미사일이 화면 위쪽으로 사라졌을 때
//				if (missile->getPosition().y >= size_.height)
//				{
//					// 미사일 삭제
//					eraseMissile.pushBack(missile);
//				}
//
//				if (missile->boundingBox().intersectsRect(enemy->boundingBox()))   // 충돌처리
//				{
//					score++;
//					auto myScorel = (Sprite*)getChildByTag(40);
//
//					//스코어 화면에서 지워주고
//					this->removeChild(myScorel);
//
//					//스코어 재생성
//					auto myScoreRe = LabelTTF::create(itoa(score, c, 10), "Arial", 20);
//
//					//score 위치 설정             
//					myScoreRe->setPosition(Vec2(size_.width / 2, size_.height - 50));
//					myScoreRe->setColor(Color3B(0, 0, 200));
//					this->addChild(myScoreRe, 1);
//					myScoreRe->setTag(40);
//
//					//미사일과 적 비행기가 부딪혔을 때
//					PutCrashEffect(enemy->getPosition());
//
//					//적 비행기 삭제
//					eraseEnemy.pushBack(enemy);
//
//					//미사일 삭제
//					eraseMissile.pushBack(missile);
//				}
//			}
//		}
//		if (player_)
//		{
//			//intersectsRect함수 리턴값이 충돌하면 true를 리턴
//			if (player_->boundingBox().intersectsRect(enemy->boundingBox()))
//			{
//				//충돌했을때 처리
//
//				//1.폭파 이펙트 출력
//				PutCrashEffect(player_->getPosition());
//				PutCrashEffect(enemy->getPosition());
//
//				//2.적 비행기를 화면에서 없애준다
//				//Scene에서 적비행기 스프라이트를 Remove한다
//				eraseEnemy.pushBack(enemy);
//
//				// 라이프 벡터로된 오브젝트를 만든다.
//				auto lifeObj = life_[lifeNum];
//				eraseLife.pushBack(lifeObj);
//				log("life: %d", lifeNum);
//
//				lifeNum = lifeNum + 1;
//				auto pScene = HelloWorld::create(AddEnemy);
//				Director::getInstance()->replaceScene(pScene);
//
//				regenCheckTime_ = 0.f;
//			}
//		}
//
//	}
//	//화면을 넘어가 사라져버리거나 충돌했던  적 비행기 지워주기 
//	for (auto eraseAllEnemy : eraseEnemy)// eraseAllEnemy 새로만들기
//	{
//		removeChild(eraseAllEnemy, false);
//		enemy_.eraseObject(eraseAllEnemy);
//	}
//
//	//화면을 넘어가 사라져버리거나 충돌했던  미사일 지워주기 
//	for (auto eraseAllMissile : eraseMissile) //eraseAllMissile새로만들기
//	{
//		removeChild(eraseAllMissile, false);
//
//		for (int a = 0; a < MAX_MISSILE; ++a)
//		{
//			missile_[a].eraseObject(eraseAllMissile);
//		}
//	}
//
//	for (auto eraseAllLife : eraseLife) //eraseAllLife새로만들기
//	{
//		removeChild(eraseAllLife, false);
//
//		for (int a = 0; a < 30; ++a)
//		{
//			life_[a].eraseObject(eraseAllLife);
//		}
//	}
//	eraseLife.clear();
//	eraseMissile.clear();//미사일에 들어간 벡터 메모리 삭제
//	eraseEnemy.clear();// eraseEnemy에 들어간 것들을 삭제
//	
//
//	//유저 비행기가 죽었을때만 체크하여 부활처리
//	if (!player_ && regenCheckTime_ != -1.f)
//	{
//		//시간을 누적시켜서 3초 이상 경과되었을 때만 부활시킨다
//		regenCheckTime_ = regenCheckTime_ + time;
//
//		if (regenCheckTime_ >= 3.f)
//		{
//			//유저 비행기 부활처리
//			player_ = Sprite::create("Images/airplain_01.png");
//			player_->setPosition(Vec2(size_.width / 2, 50));
//			this->addChild(player_, 1);
//		}
//	}
//
//
//
//
//   log("%d", enemy_.size()); //현재 배열의 크기 = 적 비행기 갯수
//
//}
//
//void HelloWorld::Shooting(float time)
//{
//	if (!player_)
//		return;
//
//	for (int a = 0; a < MAX_MISSILE; ++a)
//	{
//		auto missile = Sprite::create("Images/missile.png");
//
//		if (a == 0)
//		{
//			missile->setPosition(Vec2(player_->getPosition().x - 16.f, player_->getPosition().y));
//		}
//		else if (a == 1)
//		{
//			missile->setPosition(Vec2(player_->getPosition().x + 16.f, player_->getPosition().y));
//		}
//
//		this->addChild(missile);
//
//		Size size = this->getContentSize();
//		auto move = MoveBy::create(0.75f, Vec2(0, size.height));
//		missile->runAction(move);
//
//		missile_[a].pushBack(missile);
//	}
//}
////
////void HelloWorld::RegenEnemy(float time)
////{
////	for (int a = 0; a < RANDOM_INT(1, 5); ++a) {
////		Size size = Director::sharedDirector()->getWinSize();
////		AddEnemy(ccp(RANDOM_INT(0, (int)size.width), size.height));
////	}
////}
//
//void HelloWorld::PutCrashEffect(const cocos2d::Vec2 &pos)
//{
//	//맨첫번째 스프라이트가 로딩되어 베이스가 된다
//	auto expBase = Sprite::create("exps/exp_01.png");
//	expBase->setPosition(pos);
//
//	//애니메이션 스프라이트 5프레임을 차례로 등록해줌
//	auto animation = Animation::create();
//	animation->addSpriteFrameWithFileName("exps/exp_01.png");
//	animation->addSpriteFrameWithFileName("exps/exp_02.png");
//	animation->addSpriteFrameWithFileName("exps/exp_03.png");
//	animation->addSpriteFrameWithFileName("exps/exp_04.png");
//	animation->addSpriteFrameWithFileName("exps/exp_05.png");
//
//	//0.1초 간격으로 애니메이션 처리 설정
//	animation->setDelayPerUnit(0.1f);
//
//	//애니메이션이 끝나고 시작 프레임(1번)으로 돌아간다
//	animation->setRestoreOriginalFrame(true);
//
//	//에니메이션 생성
//	auto animate = Animate::create(animation);
//
//	//애니메이션이 끝나면 removeChild를 자동으로 호출하여 메모리 삭제.. 이것도 일종의 액션이다
//	auto removeAction = CallFunc::create(expBase, CC_CALLFUNC_SELECTOR(Node::removeFromParent));
//
//	//애니메이션과 리무브액션 2개의 액션을 하나의 시퀀스로 등록후 runAction 실행
//	auto seq = Sequence::create(animate, removeAction, NULL);
//
//	this->addChild(expBase);//씬에 이펙트(베이스 스프라이트)를 추가
//	expBase->runAction(seq);//베이스 스프라이트는 애니메이션 시퀀스를 실행한다
//}
//
//void HelloWorld::onEnter()
//{
//	Layer::onEnter();
//
//	//싱글 터치  모드로 등록
//
//	auto listener = EventListenerTouchOneByOne::create();
//
//	listener->setSwallowTouches(true);
//
//	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
//
//	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//}
//
//void HelloWorld::onExit()
//{
//	//이거 지우면 다음화면에서 터치가 안된다. 
//
//	///*_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);*/
//	//Layer::onExit();
//
//	log("HelloWorld::onExit");
//
//}
//
//bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
//{
//	if (!player_)
//		return true;
//
//	Vec2 touchPoint = touch->getLocation();
//	Vec2 touchGlPoint = Director::getInstance()->convertToGL(touchPoint);
//
//	Vec2 playerPos = player_->getPosition();
//	distance_ = Vec2(playerPos.x - touchGlPoint.x, playerPos.y - touchGlPoint.y);
//
//	return true;
//}
//
//void HelloWorld::onTouchMoved(Touch *touch, Event *event)
//{
//	if (!player_)
//		return;
//
//	Vec2 touchPoint = touch->getLocationInView();
//	Vec2 touchGlPoint = Director::getInstance()->convertToGL(touchPoint);
//
//	Vec2 pos = Vec2(touchGlPoint.x + distance_.x, touchGlPoint.y + distance_.y);
//
//	if (pos.x < 0.f)
//	{
//		pos.x = 0.f;
//		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
//	}
//	else if (pos.x > size_.width)
//	{
//		pos.x = size_.width;
//		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
//	}
//
//	if (pos.y < 0.f)
//	{
//		pos.y = 0.f;
//		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
//	}
//	else if (pos.y > size_.height)
//	{
//		pos.y = size_.height;
//		distance_ = Vec2(pos.x - touchGlPoint.x, pos.y - touchGlPoint.y);
//	}
//
//	player_->setPosition(pos);
//}
//
//void HelloWorld::onTouchEnded(Touch *touch, Event *event)
//{
//
//}
//
//void HelloWorld::menuCloseCallback(Ref* pSender)
//{
//	Director::getInstance()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//}
//


