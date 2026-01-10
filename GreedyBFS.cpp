#include "Pathfinding.h"
#include <math.h>
#include <stdlib.h>

#include <windows.h>

void GreedyBFS::Init(Grid *g)
{
    Algorithm::Init(g);
    Node *start = grid->GetStartNode();
    if (start)
    {
        pq.clear();
        pq.push({start, Heuristic(start, grid->GetEndNode())});
        start->type = NODE_FRONTIER;
        start->visited = true;
    }
}

float GreedyBFS::Heuristic(Node *a, Node *b)
{
    if (!a || !b)
        return 0;
    return (float)(abs(a->x - b->x) + abs(a->y - b->y));
}

bool GreedyBFS::Step()
{
    if (finished)
        return true;
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
        if (!neighbor->visited)
        {
            neighbor->visited = true;
            neighbor->parent = current;
            neighbor->type = NODE_FRONTIER;
            pq.push({neighbor, Heuristic(neighbor, grid->GetEndNode())});
        }
    }
    return false;
}