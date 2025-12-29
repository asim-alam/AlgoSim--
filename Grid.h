#ifndef GRID_H
#define GRID_H
#include "Core.h"
#include "Graphics.h"

enum NodeType
{
    NODE_EMPTY,
    NODE_WALL,
    NODE_START,
    NODE_END,
    NODE_VISITED,
    NODE_PATH,
    NODE_FRONTIER
};

struct Node
{
    int x, y;
    NodeType type;
    float g, h, f;
    Node *parent;
    bool visited;

    Node() : x(0), y(0), type(NODE_EMPTY), g(0), h(0), f(0), parent(nullptr), visited(false) {}
};

class Grid
{
private:
    int width, height;
    int cellSize;
    Node **nodes;
    Node *startNode;
    Node *endNode;

public:
    Grid(int w, int h, int size);
    ~Grid();

    void Resize(int width, int height);
    void Draw();
    void Reset();
    void ResetPath();

    Node *GetNode(int x, int y);
    void SetType(int x, int y, NodeType type);

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetCellSize() const { return cellSize; }

    Node *GetStartNode() { return startNode; }
    Node *GetEndNode() { return endNode; }

    void SetStartNode(Node *n);
    void SetEndNode(Node *n);

    Vector<Node *> GetNeighbors(Node *node, bool allowDiagonals = false);
};

#endif