#include "LayerDungeon.h"
#include "TouchEventProcess.h"
#include "TouchEventSimulatorMapControl.h"
#include "GameProcess.h"
#include "LayerUnit.h"
#include "Unit.h"


USING_NS_CC;


Scene* LayerDungeon::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	// 'layer' is an autorelease object
	auto layer = LayerDungeon::create();
	// add layer as a child to scene
	scene->addChild(layer);
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LayerDungeon::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	_visibleSize = Director::getInstance()->getVisibleSize();
// 	auto	scaleHeight = _visibleSize.height / (MAX_HEIGHT * TILE_SIZE);
// 	auto	scaleWidth = _visibleSize.width / (MAX_WIDTH * TILE_SIZE);
// 	auto	deviceHeightCenter = _visibleSize.height / 2;
		
	_mapScrollView = MapScrollView::create(Size(MAX_WIDTH * TILE_SIZE, MAX_HEIGHT * TILE_SIZE), 2.0f);
	_mapScrollView->setAnchorPoint(Vec2::ZERO);
	_mapScrollView->setContentSize(_visibleSize);
	_mapScrollView->setPosition(Vec2(0.0f, 0.0f));
	_mapScrollView->setScale(2.0f);
	addChild(_mapScrollView);
	_gameProcess->_mapScrollView = _mapScrollView;

	_layerUnit = LayerUnit::create();
	_layerUnit->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	_layerUnit->setZOrder(100);
	_mapScrollView->addChild(_layerUnit);
		
		
	_touchEventController = new TouchEventProcessController;
	_touchEventController->addTouchEvent(new TouchEventProcess);

	TouchEventSimulatorMapControl*	touchEventMapControl = new TouchEventSimulatorMapControl(_mapScrollView);
	// 	touchEventMapControl->setMoveToCallback(CC_CALLBACK_1(LayerGame::moveTo, this));
	_touchEventController->addTouchEvent(touchEventMapControl);
	_touchEventController->changeMode(1);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(LayerDungeon::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(LayerDungeon::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(LayerDungeon::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	scheduleUpdate();

	_gameProcess->createDungeon(_mapScrollView);
	_gameProcess->createPlayer(_layerUnit);
	_gameProcess->createEnemy(_layerUnit);

 	return true;
}

void	LayerDungeon::update(float delta)
{
	_gameProcess->onUdate(delta);
}

void	LayerDungeon::drawTile()
{

}

void	LayerDungeon::onTouchesBegan(const std::vector<Touch *> &touches, Event * event)
{
	CCLOG("LayerDungeon::onTouchesBegan");
	//_touchEventController->onTouchesBegan(touches, event);

}

void LayerDungeon::onTouchesMoved(const std::vector<Touch *> &touches, Event * event)
{
	CCLOG("LayerDungeon::onTouchesMoved");
	//_touchEventController->onTouchesMoved(touches, event);

}
void LayerDungeon::onTouchesEnded(const std::vector<Touch *> &touches, Event * event)
{
	CCLOG("LayerDungeon::onTouchesEnded");
	//_touchEventController->onTouchesEnded(touches, event);
	auto	&item = touches[0];
	auto	location = item->getLocation();
	Vec2	worldPos;
	_mapScrollView->convertScreenPosToWorldPos(location, &worldPos);
	
	auto player = _gameProcess->getPlayer();
	player->setGoalX(worldPos.x);
	player->setGoalY(worldPos.y);
	player->setIsFinding(true);
}


void LayerDungeon::setViewPointCenter(cocos2d::Vec2 position) 
{
	Size winSize = Director::sharedDirector()->getWinSize();
	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (79 * 100) - winSize.width / 2);
	y = MIN(y, (24 * 100) - winSize.height / 2);
	Point actualPosition = ccp(x, y);

	Point centerOfView = ccp(winSize.width / 2, winSize.height / 2);
	Point viewPoint = ccpSub(centerOfView, actualPosition);
	this->setPosition(viewPoint);
}
