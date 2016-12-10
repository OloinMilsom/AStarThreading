#include "Player.h"
#include <iostream>

Player::Player(int pos)
	:GameEntity(pos) {
	m_keydowns = std::vector<bool>(4, false);
}

int Player::getIndexPos() const {
	return m_indexPos;
}

void Player::update(Graph<Tile*> * graph, int size) {
	if (m_keydowns[0] && graph->connectionExists(m_indexPos, m_indexPos - size)) {
		m_indexPos -= size;
		m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
	}
	if (m_keydowns[1] && graph->connectionExists(m_indexPos, m_indexPos - 1)) {
		m_indexPos -= 1;
		m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
	}
	if (m_keydowns[2] && graph->connectionExists(m_indexPos, m_indexPos + size)) {
		m_indexPos += size;
		m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
	}
	if (m_keydowns[3]) {
		if (graph->connectionExists(m_indexPos, m_indexPos + 1)) {
			m_indexPos += 1;
			m_worldPos = graph->getNode(m_indexPos)->getVal()->getRect().pos;
		}
	}
}

void Player::render(Renderer * renderer) const
{
	renderer->drawWorldFillRect(Rect(m_worldPos, Size(1.5, 1.5)), Colour(0, 255, 0));
}

void Player::onEvent(Event e) {
	switch (e) {
	case Event::W_KEY_DOWN :
		m_keydowns[0] = true;
		break;
	case Event::A_KEY_DOWN:
		m_keydowns[1] = true;
		break;
	case Event::S_KEY_DOWN:
		m_keydowns[2] = true;
		break;
	case Event::D_KEY_DOWN:
		m_keydowns[3] = true;
		break;
	case Event::W_KEY_UP:
		m_keydowns[0] = false;
		break;
	case Event::A_KEY_UP:
		m_keydowns[1] = false;
		break;
	case Event::S_KEY_UP:
		m_keydowns[2] = false;
		break;
	case Event::D_KEY_UP:
		m_keydowns[3] = false;
		break;
	}
}
