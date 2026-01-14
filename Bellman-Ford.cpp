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

    // Perform multiple passes per step to speed up finishing
    int passesPerStep = 40; 
    bool changed = false;
    for (int p = 0; p < passesPerStep && iterations < maxIterations; p++)
    {
        changed = false;
        for (int y = 0; y < grid->GetHeight(); y++)
        {
            for (int x = 0; x < grid->GetWidth(); x++)
            {
                Node *u = grid->GetNode(x, y);
                if (u->g >= 1e8f) continue;
                if (u->type == NODE_WALL) continue;

                Vector<Node *> neighbors = grid->GetNeighbors(u, true);
                for (uint32 i = 0; i < neighbors.size(); i++)
                {
                    Node *v = neighbors[i];
                    if (v->type == NODE_WALL) continue;

                    float weight = (u->x != v->x && u->y != v->y) ? 1.414f : 1.0f;
                    if (v->type == NODE_BOOST) weight -= 2.0f;
                    if (v->type == NODE_MUD) weight += 5.0f;

                    if (u->g + weight < v->g)
                    {
                        v->g = u->g + weight;
                        v->parent = u;
                        changed = true;
                        if (v->type == NODE_EMPTY)
                            v->type = NODE_VISITED;
                    }
                }
            }
        }
        iterations++;
        if (!changed) break;
    }

   
    for (int y = 0; y < grid->GetHeight(); y++)
    {
        for (int x = 0; x < grid->GetWidth(); x++)
        {
            Node *n = grid->GetNode(x, y);
            if (n->type == NODE_PATH)
                n->type = NODE_VISITED;
        }
    }

    Node *end = grid->GetEndNode();
    if (end && end->parent)
    {
        Node *curr = end->parent;
        int count = 0;
        while (curr && curr != grid->GetStartNode() && count < maxIterations)
        {
           
            if (curr->type == NODE_VISITED || curr->type == NODE_EMPTY)
                curr->type = NODE_PATH;
            
            // Safety check for parent loops (negative cycles)
            if (curr == curr->parent) break; 
            
            curr = curr->parent;
            count++;
        }
    }

    if (!changed || iterations >= maxIterations)
    {
        finished = true;
        return true;
    }

    return false;
}