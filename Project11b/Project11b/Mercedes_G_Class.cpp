#include "Mercedes_G_Class.h"
#include "Mercedes_G_Class_pieces.h"
#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

Mercedes_G_Class::Mercedes_G_Class()
{
	moveStep = 0.0f;
	turnStep = 2.0f;
	cosAngle = (GLfloat)cos(glm::radians(turnStep));
	sinAngle = (GLfloat)sin(glm::radians(turnStep));

	model = glm::mat4(1.0f);

	mtl[0] = new CGMaterial();
	mtl[0]->SetAmbientReflect(1.0f, 1.0f, 1.0f);
	mtl[0]->SetDifusseReflect(1.0f, 1.0f, 1.0f);
	mtl[0]->SetSpecularReflect(0.8f, 0.8f, 0.8f);
	mtl[0]->SetShininess(16.0f);
	mtl[0]->InitTexture("textures/coche.png");

	pieces[0] = new Mercedes_G_Class_0(mtl[0]);
	pieces[1] = new Mercedes_G_Class_1(mtl[0]);
	pieces[2] = new Mercedes_G_Class_2(mtl[0]);
	pieces[3] = new Mercedes_G_Class_3(mtl[0]);
	pieces[4] = new Mercedes_G_Class_4(mtl[0]);
	pieces[5] = new Mercedes_G_Class_5(mtl[0]);
	pieces[6] = new Mercedes_G_Class_6(mtl[0]);
	pieces[7] = new Mercedes_G_Class_7(mtl[0]);
	pieces[8] = new Mercedes_G_Class_8(mtl[0]);
	pieces[9] = new Mercedes_G_Class_9(mtl[0]);
	pieces[10] = new Mercedes_G_Class_10(mtl[0]);
	pieces[11] = new Mercedes_G_Class_11(mtl[0]);
	pieces[12] = new Mercedes_G_Class_12(mtl[0]);
	pieces[13] = new Mercedes_G_Class_13(mtl[0]);
	pieces[14] = new Mercedes_G_Class_14(mtl[0]);
}

Mercedes_G_Class::~Mercedes_G_Class()
{
	for (int i = 0; i < 15; i++) delete pieces[i];
	delete[] pieces;
	for (int i = 0; i < 1; i++) delete mtl[i];
	delete[] mtl;
}

int Mercedes_G_Class::GetNumPieces()
{
	return 15;
}

CGPiece* Mercedes_G_Class::GetPiece(int index)
{
	return pieces[index];
}

void Mercedes_G_Class::SetMoveStep(GLfloat step)
{
	moveStep = step;
}

//
// FUNCIÓN: CGCamera::SetTurnStep(GLfloat step)
//
// PROPÓSITO: Asigna el ángulo de giro en cada paso.
//
void Mercedes_G_Class::SetTurnStep(GLfloat step)
{
	turnStep = step;
	cosAngle = (GLfloat)cos(glm::radians(turnStep));
	sinAngle = (GLfloat)sin(glm::radians(turnStep));
}
GLfloat Mercedes_G_Class::GetMoveStep()
{
	return moveStep;
}

//
// FUNCIÓN: CGCamera::GetTurnStep()
//
// PROPÓSITO: Obtiene el ángulo de giro en cada paso.
//
GLfloat Mercedes_G_Class::GetTurnStep()
{
	return turnStep;
}

// Movement methods
void Mercedes_G_Class::MoveFront() {
	Translate(glm::vec3(-moveStep, 0.0f, 0.0f));
}

void Mercedes_G_Class::MoveBack() {
	Translate(glm::vec3(moveStep, 0.0f, 0.0f));
}

void Mercedes_G_Class::TurnRight() {
	Rotate(-turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Mercedes_G_Class::TurnLeft() {
	Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Mercedes_G_Class::TurnCW()
{
	Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}

//
// FUNCIÓN: CGCamera::TurnDown()
//
// PROPÓSITO: Rota el observador un paso (angleStep) en sentido contrario al reloj.
//
void Mercedes_G_Class::TurnCCW()
{
	Rotate(turnStep, glm::vec3(0.0f, 1.0f, 0.0f));
}
