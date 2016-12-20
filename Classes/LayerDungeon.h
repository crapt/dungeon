#pragma once

#include "cocos2d.h"
#include "MapScrollView.h"
#include "TouchEventProcessController.h"

class LayerUnit;

class LayerDungeon : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool		init();
	virtual void		update(float time) override;

	void		drawTile();
	void		setViewPointCenter(cocos2d::Vec2 position);

	// a selector callback
	// implement the "static create()" method manually
	
	CREATE_FUNC(LayerDungeon);

	virtual void	onTouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	virtual void	onTouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);
	virtual void	onTouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event);

private:

	MapScrollView*					_mapScrollView;
	TouchEventProcessController*	_touchEventController;
	cocos2d::Size					_visibleSize;
	LayerUnit*						_layerUnit;
};