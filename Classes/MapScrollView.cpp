#include	"MapScrollView.h"

USING_NS_CC;

MapScaleTo* MapScaleTo::create(float duration, float s)
{
	MapScaleTo *scaleTo = new (std::nothrow) MapScaleTo();
	scaleTo->initWithDuration(duration, s);
	scaleTo->autorelease();

	return scaleTo;
}

void	MapScaleTo::update(float time)
{
	if (_target)
	{
		_target->setScale(_startScaleX + _deltaX * time);
	}
}

static const float AUTOSCROLLMAXSPEED = 1000.0f;

MapScrollView::MapScrollView() :
_autoScrollDir(Vec2::ZERO),
_topBoundary(0.0f),
_bottomBoundary(0.0f),
_leftBoundary(0.0f),
_rightBoundary(0.0f),
_autoScroll(false),
_autoScrollAddUpTime(0.0f),
_autoScrollOriginalSpeed(0.0f),
_autoScrollAcceleration(-1000.0f),
_isAutoScrollSpeedAttenuated(false),
_needCheckAutoScrollDestination(false),
_autoScrollDestination(Vec2::ZERO),
_bePressed(false),
_slidTime(0.0f),
_moveChildPoint(Vec2::ZERO),
_inertiaScrollEnabled(true)
{

}

MapScrollView::~MapScrollView()
{

}

MapScrollView* MapScrollView::create(const cocos2d::Size& mapDotSize, float max_Scale)
{
	MapScrollView *pRet = new(std::nothrow) MapScrollView();
	if (pRet && pRet->init(mapDotSize, max_Scale))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool	MapScrollView::init(const Size& mapDotSize, float max_Scale)
{
	if (Node::init())
	{
		auto size = Director::getInstance()->getVisibleSize();
		
		maxScale(max_Scale);
		
		auto	scaleWidth = size.width / mapDotSize.width;
		
		minScaleX(scaleWidth);
		minScaleY(scaleWidth);
		setContentSize(size);
		setMapSize(mapDotSize);
		setAnchorPoint(Vec2::ZERO);
		scheduleUpdate();
		
		return true;
	}
	return false;
}

void	MapScrollView::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	//	CCLOG("onTouchesBegan (%d)-", touches.size());
	if (touches.size() == 1)
	{
		auto	&item = touches[0];
		auto	location = item->getLocation();
		auto	preLocation = item->getPreviousLocation();
		handlePressLogic(location, preLocation);
	}
}
void	MapScrollView::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	//	CCLOG("onTouchesEnded (%d)-", touches.size());
	if (touches.size() == 1)
	{
		auto	&item = touches[0];
		auto	location = item->getLocation();
		auto	preLocation = item->getPreviousLocation();
		
		Vec2	worldPos;
		
		handleReleaseLogic(location, preLocation);
		//		convertScreenPosToWorldPos(location, &worldPos);
		//
		//		if (_moveToCallback != nullptr)
		//		{
		//			_moveToCallback(worldPos);
		//		}
	}
}

void	MapScrollView::setCenterPos(cocos2d::Vec2 pos)
{
	

	float scale = getScale();

	float	maxDotWidth;
	float	maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;
	if (scale > _minScaleX)
	{
		maxDotWidth = _contentSize.width - (_mapSize.width*scale);
		maxDotHeight = (_contentSize.height - (_mapSize.height*scale));
	}
	else
	{
		maxDotWidth = (_contentSize.width - (_mapSize.width*scale)) / 2;
	}

	if (maxDotHeight > 0.0f)
		maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;

	Vec2	newPos = pos;

	if (newPos.x > 0.0f)
	{
		newPos.x = 0;
		
	}
	if (newPos.y > 0.0f)
	{
		newPos.y = 0;
		
	}
	if (newPos.x < maxDotWidth)
	{
		newPos.x = maxDotWidth;
		
	}
	if (newPos.y < maxDotHeight)
	{
		newPos.y = maxDotHeight;
		
	}
	setPosition(newPos);

}
void	MapScrollView::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	//	CCLOG("onTouchesMoved (%d)-", touches.size());
	float	scale = getScale();
	Vec2	location, preLocation, locationCenter;
	if (touches.size() == 1)
	{
		auto	&item = touches[0];
		location = item->getLocation();
		preLocation = item->getPreviousLocation();

		CCLOG("x :%f y : %f", location.x, location.y);
	}
	else
	{
		auto	location1 = touches[0]->getLocation();
		auto	location2 = touches[1]->getLocation();
		locationCenter = (location1 + location2) / 2;
		auto	distance = (location1 - location2).length();
		auto	prelocation1 = touches[0]->getPreviousLocation();
		auto	prelocation2 = touches[1]->getPreviousLocation();
		auto	prelocationCenter = (prelocation1 + prelocation2) / 2;
		
		location = locationCenter;
		preLocation = prelocationCenter;
		
		auto	preDistance = (prelocation1 - prelocation2).length();
		auto	leftDown = (getPosition() - locationCenter);
		auto	newscale = (distance / preDistance) * scale;
		auto	newleftDown = leftDown * newscale / scale;
		scale = newscale;
	}
	
	Vec2	worldCenter;
	convertScreenPosToWorldPos(locationCenter, &worldCenter);
	setScale(scale, worldCenter);
	auto	dir = location - preLocation;
	preLocation = location - (dir * scale);
	handleMoveLogic(location, preLocation);
}

