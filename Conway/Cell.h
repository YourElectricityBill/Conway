#ifndef CELL_H
#define CELL_H

#include "Entity.h"

class Cell
{
public:
	Cell(int &p_x, int &p_y);

	Entity* cellEntity() { return &m_cellEntity; }

	void setLivingState(bool p_boolValue);
	void increaseAliveNeighbourNo();

	bool isAlive() { return m_isAlive; }
	int xCellNumber() { return m_xCellNumber; }
	int yCellNumber() { return m_yCellNumber; }
	int currentCellNumber() { return m_cellNumber; }
	int aliveNeighbours() { return m_aliveNeighbours; }

	std::vector <Cell*> &neighbours() { return m_neighbours; }

	~Cell();
private:
	int m_xCellNumber, m_yCellNumber, m_cellNumber, m_aliveNeighbours;
	bool m_isAlive;
	Entity m_cellEntity;

    std::vector <Cell*> m_neighbours;
};

#endif