#pragma once

#include <GL/glew.h>
#include "CGFigure.h"

//
// CLASE: CGSphere
//
// DESCRIPCIÓN: Representa una esfera de radio 'r', dividida en 'p' 
//              capas (paralelos) y 'm' líneas (meridianos).
//
class CGSphere : public CGFigure {
private:
	GLfloat moveStep;
	GLfloat turnStep;
	GLfloat cosAngle;
	GLfloat sinAngle;
public:
	virtual ~CGSphere() {}
	CGSphere(GLint p, GLint m, GLfloat r);
	void SetMoveStep(GLfloat step);
	void SetTurnStep(GLfloat step);
	GLfloat GetMoveStep();
	GLfloat GetTurnStep();
	void MoveFront();
	void MoveBack();
	void TurnRight();
	void TurnLeft();
	void TurnCW();
	void TurnCCW();
	void Move( GLfloat vy);
};
