#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CustomTableViewCell.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class HelloWorld
	: public cocos2d::LayerColor
	, public cocos2d::extension::TableViewDataSource
	, public cocos2d::extension::TableViewDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	cocos2d::EventListenerTouchAllAtOnce* listener;
	cocos2d::Size winSize;
	cocos2d::RenderTexture* m_pTarget;
	cocos2d::Vector<Sprite*> m_pBrush;
	cocos2d::Color3B mycolor;
	cocos2d::Rect checkRect;

	~HelloWorld();
	virtual void onEnter();
	virtual void onExit();
	void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
	//void saveImage(Ref* sender);
	void clearImage(Ref* sender);

	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
	virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView* table);

	
};

#endif // __HELLOWORLD_SCENE_H__
