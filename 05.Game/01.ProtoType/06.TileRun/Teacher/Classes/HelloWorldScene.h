#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  
    
    CREATE_FUNC(HelloWorld);


    cocos2d::Sprite* m_tamara;
    cocos2d::TMXTiledMap* map;
    cocos2d::TMXLayer* background;
    void checkPosition(float dt);
    cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
};

#endif // __HELLOWORLD_SCENE_H__
