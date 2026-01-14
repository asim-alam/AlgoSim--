#include "pathfinding.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>

void AStar::Init(Grid *g)
{
    Algorithm::Init(g);

    // Initialize all distances to infinity
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

    Node *start = grid->GetStartNode();
    if (start)
    {
        start->g = 0;
        start->h = Heuristic(start, grid->GetEndNode());
        start->f = start->g + start->h;
        pq.clear();
        pq.push({start, start->f});
        start->type = NODE_FRONTIER;
        start->visited = true;
    }
}

float AStar::Heuristic(Node *a, Node *b)
{
    if (!a || !b)
        return 0;
    // Manhattan distance
    return (float)(abs(a->x - b->x) + abs(a->y - b->y));
}

bool AStar::Step()
{
    if (finished)
        return true;
    if (pq.empty())
    {
        finished = true;
        return true; //No path found
    }

    NodeRecord currentRecord = pq.top();
    pq.pop();
    Node *current = currentRecord.node;

    if (current->type == NODE_FRONTIER || current->type == NODE_EMPTY)
    {
        current->type = NODE_VISITED;
    }
    if (current == grid->GetStartNode())
        current->type = NODE_START;

    if (current == grid->GetEndNode())
    {
        // Reconstruct path
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
        float tentative_g = current->g + 1;

        if (tentative_g < neighbor->g)
        {
            neighbor->parent = current;
            neighbor->g = tentative_g;
            neighbor->h = Heuristic(neighbor, grid->GetEndNode());
            neighbor->f = neighbor->g + neighbor->h;

            neighbor->visited = true;
            if (neighbor->type == NODE_EMPTY)
                neighbor->type = NODE_FRONTIER;
            pq.push({neighbor, neighbor->f});
        }
    }

    return false; 
}