void	MapScrollView::onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	
}


void	MapScrollView::onMouseScroll(cocos2d::Event *event)
{
	EventMouse* e = (EventMouse*)event;

	switch (e->getMouseButton())
	{
	case	-1:	//move
	{
					auto	scale = getScale();
					auto	mousePos = Vec2(e->getCursorX(), e->getCursorY());
					//CCLOG("Mouse Scroll %f,%f", mousePos.x, mousePos.y);
					float y = Director::getInstance()->getVisibleSize().height;
					mousePos.y += Director::getInstance()->getVisibleSize().height;
					Vec2	worldCenter;
					convertScreenPosToWorldPos(mousePos, &worldCenter);

					scale += (e->getScrollY() * 0.1f);
					if (scale < minScaleX())
						scale = minScaleX();
					if (scale > maxScale())
						scale = maxScale();
					setScale(scale, worldCenter);
	}
		break;
	default:
		break;
	}
}



void	MapScrollView::convertScreenPosToWorldPos(const cocos2d::Vec2& screenPos, cocos2d::Vec2* worldPos)
{
	*worldPos = (screenPos - getPosition()) / getScale();
}

void	MapScrollView::convertWorldPosToScreenPos(const cocos2d::Vec2& worldPos, cocos2d::Vec2* screenPos)
{
	*screenPos = (worldPos * getScale()) + getPosition();
}

void	MapScrollView::setMapSize(const cocos2d::Size &size)
{
	_mapSize = size;
}

void	MapScrollView::handlePressLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition)
{
	startRecordSlidAction();
	_bePressed = true;
	_touchBeganPosition = position;
}

void	MapScrollView::handleMoveLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition)
{
	_touchMovePosition = position;

	Vec2 touchPositionInNodeSpace = this->convertToNodeSpace(position);
	Vec2 previousTouchPositionInNodeSpace = this->convertToNodeSpace(prePosition);
	Vec2 delta = touchPositionInNodeSpace - previousTouchPositionInNodeSpace;

	scrollChildren(delta.x, delta.y);
}

void	MapScrollView::handleReleaseLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition)
{
	_touchEndPosition = position;
	endRecordSlidAction();
	_bePressed = false;
}

void	MapScrollView::startRecordSlidAction()
{
	if (_autoScroll)
	{
		stopAutoScrollChildren();
	}
	_slidTime = 0.0f;
}

void	MapScrollView::stopAutoScrollChildren()
{
	_autoScroll = false;
	_autoScrollOriginalSpeed = 0.0f;
	_autoScrollAddUpTime = 0.0f;
}

bool	MapScrollView::scrollChildren(float touchOffsetX, float touchOffsetY)
{
	bool scrollenabled = true;
	//scrollingEvent();

	//scrollenabled = scrollChildrenBoth(touchOffsetX, touchOffsetY);

	float scale = getScale();

	_maxDotWidth;
	_maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;
	if (scale > _minScaleX)
	{
		_maxDotWidth = _contentSize.width - (_mapSize.width*scale);
		_maxDotHeight = (_contentSize.height - (_mapSize.height*scale));
	}
	else
	{
		_maxDotWidth = (_contentSize.width - (_mapSize.width*scale)) / 2;
	}

	if(_maxDotHeight >0.0f)
		_maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;

	Vec2	newPos = getPosition() + Vec2(touchOffsetX, touchOffsetY);

	if (newPos.x > 0.0f)
	{
		newPos.x = 0;
		scrollenabled = false;
	}

 	if (newPos.y > 0.0f)
 	{
 		newPos.y = 0;
 		scrollenabled = false;
 	}

	if (newPos.x < _maxDotWidth)
	{
		newPos.x = _maxDotWidth;
		scrollenabled = false;
	}
	if (newPos.y < _maxDotHeight)
	{
		newPos.y = _maxDotHeight;
		scrollenabled = true;
	}


	//CCLOG("setpositon x : %f   y : %f", newPos.x, newPos.y);
	setPosition(newPos);
	return scrollenabled;
}

void	MapScrollView::endRecordSlidAction()
{
	if (_inertiaScrollEnabled)
	{
		if (_slidTime <= 0.016f)
		{
			return;
		}
		float totalDis = 0.0f;
		Vec2 dir;
		Vec2 touchEndPositionInNodeSpace = this->convertToNodeSpace(_touchEndPosition);
		Vec2 touchBeganPositionInNodeSpace = this->convertToNodeSpace(_touchBeganPosition);

		Vec2 subVector = touchEndPositionInNodeSpace - touchBeganPositionInNodeSpace;
		totalDis = subVector.getLength();
		dir = subVector.getNormalized();

		float orSpeed = MIN(fabs(totalDis) / (_slidTime), AUTOSCROLLMAXSPEED);
		startAutoScrollChildrenWithOriginalSpeed(dir, orSpeed, true, -1000);
		_slidTime = 0.0f;
	}
}

