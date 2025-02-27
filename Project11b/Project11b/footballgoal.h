#pragma once

#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

class footballgoal : public CGObject {
private:
	CGMaterial* mtl[1];
	CGPiece* pieces[2];

public:
	footballgoal();
	~footballgoal();
	virtual int GetNumPieces();
	virtual CGPiece* GetPiece(int i);
};

