#include "stdafx.h"

bool AStar::ReadNavMesh(const string &file)
{
	nodes.clear();

	FILE *navFile = nullptr;
	navFile = fopen(file.c_str(), "rb");

	if (navFile == nullptr)
	{
		HWND* hWnd = appMain.oglControl.get_hWnd();
		wchar_t errorMessage;
		wsprintf(&errorMessage, L"Cannot open file for reading: %s", file);
		MessageBox(*hWnd, &errorMessage, L"File Error", MB_ICONERROR);
		return false;
	}

	if (navFile != NULL)
	{
		//Get the number of nodes we have in the mesh
		fscanf(navFile, "%d", &numNodes);

		glm::vec3 pos;
		int nodeID;
		while (fscanf(navFile, "%d %f %f %f ", &nodeID, &pos.x, &pos.y, &pos.z) != EOF)
		{
			nodes[nodeID].position = pos;
			nodes[nodeID].id = nodeID;

			//Scan for neighbor nodes
			int neighborNode;

			while (fscanf(navFile, "%d", &neighborNode))
			{
				if (neighborNode == -1)
				{
					break;
				}

				nodes[nodeID].neighbors.push_back(neighborNode);
			}

			glm::vec3 smallPos = pos / 15.f;
			smallPos = glm::floor(smallPos);

			int posToInt = smallPos.x * 10000 + smallPos.y * 100 + smallPos.z;

			nodeQuadrant[posToInt][nodeID] = &nodes[nodeID];
		}
	}

	fclose(navFile);

	return true;
}


int AStar::GetIDFromPosition(glm::vec3 pos)
{
	glm::vec3 smallPos = pos / 15.f;
	smallPos = glm::floor(smallPos);

	int posToInt = smallPos.x * 10000 + smallPos.y * 100 + smallPos.z;

	int ID;

	float smallest = 99999.f;

	//Iterate through the nodes in the quadrant
	typedef std::map<int, NodeData*>::const_iterator MapIterator;
	for (MapIterator iter = nodeQuadrant[posToInt].begin(); iter != nodeQuadrant[posToInt].end(); iter++)
	{
		float tempLength = glm::vec3(iter->second->position - pos).length();
		if (tempLength <= smallest)
		{
			smallest = tempLength;
			ID = iter->first;
		}
	}

	return ID;
}

glm::vec3 AStar::GetPositionFromID(int id)
{
	return nodes[id].position;
}

void AStar::SetStartAndGoal(glm::vec3 a, glm::vec3 b)
{
	start = nodes[GetIDFromPosition(a)];
	goal = nodes[GetIDFromPosition(b)];
	pathBack.clear();

	currentNode = start;

	for (int i = 0; i < numNodes; i++)
	{
		List[i] = new Node();
		List[i]->id = i;
	}
}

bool AStar::FindPath()
{
	while (!IterateSolver());

	while (true)
	{
		pathBack.push_back(currentNode.position);

		if (currentNode.id == start.id)
		{
			return true;
		}
		currentNode = nodes[List[currentNode.id]->parentNode->id];
	}

	return true;
}

bool AStar::IterateSolver()
{
	int currentID = currentNode.id;
	int goalID = goal.id;

	if (currentID == goalID)
	{
		return true;
	}

	for (unsigned int i = 0; i < currentNode.neighbors.size(); i++)
	{
		Node *currentNeighborNode = List[currentNode.neighbors[i]];

		List[currentID]->list = ListState::Closed; //Close the current node

		//If the node has no value
		if (currentNeighborNode->list == ListState::Unchecked || currentNeighborNode->list == ListState::Open)
		{
			glm::vec3 nodePos = nodes[currentNode.neighbors[i]].position;	//neighbor node position

			if (currentNeighborNode->list == ListState::Open)
			{
				if (List[currentNode.id]->fValue + glm::distance(nodePos, currentNode.position) < currentNeighborNode->movementCost)
				{
					currentNeighborNode->parentNode = List[currentID];
				}
			}
			else
			{
				currentNeighborNode->parentNode = List[currentID];
			}
			currentNeighborNode->list = ListState::Open;	//Make it open

			currentNeighborNode->movementCost = currentNeighborNode->parentNode->fValue + glm::distance(nodePos, currentNode.position);
			currentNeighborNode->heuristic = glm::distance(nodePos, goal.position);
			currentNeighborNode->fValue = currentNeighborNode->heuristic + currentNeighborNode->movementCost;
		}
	}

	float smallest = 99999.f;

	for (unsigned int i = 0; i < List.size(); i++)
	{
		if (List[i]->fValue < smallest && List[i]->list == ListState::Open)
		{
			smallest = List[i]->fValue;
			currentNode = nodes[i];
		}
	}

	return false;
}