#pragma once

#include <random>

static std::mt19937 mt;
static std::uniform_real_distribution<double> u(0.0, 1.0);

static double mtunif() { return u(mt); }
static void mtinit(int seed) { mt.seed(seed); }