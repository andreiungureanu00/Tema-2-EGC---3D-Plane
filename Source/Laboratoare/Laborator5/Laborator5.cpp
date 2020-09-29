#include "Laborator5.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator5::Laborator5()
{
}

Laborator5::~Laborator5()
{
}

float Laborator5::randomNum(float min, float max) {
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}

void Laborator5::Init()
{
	renderCameraTarget = false;
	rotatePlaneAux = rotatePlane;

	{
		Shader* shader = new Shader("ShaderJoc");
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/VertexShader1.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderBara");
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/VertexShader2.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	culoareMare = glm::vec3(0.42f, 0.65f, 0.92f);
	culoareNor = glm::vec3(0.89, 0.89, 0.89);
	culoareViata = glm::vec3(0.11f, 0.57f, 0.20f);
	culoareObstacol = glm::vec3(1, 0, 0);

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	timePassed = 0;
	timePassed2 = 1;
	timePassed3 = 0;

	rotatePlane = 0;
	rotateElice = 0;

	translatePlaneX = 0.3f;
	translateEliceX = -1;
	translateEliceY = translateY + 0.44f;

	rotatieViata1 = 0;
	rotatieViata2 = 0;
	rotatieViata3 = 0;


	for (int i = 0; i < 5; i++) {
		translateYObstacol[i] = randomNum(-1, 4);
	}

	for (int i = 0; i < 2; i++) {
		translateXObstacol[i] = randomNum(-6, -1);
	}

	for (int i = 3; i < 5; i++) {
		translateXObstacol[i] = randomNum(0, 5);
	}

	for (int i = 0; i < 4; i++) {
		translateXViata[i] = randomNum(-1, 1);
		translateXViata2[i] = randomNum(-20, -14);
		translateXViata3[i] = randomNum(14, 20);
		translateXViata4[i] = randomNum(-1, 1);
		translateXViata5[i] = randomNum(-17, -12);
		translateXViata6[i] = randomNum(12, 17);

		translateYViata[i] = randomNum(-34, -28);
		translateYViata2[i] = randomNum(-20, -14);
		translateYViata3[i] = randomNum(-20, -14);
		translateYViata4[i] = randomNum(3, 7);
		translateYViata5[i] = randomNum(-24, -20);
		translateYViata6[i] = randomNum(-24, -20);
	}

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bara");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "baraCombustibil.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("life");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "life.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "avion.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("mare");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "mare.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("nor1");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "nor1.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("nor2");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "nor2.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("nor3");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "nor3.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("viata");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "viata.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("elice");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "elice.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("obstacol");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "obstacol.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	fv = RADIANS(60);
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Laborator5::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(1, 0.93f, 0.83f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator5::Update(float deltaTimeSeconds) {

	timePassed += deltaTimeSeconds;
	timePassed2 += deltaTimeSeconds;
	timePassed3 += deltaTimeSeconds;
	scaleX -= deltaTimeSeconds * 0.02f;

	if (scaleX < 0.5f && count != 3) {
		scaleX += lifeAdd;
		lifeAdd = 0;
	}
	if (scaleX >= 0.5f) {
		scaleX = 0.5f;
	}

	if (scaleX <= 0 || count == 3) {
		gameOver = 1;
		if (translateY >= 0 && translatePlaneX <= 2) {
			translatePlaneX += deltaTimeSeconds * 0.5f;
			translateY -= deltaTimeSeconds * 2;
			if (translateY < -3) {
				translateAuxY = -5;
			}

		}
		else {
			translatePlaneX = 1;
			translateY = -5;
		}
		rotatePlane2 = RADIANS(50);
		scaleX = 0;
		gameOver = 0;

	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXbara, 3.5f, translateZbara));
		modelMatrix *= Transform3D::RotateOY(rotateBara);
		modelMatrix *= Transform3D::RotateOZ(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.2f, 1));
		RenderMesh(meshes["bara"], shaders["ShaderBara"], modelMatrix, glm::vec3(1, 0.93f, 0.83f));
	}

	if (count == 0) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife1X, translateLifeY1, translateLife1Z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife2X, translateLifeY2, translateLife2Z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife3X, translateLifeY3, translateLife3Z));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}
	}
	else if (count == 1) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife1X, translateLifeY1, -4));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife2X, translateLifeY2, -4));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}
	}
	else if (count == 2) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateLife1X, translateLifeY1, -4));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
			RenderMesh(meshes["life"], shaders["VertexNormal"], modelMatrix, glm::vec3(1, 0, 0));
		}

	}


	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.7f, 3.5f, translateZbara));
		modelMatrix *= Transform3D::RotateOY(rotateBara);
		modelMatrix *= Transform3D::RotateOZ(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, 0.2f, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.2f, 1));
		RenderMesh(meshes["bara"], shaders["ShaderBara"], modelMatrix, glm::vec3(0.92f, 0.03f, 0.03f));
	}

	if ((int)rotatieViata1 % contor == 0) {
		for (int i = 0; i < 4; i++) {
			translateXViata[i] = randomNum(-1, 1);
			translateXViata2[i] = randomNum(-20, -14);
			translateXViata3[i] = randomNum(14, 20);
			translateXViata4[i] = randomNum(-1, 1);
			translateXViata5[i] = randomNum(-17, -12);
			translateXViata6[i] = randomNum(12, 17);

			translateYViata[i] = randomNum(-34, -28);
			translateYViata2[i] = randomNum(-20, -14);
			translateYViata3[i] = randomNum(-20, -14);
			translateYViata4[i] = randomNum(3, 7);
			translateYViata5[i] = randomNum(-24, -20);
			translateYViata6[i] = randomNum(-24, -20);
		}
		contor--;
		if (contor == 4) {
			contor = 7;
		}
	}

	if (timePassed2 < 0.5f) {
		rotatieObstacol -= deltaTimeSeconds * 7;
	}

	if (timePassed2 >= 0.5f) {
		rotatieObstacol -= deltaTimeSeconds * 0.6f;
	}
	if (timePassed2 >= 3.5f) {
		rotatieObstacol -= deltaTimeSeconds * 13.7f;
		timePassed2 = -0.1f;
	}


	rotatieNor1 -= deltaTimeSeconds * 1;
	rotatieNor2 -= deltaTimeSeconds * 0.85f;
	rotatieNor3 -= deltaTimeSeconds * 0.9f;

	rotatieViata1 -= deltaTimeSeconds * 0.4f;

	rotatieElice += deltaTimeSeconds * 20;
	angularStepOX -= deltaTimeSeconds;
	angularStepOY -= deltaTimeSeconds;
	angularStepOZ -= deltaTimeSeconds;

	if (timePassed > 8) {

		for (int i = 0; i < 5; i++) {
			translateYObstacol[i] = randomNum(-1, 4);
		}

		for (int i = 0; i < 5; i++) {
			translateXObstacol[i] = randomNum(1, 6);
		}
		timePassed = 0;
	}

	glm::vec3 a = modelMatrixAvion * glm::vec4(0, 0, 0, 1);

	for (int i = 0; i < 4; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata2[i], translateYViata2[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata2[i] = 0, translateYViata2[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata2[i], translateYViata2[i] - 0.2f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata2[i] = 0, translateYViata2[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata2[i], translateYViata2[i] - 0.4f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata2[i] = 0, translateYViata2[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata2[i], translateYViata2[i] - 0.6f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata2[i] = 0, translateYViata2[i] = -7;
			}
		}

	}

	for (int i = 0; i < 3; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata3[i], translateYViata3[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata3[i] = 0, translateYViata3[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata3[i] - 0.2f, translateYViata3[i] - 0.1f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata3[i] = 0, translateYViata3[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata3[i] - 0.4f, translateYViata3[i] - 0.2f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata3[i] = 0, translateYViata3[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata3[i] - 0.6f, translateYViata3[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata3[i] = 0, translateYViata3[i] = -7;
			}
		}

	}

	for (int i = 0; i < 4; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata4[i], translateYViata4[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata4[i] = 0, translateYViata4[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata4[i] - 0.2f, translateYViata4[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata4[i] = 0, translateYViata4[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata4[i] - 0.4f, translateYViata4[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata4[i] = 0, translateYViata4[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata4[i] - 0.6f, translateYViata4[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata4[i] = 0, translateYViata4[i] = -7;
			}
		}

	}

	for (int i = 0; i < 4; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata5[i], translateYViata5[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata5[i] = 0, translateYViata5[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata5[i], translateYViata5[i] - 0.2f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata5[i] = 0, translateYViata5[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata5[i], translateYViata5[i] - 0.4f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata5[i] = 0, translateYViata5[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata5[i], translateYViata5[i] - 0.6f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata5[i] = 0, translateYViata5[i] = -7;
			}
		}

	}

	for (int i = 0; i < 4; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata6[i], translateYViata6[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata6[i] = 0, translateYViata6[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata6[i], translateYViata6[i] - 0.2f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata6[i] = 0, translateYViata6[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata6[i], translateYViata6[i] - 0.4f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata6[i] = 0, translateYViata6[i] = -7;
			}

		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata6[i], translateYViata6[i] - 0.6f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.5f) {
				lifeAdd += 0.03f;
				translateXViata6[i] = 0, translateYViata6[i] = -7;
			}

		}
	}

	for (int i = 0; i < 5; i++) {


		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieObstacol);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXObstacol[i], translateYObstacol[i], -3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		RenderMesh(meshes["obstacol"], shaders["ShaderBara"], modelMatrix, culoareObstacol);

		glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
		distanceObstacol = glm::distance(a, lf);

		if (distanceObstacol < 0.5f) {
			rotatieObstacol = -0.22f;
			count++;
		}
	}



	for (int i = 0; i < 5; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata[i], translateYViata[i], -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.2f) {
				lifeAdd++;
				translateXViata[i] = 0, translateYViata[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata[i] + 0.2f, translateYViata[i] - 0.2f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.2f) {
				lifeAdd++;
				translateXViata[i] = 0, translateYViata[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata[i] + 0.4f, translateYViata[i] - 0.4f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.2f) {
				lifeAdd++;
				translateXViata[i] = 0, translateYViata[i] = -7;
			}
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
			modelMatrix *= Transform3D::RotateOX(rotatieViata1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(translateXViata[i] + 0.6f, translateYViata[i] - 0.6f, -3));
			modelMatrix *= Transform3D::RotateOX(rotatieNor1);
			modelMatrix *= Transform3D::RotateOY(RADIANS(-80));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.11f, 0.11f, 0.11f));
			RenderMesh(meshes["viata"], shaders["ShaderBara"], modelMatrix, culoareViata);

			glm::vec3 lf = modelMatrix * glm::vec4(0, 0, 0, 1);
			distanceViata = glm::distance(a, lf);

			if (distanceViata < 0.2f) {
				lifeAdd++;
				translateXViata[i] = 0, translateYViata[i] = -7;
			}
		}

	}

	// nor 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.8, 0.85, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.7, 1.1f, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4, 1, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.23f, 0.23f, 0.23f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4.1, 0.8, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-40));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4.2, 1, -6.5f));
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	// nor 1
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.8, -27.85, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.7, -28.1f, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4, -28, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.23f, 0.23f, 0.23f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4.1, -27.8f, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor1);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-40));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(4.2, -28, -7));
		modelMatrix *= Transform3D::RotateOZ(rotatieNor1);
		modelMatrix *= Transform3D::RotateOX(rotatieNor1);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}


	//nor2

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.8, 0.75, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.6, 1, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.4, 0.78, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.23f, 0.23f, 0.23f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.1f, 0.8, -6.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-40));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	//nor2

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.8, -30.75, -5.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(+3.6, -31, -5.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.4, -30.78, -5.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.23f, 0.23f, 0.23f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(3.1f, -30.8, -5.5f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-40));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	//nor2

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.8, -31.75, -5.75f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.6, -32, -5.75f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.26f, 0.26f, 0.26f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.4, -31.78, -5.75f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.23f, 0.23f, 0.23f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.1f, -31.8, -5.75f));
		modelMatrix *= Transform3D::RotateOY(rotatieNor2);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor2);
		modelMatrix *= Transform3D::RotateOX(rotatieNor2);
		modelMatrix *= Transform3D::RotateOY(RADIANS(-40));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	//nor3
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 1.75, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.36f, 0.36f, 0.36f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4, 1.55, -6));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.6, 1.45, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.7, 1.45, -6));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.9, 1.65, -6));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.43, 1.65, -6));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	//nor3
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.1f, 3.75, -7));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.36f, 0.36f, 0.36f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.4, 3.55, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.6, 3.45, -7));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.27f, 0.27f, 0.27f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.7, 3.45, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.9, 3.65, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -15, -6));
		modelMatrix *= Transform3D::RotateOX(rotatieNor3);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, +15, +6));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.43, 3.65, -7));
		modelMatrix *= Transform3D::RotateOY(rotatieNor3);
		modelMatrix *= Transform3D::RotateOZ(rotatieNor3);
		modelMatrix *= Transform3D::RotateOY(RADIANS(45));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.28f, 0.28f, 0.28f));
		RenderMesh(meshes["box"], shaders["ShaderJoc"], modelMatrix, culoareNor);
	}

	{
		modelMatrixAvion = glm::mat4(1);
		modelMatrixAvion *= Transform3D::RotateOX(RADIANS(rotatePlaneAux));
		if (gameOver == 0) {
			modelMatrixAvion = glm::translate(modelMatrixAvion, glm::vec3(translatePlaneX, translateY, -3));
		}
		else {
			modelMatrixAvion = glm::translate(modelMatrixAvion, glm::vec3(translatePlaneX, translateAuxY, -3));
		}
		modelMatrixAvion *= Transform3D::RotateOX(RADIANS(rotatePlane));
		modelMatrixAvion = glm::scale(modelMatrixAvion, glm::vec3(0.14f, 0.14f, 0.14f));
		RenderMesh(meshes["plane"], shaders["VertexNormal"], modelMatrixAvion);


		modelMatrixAvion = glm::translate(modelMatrixAvion, glm::vec3(-0.2f, 3, 1.4f));
		modelMatrixAvion *= Transform3D::RotateOZ(rotatieElice);
		modelMatrixAvion *= Transform3D::RotateOY(RADIANS(90));
		modelMatrixAvion = glm::scale(modelMatrixAvion, glm::vec3(0.11f, 0.11f, 0.11f));

		modelMatrixAvion = glm::scale(modelMatrixAvion, glm::vec3(7, 7, 7));
		RenderMesh(meshes["elice"], shaders["VertexNormal"], modelMatrixAvion);

	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -13, -5));
		modelMatrix *= Transform3D::RotateOX(angularStepOX);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
		RenderMesh(meshes["mare"], shaders["ShaderJoc"], modelMatrix, culoareMare, timePassed3);
	}

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{

	}
}

