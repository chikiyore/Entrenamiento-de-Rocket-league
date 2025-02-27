#include "CGScene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CGShaderProgram.h"
#include "CGFigure.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "CGSphere.h"
#include "CGGround.h"
#include "Mercedes_G_Class.h"
#include "footballgoal.h"
#include "resource.h"

//
// FUNCIÓN: CGScene::CGScene()
//
// PROPÓSITO: Construye el objeto que representa la escena
//
CGScene::CGScene()
{
    glm::vec3 Ldir = glm::vec3(1.0f, -0.8f, -1.0f);
    Ldir = glm::normalize(Ldir);
    light = new CGLight();
    light->SetLightDirection(Ldir);
    light->SetAmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));
    light->SetDifusseLight(glm::vec3(0.8f, 0.8f, 0.8f));
    light->SetSpecularLight(glm::vec3(1.0f, 1.0f, 1.0f));

    matg = new CGMaterial();
    matg->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    matg->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg->SetShininess(16.0f);
    matg->InitTexture("textures/hierba.jpg");
    //matg->InitTexture(IDR_IMAGE2);

    matmarcador = new CGMaterial();
    matmarcador->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    matmarcador->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matmarcador->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matmarcador->SetShininess(16.0f);
    matmarcador->InitTexture("textures/marcador.jpg");


    //COCHE
    coche = new Mercedes_G_Class();
    coche->Translate(glm::vec3(0.0f, 0.0f, -1.0f));
    
    




 // Creación del material para las paredes
    matpared = new CGMaterial();
    matpared->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    matpared->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matpared->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matpared->SetShininess(16.0f);
    matpared->InitTexture("textures/concrete.jpg");


    // Configuración del suelo
    ground = new CGGround(100.0f, 50.0f); // Normal hacia arriba
    ground->SetMaterial(matg);
    ground->Translate(glm::vec3(0.0f, 0.0f, 0.0f));

    sueloporteria1 = new CGGround(7.0f, 10.0f); // Normal hacia arriba
    sueloporteria1->SetMaterial(matg);
    sueloporteria1->Translate(glm::vec3(100.0f, 0.0f, 0.0f));

    sueloporteria2 = new CGGround(7.0f, 10.0f); // Normal hacia arriba
    sueloporteria2->SetMaterial(matg);
    sueloporteria2->Translate(glm::vec3(-100.0f, 0.0f, 0.0f));
   
    //Marcador
    marcador = new CGGround(10.0f, 10.0f); // Normal hacia arriba
    marcador->SetMaterial(matmarcador);
    marcador->Translate(glm::vec3(0.0f, 18.0f, -50.0f));
    marcador->Rotate(90, glm::vec3(1.0f, 0.0f, 0.0f));
    marcador->Rotate(-90, glm::vec3(0.0f, 1.0f, 0.0f));
   

    // Creación y configuración de la pared derecha
    pared1 = new CGGround(2.0f, 20.0f); // Normal hacia la izquierda
    pared1->SetMaterial(matpared);
    pared1->Translate(glm::vec3(100.0f, 0.0f, -30.0f)); // Ajusta la posición si es necesario
    pared1->Rotate(90, glm::vec3(0.0f, 0.0f, 1.0f));

    // Creación y configuración de la pared izquierda
    pared2 = new CGGround(2.0f, 20.0f); // Normal hacia la derecha
    pared2->SetMaterial(matpared);
    pared2->Translate(glm::vec3(-100.0f, 0.0f, -30.0f)); // Ajusta la posición si es necesario
    pared2->Rotate(-90, glm::vec3(0.0f, 0.0f, 1.0f));

    pared5 = new CGGround(2.0f, 20.0f); // Normal hacia la derecha
    pared5->SetMaterial(matpared);
    pared5->Translate(glm::vec3(100.0f, 0.0f, 30.0f)); // Ajusta la posición si es necesario
    pared5->Rotate(90, glm::vec3(0.0f, 0.0f, 1.0f));

    pared6 = new CGGround(2.0f, 20.0f); // Normal hacia la derecha
    pared6->SetMaterial(matpared);
    pared6->Translate(glm::vec3(-100.0f, 0.0f, 30.0f)); // Ajusta la posición si es necesario
    pared6->Rotate(-90, glm::vec3(0.0f, 0.0f, 1.0f));
 
    pared3 = new CGGround(100.0f, 2.0f); // Normal hacia la derecha
    pared3->SetMaterial(matpared);
    pared3->Translate(glm::vec3(0.0f, 0.0f, -50.0f)); // Ajusta la posición si es necesario
    pared3->Rotate(90, glm::vec3(1.0f, 0.0f, 0.0f));

    pared4 = new CGGround(100.0f, 2.0f); // Normal hacia la derecha
    pared4->SetMaterial(matpared);
    pared4->Translate(glm::vec3(0.0f, 0.0f, 50.0f)); // Ajusta la posición si es necesario
    pared4->Rotate(-90, glm::vec3(1.0f, 0.0f, 0.0f));

    

    
    mat4 = new CGMaterial();
    mat4->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    mat4->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat4->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat4->SetShininess(16.0f);
    mat4->InitTexture("textures/ball.jpg");
    

    fig4 = new CGSphere(20, 40, 1.0f);
    fig4->SetMaterial(mat4);
    fig4->Translate(glm::vec3(-5.0f, 1.0f, 0.0f));

    porteria = new footballgoal();
    porteria->Translate(glm::vec3(103.0f, 0.0f, 10.0f));
    porteria->Rotate(90, glm::vec3(0.0f, 1.0f, 0.0f));

    porteria2 = new footballgoal();
    porteria2->Translate(glm::vec3(-103.0f, 0.0f, -10.0f));
    porteria2->Rotate(-90, glm::vec3(0.0f, 1.0f, 0.0f));

    
}

