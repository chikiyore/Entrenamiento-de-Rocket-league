#include "CGModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "CGCamera.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "Mercedes_G_Class.h"
#include "CGApplication.h"
#include "CGSphere.h"
#include "resource.h"

//
// FUNCIÓN: CGModel::initialize(int, int)
//
// PROPÓSITO: Initializa el modelo 3D
//
void CGModel::initialize(int w, int h)
{
    // Crea el programa gráfico para el entorno
    skyboxProgram = new CGShaderProgram(IDR_SHADER1, IDR_SHADER2, -1, -1, -1);
    if (skyboxProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la sombra
    shadowProgram = new CGShaderProgram(IDR_SHADER5, IDR_SHADER6, -1, -1, -1);
    if (shadowProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la escena
    sceneProgram = new CGShaderProgram(IDR_SHADER3, IDR_SHADER4, -1, -1, -1);
    if (sceneProgram->IsLinked() == GL_FALSE) return;

    // Crea la cámara
    camera = new CGCamera();
    camera->SetPosition(0.0f, 5.0f, 30.0f);
    direccioncamara2 = camera->GetDirection();
    // Crea el skybox
    skybox = new CGSkybox();

    // Crea la escena
    scene = new CGScene();
    
    // Crea el Framebuffer de la sombra
    bool frameBufferStatus = InitShadowMap();
    if (!frameBufferStatus) return;

    // Asigna el viewport y el clipping volume
    resize(w, h);

    // Opciones de dibujo
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//
// FUNCIÓN: CGModel::finalize()
//
// PROPÓSITO: Libera los recursos del modelo 3D
//
void CGModel::finalize()
{
    delete camera;
    delete scene;
    delete skybox;
    delete sceneProgram;
    delete skyboxProgram;
    delete shadowProgram;
}

//
// FUNCIÓN: CGModel::resize(int w, int h)
//
// PROPÓSITO: Asigna el viewport y el clipping volume
//
void CGModel::resize(int w, int h)
{
    double fov = glm::radians(15.0);
    double sin_fov = sin(fov);
    double cos_fov = cos(fov);
    if (h == 0) h = 1;
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
    GLfloat wHeight = (GLfloat)(sin_fov * 0.2 / cos_fov);
    GLfloat wWidth = wHeight * aspectRatio;

    wndWidth = w;
    wndHeight = h;

    glViewport(0, 0, w, h);
    projection = glm::frustum(-wWidth, wWidth, -wHeight, wHeight, 0.2f, 400.0f);
}

//
// FUNCIÓN: CGModel::render()
//
// PROPÓSITO: Genera la imagen
//
void CGModel::render()
{
    //*********************************************************//
    //                  Genera el ShadowMap                    //
    //*********************************************************//

    // Activa el framebuffer de la sombra
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    // Activa el programa que genera el shadowmap
    shadowProgram->Use();

    // Asigna las matrices Viewport, View y Projection de la luz.
    glm::mat4 lightViewMatrix = scene->GetLightViewMatrix();
    glm::mat4 lightPerspective = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, 0.0f, 400.0f);
    glm::mat4 lightMVP = lightPerspective * lightViewMatrix;

    // Limpia la información de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa front-face culling
    glCullFace(GL_FRONT);

    //Asigna el viewport
    glViewport(0, 0, 1024, 1024);

    // Dibuja la escena
    scene->DrawShadow(shadowProgram, lightMVP);

    //*********************************************************//
    //                  Dibuja el skybox                       //
    //*********************************************************//
    
    // Activa el framebuffer de la imagen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Limpia el framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa back-face culling
    glCullFace(GL_BACK);

    // Asigna el viewport
    glViewport(0, 0, wndWidth, wndHeight);

    // Dibuja el skybox
    glm::mat4 view = camera->ViewMatrix();
    skyboxProgram->Use();
    skybox->Draw(skyboxProgram, projection, view);

    //*********************************************************//
    //                  Dibuja la escena                       //
    //*********************************************************//

    // Activa el programa de la escena
    sceneProgram->Use();
    sceneProgram->SetUniformI("ShadowMap", 1);

    // Dibuja la escena
    glm::mat4 viewMatrix = camera->ViewMatrix();
    scene->Draw(sceneProgram, projection, viewMatrix, lightMVP);
    
}

//
// FUNCIÓN: CGModel::update()
//
// PROPÓSITO: Anima la escena
//
void CGModel::update()
{
    Mercedes_G_Class* mercedes = dynamic_cast<Mercedes_G_Class*>(scene->coche);
    CGSphere* bola = dynamic_cast<CGSphere*>(scene->fig4);
    if (modocamara == 1) {
        // Obtener la posición y la orientación del coche
        glm::mat4 loc = scene->coche->GetLocation();
        glm::vec3 cochePos = glm::vec3(loc[3][0], loc[3][1], loc[3][2]); // Posición del coche
        glm::vec3 cocheDir = glm::vec3(loc[2][0], loc[2][1], loc[2][2]); // Dirección del coche
        glm::vec3 cocheRight = glm::vec3(loc[0][0], loc[0][1], loc[0][2]); // Vector Right del coche

       
        glm::vec3 cameraPos = cochePos;
        
        // Calcular la nueva dirección de la cámara apuntando 90 grados a la izquierda
        glm::vec3 leftDir = glm::cross( glm::vec3(0.0f, 1.0f, 0.0f), cocheDir); // Vector a la izquierda, perpendicular al cocheDir y al eje Y global
        leftDir = glm::normalize(leftDir);
          // Establecer la posición y dirección de la cámara
        
        camera->SetPosition(cameraPos.x , cameraPos.y+1.5f, cameraPos.z);
         camera->SetDirection(leftDir.x, leftDir.y, leftDir.z, 0.0f, 1.0f, 0.0f);
         for (int i = 1; i <= 8; i++)
             camera->TurnDown();
       

    }
    
    camera->MoveFront();
    mercedes->MoveFront();
    bola->Move(vy);     
    
    CameraConstraints();
    
    //Actualizamos velocidad eje Y
    if (vy != 0)
       vy = vy - (0.098f/60.0f);

}

//
// FUNCIÓN: CGModel::key_pressed(int)
//
// PROPÓSITO: Respuesta a acciones de teclado
//
void CGModel::key_pressed(int key)
{
    Mercedes_G_Class* mercedes = dynamic_cast<Mercedes_G_Class*>(scene->coche);
    CGSphere* bola = dynamic_cast<CGSphere*>(scene->fig4);
    if (modocamara == 1) {
        switch (key)
        {
        case GLFW_KEY_UP:
            mercedes->SetMoveStep(mercedes->GetMoveStep() + 0.1f);
            break;
        case GLFW_KEY_DOWN:
            mercedes->SetMoveStep(mercedes->GetMoveStep() - 0.1f);
            break;
        case GLFW_KEY_LEFT:
            mercedes->TurnLeft();
            
            break;
        case GLFW_KEY_RIGHT:
            mercedes->TurnRight();
            
            break;
        case GLFW_KEY_S:
            mercedes->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_2:
            modocamara = 2;
            camera->SetPosition(0.0f, 220.0f, 0.0f);            
            camera->SetDirection(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            camera->SetTurnStep(90);
            camera->TurnDown();
            camera->SetTurnStep(1.0);
            break;
        case GLFW_KEY_1:
            modocamara = 1;
            break;
        case GLFW_KEY_3:
            modocamara = 3;
            break;
        
        }
    }
    else if (modocamara == 2) {
        switch (key)
        {
        case GLFW_KEY_UP:
            mercedes->SetMoveStep(mercedes->GetMoveStep() + 0.1f);
            break;
        case GLFW_KEY_DOWN:
            mercedes->SetMoveStep(mercedes->GetMoveStep() - 0.1f);
            break;
        case GLFW_KEY_LEFT:
            mercedes->TurnLeft();
            break;
        case GLFW_KEY_RIGHT:
            mercedes->TurnRight();
            break;
        case GLFW_KEY_S:
            mercedes->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_1:
            modocamara = 1;
            break;
        case GLFW_KEY_3:
            modocamara = 3;
            break;

        }
    }
   else{
        switch (key)
        {
        case GLFW_KEY_R:
            mercedes->SetMoveStep(mercedes->GetMoveStep() + 0.1f);
            break;
        case GLFW_KEY_F:
            mercedes->SetMoveStep(mercedes->GetMoveStep() - 0.1f);
            break;
        case GLFW_KEY_D:
            mercedes->TurnLeft();
            break;
        case GLFW_KEY_G:
            mercedes->TurnRight();
            break;
        case GLFW_KEY_UP:
            camera->TurnDown();
            break;
        case GLFW_KEY_DOWN:
            camera->TurnUp();
            break;
        case GLFW_KEY_LEFT:
            camera->TurnCCW();
            break;
        case GLFW_KEY_RIGHT:
            camera->TurnCW();
            break;
        case GLFW_KEY_S:
            camera->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_RIGHT_BRACKET:
        case GLFW_KEY_KP_ADD:
            camera->SetMoveStep(camera->GetMoveStep() + 0.1f);
            break;
        case GLFW_KEY_LEFT_BRACKET:
        case GLFW_KEY_MINUS:
        case GLFW_KEY_KP_SUBTRACT:
        case GLFW_KEY_DELETE:
            camera->SetMoveStep(camera->GetMoveStep() - 0.1f);
            break;
        case GLFW_KEY_Q:
            camera->SetMoveStep(0.1f);
            camera->MoveUp();
            camera->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_A:
            camera->SetMoveStep(0.1f);
            camera->MoveDown();
            camera->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_O:
            camera->SetMoveStep(0.1f);
            camera->MoveLeft();
            camera->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_P:
            camera->SetMoveStep(0.1f);
            camera->MoveRight();
            camera->SetMoveStep(0.0f);
            break;
        case GLFW_KEY_K:
            camera->TurnLeft();
            break;
        case GLFW_KEY_L:
            camera->TurnRight();
            break;
        case GLFW_KEY_2:
            modocamara = 2;
            camera->SetPosition(0.0f, 220.0f, 0.0f);
            camera->SetDirection(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            camera->SetTurnStep(90);
            camera->TurnDown();
            camera->SetTurnStep(1.0);
            break;
        case GLFW_KEY_1:
            modocamara = 1;
            break;
        case GLFW_KEY_3:
            modocamara = 3;
            break;
        
        }

   }
    
}

//
//  FUNCIÓN: CGModel:::mouse_button(int button, int action)
//
//  PROPÓSITO: Respuesta del modelo a un click del ratón.
//
void CGModel::mouse_button(int button, int action)
{
}

//
//  FUNCIÓN: CGModel::mouse_move(double xpos, double ypos)
//
//  PROPÓSITO: Respuesta del modelo a un movimiento del ratón.
//
void CGModel::mouse_move(double xpos, double ypos)
{
}

//
//  FUNCIÓN: CGModel::CameraConstraints()
//
//  PROPÓSITO: Limita el movimiento de la cámara a una cierta zona
//
void CGModel::CameraConstraints()
{
    Mercedes_G_Class* mercedes = dynamic_cast<Mercedes_G_Class*>(scene->coche);
    CGSphere* bola = dynamic_cast<CGSphere*>(scene->fig4);
    const float cocheRadius = 2.0f;
    const float ballRadius = 1.0f;
    glm::vec3 pos = camera->GetPosition();
    
    int constraint = 0;
    if (pos.y < 1.0f) { pos.y = 1.0f; constraint = 1; }
    if (pos.y > 300.0f) { pos.y = 300.0f; constraint = 1; }
    if (pos.x > 100.0f) { pos.x = 100.0f; constraint = 1; }
    if (pos.x < -100.0f) { pos.x = -100.0f; constraint = 1; }
    if (pos.z > 50.0f) { pos.z = 50.0f; constraint = 1; }
    if (pos.z < -50.0f) { pos.z = -50.0f; constraint = 1; }
    if (constraint == 1)
    {
        camera->SetPosition(pos.x, pos.y, pos.z);
        camera->SetMoveStep(0.0f);
    }

    glm::mat4 loc = scene->coche->GetLocation();
    glm::vec3 cochepos = glm::vec3(loc[3][0], loc[3][1], loc[3][2]); // Posición del coche
    glm::vec3 cocheDir = glm::vec3(loc[2][0], loc[2][1], loc[2][2]); // Dirección del coche
    glm::vec3 cocheRight = glm::vec3(loc[0][0], loc[0][1], loc[0][2]); // Vector Right del coche
    glm::vec3 cocheup = glm::vec3(loc[1][0], loc[1][1], loc[1][2]); // Vector Right del coche

    int res = 0;
    
    if (cochepos.x > 96.0f) { cochepos.x = 96.0f;  res = 1; cocheDir = glm::reflect(-cocheDir, glm::vec3(-1.0f, 0.0f, 0.0f)); }
    if (cochepos.x < -96.0f) { cochepos.x = -96.0f;  res = 1; cocheDir = glm::reflect(-cocheDir, glm::vec3(1.0f, 0.0f, 0.0f)); }
    if (cochepos.z > 46.0f) { cochepos.z = 46.0f;  res = 1; cocheDir = glm::reflect(-cocheDir, glm::vec3(0.0f, 0.0f, 1.0f)); }
    if (cochepos.z < -46.0f) { cochepos.z = -46.0f; res = 1; cocheDir = glm::reflect(-cocheDir, glm::vec3(0.0f, 0.0f, 1.0f)); }
    if (res == 1)
    {
        loc[2][0] = cocheDir.x;
        loc[2][1] = cocheDir.y;
        loc[2][2] = cocheDir.z;
        cocheRight = glm::cross(cocheup, cocheDir);
        loc[0][0] = cocheRight.x;
        loc[0][1] = cocheRight.y;
        loc[0][2] = cocheRight.z;
        scene->coche->SetLocation(loc);
    }

    glm::mat4 locball = scene->fig4->location;
    glm::vec3 ballpos = glm::vec3(locball[3][0], locball[3][1], locball[3][2]); // Posición de la pelota
    glm::vec3 ballDir = glm::vec3(locball[2][0], locball[2][1], locball[2][2]); // Dirección de la pelota
    glm::vec3 ballright = glm::vec3(locball[0][0], locball[0][1], locball[0][2]); // Vector Right de la pelota
    int balllimit = 0;
   

    if (ballpos.x > 101.0f &&( ballpos.z < 10.0f && ballpos.z > -10.0f && ballpos.y < 6.5f)) { balllimit = 4; }
    if (ballpos.x < -101.0f && (ballpos.z < 10.0f && ballpos.z > -10.0f && ballpos.y < 6.5f)) { balllimit = 4; }
   
      
if (balllimit == 4)
    {
    
    CGApplication terminar;
    terminar.cleanup();
    
    }

 float distance = glm::distance(cochepos, ballpos);
//SI HAY CONTACTO COCHE BOLA

if (distance < cocheRadius + ballRadius) {

    GLfloat moveStep = mercedes->GetMoveStep();
    vy = 9.8/60.f;
    locball[2][0] = loc[2][0];
    locball[2][1] = loc[2][1];
    locball[2][2] = loc[2][2];
    locball[0][0] = loc[0][0];
    locball[0][1] = loc[0][1];
    locball[0][2] = loc[0][2];

    bola->location = locball;
    
    //Velocidad X
        bola->SetMoveStep(moveStep);
    //Velocidad Y
        bola->Move(vy*0.5f);
    

}
GLfloat moveStep = bola->GetMoveStep();
 locball = scene->fig4->location;
 ballpos = glm::vec3(locball[3][0], locball[3][1], locball[3][2]); // Posición de la pelota
ballDir = glm::vec3(locball[2][0], locball[2][1], locball[2][2]); // Dirección de la pelota
 ballright = glm::vec3(locball[0][0], locball[0][1], locball[0][2]); // Vector Right de la pelota
 glm::vec3 ballup = glm::vec3(locball[1][0], locball[1][1], locball[1][2]); // Vector UP de la pelota

    if (ballpos.y < 1.0f) { ballpos.y = 1.0f; balllimit = 1; vy = vy * -0.6; bola->SetMoveStep(moveStep * 0.8f);    }
    if (ballpos.y > 100.0f) {  balllimit = 1; vy = vy * -0.6;   bola->SetMoveStep(moveStep*0.8f);    }
    if (ballpos.x > 99.0f && (ballpos.z > 10.0f || ballpos.z < -10.0f || ballpos.y > 6.5f)) { ballpos.x = 99.0f;
    ballDir = glm::reflect(-ballDir, glm::vec3(-1.0f, 0.0f, 0.0f)); balllimit = 2; bola->SetMoveStep(moveStep * 0.8f); }
    if (ballpos.x < -99.0f && (ballpos.z > 10.0f || ballpos.z < -10.0f || ballpos.y > 6.5f)) { ballpos.x = -99.0f; 
    ballDir= glm::reflect(-ballDir, glm::vec3(1.0f, 0.0f, 0.0f)); balllimit = 2; bola->SetMoveStep(moveStep * 0.8f); }
    if (ballpos.z > 49.0f) { ballpos.z = 49.0f;  
    ballDir = glm::reflect(-ballDir, glm::vec3(0.0f, 0.0f, 1.0f)); balllimit = 3;  bola->SetMoveStep(moveStep * 0.8f); }
    if (ballpos.z < -49.0f) { ballpos.z = -49.0f; 
    ballDir = glm::reflect(-ballDir, glm::vec3(0.0f, 0.0f, -1.0f)); balllimit = 3; bola->SetMoveStep(moveStep * 0.8f);}
    if (balllimit == 1)
    {
        locball[3][0] = ballpos.x;
        locball[3][1] = ballpos.y;
        locball[3][2] = ballpos.z;
        scene->fig4->location = locball;
    }
    if (balllimit == 2)
    {
        locball[3][0] = ballpos.x;
        locball[3][1] = ballpos.y;
        locball[3][2] = ballpos.z;
        locball[2][0] = ballDir.x;
        locball[2][1] = ballDir.y;
        locball[2][2] = ballDir.z;
        ballright = glm::cross(ballup, ballDir);
        locball[0][0] = ballright.x;
        locball[0][1] = ballright.y;
        locball[0][2] = ballright.z;
        scene->fig4->location = locball;
    }
    if (balllimit == 3)
    {
        locball[3][0] = ballpos.x;
        locball[3][1] = ballpos.y;
        locball[3][2] = ballpos.z;
        locball[2][0] = ballDir.x;
        locball[2][1] = ballDir.y;
        locball[2][2] = ballDir.z;
        ballright = glm::cross(ballup, ballDir);
        locball[0][0] = ballright.x;
        locball[0][1] = ballright.y;
        locball[0][2] = ballright.z;
        scene->fig4->location = locball;
    }
    

}

//
// FUNCIÓN: CGModel::InitShadowMap()
//
// PROPÓSITO: Inicializa el FBO para almacenar la textura de sombra
//
bool CGModel::InitShadowMap()
{
    GLfloat border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLsizei shadowMapWidth = 1024;
    GLsizei shadowMapHeight = 1024;

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glGenTextures(1, &depthTexId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapWidth,
        shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTexId, 0);

    glDrawBuffer(GL_NONE);

    bool result = true;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        result = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

