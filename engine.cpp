#include "Graphics.h"
#include "Grid.h"
#include "Pathfinding.h"
#include <iostream>
#include <windows.h>
#include <ctime>
using namespace std;

struct SimulationState
{
    Grid *grid;
    Algorithm *currentAlgorithm;
    bool running;
    bool finished;
} simState;

void OnResize(int w, int h)
{
    const int cellSize = 20;
    if (simState.grid)
    {
        simState.running = false; // Stop simulation
        simState.grid->Resize(w / cellSize, h / cellSize);
        // Re-init algorithm if selected
        if (simState.currentAlgorithm)
        {
            simState.currentAlgorithm->Init(simState.grid);
        }
    }
}

int main()
{
    srand(time(0));

    bool appRunning = true;
    while (appRunning)
    {
        // CLI Selection
        cout << "\n=== 2D Grid Algorithm Simulation ===\n";
        cout << "Select an Algorithm:\n";

        cout << "1. A* Algorithm\n";
        cout << "2. Dijkstra's Algorithm\n";
        cout << "3. Greedy Best-First Search\n";
        cout << "4. Bellman-Ford Algorithm\n";
        cout << "5. Conway's Game of Life\n";
        cout << "6. Ant Colony Optimization\n";
        cout << "7. Q-Learning\n";
        cout << "8. Bresenham's Line Demo\n";
        cout << "9. Dynamic A* (Replanning)\n";
        cout << "10. Exit\n";
        cout << "Enter choice (1-10): ";

        int choice;
        cin >> choice;

        if (choice == 10)
        {
            appRunning = false;
            break;
        }

        const int cellSize = 20;
        int width = 800;
        int height = 600;

        Graphics::SetResizeCallback(OnResize);

        if (!Graphics::Init(width, height, "SPL1 - Algorithm Simulation"))
        {
            cerr << "Error: Failed to initialize graphics window.\n";
        }

        Grid grid(width / cellSize, height / cellSize, cellSize);
        simState.grid = &grid;
        simState.currentAlgorithm = nullptr;
        simState.running = false;
        simState.finished = false;

        AStar astar;
        Dijkstra dijkstra;
        GreedyBFS greedy;
        BellmanFord bellman;

        // Initial setup
        grid.SetStartNode(grid.GetNode(5, 5));
        grid.SetEndNode(grid.GetNode(35, 25));

        // Set algorithm based on choice
        switch (choice)
        {

        case 1:
            simState.currentAlgorithm = &astar;
            break;
        case 2:
            simState.currentAlgorithm = &dijkstra;
            break;
        case 3:
            simState.currentAlgorithm = &greedy;
            break;
        case 4: simState.currentAlgorithm = &bellman; break;
        default:
            cout << "Invalid choice. Defaulting to A*.\n";
            simState.currentAlgorithm = &dijkstra;
            break;
        }

        if (simState.currentAlgorithm)
        {
            cout << "Selected: " << simState.currentAlgorithm->GetName() << "\n";
            cout << "Press SPACE to Start/Pause. R to Reset. C to Clear Path.\n";
            cout << "Left Click: Wall. Right Click: Clear.\n";
            cout << "S: Set Start. E: Set End.\n";
            cout << "ESC: Exit to Menu.\n";
        }

        while (Graphics::ProcessMessages())
        {

            if (Graphics::IsKeyDown(27))
            {          // ESC
                break; // Exit simulation loop
            }

            int mx = Graphics::GetMouseX();
            int my = Graphics::GetMouseY();
            int gx = mx / cellSize;
            int gy = my / cellSize;

            if (Graphics::IsMouseButtonDown(0))
            { // Left Click
                Node *n = grid.GetNode(gx, gy);
                if (n && n->type != NODE_START && n->type != NODE_END)
                {
                    grid.SetType(gx, gy, NODE_WALL);
                }
            }
            if (Graphics::IsMouseButtonDown(1))
            { // Right Click
                Node *n = grid.GetNode(gx, gy);
                if (n && n->type != NODE_START && n->type != NODE_END)
                {
                    grid.SetType(gx, gy, NODE_EMPTY);
                }
            }

            if (Graphics::IsKeyDown('S'))
            {
                Node *n = grid.GetNode(gx, gy);
                if (n)
                    grid.SetStartNode(n);
            }
            if (Graphics::IsKeyDown('E'))
            {
                Node *n = grid.GetNode(gx, gy);
                if (n)
                    grid.SetEndNode(n);
            }

            if (Graphics::IsKeyDown(' '))
            { // Space to start
                if (simState.currentAlgorithm && !simState.running)
                {
                    simState.currentAlgorithm->Init(&grid);
                    simState.running = true;
                    simState.finished = false;
                }
                else if (simState.currentAlgorithm && simState.running)
                {
                    simState.running = false; // Pause
                }
            }

            if (Graphics::IsKeyDown('R'))
            { // Reset
                grid.Reset();
                simState.running = false;
                simState.finished = false;
                // Reset start/end for convenience
                if (grid.GetWidth() > 35 && grid.GetHeight() > 25)
                {
                    grid.SetStartNode(grid.GetNode(5, 5));
                    grid.SetEndNode(grid.GetNode(35, 25));
                }
            }

            if (Graphics::IsKeyDown('C'))
            { // Clear Path only
                grid.ResetPath();
                simState.running = false;
                simState.finished = false;
            }

            // Update
            if (simState.running && simState.currentAlgorithm)
            {
                if (simState.currentAlgorithm->Step())
                {
                    simState.running = false;
                    simState.finished = true;
                }
            }

            Graphics::Clear(0x1E1E1E); // Dark Gray Background
            grid.Draw();
            Graphics::Present();

            Sleep(10);
        }

        Graphics::Shutdown();
    }
    return 0;
}