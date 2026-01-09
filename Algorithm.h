#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Grid.h"

class Algorithm
{
protected:
    Grid *grid;
    bool initialized;
    bool finished;

public:
    Algorithm() : grid(nullptr), initialized(false), finished(false) {}
    virtual ~Algorithm() {}

    virtual void Init(Grid *g)
    {
        grid = g;
        initialized = true;
        finished = false;
        grid->ResetPath();
    }

    virtual bool Step() = 0;
    virtual const char *GetName() = 0;

    bool IsFinished() const { return finished; }
};

#endif
