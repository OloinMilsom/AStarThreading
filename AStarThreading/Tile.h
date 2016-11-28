#pragma once
#include "BasicTypes.h"
#include "Renderer.h"

class Tile {
private:
	Rect m_rect;
	Colour m_originalCol;
	Colour m_col;
	bool m_isWall;
public:
	// constructor
	Tile(Rect rect = Rect(), bool isWall = false);

	// accessors
	Rect getRect() const;
	bool getIsWall() const;
	void setColour(Colour col);
	void setOriginalColour(Colour col);
	void resetColour();

	// public member functions
	void render(Renderer * renderer) const;

	// heuristic functions for graph
	static float manhattanDistance(Tile * a, Tile * b);
	static float euclidianDistance(Tile * a, Tile * b);
};