#include "raylib.h"
#include <stdio.h>
#include <iostream>
#include <sstream>


struct Ball {
	float x, y;
	float rad;
	float speedX, speedY;
	
	void Draw() {
		DrawCircle((int)x, (int)y, rad, WHITE);
	}
};
struct Padle {
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRec() {
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw() {
		DrawRectangle(x - width / 2, y - height / 2, 10, 100, WHITE);
	}
};
struct Player {
	const char* message;
	int wins = 0;
	bool won = 0;

	void addWin() {
		wins++;
	}
};

int main() {
	
	InitWindow(800, 600, "Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	const char* winner = nullptr;
	int winsWASD = 0;
	int winsARROWS = 0;
	
	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.rad = 5.0;
	ball.speedX = 300;
	ball.speedY = 300;

	Padle padleLeft;
	padleLeft.x = 30.0;
	padleLeft.y = GetScreenHeight() / 2.0;
	padleLeft.width = 10;
	padleLeft.height = 100;
	padleLeft.speed = 500;

	Padle padleRight;
	padleRight.x = GetScreenWidth() - 50.0;
	padleRight.y = GetScreenHeight() / 2.0;
	padleRight.width = 10;
	padleRight.height = 100;
	padleRight.speed = 500;

	Player WASD;
	WASD.message = "WASD Wins!";
	
	Player Arrows;
	Arrows.message = "Arrows Wins!";

	InitAudioDevice();              // Initialize audio device

	Music music = LoadMusicStream("demo/BlankBanshee_StartUp.mp3");

	PlayMusicStream(music);
	

	while (!WindowShouldClose()) {

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();
		//top of screen (y=0)
		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;

		}//bottom of screen (y=height)
		if (ball.y >= GetScreenHeight()) {
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}
		if (IsKeyDown(KEY_W) && padleLeft.y - 50 > 0) {
			padleLeft.y -= padleLeft.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S) && padleLeft.y + 50 < GetScreenHeight()) {
			padleLeft.y += padleLeft.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP) && padleRight.y - 50 > 0) {
			padleRight.y -= padleRight.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN) && padleRight.y + 50 < GetScreenHeight()) {
			padleRight.y += padleRight.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.rad, padleLeft.GetRec())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - padleLeft.y) / (padleLeft.height) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.rad, padleRight.GetRec())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - padleRight.y) / (padleRight.height) * -ball.speedX;
			}
		}

		if (ball.x < 0) {
			Arrows.won = 1;
			Arrows.addWin();
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 0;
			ball.speedY = 0;
		}
		if (ball.x > GetScreenWidth()) {
			WASD.won = 1;
			WASD.addWin();
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 0;
			ball.speedY = 0;
		}
		if (IsKeyPressed(KEY_SPACE) && (Arrows.won == 1 || WASD.won == 1)) {
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			ball.speedY = 300;
			padleLeft.y = GetScreenHeight() / 2.0;
			padleRight.y = GetScreenHeight() / 2.0;
			WASD.won = 0;
			Arrows.won = 0;
		}
		

		BeginDrawing();
		
			ClearBackground(BLACK);

			ball.Draw();
			padleLeft.Draw();
			padleRight.Draw();

			if (WASD.won == 1 || Arrows.won == 1) {
				if (WASD.won == 1) {
					DrawText(WASD.message, 255, 215, 50, SKYBLUE);
				}
				if (Arrows.won == 1) {
					DrawText(Arrows.message, 245, 215, 50, SKYBLUE);
				}
				DrawText("Press SPACE to play again", 255, 340, 20, YELLOW);
				std::stringstream bufferWASD;
				bufferWASD << WASD.wins;
				
				DrawText("WASD WINS:",20,20,20,YELLOW);
				DrawText(bufferWASD.str().c_str(),150,20,20,YELLOW );

				std::stringstream bufferArrows;
				bufferArrows << Arrows.wins;

				DrawText("ARROWS WINS:", 620, 20, 20, YELLOW);
				DrawText(bufferArrows.str().c_str(),780 , 20, 20, YELLOW);
			}
			DrawFPS(50,0);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}