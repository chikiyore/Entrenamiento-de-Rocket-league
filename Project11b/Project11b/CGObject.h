#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGPiece.h"

//
// CLASE: CGObject
//
// DESCRIPCI�N: Clase abstracta que representa un objeto formado por varias piezas
// 
class CGObject
{
protected:
	glm::mat4 model;  // Model matrix

public:
	void ResetLocation();
	void Translate(glm::vec3 t);
	void Rotate(GLfloat angle, glm::vec3 axis);
	void SetLocation(glm::mat4 loc);
	glm::mat4 GetLocation();
	void Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view);

	virtual int GetNumPieces() = 0;
	virtual CGPiece* GetPiece(int i) = 0;



};

