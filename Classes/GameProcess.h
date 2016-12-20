#pragma once
#include "cocos2d.h"
#include "GameDefine.h"
#include "dmap.h"

class Unit;
class MapScrollView;


class GameProcess
{

	GETSET(Unit*, Player);
	GETSET(std::list<Unit*>, UserList);
	GETSET(std::list<Unit*>, EnemyList);
	GETSET(Dungeon*, DungeonMap);
	GETSET(int, TickCount);

public:

	static		GameProcess*	getInstance();
	GameProcess();
	~GameProcess();

	void	init();
	void	onUdate(float dt);
	void	updateUnit(float dt);
	void	createEnemy(cocos2d::Layer * layer);
	void	createPlayer(cocos2d::Layer * layer);
	void	createDungeon(cocos2d::Node* layer);
	void	findPath();
	void	addUser();
	void	moveScrollView(cocos2d::Vec2 pos, cocos2d::Vec2 prePos);
	

	MapScrollView*					_mapScrollView;

private:
	
	static		GameProcess*		_instance;
	

};

#define _gameProcess   GameProcess::getInstance()