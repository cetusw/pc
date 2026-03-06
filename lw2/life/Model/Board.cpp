#include "Board.h"
#include "../FileDescriptor.h"
#include <chrono>
#include <random>

Board::Board(const int width, const int height)
	: m_width(width)
	, m_height(height)
	, m_current(width * height, false)
	, m_next(width * height, false)
{
}
void Board::StartSynchronizer(int numThreads)
{
	if (m_synchronizer)
	{
		return;
	}

	m_synchronizer = std::make_unique<StepSynchronizer>(
		numThreads,
		[this](const int id, const int count) {
			this->ProcessRows(id, count);
		});
}

void Board::GenerateCells(const float probability)
{
	std::mt19937 gen(std::random_device{}());
	std::bernoulli_distribution dist(probability);

	for (unsigned char& i : m_current)
	{
		i = static_cast<uint8_t>(dist(gen));
	}

	std::ranges::fill(m_next, false);
}

void Board::Step()
{
	if (!m_synchronizer)
	{
		throw std::runtime_error("Threads not started");
	}
	const auto start = std::chrono::high_resolution_clock::now();

	m_synchronizer->ExecuteStep();
	m_current.swap(m_next);

	const auto end = std::chrono::high_resolution_clock::now();
	m_lastStepTime = std::chrono::duration<double, std::milli>(end - start).count();
}

Board Board::Load(const std::string& filename)
{
	const FileDescriptor file(filename, "r");

	int width = 0;
	int height = 0;
	char headerBuffer[128];
	if (std::fgets(headerBuffer, sizeof(headerBuffer), file.Get()) == nullptr)
	{
		throw std::runtime_error("Failed to read header line");
	}

	std::stringstream ss(headerBuffer);
	if (!(ss >> width >> height))
	{
		throw std::runtime_error("Invalid file format: Header missing or corrupt");
	}

	Board board(width, height);
	ReadBoardContent(file.Get(), board);

	return board;
}

void Board::Save(const Board& board, const std::string& filename)
{
	const FileDescriptor file(filename, "w");

	if (std::fprintf(file.Get(), "%d %d\n", board.GetWidth(), board.GetHeight()) < 0)
	{
		throw std::runtime_error("Failed to write header to file");
	}

	for (int y = 0; y < board.GetHeight(); ++y)
	{
		for (int x = 0; x < board.GetWidth(); ++x)
		{
			const char ch = board.GetCell(x, y) ? '#' : ' ';
			if (std::fputc(ch, file.Get()) == EOF)
			{
				throw std::runtime_error("Failed to write cell data");
			}
		}
		std::fputc('\n', file.Get());
	}
}

int Board::GetWidth() const
{
	return m_width;
}

int Board::GetHeight() const
{
	return m_height;
}

bool Board::GetCell(const int x, const int y) const
{
	return m_current[GetIndex(x, y)];
}

void Board::SetCell(const int x, const int y, const bool value)
{
	m_current[GetIndex(x, y)] = value;
}

double Board::GetLastStepTime() const
{
	return m_lastStepTime;
}

int Board::GetIndex(int x, int y) const
{
	x = (x % m_width + m_width) % m_width;
	y = (y % m_height + m_height) % m_height;
	return y * m_width + x;
}

int Board::CountAliveNeighbors(const int x, const int y) const
{
	int count = 0;
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			if (dx == 0 && dy == 0)
			{
				continue;
			}
			if (GetCell(x + dx, y + dy))
			{
				count++;
			}
		}
	}
	return count;
}

void Board::ProcessRows(const int threadId, const int numThreads)
{
	const int rowsPerThread = m_height / numThreads;
	const int startY = threadId * rowsPerThread;
	const int endY = (threadId == numThreads - 1) ? m_height : startY + rowsPerThread;

	for (int y = startY; y < endY; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			UpdateCellState(x, y);
		}
	}
}

void Board::UpdateCellState(const int x, const int y)
{
	const int aliveNeighbors = CountAliveNeighbors(x, y);
	const int index = GetIndex(x, y);
	const bool isAlive = m_current[index];

	const bool willBeAlive = (isAlive && (aliveNeighbors < 4 && aliveNeighbors > 1)) || (!isAlive && (aliveNeighbors == 3));

	m_next[index] = willBeAlive ? 1 : 0;
}

void Board::ReadBoardContent(FILE* file, Board& board)
{
	int x = 0;
	int y = 0;
	int ch;

	while ((ch = std::fgetc(file)) != EOF)
	{
		if (ch == '#' || ch == ' ')
		{
			board.SetCell(x, y, (ch == '#'));
			x++;
			if (x >= board.GetWidth())
			{
				x = 0;
				y++;
			}
		}
		if (y >= board.GetHeight())
			break;
	}
}