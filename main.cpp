#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <glm/glm.hpp>

#include "linear_algebra/linalg.h"
#include "loading/WavefrontObj.h"

constexpr double     PI      = 3.14159265358979323846;
constexpr double     EPSILON = 1e-12;
constexpr glm::dvec3 wind(0.0, 1.0, 0.0);  // Coming from +x

glm::dvec3 vortex_segment_induced_velocity(const glm::dvec3& A, const glm::dvec3& B, const glm::dvec3& P, const double gamma)
{
    const glm::dvec3 r1 = P - A;
    const glm::dvec3 r2 = P - B;
    const glm::dvec3 r0 = B - A;

    const glm::dvec3 r0_hat      = glm::normalize(r0);
    const glm::dvec3 cross_r1_r2 = glm::cross(r1, r2);
    const double     norm_cross  = glm::dot(cross_r1_r2, cross_r1_r2);

    if (norm_cross < EPSILON)
        return glm::dvec3(0.0); // Avoid singularity

    const double term1 = glm::dot(r1, r0_hat) / glm::length(r1);
    const double term2 = glm::dot(r2, r0_hat) / glm::length(r2);

    const glm::dvec3 induced = (gamma / (4.0 * PI)) * (cross_r1_r2 / norm_cross) * (term1 - term2);
    return induced;
}

glm::dvec3 triangle_panel_induced_velocity_unit(const glm::dvec3& P, const glm::dvec3& v0, const glm::dvec3& v1, const glm::dvec3& v2)
{
    glm::dvec3 vel(0.0);
    vel += vortex_segment_induced_velocity(v0, v1, P, 1.0);
    vel += vortex_segment_induced_velocity(v1, v2, P, 1.0);
    vel += vortex_segment_induced_velocity(v2, v0, P, 1.0);
    return vel;
}

int main()
{
    std::vector<WavefrontObj::Vertex> vertices;
    std::vector<uint32_t> indices;
    WavefrontObj::LoadObj("../wing.obj", vertices, indices);

    struct Triangle
    {
        glm::dvec3 a, b, c;
        glm::dvec3 normal;
        glm::dvec3 midpoint;
        double area;
    };

    std::vector<Triangle> triangles;
    for(size_t i = 0; i < indices.size() / 3; i++)
    {
        Triangle &working = triangles.emplace_back();
        working.a = vertices[indices[i * 3 + 0]].Position;
        working.b = vertices[indices[i * 3 + 1]].Position;
        working.c = vertices[indices[i * 3 + 2]].Position;

        working.normal = vertices[indices[i * 3 + 0]].Normal;
        working.midpoint = (working.a + working.b + working.c) / 3.;

        working.area = 0.5 * glm::length(glm::cross(working.a - working.b, working.c - working.a));
    }


    const size_t DIMENSION = triangles.size();

    linalg::matrix A(DIMENSION);
    linalg::matrix B(DIMENSION, 1);

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const glm::dvec3& Pi = triangles[i].midpoint;
        const glm::dvec3& ni = triangles[i].normal;

        for (size_t j = 0; j < triangles.size(); ++j)
        {
            const auto& tri = triangles[j];
            glm::dvec3 Vij = triangle_panel_induced_velocity_unit(Pi, tri.a, tri.b, tri.c);
            A(i, j) = glm::dot(Vij, ni) + (i == j ? EPSILON : 0.);
        }

        B(i, 0) = -glm::dot(wind, ni);
    }

    std::cout<<A.to_string()<<std::endl;

    auto GAMMA = linalg::solve_LU(A, B);


    const double V_inf  = glm::length(wind);
    const glm::dvec3 wind_dir = glm::normalize(wind);
    const glm::dvec3 lift_dir(0.0, 0.0, 1.0);  // +Z direction for lift

    double total_lift_contribution = 0.0;
    double S = 0.0;

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const auto& tri = triangles[i];
        double gamma = GAMMA(i, 0);

        double projected_area = tri.area * std::abs(glm::dot(tri.normal, wind_dir));
        S += projected_area;

        double lift_factor = glm::dot(glm::cross(wind_dir, tri.normal), lift_dir);
        total_lift_contribution += gamma * tri.area * lift_factor;
    }

    // Use circulation-based lift estimate
    double CL = (2.0 / (V_inf * S)) * total_lift_contribution;

    std::cout << "Lift coefficient (C_L) = " << CL << std::endl;

    return 0;
}
