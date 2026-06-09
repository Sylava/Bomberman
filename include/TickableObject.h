#pragma once

class TickableObject
{
public:
    TickableObject();
    virtual ~TickableObject();

    virtual void tick(float deltaTime) = 0;
};