void Laborator5::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	mesh->Render();
}

void Laborator5::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color, float time)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(shader->program, "time");
	glUniform1f(location, time);

	mesh->Render();
}

void Laborator5::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator5::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->MoveForward(cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->MoveForward(-cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(cameraSpeed * deltaTime * 10);
		}

		if (window->KeyHold(GLFW_KEY_Z)) {
			fv += deltaTime * 10;
			projectionMatrix = glm::perspective(fv, window->props.aspectRatio, 0.01f, 200.0f);
		}

		if (window->KeyHold(GLFW_KEY_X)) {
			fv -= deltaTime * 10;
			projectionMatrix = glm::perspective(fv, window->props.aspectRatio, 0.01f, 200.0f);
		}

		if (window->KeyHold(GLFW_KEY_J)) {
			left += deltaTime * 100;
			top += deltaTime * 100;
			right += deltaTime * 100;
			bottom += deltaTime * 100;
			projectionMatrix = glm::ortho(-left, right, -bottom, top, 0.1f, 200.0f);
		}

		if (window->KeyHold(GLFW_KEY_K)) {

			left -= deltaTime * 100;
			top -= deltaTime * 100;
			right -= deltaTime * 100;
			bottom -= deltaTime * 100;
			projectionMatrix = glm::ortho(-left, right, -bottom, top, 0.1f, 200.0f);
		}
	}
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (rotatePlane < 0) {
			rotatePlaneAux += deltaTime * 0.4f;
		}
		else if (rotatePlane > 0) {
			rotatePlaneAux -= deltaTime * 0.4f;
		}
	}
}

