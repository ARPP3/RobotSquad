#pragma once
#include "stdafx.h"

enum class ListState { Unchecked = 0, Open = 1, Closed = 2 };

struct Node
{
	float heuristic = 0;
	float movementCost = 0;
	float fValue = 0;

	Node *parentNode;
	
	ListState list = ListState::Unchecked;
	int id;
};

struct NodeData
{
	std::vector<int> neighbors;
	glm::vec3 position;
	int id;
};

class AStar
{
public:
	void SetStartAndGoal(glm::vec3 a, glm::vec3 b);
	bool IterateSolver();
	bool FindPath();

	bool ReadNavMesh(const string &file);
	int GetIDFromPosition(glm::vec3 pos);
	glm::vec3 GetPositionFromID(int id);

	std::vector<glm::vec3> pathBack;

private:
	int numNodes;

	std::map<int, NodeData> nodes;
	std::map<int, std::map<int, NodeData*>> nodeQuadrant;

	std::map<int, Node*> List;

	NodeData start;
	NodeData goal;
	NodeData currentNode;
};