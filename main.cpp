#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <span>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>

#include "linear_algebra/linalg.h"

constexpr double     PI      = 3.14159265358979323846;
constexpr double     EPSILON = 1e-12;
constexpr glm::dvec2 wind(-1.0, -1.0);

void subdivide(std::vector<glm::dvec2> &in_points)
{
    const size_t count = in_points.size();
    std::vector<glm::dvec2> new_points; new_points.reserve(2 * count);
    for (size_t x = 0; x < count; ++x)
    {
        const glm::dvec2 &p0  = in_points[x];
        const glm::dvec2 &p1  = in_points[(x + 1) % count];
        new_points.push_back(0.75 * p0 + 0.25 * p1);
        new_points.push_back(0.25 * p0 + 0.75 * p1);
    }
    in_points.swap(new_points);
}

void make_panels(const std::vector<glm::dvec2> &points, std::vector<std::pair<glm::dvec2, glm::dvec2>> &out)
{
    out.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        out.emplace_back(points[i], points[(i + 1) % points.size()]);
}

void calculate_midpoints(const std::vector<std::pair<glm::dvec2, glm::dvec2>> &panels, std::vector<glm::dvec2> &out)
{
    out.reserve(panels.size());
    for (const auto &[fst, snd] : panels)
        out.push_back(0.5 * (fst + snd));
}

void calculate_tangents(const std::vector<std::pair<glm::dvec2, glm::dvec2>>& panels, std::vector<glm::dvec2> &out)
{
    out.reserve(panels.size());
    for (const auto &[fst, snd] : panels)
        out.push_back(glm::normalize(snd - fst));
}

void normals_from_tangents(const std::vector<glm::dvec2>& tangents, std::vector<glm::dvec2> &out)
{
    out.reserve(tangents.size());
    for (const auto& t : tangents)
        out.emplace_back(t[1], -t[0]);
}

double matrix_entry_at(const size_t i, const size_t j, const std::vector<glm::dvec2>& vor, const std::vector<glm::dvec2>& col, const std::vector<glm::dvec2>& normals) {
    const glm::dvec2 &c = col[i];
    const glm::dvec2 &v = vor[j];
    const glm::dvec2  d = c - v;
    const double r_sq   = std::max(glm::dot(d, d), EPSILON);
    const double u      =  (1.0 / (2.0 * PI * r_sq)) * (c[1] - v[1]);
    const double v_comp = -(1.0 / (2.0 * PI * r_sq)) * (c[0] - v[0]);
    return u * normals[i][0] + v_comp * normals[i][1];
}

int main()
{
    std::vector<glm::dvec2> points = {
        {-2.0,  0.00},
        {-1.5,  0.35},
        {-0.8,  0.60},
        {-0.2,  0.70},
        { 0.2,  0.60},
        { 0.6,  0.50},
        { 2.0,  0.00},
        { 1.2, -0.15},
        { 0.5, -0.30},
        { 0.0,  0.40},
        {-0.5,  0.20},
        {-1.2, -0.15}
    };

    subdivide(points);
    subdivide(points);

    std::vector<std::pair<glm::dvec2, glm::dvec2>> panels;    make_panels(          points,   panels);
    std::vector<glm::dvec2>                        midpoints; calculate_midpoints(  panels,   midpoints);
    std::vector<glm::dvec2>                        tangents;  calculate_tangents(   panels,   tangents);
    std::vector<glm::dvec2>                        normals;   normals_from_tangents(tangents, normals);

    const size_t DIMENSION = points.size();

    linalg::matrix A(DIMENSION);
    linalg::matrix B(DIMENSION, 1);

    for(size_t i = 0; i < DIMENSION; i++)
    {
        B(i, 0) = glm::dot(normals[i], wind);
        for(size_t j = 0; j < DIMENSION; j++)
            A(i, j) = matrix_entry_at(i, j, midpoints, points, normals);
    }

    auto GAMMA = linalg::solve_LU(A, B);


    // Circulation
    double gamma_total = 0.0;
    for (size_t i = 0; i < GAMMA.get_dimension_x(); ++i)
        gamma_total += GAMMA(i, 0) * glm::length(panels[i].second - panels[i].first);

    const     double V_inf = glm::length(wind);
    constexpr double chord = 2.0;
    const     double C_L   = -2. * gamma_total / (chord * V_inf);

    std::cout << "Lift coefficient C_L: " << C_L << std::endl;
    return 0;
}
