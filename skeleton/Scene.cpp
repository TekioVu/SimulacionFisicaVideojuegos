#include "Scene.h"

Vector4 Scene::Color(Colors color)
{
    switch (color) {
    case RED:
        return Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    case DARK_RED:
        return Vector4(0.5f, 0.0f, 0.0f, 1.0f);
    case GREEN:
        return Vector4(0.0f, 1.0f, 0.0f, 1.0f);
    case DARK_GREEN:
        return Vector4(0.0f, 0.5f, 0.0f, 1.0f);
    case BLUE:
        return Vector4(0.0f, 0.0f, 1.0f, 1.0f);
    case YELLOW:
        return Vector4(1.0f, 1.0f, 0.0f, 1.0f);
    case BLACK:
        return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    case GRAY:
        return Vector4(0.5f, 0.5f, 0.5f, 1.0f);
    case WHITE:
        return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    default:
        return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}