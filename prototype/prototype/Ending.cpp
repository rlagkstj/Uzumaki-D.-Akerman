#include "Ending.h"
#include "raylib.h"
#include "GameState.h"



void EndGame1() {
	DrawText("Human Win!", 300, 200, 40, BLACK);
}

void EndGame2() {
	DrawText("Lobot Win!", 300, 200, 40, BLACK);
}