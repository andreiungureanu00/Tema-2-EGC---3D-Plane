#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"

class Laborator5 : public SimpleScene
{
	public:
		Laborator5();
		~Laborator5();

		void Init() override;
		float randomNum(float min, float max);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;


		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, const glm::vec3 color);
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3 color, float time);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix, modelMatrixAvion;
		bool renderCameraTarget;
        float fv;
        float right, left, bottom, top;
		float ortL = -6.0f, ortR = 6.0f, ortUp = 4.0f, ortDown = -4.0f;
		float length = 10.0f;
		float translateX, translateY, translateEliceY, translatePlaneX;
		float angularStepOX, angularStepOY, angularStepOZ;
		float rotatieNor1, rotatieNor2, rotatieNor3, rotatieElice, rotatieObstacol = 0, rotatieObstacolAux = 0;
		float rotatePlane, rotatePlane2, rotateElice, translateEliceX, rotatieViata1, rotatieViata2, rotatieViata3;
		bool flag = 0;
		int inclinaTare = 1, stabil = 0, first = 1, array[10], contor = 7, gameOver = 0, newGame = 0;
		float timePassed, timePassed2, timePassed3, translateAuxY = -5;
		float indicator = 0, nd, trX1 = 15, trX2 = 15, trX3 = 15;
		float translateNorX, translateNorY, translateNorZ, translateYViata[10], translateYViata2[10], translateXViata[10], translateXViata2[10];
		float translateYViata3[10], translateYViata4[10], translateYViata5[10], translateYViata6[10], translateXViata3[10], translateXViata4[10], translateXViata5[10], translateXViata6[10];
		float translateXObstacol[20], translateXbara = 3.7f, translateZbara = -4;
		float nr, nr2, scaleX = 0.5f, lifeAdd = 0, rotateBara = 0;
		float translateYObstacol[20], translateLife1X = -5.7f, translateLife2X = -4.7f, translateLife3X = -3.7f;
		float translateLifeY1 = 3.5f, distanceObstacol, distanceViata, translateLifeY2 = 3.5f, translateLifeY3 = 3.5f;
		glm::vec3 culoareMare, culoareNor, culoareViata, culoareObstacol;
		int count = 0, gainLife = 0, cameraChange = 0, pressC = 0;
		float translateLife1Z = -4, translateLife2Z = -4, translateLife3Z = -4, rotatePlaneAux;
};
