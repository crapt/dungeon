#pragma once

#include "GameDefine.h"

class UnitProperty;


class Unit
{
public:

	enum UnitState
	{
		None,
		Move,
		Attack,
		Die,
		Max
	};

	enum Direction
	{
		North,
		East,
		South,
		West,
		DirectionCount
	};
	
	Unit();
	~Unit();

	void		onUdate(float dt);
	void		chageState(UnitState state);
	
	GETSET(int, Uid);
	GETSET(int, PosX);
	GETSET(int, PosY);
	GETSET(int, PrePosX);
	GETSET(int, PrePosY);
	GETSET(int, GoalX);
	GETSET(int, GoalY);
	GETSET(UnitState, CurrState);
	GETSET(UnitState, PreState);
	GETSET(void*, SpriteData);
	GETSET(UnitProperty*, Property);
	GETSET(bool, IsFinding);
	GETSET(Direction, Direcetion);
	GETSET(Direction, PreDirecetion);

};