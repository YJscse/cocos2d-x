#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

/*

cocos-ext.h 인클루드를 위해 참조 디렉터리 추가 :

[프로젝트 - 우클릭 - 속성 - 구성 속성 - C/C++ - 일반 - 추가 포함 디렉터리]에

$(EngineRoot) 추가
*/

#include "cocos2d.h"
#include "cocos-ext.h"

class HelloWorld 
	: public cocos2d::LayerColor
	, public cocos2d::ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
        
    CREATE_FUNC(HelloWorld);

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);

	cocos2d::ui::EditBox* m_pEditName;
	cocos2d::ui::EditBox* m_pEditPassword;
	cocos2d::ui::EditBox* m_pEditEmail;
};

#endif // __HELLOWORLD_SCENE_H__
