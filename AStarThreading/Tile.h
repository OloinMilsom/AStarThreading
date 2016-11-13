#pragma once
#include "BasicTypes.h"
#include "Renderer.h"

class Tile {
private:
	Rect m_rect;
	Colour m_col;
public:
	// constructor
	Tile(Rect rect = Rect(), Colour col = Colour());

	// accessors
	Rect getRect() const;

	// public member functions
	void render(Renderer * renderer) const;

	// heuristic functions for graph
	static float manhattanDistance(Tile * a, Tile * b);
	static float euclidianDistance(Tile * a, Tile * b);
};