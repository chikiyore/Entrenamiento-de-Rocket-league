#include "CGSphere.h"
#include <GL/glew.h>
#include <math.h>
#include "CGFigure.h"

///
/// FUNCION: CGSphere::CGSphere(GLint p, GLint m, GLfloat r)
///
/// PROP�SITO: Construye una espera de radio 'r' con 'p' paralelos
///            y 'm' meridianos
///
CGSphere::CGSphere(GLint p, GLint m, GLfloat r)
{
    moveStep = 0.0f;
    turnStep = 2.0f;
    cosAngle = (GLfloat)cos(glm::radians(turnStep));
    sinAngle = (GLfloat)sin(glm::radians(turnStep));
    numFaces = 2 * m * (p - 1); // Number of faces
    numVertices = (m + 1) * (p + 1); // Number of vertices
    normals = new GLfloat[numVertices * 3];
    textures = new GLfloat[numVertices * 2];
    vertices = new GLfloat[numVertices * 3];
    indexes = new GLushort[numFaces * 3];

    int texturesIndex = 0;
    int normalsIndex = 0;
    int verticesIndex = 0;
    int indexesIndex = 0;

    /* northern polar cap*/
    for (int j = 0; j <= m; j++)
    {
        textures[texturesIndex] = (j + 0.5) / m;
        textures[texturesIndex + 1] = 1.0f;
        texturesIndex += 2;
        normals[normalsIndex] = 0.0f;
        normals[normalsIndex + 1] = 0.0f;
        normals[normalsIndex + 2] = 1.0f;
        normalsIndex += 3;
        vertices[verticesIndex] = 0.0;
        vertices[verticesIndex + 1] = 0.0f;
        vertices[verticesIndex + 2] = r;
        verticesIndex += 3;
    }

    for (int i = 1; i < p; i++)
    {
        for (int j = 0; j <= m; j++)
        {
            GLfloat pCos = (GLfloat)cos(glm::radians(180.0 * i / p));
            GLfloat pSin = (GLfloat)sin(glm::radians(180.0 * i / p));
            GLfloat mCos = (GLfloat)cos(glm::radians(360.0 * j / m));
            GLfloat mSin = (GLfloat)sin(glm::radians(360.0 * j / m));

            textures[texturesIndex] = ((GLfloat)j) / m;
            textures[texturesIndex + 1] = 1.0 - ((GLfloat)i / p);
            texturesIndex += 2;
            normals[normalsIndex] = pSin * mCos;
            normals[normalsIndex + 1] = pSin * mSin;
            normals[normalsIndex + 2] = pCos;
            normalsIndex += 3;
            vertices[verticesIndex] = pSin * mCos * r;
            vertices[verticesIndex + 1] = pSin * mSin * r;
            vertices[verticesIndex + 2] = pCos * r;
            verticesIndex += 3;
        }
    }

    /* southern polar cap*/
    for (int j = 0; j <= m; j++)
    {
        textures[texturesIndex] = (j + 0.5) / m;
        textures[texturesIndex + 1] = 0.0f;
        texturesIndex += 2;
        normals[normalsIndex] = 0.0f;
        normals[normalsIndex + 1] = 0.0f;
        normals[normalsIndex + 2] = -1.0f;
        normalsIndex += 3;
        vertices[verticesIndex] = 0.0;
        vertices[verticesIndex + 1] = 0.0f;
        vertices[verticesIndex + 2] = -r;
        verticesIndex += 3;
    }

    /* northern polar cap*/
    for (int j = 0; j < m; j++)
    {
        indexes[indexesIndex] = j;
        indexes[indexesIndex + 1] = m + j + 1;
        indexes[indexesIndex + 2] = m + j + 2;
        indexesIndex += 3;
    }
    for (int i = 1; i < p - 1; i++)
    {
        for (int j = 0; j < m; j++)
        {
            indexes[indexesIndex] = i * (m + 1) + j;
            indexes[indexesIndex + 1] = (i + 1) * (m + 1) + j;
            indexes[indexesIndex + 2] = i * (m + 1) + j + 1;
            indexes[indexesIndex + 3] = (i + 1) * (m + 1) + j;
            indexes[indexesIndex + 4] = (i + 1) * (m + 1) + j + 1;
            indexes[indexesIndex + 5] = i * (m + 1) + j + 1;
            indexesIndex += 6;
        }
    }
    for (int j = 0; j < m; j++)
    {
        indexes[indexesIndex] = (p - 1) * (m + 1) + j;
        indexes[indexesIndex + 1] = p * (m + 1) + j;
        indexes[indexesIndex + 2] = (p - 1) * (m + 1) + j + 1;
        indexesIndex += 3;
    }

    InitBuffers();
}
void CGSphere::SetMoveStep(GLfloat step)
{
    moveStep = step;
}

//
// FUNCI�N: CGCamera::SetTurnStep(GLfloat step)
//
// PROP�SITO: Asigna el �ngulo de giro en cada paso.
//
void CGSphere::SetTurnStep(GLfloat step)
{
    turnStep = step;
    cosAngle = (GLfloat)cos(glm::radians(turnStep));
    sinAngle = (GLfloat)sin(glm::radians(turnStep));
}
GLfloat CGSphere::GetMoveStep()
{
    return moveStep;
}

//
// FUNCI�N: CGCamera::GetTurnStep()
//
// PROP�SITO: Obtiene el �ngulo de giro en cada paso.
//
GLfloat CGSphere::GetTurnStep()
{
    return turnStep;
}

// Movement methods
void CGSphere::MoveFront() {
    Translate(glm::vec3(-moveStep, 0.0f, 0.0f));
}

void CGSphere::MoveBack() {
    Translate(glm::vec3(moveStep, 0.0f, 0.0f));
}

void CGSphere::TurnRight() {
    Rotate(-turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CGSphere::TurnLeft() {
    Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CGSphere::TurnCW()
{
    Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

//
// FUNCI�N: CGCamera::TurnDown()
//
// PROP�SITO: Rota el observador un paso (angleStep) en sentido contrario al reloj.
//
void CGSphere::TurnCCW()
{
    Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CGSphere::Move( GLfloat vy) {
    Translate(glm::vec3(-moveStep, vy, 0.0f));
}