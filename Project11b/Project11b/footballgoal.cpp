#include "footballgoal.h"
#include "footballgoal_pieces.h"
#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

footballgoal::footballgoal()
{
	model = glm::mat4(1.0f);

	mtl[0] = new CGMaterial();
	mtl[0]->SetAmbientReflect(1.0f, 1.0f, 1.0f);
	mtl[0]->SetDifusseReflect(1.0f, 1.0f, 1.0f);
	mtl[0]->SetSpecularReflect(0.0f, 0.0f, 0.0f);
	mtl[0]->SetShininess(16.0f);
	mtl[0]->InitTexture("textures/porteriatexture.jpg");
	pieces[0] = new footballgoal_0(mtl[0]);
	pieces[1] = new footballgoal_1(mtl[0]);
}

footballgoal::~footballgoal()
{
	for (int i = 0; i < 2; i++) delete pieces[i];
	delete[] pieces;
	for (int i = 0; i < 1; i++) delete mtl[i];
	delete[] mtl;
}

int footballgoal::GetNumPieces()
{
	return 2;
}

CGPiece* footballgoal::GetPiece(int index)
{
	return pieces[index];
}

