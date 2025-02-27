#pragma once

#include <GL/glew.h>
#include "CGShaderProgram.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "CGCamera.h"

class CGModel
{
public:
	void initialize(int w, int h);
	void finalize();
	void render();
	void update();
	void key_pressed(int key);
	void mouse_button(int button, int action);
	void mouse_move(double xpos, double ypos);
	void resize(int w, int h);
	

private:
	CGShaderProgram* sceneProgram;
	CGShaderProgram* skyboxProgram;
	CGShaderProgram* shadowProgram;
	CGScene* scene;
	CGCamera* camera;
	CGSkybox* skybox;
	glm::mat4 projection;
	glm::mat4 loc;
	GLsizei wndWidth;
	GLsizei wndHeight;
	GLuint shadowFBO;
	GLuint depthTexId;
	int modocamara;
	const float deltaTime = 1.0f / 60.0f;
	bool InitShadowMap();
	void CameraConstraints();
	glm::vec3 direccioncamara;
	glm::vec3 direccioncamara2;
	GLfloat vy=0;
	
};
