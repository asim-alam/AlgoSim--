#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Algorithm.h"
#include "Core.h"
#include "Grid.h"

struct NodeRecord
{
    Node *node;
    float cost; // f-score for A*, distance for Dijkstra

    bool operator<(const NodeRecord &other) const
    {
        return cost < other.cost;
    }
    bool operator>(const NodeRecord &other) const
    {
        return cost > other.cost;
    }
};

class Dijkstra : public Algorithm
{
private:
    PriorityQueue<NodeRecord> pq;

public:
    void Init(Grid *g) override;
    bool Step() override;
    const char *GetName() override { return "Dijkstra's Algorithm"; }
};

class AStar : public Algorithm {
private:
    PriorityQueue<NodeRecord> pq;
    
public:
    void Init(Grid* g) override;
    bool Step() override;
    const char* GetName() override { return "A* Search"; }
    
    float Heuristic(Node* a, Node* b);
};

class GreedyBFS : public Algorithm {
public:
    void Init(Grid* g) override;
    bool Step() override;
    const char* GetName() override { return "Greedy Best-First Search"; }
    
    float Heuristic(Node* a, Node* b);
private:
    PriorityQueue<NodeRecord> pq;
};

class BellmanFord : public Algorithm {
public:
    void Init(Grid* g) override;
    bool Step() override;
    const char* GetName() override { return "Bellman-Ford Algorithm"; }
private:
    Vector<Node*> edges; 
    int iterations;
    int maxIterations;
};
#endif