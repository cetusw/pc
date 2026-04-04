#pragma once
#include "Piano.h"
#include <SFML/Window.hpp>
#include <map>

class KeyboardController
{
public:
	explicit KeyboardController(Piano& piano);
	void HandleEvent(const sf::Event& event);

private:
	Piano& m_piano;

	std::map<sf::Keyboard::Key, int> m_keyToNote;

	void InitKeyMap();
};