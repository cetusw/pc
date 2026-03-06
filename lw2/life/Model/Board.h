#pragma once
#include "../StepSynchronizer.h"
#include <barrier>
#include <memory>
#include <vector>

class Board
{
public:
	Board(int width, int height);

	void StartSynchronizer(int numThreads);
	void GenerateCells(float probability);
	void Step();

	static Board Load(const std::string& filename);
	static void Save(const Board& board, const std::string& filename);

	[[nodiscard]] int GetWidth() const;
	[[nodiscard]] int GetHeight() const;
	[[nodiscard]] bool GetCell(int x, int y) const;
	void SetCell(int x, int y, bool value);
	[[nodiscard]] double GetLastStepTime() const;

private:
	int m_width;
	int m_height;
	std::vector<uint8_t> m_current;
	std::vector<uint8_t> m_next;
	std::unique_ptr<StepSynchronizer> m_synchronizer;
	double m_lastStepTime = 0.0;

	[[nodiscard]] int GetIndex(int x, int y) const;
	[[nodiscard]] int CountAliveNeighbors(int x, int y) const;
	void ProcessRows(int threadId, int numThreads);
	void UpdateCellState(int x, int y);
	static void ReadBoardContent(FILE* file, Board& board);
};
