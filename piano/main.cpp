#include "KeyboardController.h"
#include "Piano.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(400, 200), "Piano");

	Piano piano(48000);
	Player player(ma_format_f32, 1, 48000);

	player.SetDataCallback([&piano](void* output, ma_uint32 frameCount) {
		piano.Mix(static_cast<float*>(output), frameCount);
	});

	KeyboardController controller(piano);

	player.Start();

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
			{
				window.close();
			}

			controller.HandleEvent(event);
		}

		window.clear();
		window.display();
	}

	player.Stop();
	return 0;
}