#pragma once
#ifndef __GAMEMAIN__
#define __GAMEMAIN__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // �ѱ۳����� �ϴ� ���
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "extensions/cocos-ext.h" 
// ������Ʈ -> NULL �Ӽ� -> C/C++ -> �Ϲ� -> �߰� ���� ���͸�-> $(EngineRoot) �߰� �Ͻÿ�

using namespace cocos2d;
using namespace cocos2d::extension;

class GameMain 
	: public cocos2d::Layer
	, public cocos2d::ui::EditBoxDelegate
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GameMain);

	Size winSize;

	Sprite* title;
	Sprite* play;
	Sprite* score;
	Sprite* background;
	Sprite* home;
	Sprite* effect;
	Sprite* bgm;
	Sprite* stageBord;
	Sprite* stage[9];

	bool sselect = false;
	bool effectBool = true;
	bool bgmBool = true;
	unsigned int m_nSoundId;

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	//void SceneTrans(float dt);
	void createBackground();
	void createTitle();
	void createPlay();
	void createScore();
	void selectStage();

	// ��������
	cocos2d::ui::EditBox* _editNum;
	std::string txtNum;

	void doSendScore(Ref* pSender);
	void doSendTime(Ref* pSender);

	void doSendOne(Ref* pSender);
	void doSendMulti(Ref* pSender);

	void doShowLeaderBoard(Ref* pSender);
	void doShowAchivement(Ref* pSender);

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	void editBoxReturn(cocos2d::ui::EditBox* editBox);

	// ����
	float _soundVolume;

	int audioId1;
	int audioId2;


};

#endif  __GAMEMAIN__ //  __GAMEMAIN__