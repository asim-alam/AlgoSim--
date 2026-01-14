#include "Grid.h"

Grid::Grid(int w, int h, int size) : width(w), height(h), cellSize(size), startNode(nullptr), endNode(nullptr)
{
    nodes = new Node *[height];
    for (int y = 0; y < height; y++)
    {
        nodes[y] = new Node[width];
        for (int x = 0; x < width; x++)
        {
            nodes[y][x].x = x;
            nodes[y][x].y = y;
            nodes[y][x].type = NODE_EMPTY;
        }
    }
}

Grid::~Grid()
{
    for (int y = 0; y < height; y++)
    {
        delete[] nodes[y];
    }
    delete[] nodes;
}

void Grid::Draw()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int color = 0x2D2D2D; // Empty Cell (Dark Gray)
            switch (nodes[y][x].type)
            {
            case NODE_WALL:
                color = 0x808080;
                break; // Wall (Light Gray)
            case NODE_START:
                color = 0x00FF00;
                break; // Start (Green)
            case NODE_END:
                color = 0xFF0000;
                break; // End (Red)
            case NODE_VISITED:
                color = 0x4A90E2;
                break; // Visited (Nice Blue)
            case NODE_PATH:
                color = 0xF5A623;
                break; // Path (Gold/Orange)
            case NODE_FRONTIER:
                color = 0xBD10E0;
                break; // Frontier (Purple)
            case NODE_BOOST:
                color = 0x00FFFF;
                break; // Boost (Cyan) - Negative Weight
            case NODE_MUD:
                color = 0x8B4513;
                break; // Mud (Brown) - Positive Weight
            default:
                break;
            }

            // grid lines (background is 0x1E1E1E)
            Graphics::DrawRect(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2, color);
        }
    }
}

void Grid::Resize(int w, int h)
{
    // Clean up old nodes
    if (nodes)
    {
        for (int i = 0; i < height; i++)
        {
            delete[] nodes[i];
        }
        delete[] nodes;
    }

    
    width = w;
    height = h;

    // Allocate new nodes
    nodes = new Node *[height];
    for (int i = 0; i < height; i++)
    {
        nodes[i] = new Node[width];
        for (int j = 0; j < width; j++)
        {
            nodes[i][j].x = j;
            nodes[i][j].y = i;
            nodes[i][j].type = NODE_EMPTY;
        }
    }


    startNode = nullptr;
    endNode = nullptr;

    // Set default start/end if within bounds
    if (width > 5 && height > 5)
    {
        SetStartNode(&nodes[5][5]);
    }
    if (width > 35 && height > 25)
    {
        SetEndNode(&nodes[25][35]);
    }
    else if (width > 10 && height > 10)
    {
        SetEndNode(&nodes[height - 5][width - 5]);
    }
}

void Grid::Reset()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            nodes[y][x].type = NODE_EMPTY;
            nodes[y][x].g = nodes[y][x].h = nodes[y][x].f = 0;
            nodes[y][x].parent = nullptr;
            nodes[y][x].visited = false;
        }
    }
    startNode = nullptr;
    endNode = nullptr;
}

void Grid::ResetPath()
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (nodes[y][x].type == NODE_VISITED || nodes[y][x].type == NODE_PATH || nodes[y][x].type == NODE_FRONTIER)
            {
                nodes[y][x].type = NODE_EMPTY;
            }
            nodes[y][x].g = nodes[y][x].h = nodes[y][x].f = 0;
            nodes[y][x].parent = nullptr;
            nodes[y][x].visited = false;
        }
    }
}

Node *Grid::GetNode(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return &nodes[y][x];
    }
    return nullptr;
}

void Grid::SetType(int x, int y, NodeType type)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        nodes[y][x].type = type;
    }
}

void Grid::SetStartNode(Node *n)
{
    if (startNode)
        startNode->type = NODE_EMPTY;
    startNode = n;
    if (startNode)
        startNode->type = NODE_START;
}

void Grid::SetEndNode(Node *n)
{
    if (endNode)
        endNode->type = NODE_EMPTY;
    endNode = n;
    if (endNode)
        endNode->type = NODE_END;
}

Vector<Node *> Grid::GetNeighbors(Node *node, bool allowDiagonals)
{
    Vector<Node *> neighbors;
    int x = node->x;
    int y = node->y;

    int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
    int dy[] = {-1, 1, 0, 0, -1, 1, -1, 1};

    int numDirs = allowDiagonals ? 8 : 4;

    for (int i = 0; i < numDirs; i++)
    {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < width && ny >= 0 && ny < height)
        {
            if (nodes[ny][nx].type != NODE_WALL)
            {
                neighbors.push_back(&nodes[ny][nx]);
            }
        }
    }
    return neighbors;
}
