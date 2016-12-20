#pragma once


#define GETSET(varType, funName)\
	protected: varType _##funName; \
	public: virtual varType get##funName(void) const { return _##funName; } \
	public: virtual void set##funName(varType var){ _##funName = var; }

static int TILE_SIZE = 61;
static float MAP_SCALE = 1.0f;
static int MAX_WIDTH = 20;
static int MAX_HEIGHT = 20;
