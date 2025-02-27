#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "CGFigure.h"
#include "CGObject.h"

class CGScene {
public:
    CGScene();
    ~CGScene();
    void Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix);
    void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);
    glm::mat4 GetLightViewMatrix();
 CGObject* coche;
 CGFigure* fig4;
private:
   
    CGFigure* ground;
    CGFigure* pared1;
    CGFigure* pared2;
    CGFigure* pared3;
    CGFigure* pared4;
    CGFigure* pared5;
    CGFigure* pared6;
    CGObject* porteria;
    CGObject* porteria2;
    CGFigure* marcador;
    
    CGFigure* sueloporteria1;
    CGFigure* sueloporteria2;
    
    CGMaterial* matb;
    CGMaterial* matg;
    
    CGMaterial* mat4;
    CGMaterial* matmarcador;
 
    CGMaterial* matpared;
    CGLight* light;
};
