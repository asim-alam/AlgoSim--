#include "pathfinding.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>

void Dijkstra::Init(Grid *g)
{
    Algorithm::Init(g);
    // Set all distances to infinity
    for (int y = 0; y < grid->GetHeight(); y++)
    {
        for (int x = 0; x < grid->GetWidth(); x++)
        {
            Node *n = grid->GetNode(x, y);
            n->g = 1e9f;
            n->parent = nullptr;
            n->visited = false;
        }
    }
    Node *start = grid->GetStartNode();
    if (start)
    {
        start->g = 0;
        pq.clear();
        pq.push({start, 0});
        start->type = NODE_FRONTIER;
        start->visited = true;
    }
}
bool Dijkstra::Step()
{
    if (finished)
    {
        return true;
    }
    if (pq.empty())
    {
        finished = true;
        return true;
    }
    NodeRecord currentRecord = pq.top();
    pq.pop();
    Node *current = currentRecord.node;

    if (current->type == NODE_FRONTIER)
        current->type = NODE_VISITED;
    if (current == grid->GetStartNode())
        current->type = NODE_START;

    if (current == grid->GetEndNode())
    {
        Node *temp = current;
        while (temp->parent)
        {
            if (temp != grid->GetStartNode())
                temp->type = NODE_PATH;
            temp = temp->parent;
        }
        finished = true;
        return true;
    }

    Vector<Node *> neighbors = grid->GetNeighbors(current);
    for (int i = 0; i < neighbors.size(); i++)
    {
        Node *neighbor = neighbors[i];
        float alt = current->g + 1;

        if (alt < neighbor->g)
        {
            neighbor->g = alt;
            neighbor->parent = current;
            if (!neighbor->visited)
            {
                neighbor->visited = true;
                neighbor->type = NODE_FRONTIER;
            }
            pq.push({neighbor, neighbor->g});
        }
    }
    return false;
}