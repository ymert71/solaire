#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <vector>

class Sphere{
public:
    Sphere(float radius=1.0f, int sectorCount=36, int stackCount=18, bool smooth=true);
    ~Sphere() {}
    float getRadius() const                 { return radius; }
    int getSectorCount() const              { return sectorCount; }
    int getStackCount() const               { return stackCount; }
    void set(float radius, int sectorCount, int stackCount, bool smooth=true);
    void setRadius(float radius);
    void setSectorCount(int sectorCount);
    void setStackCount(int stackCount);
    void setSmooth(bool smooth);

    unsigned int getVertexCount() const     { return (unsigned int)vertices.size() / 3; }
    unsigned int getNormalCount() const     { return (unsigned int)normals.size() / 3; }
    unsigned int getTexCoordCount() const   { return (unsigned int)texCoords.size() / 2; }
    unsigned int getIndexCount() const      { return (unsigned int)indices.size(); }
    unsigned int getLineIndexCount() const  { return (unsigned int)lineIndices.size(); }
    unsigned int getTriangleCount() const   { return getIndexCount() / 3; }
    unsigned int getVertexSize() const      { return (unsigned int)vertices.size() * sizeof(float); }
    unsigned int getNormalSize() const      { return (unsigned int)normals.size() * sizeof(float); }
    unsigned int getTexCoordSize() const    { return (unsigned int)texCoords.size() * sizeof(float); }
    unsigned int getIndexSize() const       { return (unsigned int)indices.size() * sizeof(unsigned int); }
    unsigned int getLineIndexSize() const   { return (unsigned int)lineIndices.size() * sizeof(unsigned int); }
    const float* getVertices() const        { return vertices.data(); }
    const float* getNormals() const         { return normals.data(); }
    const float* getTexCoords() const       { return texCoords.data(); }
    const unsigned int* getIndices() const  { return indices.data(); }
    const unsigned int* getLineIndices() const  { return lineIndices.data(); }

    unsigned int getInterleavedVertexCount() const  { return getVertexCount(); }
    unsigned int getInterleavedVertexSize() const   { return (unsigned int)interleavedVertices.size() * sizeof(float); }
    int getInterleavedStride() const                { return interleavedStride; }
    const float* getInterleavedVertices() const     { return interleavedVertices.data(); }

    void draw() const;
    void drawLines(const float lineColor[4]) const;
    void drawWithLines(const float lineColor[4]) const;
    void printSelf() const;

protected:

private:
    void buildVerticesSmooth();
    void buildVerticesFlat();
    void buildInterleavedVertices();
    void clearArrays();
    void addVertex(float x, float y, float z);
    void addNormal(float x, float y, float z);
    void addTexCoord(float s, float t);
    void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
    std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);
    float radius;
    int sectorCount;
    int stackCount;
    bool smooth;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> lineIndices;
    std::vector<float> interleavedVertices;
    int interleavedStride;
};

#endif