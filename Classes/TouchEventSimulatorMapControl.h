#pragma	once

#include	"TouchEventProcess.h"

class MapScrollView;

class TouchEventSimulatorMapControl : public TouchEventProcess
{
public:
	TouchEventSimulatorMapControl(MapScrollView* mapsv);
	~TouchEventSimulatorMapControl();
public:

	virtual	void	onEnter();
	virtual	void	onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual	void	onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual	void	onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void	onMouseScroll(cocos2d::Event *event);

	void	mapScrollView(MapScrollView* msv)	{ _mapScrollView = msv; }

public:

	void	setMoveToCallback(std::function<void(const cocos2d::Vec2&)> moveToCallback)	{ _moveToCallback = moveToCallback; }

private:

	MapScrollView*	_mapScrollView;
	std::function<void(const cocos2d::Vec2&)> _moveToCallback;
	
};