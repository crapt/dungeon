#pragma	once

#include	"cocos2d.h"

class LayerGameMain;
class LayerGameMenu;
class SceneGame;

class TouchEventProcess
{
public:
	TouchEventProcess()		{}
	~TouchEventProcess()	{}

public:
	virtual	void	onEnter()	{}
	virtual	void	onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) {}
	virtual	void	onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) {}
	virtual	void	onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)	{}
	virtual	void	onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)	{}
	virtual	void	onExit()	{}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
public:
	virtual	void	onMouseMove(cocos2d::Event *event) {}
	virtual	void	onMouseDown(cocos2d::Event *event) {}
	virtual	void	onMouseUp(cocos2d::Event *event)	{}
	virtual	void	onMouseScroll(cocos2d::Event *event) {}
#endif

public:
	LayerGameMain*	layerGameMain()						{ return _layerGameMain; }
	void			layerGameMain(LayerGameMain* layer)	{ _layerGameMain = layer; }
	LayerGameMenu*	layerGameMenu()						{ return _layerGameMenu; }
	void			layerGameMenu(LayerGameMenu* layer)	{ _layerGameMenu = layer; }
	SceneGame*		sceneGame()					{ return _sceneGame; }
	void			sceneGame(SceneGame* scene)	{ _sceneGame = scene; }

protected:
	LayerGameMain*	_layerGameMain;
	LayerGameMenu*	_layerGameMenu;
	SceneGame*		_sceneGame;
};
