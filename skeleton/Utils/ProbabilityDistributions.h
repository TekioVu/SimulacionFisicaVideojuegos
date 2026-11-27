#pragma once
#include <random>

inline std::mt19937 _mt;

enum Distribution
{
    UNIFORM,
    NORMAL
};

inline double UniformDistribution(double min, double max)
{
    std::uniform_real_distribution<double> u(min, max);
    return u(_mt);
}

inline double NormalDistribution(double mean, double sd)
{
    if (sd <= 0.0)
        return mean;

    std::normal_distribution<double> n(mean, sd);
    return n(_mt);
}