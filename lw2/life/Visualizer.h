#pragma once
#include "Model/Board.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <memory>

class Visualizer
{
public:
	void Run(int argc, char** argv);

private:
	void ParseArguments(int argc, char** argv);
	void InitWindow();
	void MainLoop();

	void HandleEvents();
	void UpdateSimulation();
	void UpdateStatistics();

	void UpdateVertexArray();
	void UpdateCellQuad(int x, int y);
	void Render();

	std::unique_ptr<Board> m_board;
	sf::RenderWindow m_window;
	sf::VertexArray m_vertices;
	sf::Clock m_statsTimer;

	int m_cellSize = 5;
	int m_numThreads = 1;
	double m_totalStepTime = 0;
	int m_frameCount = 0;
};