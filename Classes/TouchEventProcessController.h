#pragma	once

#include	"cocos2d.h"

class TouchEventProcess;
class LayerGameMain;
class LayerGameMenu;
class SceneGame;

class TouchEventProcessController
{
public:
	TouchEventProcessController();
	~TouchEventProcessController();

	void	init();
	void	addTouchEvent(TouchEventProcess* event);
	void	changeMode(int mode);
	TouchEventProcess*	touchEveneProcess(int mode);
public:
	void	onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
public:
	void	onMouseMove(cocos2d::Event *event);
	void	onMouseDown(cocos2d::Event *event);
	void	onMouseUp(cocos2d::Event *event);
	void	onMouseScroll(cocos2d::Event *event);
#endif

private:
	std::vector<TouchEventProcess*>	_eventProcessors;
	TouchEventProcess*	_currentProcess;
};