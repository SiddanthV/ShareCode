#include "frame.h"
#include <math.h>
#include <stdio.h>

double project(double projection, double z);
int main()
{
    if (!frameSetup())
    {
        printf("Memory problems lol or Terminal Setup is very very wrong");
    }
    clearFrame(); // Run it once to set each value in frame to -

    int cubeVertex[8][3] = {
        {-1, -1, -1}, // Vertex 0
        {1, -1, -1},  // Vertex 1
        {1, 1, -1},   // Vertex 2
        {-1, 1, -1},  // Vertex 3
        {-1, -1, 1},  // Vertex 4
        {1, -1, 1},   // Vertex 5
        {1, 1, 1},    // Vertex 6
        {-1, 1, 1}    // Vertex 7
    };
    double angle = 0;
    for (int v = 0; v < 8; v++)
    {
        int x = cubeVertex[v][0];
        int y = cubeVertex[v][1];
        int z = cubeVertex[v][2];
        cubeVertex[v][1] = y * cos(angle) - z * sin(angle);
        cubeVertex[v][2] = y * cos(angle) + z * sin(angle);
    }

    int projection[8][2];
    int camera = 4;
    int shift = 15;

    for (int i = 0; i < 8; i++)
    {
        // Project 3D coordinates to 2D
        projection[i][0] = (int) (cubeVertex[i][0] * camera) + shift;
        projection[i][1] = (int) (cubeVertex[i][1] * camera) + shift;
    }

    // Connect the cube edges
    for (int i = 0; i < 4; i++)
    {
        int j = (i + 1) % 4;
        int k = i + 4;
        DrawLine(projection[i][0], projection[i][1], projection[j][0], projection[j][1]);
        DrawLine(projection[i][0], projection[i][1], projection[k][0], projection[k][1]);
        DrawLine(projection[k][0], projection[k][1], projection[(j + 4) % 8][0], projection[(j + 4) % 8][1]);
    }

    // Connect the top and bottom faces
    for (int i = 0; i < 4; i++)
    {
        DrawLine(projection[i][0], projection[i][1], projection[i + 4][0], projection[i + 4][1]);
    }
    printFrame();

    freeFrame();
}
