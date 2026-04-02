#include<iostream>
#include "raylib.h"
#include "object.h"
#include "Ending.h"
#include "GameState.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "prototype");
	SetTargetFPS(60);


	

	Map map;
	map.Load("map.txt", 60);

	Player player1(1, map.player1SpawnPos);
	Player player2(2, map.player2SpawnPos);
	player1.target = &player2;
	player2.target = &player1;
	player1.map = &map;
	player2.map = &map;
	float dt;

	GameState gameState = GameState::Playing;


	while (!WindowShouldClose()) {
		ClearBackground(RAYWHITE);
		dt = GetFrameTime();
		player1.Update(dt);
		player2.Update(dt);


		if (player1.hp <= 0) {
			gameState = GameState::Player2Win;
		}

		if (player2.hp <= 0) {
			gameState = GameState::Player1Win;
		}

		BeginDrawing();
		map.Draw();
		player1.Draw();
		player2.Draw();

		switch (gameState)
		{
		case GameState::Player1Win:
			EndGame1();
			break;

		case GameState::Player2Win:
			EndGame2();
			break;
		}

		EndDrawing();
	}



	

}