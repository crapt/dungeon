#pragma once

#include	"cocos2d.h"
#include	"cocostudio/CocoStudio.h"
#include	"cocostudio/CCArmature.h"

class Unit;


class	SpriteUnit : public cocos2d::Node
{
public:
	
	static	SpriteUnit* create(Unit* unit);
	bool	init(Unit* unit);
	void	insterUnit(cocos2d::Layer * layer);

public:
	
	virtual void setPosition(const cocos2d::Vec2 &position);

	void	changeSpriteAnimation();
	void	releaseSpriteUnit();
	void	callbackAttackAnimation();
	void	setHp();

public:
	void	setDirNode(int index, cocos2d::Node* node)
	{
		_dirNode[index] = node;
	}
	void	setTurretDirNode(int index, cocos2d::Node* node)
	{
		_dirTurretNode[index] = node;
	}
	cocos2d::Node*	getDirNode(int index)
	{
		return _dirNode[index];
	}
	cocos2d::Node*	getTurretDirNode(int index)
	{
		return _dirTurretNode[index];
	}
	cocos2d::Sprite*	spriteShadow()	{ return _sprShadow; }

	void	playDieAction();
		
protected:
	Unit*					_unit;
	signed	short			_curAngle;
	
	bool	_hasTurret;

	cocos2d::Node*		_dirNode[16];
	cocos2d::Node*		_dirTurretNode[16];
	cocos2d::Sprite*	_sprShadow;
	cocos2d::Node*		_actionNode;
	cocostudio::timeline::ActionTimeline*	_action;

	cocos2d::Node*		_moveNode[4];
	cocos2d::Node*		_attackNode[2];
	cocos2d::Node*		_dieNode[2];
	//hp
	cocos2d::DrawNode*	 _hpDrawNode;

	float		_originX;
	float		_originY;
	float		_hpLength;

};