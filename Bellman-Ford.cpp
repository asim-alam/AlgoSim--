#include "Pathfinding.h"
#include <math.h>
#include <stdlib.h>

#include <windows.h>

void BellmanFord::Init(Grid *g)
{
    Algorithm::Init(g);
    Node *start = grid->GetStartNode();
    if (start)
    {
        start->g = 0;
        start->type = NODE_START;
    }

    // Initialize all other nodes to infinity
    for (int y = 0; y < grid->GetHeight(); y++)
    {
        for (int x = 0; x < grid->GetWidth(); x++)
        {
            Node *n = grid->GetNode(x, y);
            if (n != start)
                n->g = 1e9f;
            n->parent = nullptr;
        }
    }

    iterations = 0;
    maxIterations = grid->GetWidth() * grid->GetHeight();
}

bool BellmanFord::Step()
{
    if (finished)
        return true;

    bool changed = false;
    // Relax edges (iterate all nodes and their neighbors)

    for (int y = 0; y < grid->GetHeight(); y++)
    {
        for (int x = 0; x < grid->GetWidth(); x++)
        {
            Node *u = grid->GetNode(x, y);
            if (u->g >= 1e9f)
                continue;
            if (u->type == NODE_WALL)
                continue;

            Vector<Node *> neighbors = grid->GetNeighbors(u, true);
            for (uint32 i = 0; i < neighbors.size(); i++)
            {
                Node *v = neighbors[i];
                if (v->type == NODE_WALL)
                    continue;

                float weight = (u->x != v->x && u->y != v->y) ? 1.414f : 1.0f;;
                if (u->g + weight < v->g)
                {
                    v->g = u->g + weight;
                    v->parent = u;
                    changed = true;
                    if (v->type != NODE_START && v->type != NODE_END)
                        v->type = NODE_VISITED;
                }
            }
        }
    }

    iterations++;
    if (!changed || iterations >= maxIterations)
    {
        finished = true;

        // Reconstruct path
        Node *end = grid->GetEndNode();
        if (end && end->g < 1e9f && end->parent)
        {
            Node *curr = end;
            while (curr && curr->parent)
            {
                if (curr->type != NODE_START && curr->type != NODE_END)
                    curr->type = NODE_PATH;
                curr = curr->parent;
            }
        }
        return true;
    }

    return false;
}