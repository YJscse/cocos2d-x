#ifndef __SpriteExtendEx__Monster__
#define __SpriteExtendEx__Monster__


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)             // 한글나오게 하는 헤더
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"

class Monster : public cocos2d::Sprite
{
public:
	Monster();

	void setPriority(int fixedPriority);
	void setPriorityWithThis(bool useNodePriority);

	virtual void onEnter();
	virtual void onExit();

private:
	cocos2d::EventListener* _listener;
	int _fixedPriority;
	bool _useNodePriority;
};

#endif // defined(__SpriteExtendEx__Monster__)

