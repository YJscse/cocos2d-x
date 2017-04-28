#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "cocos-ext.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__


/*

cocos-ext.h 인클루드를 위해 참조 디렉터리 추가 :

[프로젝트 - 우클릭 - 속성 - 구성 속성 - C/C++ - 일반 - 추가 포함 디렉터리]에

$(EngineRoot) 추가
*/
