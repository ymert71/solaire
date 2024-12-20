

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Sphere.h"

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT  = 2;

Sphere::Sphere(float radius, int sectors, int stacks, bool smooth) : interleavedStride(32)
{
    set(radius, sectors, stacks, smooth);
}

void Sphere::set(float radius, int sectors, int stacks, bool smooth)
{
    this->radius = radius;
    this->sectorCount = sectors;
    if(sectors < MIN_SECTOR_COUNT)
        this->sectorCount = MIN_SECTOR_COUNT;
    this->stackCount = stacks;
    if(sectors < MIN_STACK_COUNT)
        this->sectorCount = MIN_STACK_COUNT;
    this->smooth = smooth;

    if(smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();
}

void Sphere::setRadius(float radius)
{
    if(radius != this->radius)
        set(radius, sectorCount, stackCount, smooth);
}

void Sphere::setSectorCount(int sectors)
{
    if(sectors != this->sectorCount)
        set(radius, sectors, stackCount, smooth);
}

void Sphere::setStackCount(int stacks)
{
    if(stacks != this->stackCount)
        set(radius, sectorCount, stacks, smooth);
}

void Sphere::setSmooth(bool smooth)
{
    if(this->smooth == smooth)
        return;

    this->smooth = smooth;
    if(smooth)
        buildVerticesSmooth();
    else
        buildVerticesFlat();
}




void Sphere::printSelf() const
{
    std::cout << "===== Sphere =====\n"
              << "        Radius: " << radius << "\n"
              << "  Sector Count: " << sectorCount << "\n"
              << "   Stack Count: " << stackCount << "\n"
              << "Smooth Shading: " << (smooth ? "true" : "false") << "\n"
              << "Triangle Count: " << getTriangleCount() << "\n"
              << "   Index Count: " << getIndexCount() << "\n"
              << "  Vertex Count: " << getVertexCount() << "\n"
              << "  Normal Count: " << getNormalCount() << "\n"
              << "TexCoord Count: " << getTexCoordCount() << std::endl;
}

void Sphere::draw() const
{

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, interleavedStride, &interleavedVertices[0]);
    glNormalPointer(GL_FLOAT, interleavedStride, &interleavedVertices[3]);
    glTexCoordPointer(2, GL_FLOAT, interleavedStride, &interleavedVertices[6]);

    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Sphere::drawLines(const float lineColor[4]) const
{

    glColor4fv(lineColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   lineColor);


    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices.data());

    glDrawElements(GL_LINES, (unsigned int)lineIndices.size(), GL_UNSIGNED_INT, lineIndices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}

void Sphere::drawWithLines(const float lineColor[4]) const
{
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0f);
    this->draw();
    glDisable(GL_POLYGON_OFFSET_FILL);


    drawLines(lineColor);
}

void Sphere::clearArrays()
{
    std::vector<float>().swap(vertices);
    std::vector<float>().swap(normals);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(indices);
    std::vector<unsigned int>().swap(lineIndices);
}

void Sphere::buildVerticesSmooth()
{
    const float PI = acos(-1);


    clearArrays();

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);


        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;


            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            addVertex(x, y, z);


            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            addNormal(nx, ny, nz);


            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            addTexCoord(s, t);
        }
    }


    unsigned int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {

            if(i != 0)
            {
                addIndices(k1, k2, k1+1);
            }

            if(i != (stackCount-1))
            {
                addIndices(k1+1, k2, k2+1);
            }


            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }


    buildInterleavedVertices();
}

void Sphere::buildVerticesFlat()
{
    const float PI = acos(-1);


    struct Vertex
    {
        float x, y, z, s, t;
    };
    std::vector<Vertex> tmpVertices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;


    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);


        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            Vertex vertex;
            vertex.x = xy * cosf(sectorAngle);
            vertex.y = xy * sinf(sectorAngle);
            vertex.z = z;
            vertex.s = (float)j/sectorCount;
            vertex.t = (float)i/stackCount;
            tmpVertices.push_back(vertex);
        }
    }


    clearArrays();

    Vertex v1, v2, v3, v4;
    std::vector<float> n;

    int i, j, k, vi1, vi2;
    int index = 0;
    for(i = 0; i < stackCount; ++i)
    {
        vi1 = i * (sectorCount + 1);
        vi2 = (i + 1) * (sectorCount + 1);

        for(j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
        {

            v1 = tmpVertices[vi1];
            v2 = tmpVertices[vi2];
            v3 = tmpVertices[vi1 + 1];
            v4 = tmpVertices[vi2 + 1];


            if(i == 0)
            {

                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v4.x, v4.y, v4.z);


                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v4.s, v4.t);


                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v4.x,v4.y,v4.z);
                for(k = 0; k < 3; ++k)
                {
                    addNormal(n[0], n[1], n[2]);
                }


                addIndices(index, index+1, index+2);


                lineIndices.push_back(index);
                lineIndices.push_back(index+1);

                index += 3;
            }
            else if(i == (stackCount-1))
            {

                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);


                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);


                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 3; ++k)
                {
                    addNormal(n[0], n[1], n[2]);
                }


                addIndices(index, index+1, index+2);


                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 3;
            }
            else
            {

                addVertex(v1.x, v1.y, v1.z);
                addVertex(v2.x, v2.y, v2.z);
                addVertex(v3.x, v3.y, v3.z);
                addVertex(v4.x, v4.y, v4.z);


                addTexCoord(v1.s, v1.t);
                addTexCoord(v2.s, v2.t);
                addTexCoord(v3.s, v3.t);
                addTexCoord(v4.s, v4.t);


                n = computeFaceNormal(v1.x,v1.y,v1.z, v2.x,v2.y,v2.z, v3.x,v3.y,v3.z);
                for(k = 0; k < 4; ++k)
                {
                    addNormal(n[0], n[1], n[2]);
                }


                addIndices(index, index+1, index+2);
                addIndices(index+2, index+1, index+3);


                lineIndices.push_back(index);
                lineIndices.push_back(index+1);
                lineIndices.push_back(index);
                lineIndices.push_back(index+2);

                index += 4;
            }
        }
    }


    buildInterleavedVertices();
}

void Sphere::buildInterleavedVertices()
{
    std::vector<float>().swap(interleavedVertices);

    std::size_t i, j;
    std::size_t count = vertices.size();
    for(i = 0, j = 0; i < count; i += 3, j += 2)
    {
        interleavedVertices.push_back(vertices[i]);
        interleavedVertices.push_back(vertices[i+1]);
        interleavedVertices.push_back(vertices[i+2]);

        interleavedVertices.push_back(normals[i]);
        interleavedVertices.push_back(normals[i+1]);
        interleavedVertices.push_back(normals[i+2]);

        interleavedVertices.push_back(texCoords[j]);
        interleavedVertices.push_back(texCoords[j+1]);
    }
}


void Sphere::addVertex(float x, float y, float z)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Sphere::addNormal(float nx, float ny, float nz)
{
    normals.push_back(nx);
    normals.push_back(ny);
    normals.push_back(nz);
}


void Sphere::addTexCoord(float s, float t)
{
    texCoords.push_back(s);
    texCoords.push_back(t);
}

void Sphere::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
    indices.push_back(i1);
    indices.push_back(i2);
    indices.push_back(i3);
}

std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,
                                             float x2, float y2, float z2,
                                             float x3, float y3, float z3)
{
    const float EPSILON = 0.000001f;

    std::vector<float> normal(3, 0.0f);
    float nx, ny, nz;


    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;


    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;


    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if(length > EPSILON)
    {

        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}
