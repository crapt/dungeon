#pragma	once

#include	"cocos2d.h"

class MapScaleTo : public cocos2d::ScaleTo
{
public:
	static MapScaleTo* create(float duration, float s);
	virtual void update(float time) override;
};

class MapScrollView : public cocos2d::Node
{
public:
	MapScrollView();
	~MapScrollView();

public:
	static	MapScrollView* create(const cocos2d::Size& mapDotSize, float max_Scale);
	bool	init(const cocos2d::Size& mapDotSize, float max_Scale);
	
public:
	virtual void update(float dt) override;
	virtual void setScale(float scale);
	
public:
	void	onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void	onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	void	onMouseScroll(cocos2d::Event *event);
public:
	void	convertScreenPosToWorldPos(const cocos2d::Vec2& screenPos, cocos2d::Vec2* worldPos);
	void	convertWorldPosToScreenPos(const cocos2d::Vec2& worldPos, cocos2d::Vec2* screenPos);

public:
	void	setMapSize(const cocos2d::Size &size);
	void	setScale(float scale, const cocos2d::Vec2& scaleCenter);
	void	handlePressLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition);
	void	handleMoveLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition);
	void	setCenterPos(cocos2d::Vec2 pos);
	void	handleReleaseLogic(cocos2d::Vec2& position, cocos2d::Vec2& prePosition);

	bool	scrollChildren(float touchOffsetX, float touchOffsetY);

private:
	void	startRecordSlidAction();
	void	stopAutoScrollChildren();
	
	void	endRecordSlidAction();
	void	startAutoScrollChildrenWithOriginalSpeed(const cocos2d::Vec2& dir, float v, bool attenuated, float acceleration);
	void	startBounceChildren(float v);
	void	moveChildren(float offsetX, float offsetY);
	void	autoScrollChildren(float dt);
	bool	bounceScrollChildren(float touchOffsetX, float touchOffsetY);
	bool	checkCustomScrollDestination(float* touchOffsetX, float* touchOffsetY);
	void	recordSlidTime(float dt);

public:
	void	maxScale(float scale)	{ _maxScale = scale; }
	void	minScaleX(float scale)	{ _minScaleX = scale; }
	void	minScaleY(float scale)	{ _minScaleY = scale; }
	float	getMaxDotWidth()		{ return _maxDotWidth; }
	float	getMaxDotHeight()		{ return _maxDotHeight; }
	
	
	cocos2d::Size	getMapSize()	{ return _mapSize; }
	
public:
	float	maxScale()	{ return _maxScale; }
	float	minScaleX()	{ return _minScaleX; }
	float	minScaleY()	{ return _minScaleY; }

protected:

	cocos2d::Size	_mapSize;

private:

	float	_maxScale, _minScaleX, _minScaleY;
	float	_topBoundary;	//  display Height
	float	_bottomBoundary;	// 0
	float	_leftBoundary;		// 0
	float	_rightBoundary;		//  display Width

	bool	_bePressed;
	float	_slidTime;
	cocos2d::Vec2	_moveChildPoint;

	bool	_autoScroll;
	float	_autoScrollOriginalSpeed;
	float	_autoScrollAddUpTime;
	float	_autoScrollAcceleration;
	bool	_isAutoScrollSpeedAttenuated;
	bool	_needCheckAutoScrollDestination;
	cocos2d::Vec2	_autoScrollDestination;

	bool	_inertiaScrollEnabled;
	cocos2d::Vec2	_bounceDir;

	cocos2d::Vec2	_touchBeganPosition;
	cocos2d::Vec2	_touchMovePosition;
	cocos2d::Vec2	_touchEndPosition;

	cocos2d::Vec2	_autoScrollDir;

	float			_maxDotWidth;
	float			_maxDotHeight;
	
};