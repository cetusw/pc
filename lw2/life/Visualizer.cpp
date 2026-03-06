#include "Visualizer.h"
#include "Model/Board.h"

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <stdexcept>

void Visualizer::Run(const int argc, char** argv)
{
	ParseArguments(argc, argv);
	InitWindow();
	MainLoop();
}

void Visualizer::ParseArguments(const int argc, char** argv)
{
	if (argc != 4)
	{
		throw std::invalid_argument("Usage: life visualize INPUT_FILE NUM_THREADS");
	}
	m_numThreads = std::stoi(argv[3]);
	m_board = std::make_unique<Board>(Board::Load(argv[2]));
	m_board->StartSynchronizer(m_numThreads);
}

void Visualizer::InitWindow()
{
	const unsigned int width = m_board->GetWidth() * m_cellSize;
	const unsigned int height = m_board->GetHeight() * m_cellSize;

	m_window.create(sf::VideoMode(width, height), "Game of Life");
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(m_board->GetWidth() * m_board->GetHeight() * 4);
	m_window.setFramerateLimit(10);
}

void Visualizer::MainLoop()
{
	while (m_window.isOpen())
	{
		HandleEvents();
		UpdateSimulation();
		UpdateStatistics();
		UpdateVertexArray();
		Render();
	}
}

void Visualizer::HandleEvents()
{
	sf::Event event{};
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Visualizer::UpdateSimulation()
{
	m_board->Step();
	m_totalStepTime += m_board->GetLastStepTime();
	m_frameCount++;
}

void Visualizer::UpdateStatistics()
{
	if (m_statsTimer.getElapsedTime().asSeconds() >= 1.0f)
	{
		if (m_frameCount > 0)
		{
			const double avgTime = m_totalStepTime / m_frameCount;
			std::stringstream ss;
			ss << "Life | Avg Step: " << std::fixed << std::setprecision(2) << avgTime << " ms";
			m_window.setTitle(ss.str());
		}

		m_totalStepTime = 0;
		m_frameCount = 0;
		m_statsTimer.restart();
	}
}

void Visualizer::UpdateVertexArray()
{
	for (int y = 0; y < m_board->GetHeight(); ++y)
	{
		for (int x = 0; x < m_board->GetWidth(); ++x)
		{
			UpdateCellQuad(x, y);
		}
	}
}

void Visualizer::UpdateCellQuad(int x, int y)
{
	int index = (y * m_board->GetWidth() + x) * 4;
	bool isAlive = m_board->GetCell(x, y);
	sf::Color color = isAlive ? sf::Color::Black : sf::Color::White;

	float xPos = static_cast<float>(x * m_cellSize);
	float yPos = static_cast<float>(y * m_cellSize);
	const float size = static_cast<float>(m_cellSize);

	m_vertices[index + 0].position = { xPos, yPos };
	m_vertices[index + 1].position = { xPos + size, yPos };
	m_vertices[index + 2].position = { xPos + size, yPos + size };
	m_vertices[index + 3].position = { xPos, yPos + size };

	for (int i = 0; i < 4; ++i)
	{
		m_vertices[index + i].color = color;
	}
}

void Visualizer::Render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_vertices);
	m_window.display();
}