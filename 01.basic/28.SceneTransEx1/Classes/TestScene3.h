#ifndef __SceneTrans__TestScene3__
#define __SceneTrans__TestScene3__

#include "cocos2d.h"

class TestScene3 : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(TestScene3);

	void doClose(Ref* pSender);
};

#endif // defined(__SceneTrans__TestScene3__)

