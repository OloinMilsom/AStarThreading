#include "Tile.h"

Tile::Tile(Rect rect, Colour col)
	:m_rect(rect),
	 m_col(col) {

}

Rect Tile::getRect() const
{
	return m_rect;
}

void Tile::render(Renderer * renderer) const
{
	renderer->drawWorldFillRect(m_rect, m_col);
}

float Tile::manhattanDistance(Tile * a, Tile * b)
{
	return fabsf(a->getRect().pos.x - b->getRect().pos.x) + fabsf(a->getRect().pos.x - b->getRect().pos.x);
}

float Tile::euclidianDistance(Tile * a, Tile * b)
{
	return (a->getRect().pos - b->getRect().pos).length();
}
