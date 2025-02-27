#include "CGObject.h"
#include <GL/glew.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
// FUNCI�N: CGObject::ResetLocation()
//
// PROP�SITO: Asigna la posici�n inicial del objecto 
//
void CGObject::ResetLocation()
{
	model = glm::mat4(1.0f);
}

//
// FUNCI�N: CGObject::SetLocation(glm::mat4 loc)
//
// PROP�SITO: Asigna la posici�n del objecto 
//
void CGObject::SetLocation(glm::mat4 loc)
{
	model = loc;
}

//
// FUNCI�N: CGObject::GetLocation()
//
// PROP�SITO: Obtiene la posici�n del objecto 
//
glm::mat4 CGObject::GetLocation()
{
	return model;
}

//
// FUNCI�N: CGObject::Translate(glm::vec3 t)
//
// PROP�SITO: A�ade un desplazamiento a la matriz de posici�n del objeto 
//
void CGObject::Translate(glm::vec3 t)
{
	model = glm::translate(model, t);
}

//
// FUNCI�N: CGObject::Rotate(GLfloat angle, glm::vec3 axis)
//
// PROP�SITO: A�ade una rotaci�n a la matriz de posici�n del objeto 
//
void CGObject::Rotate(GLfloat angle, glm::vec3 axis)
{
	model = glm::rotate(model, glm::radians(angle), axis);
}

//
// FUNCI�N: CGObject::Draw(CGShaderProgram * program, glm::mat4 projection, glm::mat4 view)
//
// PROP�SITO: Dibuja el objeto
//
void CGObject::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view)
{
	int num = GetNumPieces();
	for (int i = 0; i < num; i++)
	{
		GetPiece(i)->Draw(program, projection, view, model);
	}
}
