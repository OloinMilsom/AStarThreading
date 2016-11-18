#include "Tile.h"

Tile::Tile(Rect rect, bool isWall)
	:m_rect(rect),
	 m_isWall(isWall) {

}

Rect Tile::getRect() const {
	return m_rect;
}

bool Tile::getIsWall() const {
	return m_isWall;
}

void Tile::setColour(Colour col) {
	m_col = col;
}

void Tile::render(Renderer * renderer) const {
	renderer->drawWorldFillRect(m_rect, m_col);
}

float Tile::manhattanDistance(Tile * a, Tile * b) {
	return fabsf(a->getRect().pos.x - b->getRect().pos.x) + fabsf(a->getRect().pos.x - b->getRect().pos.x);
}

float Tile::euclidianDistance(Tile * a, Tile * b) {
	return (a->getRect().pos - b->getRect().pos).length();
}