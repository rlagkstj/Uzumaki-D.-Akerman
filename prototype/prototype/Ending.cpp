#include "Ending.h"
#include "raylib.h"
#include "GameState.h"


int fontSize = 40;


void EndGame1() {
    const char* text = "Human Win!";

    int textWidth = MeasureText(text, fontSize);

    int x = (GetScreenWidth() - textWidth) / 2;
    int y = GetScreenHeight() / 2;

    DrawText(text, x, y, fontSize, WHITE);
}
void EndGame2() {
    const char* text = "Robot Win!";

    int textWidth = MeasureText(text, fontSize);

    int x = (GetScreenWidth() - textWidth) / 2;
    int y = GetScreenHeight() / 2;

    DrawText(text, x, y, fontSize, WHITE);
}