//
// FUNCIÓN: CGScene3:~CGScene()
//
// PROPÓSITO: Destruye el objeto que representa la escena
//
CGScene::~CGScene()
{
    delete ground;
    
    delete fig4;
   
    delete light;
    delete matg;
    
    delete pared1;
    delete pared2;
    delete pared3;
    delete pared4;
    delete pared5;
    delete pared6;
    delete porteria;
    delete porteria2;
    delete marcador;
    delete coche;
    delete sueloporteria1;
    delete sueloporteria2;
}

//
// FUNCIÓN: CGScene::Draw()
//
// PROPÓSITO: Dibuja la escena
//
void CGScene::Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix)
{
    light->SetUniforms(program);

    ground->Draw(program, proj, view, shadowViewMatrix);
    
    fig4->Draw(program, proj, view, shadowViewMatrix);
    pared1->Draw(program, proj, view, shadowViewMatrix);
    pared2->Draw(program, proj, view, shadowViewMatrix);
    pared3->Draw(program, proj, view, shadowViewMatrix);
    pared4->Draw(program, proj, view, shadowViewMatrix);
    pared5->Draw(program, proj, view, shadowViewMatrix);
    pared6->Draw(program, proj, view, shadowViewMatrix);
    marcador->Draw(program, proj, view, shadowViewMatrix);
    porteria->Draw(program, proj, view);
    porteria2->Draw(program, proj, view);
    coche->Draw(program, proj, view);
    sueloporteria1->Draw(program, proj, view, shadowViewMatrix);
    sueloporteria2->Draw(program, proj, view, shadowViewMatrix);
}

//
// FUNCIÓN: CGScene::DrawShadow()
//
// PROPÓSITO: Dibuja las sombras de la escena
//
void CGScene::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
    
    fig4->DrawShadow(program, shadowMatrix);
   
    pared1->DrawShadow(program, shadowMatrix);
    pared2->DrawShadow(program, shadowMatrix);
    pared3->DrawShadow(program, shadowMatrix);
    pared4->DrawShadow(program, shadowMatrix);
    pared5->DrawShadow(program, shadowMatrix);
    pared6->DrawShadow(program, shadowMatrix);
    marcador->DrawShadow(program, shadowMatrix);
    sueloporteria1->DrawShadow(program, shadowMatrix);
    sueloporteria2->DrawShadow(program, shadowMatrix);
}

//
// FUNCIÓN: CGScene::GetLightViewMatrix()
//
// PROPÓSITO: Obtiene la matriz de posicionamiento de la luz
//
glm::mat4 CGScene::GetLightViewMatrix()
{
    glm::vec3 Zdir = -(light->GetLightDirection());
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Xdir = glm::normalize(glm::cross(Up, Zdir));
    glm::vec3 Ydir = glm::cross(Zdir, Xdir);
    glm::vec3 Zpos = 150.0f * Zdir;
    glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 view = glm::lookAt(Zpos, Center, Ydir);
    return view;
}