void Laborator5::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_O) {
		projectionMatrix = glm::ortho(ortL, 10.0f, ortDown, ortUp, 0.1f, 200.0f);
	}

	if (key == GLFW_KEY_P) {
		// TODO : translate the camera up
		projectionMatrix = glm::perspective(fv, window->props.aspectRatio, 0.01f, 200.0f);
	}
	

	if (key == GLFW_KEY_C) {
		pressC = !pressC;
		if (pressC == 0) {
			camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			rotateBara = RADIANS(0);
			translateZbara = -4;
			translateLife1Z = -4;
			translateLife2Z = -4;
			translateLife3Z = -4;
			translateLife1X = -5.7f;
			translateLife2X = -4.7f;
			translateLife3X = -3.7f;
			translateLifeY1 = translateLifeY2 = translateLifeY3 = 3.5f;
			
		}
		else {
			camera->Set(glm::vec3(translatePlaneX - 5, translateY + 2, -3), glm::vec3(translatePlaneX + 6, translateY, -3), glm::vec3(0, 1, 0));
			rotateBara = RADIANS(90);
			translateZbara = -8;
			translateLife1Z = -8;
			translateLife2Z = -7;
			translateLife3Z = -6;
			translateLife1X = translateXbara-2;
			translateLife2X = translateXbara-2;
			translateLife3X = translateXbara-2;
			translateLifeY1 = translateLifeY2 = translateLifeY3 = 4;

		}
	}
}

void Laborator5::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	float sensivityOX = 0.005f;
	float sensivityOY = 0.005f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
			camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
		}

	}

	rotatePlane += deltaY * 0.1f;

	if (pressC == 1) {
		camera->TranslateUpword(-sensivityOY * deltaY);
	}

	if (mouseY < 570) {
		if (flag == false) {
			translateY += -deltaY * sensivityOY;
			flag = true;
		}
		if (flag == true) {
			translateY += -deltaY * sensivityOY;

		}
	}
	else if (mouseY > 570) {
		if (flag == false) {
			translateY -= -deltaY * sensivityOY;
			flag = true;
		}
		if (flag == true) {
			translateY += -deltaY * sensivityOY;

		}
	}
	else if (mouseY == 570) {
		translateY = 0;
		flag = true; // flag care arata ca s-a atins pamantul
	}
	if (mouseY <= 5) {
		translateY = 3;
	}
}


