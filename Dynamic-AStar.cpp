#include "pathfinding.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>

void DynamicAStar::Init(Grid *g)
{
    Algorithm::Init(g);
    currentPos = grid->GetStartNode();
    targetPos = grid->GetEndNode();
    pathIndex = 0;

    Replan();
}

void DynamicAStar::Replan()
{
    // A* solver to find path from currentPos to targetPos

    currentPath.clear();
    if (!currentPos || !targetPos)
        return;

    for (int y = 0; y < grid->GetHeight(); y++)
    {
        for (int x = 0; x < grid->GetWidth(); x++)
        {
            Node *n = grid->GetNode(x, y);
            n->g = 1e9f;
            n->h = 0;
            n->f = 0;
            n->parent = nullptr;
            n->visited = false;
           
        }
    }

    // A* Logic
    PriorityQueue<NodeRecord> pq;
    currentPos->g = 0;
    currentPos->h = (float)(abs(currentPos->x - targetPos->x) + abs(currentPos->y - targetPos->y));
    currentPos->f = currentPos->g + currentPos->h;
    pq.push({currentPos, currentPos->f});

    bool found = false;
    while (!pq.empty())
    {
        NodeRecord currentRecord = pq.top();
        pq.pop();
        Node *current = currentRecord.node;
        if (current == targetPos)
        {
            found = true;
            break;
        }

        if (current->visited)
            continue;
        current->visited = true;

        Vector<Node *> neighbors = grid->GetNeighbors(current, true);
        for (uint32 i = 0; i < neighbors.size(); i++)
        {
            Node *neighbor = neighbors[i];
            if (neighbor->type == NODE_WALL)
                continue;

            float dist = (current->x != neighbor->x && current->y != neighbor->y) ? 1.414f : 1.0f;
            float tentativeG = current->g + dist;

            if (tentativeG < neighbor->g)
            {
                neighbor->parent = current;
                neighbor->g = tentativeG;
                neighbor->h = (float)(abs(neighbor->x - targetPos->x) + abs(neighbor->y - targetPos->y));
                neighbor->f = neighbor->g + neighbor->h;

                neighbor->visited = false; 
                pq.push({neighbor, neighbor->f});
            }
        }
    }

    if (found)
    {
        // Reconstruct path
        Node *curr = targetPos;
        while (curr)
        {
            currentPath.push_back(curr);
            curr = curr->parent;
        }
        
        for (int i = 0; i < currentPath.size() / 2; i++)
        {
            Node *temp = currentPath[i];
            currentPath[i] = currentPath[currentPath.size() - 1 - i];
            currentPath[currentPath.size() - 1 - i] = temp;
        }
        pathIndex = 0;

        // Visualize path
        for (uint32 i = 0; i < currentPath.size(); i++)
        {
            if (currentPath[i]->type != NODE_START && currentPath[i]->type != NODE_END)
                currentPath[i]->type = NODE_PATH;
        }
    }
}

bool DynamicAStar::Step()
{
    
    if (currentPath.empty() || pathIndex >= currentPath.size())
        return true; // Finished or stuck

    Node *nextNode = currentPath[pathIndex];

    // Check if next node is blocked (dynamic obstacle)
    if (nextNode->type == NODE_WALL)
    {
        
        Replan();
        if (currentPath.empty())
            return true; 
        pathIndex = 0;   
        return false;
    }

    
    currentPos = nextNode;
    if (currentPos->type != NODE_START && currentPos->type != NODE_END)
        currentPos->type = NODE_VISITED; 

    pathIndex++;


    if (currentPos == targetPos)
        return true;



    Sleep(100);


    return false;
}