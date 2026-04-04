#include "KeyboardController.h"

KeyboardController::KeyboardController(Piano& piano)
	: m_piano(piano)
{
	InitKeyMap();
}

void KeyboardController::InitKeyMap()
{
	// Первая октава
	m_keyToNote[sf::Keyboard::Z] = -9; // До
	m_keyToNote[sf::Keyboard::X] = -7; // Ре
	m_keyToNote[sf::Keyboard::C] = -5; // Ми
	m_keyToNote[sf::Keyboard::V] = -4; // Фа
	m_keyToNote[sf::Keyboard::B] = -2; // Соль
	m_keyToNote[sf::Keyboard::N] = 0; // Ля
	m_keyToNote[sf::Keyboard::M] = 2; // Си
	m_keyToNote[sf::Keyboard::S] = -8; // До#
	m_keyToNote[sf::Keyboard::D] = -6; // Ре#
	m_keyToNote[sf::Keyboard::G] = -3; // Фа#
	m_keyToNote[sf::Keyboard::H] = -1; // Соль#
	m_keyToNote[sf::Keyboard::J] = 1; // Ля#

	// Вторая октава
	m_keyToNote[sf::Keyboard::Q] = 3; // До
	m_keyToNote[sf::Keyboard::W] = 5; // Ре
	m_keyToNote[sf::Keyboard::E] = 7; // Ми
	m_keyToNote[sf::Keyboard::R] = 8; // Фа
	m_keyToNote[sf::Keyboard::T] = 10; // Соль
	m_keyToNote[sf::Keyboard::Y] = 12; // Ля
	m_keyToNote[sf::Keyboard::U] = 14; // Си
	m_keyToNote[sf::Keyboard::Num2] = 4; // До#
	m_keyToNote[sf::Keyboard::Num3] = 6; // Ре#
	m_keyToNote[sf::Keyboard::Num5] = 9; // Фа#
	m_keyToNote[sf::Keyboard::Num6] = 11; // Соль#
	m_keyToNote[sf::Keyboard::Num7] = 13; // Ля#
}

void KeyboardController::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		const auto it = m_keyToNote.find(event.key.code);
		if (it != m_keyToNote.end())
		{
			m_piano.NoteOn(it->second);
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		const auto it = m_keyToNote.find(event.key.code);
		if (it != m_keyToNote.end())
		{
			m_piano.NoteOff(it->second);
		}
	}
}