void MapScrollView::startAutoScrollChildrenWithOriginalSpeed(const Vec2& dir, float v, bool attenuated, float acceleration)
{
	stopAutoScrollChildren();
	_autoScrollDir = dir;
	_isAutoScrollSpeedAttenuated = attenuated;
	_autoScrollOriginalSpeed = v;
	_autoScroll = true;
	_autoScrollAcceleration = acceleration;
}


void	MapScrollView::moveChildren(float offsetX, float offsetY)
{
	_moveChildPoint = getPosition() + Vec2(offsetX, offsetY);
	setPosition(_moveChildPoint);
}

void	MapScrollView::setScale(float scale, const cocos2d::Vec2& worldCenter)
{
	Vec2	oldScreenCenter;
	convertWorldPosToScreenPos(worldCenter, &oldScreenCenter);

	Vec2	oldPosition = getPosition();
	Vec2	oldDirection = oldPosition - oldScreenCenter;

	scale = scale > _maxScale ? _maxScale : scale;
	scale = scale < _minScaleY ? _minScaleY : scale;

	float	maxDotWidth;// = (_visibleSize.width -(_tileXSize*TILE_SIZE*scale))/2;
	float	maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;
	if (scale > _minScaleX)
	{
		maxDotWidth = _contentSize.width - (_mapSize.width*scale);
		maxDotHeight = (_contentSize.height - (_mapSize.height*scale));
	}
	else
	{
		maxDotWidth = (_contentSize.width - (_mapSize.width*scale)) / 2;
	}

	if (maxDotHeight > 0.0f)
		maxDotHeight = (_contentSize.height - (_mapSize.height*scale)) / 2;


	Node::setScale(scale);

	Vec2	newScreenCenter;
	convertWorldPosToScreenPos(worldCenter, &newScreenCenter);
	Vec2	newpos = (oldScreenCenter - newScreenCenter) + oldPosition;

	newpos.x = (newpos.x > 0.0f) ? 0.0f : newpos.x;
	newpos.y = (newpos.y > 0.0f) ? 0.0f : newpos.y;
	newpos.x = (newpos.x < maxDotWidth) ? maxDotWidth : newpos.x;
	newpos.y = (newpos.y < maxDotHeight) ? maxDotHeight : newpos.y;
		
	setPosition(newpos);
		
}

void	MapScrollView::setScale(float scale)
{
	auto	displaySize = Director::getInstance()->getVisibleSize();
	auto	screenCenter = Vec2(displaySize.width / 2, displaySize.height / 2);
	Vec2	worldCenter;
	convertScreenPosToWorldPos(screenCenter, &worldCenter);
	setScale(scale, worldCenter);

	Vec2	world;
	convertScreenPosToWorldPos(Vec2(displaySize.width / 2, displaySize.height / 2), &world);

}

void	MapScrollView::update(float dt)
{
	if (_autoScroll)
	{
		autoScrollChildren(dt);
	}

	recordSlidTime(dt);
}


void	MapScrollView::recordSlidTime(float dt)
{
	if (_bePressed)
	{
		_slidTime += dt;
	}
}

void	MapScrollView::autoScrollChildren(float dt)
{
	float lastTime = _autoScrollAddUpTime;
	_autoScrollAddUpTime += dt;
	if (_isAutoScrollSpeedAttenuated)
	{
		float nowSpeed = _autoScrollOriginalSpeed + _autoScrollAcceleration * _autoScrollAddUpTime;
		if (nowSpeed <= 0.0f)
		{
			stopAutoScrollChildren();
		}
		else
		{
			float timeParam = lastTime * 2 + dt;
			float offset = (_autoScrollOriginalSpeed + _autoScrollAcceleration * timeParam * 0.5f) * dt;
			float offsetX = offset * _autoScrollDir.x;
			float offsetY = offset * _autoScrollDir.y;
			if (!scrollChildren(offsetX, offsetY))
			{
				stopAutoScrollChildren();
			}
		}
	}
	else
	{
		if (_needCheckAutoScrollDestination)
		{
			float xOffset = _autoScrollDir.x * dt * _autoScrollOriginalSpeed;
			float yOffset = _autoScrollDir.y * dt * _autoScrollOriginalSpeed;
			bool scrollCheck = scrollChildren(xOffset, yOffset);
			if (!scrollCheck)
			{
				stopAutoScrollChildren();

			}
		}
		else
		{
			if (!scrollChildren(_autoScrollDir.x * dt * _autoScrollOriginalSpeed, _autoScrollDir.y * dt * _autoScrollOriginalSpeed))
			{
				stopAutoScrollChildren();

			}
		}
	}
}

