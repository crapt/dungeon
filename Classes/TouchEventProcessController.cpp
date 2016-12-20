#include	"TouchEventProcessController.h"
#include	"TouchEventProcess.h"


USING_NS_CC;

TouchEventProcessController::TouchEventProcessController()
{

}

TouchEventProcessController::~TouchEventProcessController()
{

}

void	TouchEventProcessController::addTouchEvent(TouchEventProcess* event)
{
	_eventProcessors.push_back(event);
	_currentProcess = _eventProcessors[0];
}

void	TouchEventProcessController::init()
{
	_currentProcess->onEnter();
}

void	TouchEventProcessController::changeMode(int mode)
{
	_currentProcess->onExit();
	CCASSERT(mode < _eventProcessors.size(), "can not change mode TouchEventProcessController");

	_currentProcess = _eventProcessors[mode];
	_currentProcess->onEnter();
}

TouchEventProcess*	TouchEventProcessController::touchEveneProcess(int mode)
{
	return _eventProcessors[mode];
}

void	TouchEventProcessController::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_currentProcess->onTouchesMoved(touches, event);

}

void	TouchEventProcessController::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_currentProcess->onTouchesBegan(touches, event);
}

void	TouchEventProcessController::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_currentProcess->onTouchesEnded(touches, event);
}

void	TouchEventProcessController::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_currentProcess->onTouchesCancelled(touches, event);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void	TouchEventProcessController::onMouseMove(cocos2d::Event *event)
{
	//EventMouse* e = (EventMouse*)event;
	//auto	mousePos = Vec2(e->getCursorX(), e->getCursorY());
	//CCLOG("onMouseMove %f,%f", mousePos.x, mousePos.y);
	_currentProcess->onMouseMove(event);
}

void	TouchEventProcessController::onMouseDown(cocos2d::Event *event)
{
	_currentProcess->onMouseDown(event);
}

void	TouchEventProcessController::onMouseUp(cocos2d::Event *event)
{
	_currentProcess->onMouseUp(event);
}

void	TouchEventProcessController::onMouseScroll(cocos2d::Event *event)
{
	_currentProcess->onMouseScroll(event);
}
#endif