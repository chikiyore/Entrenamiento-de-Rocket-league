#pragma once

#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

class Mercedes_G_Class : public CGObject {
private:
	CGMaterial* mtl[1];
	CGPiece* pieces[15];
	
	GLfloat turnStep;
	GLfloat cosAngle;
	GLfloat sinAngle;

public:
	Mercedes_G_Class();
	~Mercedes_G_Class();
	virtual int GetNumPieces();
	virtual CGPiece* GetPiece(int i);
	GLfloat moveStep;
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
};

