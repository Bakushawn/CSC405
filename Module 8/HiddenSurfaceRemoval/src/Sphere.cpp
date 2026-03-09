#include "Sphere.h"

#include <iostream>
#include <cmath>

Sphere::Sphere(){
    point1 = { 0.0f, 0.0f, 0.0f };
    point2 = { 1.0f, 1.0f, 1.0f };
    point3 = { 0.0f, 1.0f, 1.0f };
    point4 = { 1.0f, 0.0f, 1.0f };
    position = {0.0f, 0.0f, 0.0f};

    generateVertices(0);
    flattenVerticesArray();
}

Sphere::Sphere(std::vector<float> p1, std::vector<float> p2, std::vector<float> p3,std::vector<float> p4, std::vector<float> pos, int subDivisions){
    point1 = p1;
    point2 = p2;
    point3 = p3;
    point4 = p4;
    position = pos;

    generateVertices(subDivisions);
    flattenVerticesArray();
}

// Private functions
void Sphere::flattenVerticesArray(){
    int currentIndex = 0;
    for(std::vector<float> Vertex : vertices){
        for(float point : Vertex){
            flatVertexArray.push_back(point);
            currentIndex += 1;
        }
    }
}

void Sphere::generateVertices(int subDivideCount){
    triangle face1;
    face1.point1 = point1;
    face1.point2 = point2;
    face1.point3 = point3;

    triangle face2;
    face2.point1 = point1;
    face2.point2 = point2;
    face2.point3 = point4;

    triangle face3;
    face3.point1 = point2;
    face3.point2 = point3;
    face3.point3 = point4;

    triangle face4;
    face4.point1 = point1;
    face4.point2 = point3;
    face4.point3 = point4;

    subdivide(face1, subDivideCount);
    subdivide(face2, subDivideCount);
    subdivide(face3, subDivideCount);
    subdivide(face4, subDivideCount);
}

void Sphere::pushbackTriangle(triangle newTriangle){
    vertices.push_back(newTriangle.point1);
    vertices.push_back(newTriangle.point2);
    vertices.push_back(newTriangle.point3);
}

void Sphere::subdivide(triangle originTriangle, int subDivideCount){

    // originTriangle.point1 = top of triangle, originTriangle.point2 = left, originTriangle.point3 = right
    if(subDivideCount > 0){
        // only middle traingle needs to be normalized
        triangle middle;
        middle.point1 = findNormalizedMidPoint(originTriangle.point2, originTriangle.point1); // mid point between left vertex and top vertex
        middle.point2 = findNormalizedMidPoint(originTriangle.point1, originTriangle.point3); // mid point between top vertex and right vertex
        middle.point3 = findNormalizedMidPoint(originTriangle.point3, originTriangle.point2); // mid point between right vertex and left top vertex
        triangle top;
        top.point1 = originTriangle.point1; // top vertex of original Triangle
        top.point2 = middle.point1; // mid point between top vertex and left vertex
        top.point3 = middle.point2; // mid point between top vertex and right vertex
        triangle left;
        left.point1 = originTriangle.point2; // left vertex of original triangle
        left.point2 = middle.point1; // mid point between left vertex and top vertex
        left.point3 = middle.point3; // mid point between left vertex and right vertex
        triangle right;
        right.point1 = originTriangle.point3; // right vertex of original triangle
        right.point2 = middle.point2;// mid point between right vertex and top vertex
        right.point3 = middle.point3;// mid point between right vertex and left vertex
        

        subdivide(top, subDivideCount - 1);
        subdivide(left, subDivideCount - 1);
        subdivide(right, subDivideCount - 1);
        subdivide(middle, subDivideCount - 1);
    }
    else{
        pushbackTriangle(originTriangle);
    }

}

std::vector<float> Sphere::findNormalizedMidPoint(std::vector<float> p1, std::vector<float> p2){
    std::vector<float> midpoint = {0.0f, 0.0f, 0.0f};

    midpoint[0] = (p1[0] + p2[0]) / 2;
    midpoint[1] = (p1[1] + p2[1]) / 2;
    midpoint[2] = (p1[2] + p2[2]) / 2;

    float normal = InverseSquare(midpoint);
    midpoint[0] = midpoint[0] * normal;
    midpoint[1] = midpoint[1] * normal;
    midpoint[2] = midpoint[2] * normal;

    return midpoint;
}

std::vector<float> Sphere::NormilizePoint(std::vector<float> point,std::vector<float> vector){
    std::vector<float> normalized = {0.0f, 0.0f, 0.0f};

    float normal = InverseSquare(vector);

    normalized[0] = point[0] * normal;
    normalized[1] = point[1] * normal;
    normalized[2] = point[2] * normal;

    return normalized;
}

float Sphere::InverseSquare(std::vector<float> p1){
    return 1.0f / sqrt((p1[0]*p1[0]) + (p1[1] * p1[1]) + (p1[2] * p1[2]));
}