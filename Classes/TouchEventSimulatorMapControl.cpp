//
//  TouchEventSimulatorMapControl.cpp
//  DropshipProject
//
//  Created by Son on 2016. 1. 6..
//
//

#include "TouchEventSimulatorMapControl.h"
#include "MapScrollView.h"

USING_NS_CC;

TouchEventSimulatorMapControl::TouchEventSimulatorMapControl(MapScrollView* mapsv)
: TouchEventProcess(),
_mapScrollView(mapsv),
_moveToCallback(nullptr)
{
	
}

TouchEventSimulatorMapControl::~TouchEventSimulatorMapControl()
{
	
}

void	TouchEventSimulatorMapControl::onEnter()
{
	
}

void	TouchEventSimulatorMapControl::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_mapScrollView->onTouchesMoved(touches, event);
}

void	TouchEventSimulatorMapControl::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	_mapScrollView->onTouchesBegan(touches, event);
}

void	TouchEventSimulatorMapControl::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size() == 1)
	{
		auto	&item = touches[0];
		auto	location = item->getLocation();
		auto	preLocation = item->getPreviousLocation();

		Vec2	worldPos;
		_mapScrollView->convertScreenPosToWorldPos(location, &worldPos);

		if (_moveToCallback != nullptr)
		{
			_moveToCallback(worldPos);
		}
	}
	_mapScrollView->onTouchesEnded(touches, event);


}

void	TouchEventSimulatorMapControl::onMouseScroll(cocos2d::Event *event)
{
	_mapScrollView->onMouseScroll(event);
}

