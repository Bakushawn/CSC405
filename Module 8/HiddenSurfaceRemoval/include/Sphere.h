#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

struct triangle {
            std::vector<float> point1;
            std::vector<float> point2;
            std::vector<float> point3;
        };

class Sphere
{
    private:
        std::vector<float> point1;
        std::vector<float> point2;
        std::vector<float> point3;
        std::vector<float> point4;

        void flattenVerticesArray();
        void generateVertices(int subDivideCount);
        void pushbackTriangle(triangle newTriangle);
        void subdivide(triangle originTriangle, int subDivideCount);

        std::vector<float> findNormalizedMidPoint(std::vector<float> p1, std::vector<float> p2);
        std::vector<float> NormilizePoint(std::vector<float> point,std::vector<float> vector);
        float InverseSquare(std::vector<float> p1);

    public:
        std::vector<std::vector<float>> vertices;
        std::vector<float> flatVertexArray;
        std::vector<float> position;

        Sphere();
        Sphere(std::vector<float> p1, std::vector<float> p2, std::vector<float> p3,std::vector<float> p4, std::vector<float> pos, int subDivisions);
};
#endif