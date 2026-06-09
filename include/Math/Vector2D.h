#pragma once

struct Vector2D
{
    float x;
    float y;
    
    Vector2D() : x(0), y(0) {}
    Vector2D(float in_x, float in_y) : x(in_x), y(in_y) {}
    
    static Vector2D zero;

    Vector2D operator*(float n) const { return Vector2D(x * n, y * n); }
    Vector2D operator/(float n) const { return Vector2D(x / n, y / n); }
    Vector2D operator+(float n) const { return Vector2D(x + n, y + n); }
    Vector2D operator-(float n) const { return Vector2D(x - n, y - n); }
    Vector2D operator+(Vector2D const& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(Vector2D const& other) const { return Vector2D(x - other.x, y - other.y); }
    void operator+=(Vector2D const& other) { x += other.x; y += other.y; }
    void operator-=(Vector2D const& other) { x -= other.x; y -= other.y; }
    bool operator==(Vector2D const& other) const { return x == other.x && y == other.y; }
    bool operator!=(Vector2D const& other) const { return !(*this == other); }
};