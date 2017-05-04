#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "AstarItem.h"

USING_NS_CC;

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	Size         winSize;
	TMXTiledMap* tmap;
	TMXLayer*    metainfo;
	Sprite*      tamara;

	void setViewpointCenter(Vec2 position);
	Vec2 tileCoordForPosition(Vec2 position);
	Vec2 positionForTileCoord(Vec2 position);

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

	// Astar Start ------------------------------------------

	Vec2                pCoordS;
	Vec2                pCoordE;

	std::vector<AstarItem*>* openPath;
	std::vector<AstarItem*>* closePath;
	std::vector<Vec2>*       myPath;

	void AStarPathFind();
	void isRoad(Vec2 mPos, Vec2 pPos, int cost1, int cost2);
	int getHCost(Vec2 pos);
	void makePass();
	void movePass();
	void moveCamera();

	// Astar End ----------------------------------------------
};

#endif // __HELLOWORLD_SCENE_H__
