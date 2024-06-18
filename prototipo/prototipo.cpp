#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include <queue>

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

typedef struct Player {
	Vector2 position;
	float speed;
	bool canJump;
} Player;

typedef struct EnvItem {
	Rectangle rect;
	int verticalBlocking;
	int horizontalBlockingL;
	int horizontalBlockingR;
	Color color;
} EnvItem;


void UpdatePlayer(Player* player, EnvItem* envItems, std::vector<EnvItem>& wals, int envItemsLength, float delta) {
	if (IsKeyDown(KEY_A)) player->position.x -= PLAYER_HOR_SPD * delta;
	if (IsKeyDown(KEY_D)) player->position.x += PLAYER_HOR_SPD * delta;
	if (IsKeyDown(KEY_SPACE) && player->canJump) {
		player->speed = -PLAYER_JUMP_SPD;
		player->canJump = false;
	}

	bool hitObstacle = false;
	for (int i = 0; i < envItemsLength; i++)
	{
		EnvItem* ei = envItems + i;
		Vector2* p = &(player->position);
		if (ei->verticalBlocking &&
			ei->rect.x <= p->x &&
			ei->rect.x + ei->rect.width >= p->x &&
			ei->rect.y >= p->y &&
			ei->rect.y <= p->y + player->speed * delta)
		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->y = ei->rect.y;
			break;
		}
		if (ei->horizontalBlockingL &&
			ei->rect.y <= p->y &&
			ei->rect.y + ei->rect.height >= p->y &&
			ei->rect.x <= p->x && p->x < ei->rect.x + ei->rect.width)

		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->x = ei->rect.x;
			break;
		}
		if (ei->horizontalBlockingR &&
			ei->rect.y <= p->y &&
			ei->rect.y + ei->rect.height >= p->y &&
			ei->rect.x + ei->rect.width >= p->x && p->x > ei->rect.x)
		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->x = ei->rect.x + ei->rect.width;
			break;
		}
	}

	for (int i = 0; i < wals.size(); i++)
	{
		EnvItem ei = wals[i];
		Vector2* p = &(player->position);
		if (ei.verticalBlocking &&
			ei.rect.x <= p->x &&
			ei.rect.x + ei.rect.width >= p->x &&
			ei.rect.y >= p->y &&
			ei.rect.y <= p->y + player->speed * delta)
		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->y = ei.rect.y;
			break;
		}
		if (ei.horizontalBlockingL &&
			ei.rect.y <= p->y &&
			ei.rect.y + ei.rect.height >= p->y &&
			ei.rect.x <= p->x && p->x < ei.rect.x + ei.rect.width)

		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->x = ei.rect.x;
			break;
		}
		if (ei.horizontalBlockingR &&
			ei.rect.y <= p->y &&
			ei.rect.y + ei.rect.height >= p->y &&
			ei.rect.x + ei.rect.width >= p->x && p->x > ei.rect.x)
		{
			hitObstacle = true;
			player->speed = 0.0f;
			p->x = ei.rect.x + ei.rect.width;
			break;
		}
	}

	if (!hitObstacle)
	{
		player->position.y += player->speed * delta;
		player->speed += G * delta;
		player->canJump = false;
	}

	else player->canJump = true;

	if (player->position.y < wals[3].rect.y) {
		std::cout << "VISHE";
		std::vector<EnvItem>(wals.begin() + 2, wals.end()).swap(wals);
		EnvItem last = wals[wals.size()-1];
		EnvItem prev_last = wals[wals.size() - 2];
		wals.push_back({ { last.rect.x, last.rect.y-175, last.rect.width, last.rect.height}, 1, 1, 0,LIGHTGRAY });
		wals.push_back({ { prev_last.rect.x, prev_last.rect.y-175, prev_last.rect.width, prev_last.rect.height}, 1, 1, 0,LIGHTGRAY });
	}
}

void UpdateCameraCenter(Camera2D* camera, Player* player, EnvItem* envItems, int envItemsLength, float delta, int width, int height) {
	camera->offset = Vector2{ width / 2.0f, height / 2.0f };
	camera->target = player->position;
}

int main(void)
{
	const int ScreenWidth = 1280;
	const int ScreenHeight = 720;

	std::cout << "Starting the game..." << std::endl;
	InitWindow(ScreenWidth, ScreenHeight, "hz");
	SetTargetFPS(60);

	Player player = { 0 };
	player.position = Vector2{ 640, 280 };
	player.speed = 0;
	player.canJump = false;
	EnvItem envItems[] = {
		{{ 0, 0, 1000, 400 }, 0, 0, 0,LIGHTGRAY },
		{{ 0, 400, 1000, 200 }, 1, 0, 0, GRAY },
		{{ 300, 200, 400, 10 }, 1, 0, 0,GRAY },
		{{ 250, 300, 100, 10 }, 1, 0, 0,GRAY },
		{{ 650, 300, 100, 10 }, 1, 0, 0,GRAY },
		{{975, 200, 10, 150},0, 1, 0, GRAY},
		{{15, 200, 10, 150}, 0, 0, 1, GRAY},
		{{975, -10, 10, 150},0, 1, 0, GRAY},

		{{15, -10, 10, 150}, 0, 0, 1, GRAY}
	};
	
	std::vector<EnvItem> wals;
	
	//wall 1
	wals.push_back({ {400, 0, 5, 150},1, 1, 0, BLACK });
	wals.push_back({ {405, 0, 5, 150},1, 0, 1, BLACK });
	//wals.push_back({{600, 0, 5, 150},1, 0, 1, BLACK});
	//wals.push_back({{595, 0, 5, 150},1, 1, 0, BLACK});
	//wall 2
	//wals.push_back({{400, -175, 5, 150},1, 1, 0, BLACK});
	//wals.push_back({{405, -175, 5, 150},1, 0, 1, BLACK});
	wals.push_back({ { 600, -175, 5, 150 }, 1, 0, 1, BLACK });
	wals.push_back({ {595, -175, 5, 150},1, 1, 0, BLACK });
	//wall 3
	wals.push_back({ {400, -350, 5, 150},1, 1, 0, BLACK });
	wals.push_back({ {405, -350, 5, 150},1, 0, 1, BLACK });
	//wals.push_back({{600, -350, 5, 150},1, 0, 1, BLACK});
	//wals.push_back({{595, -350, 5, 150},1, 1, 0, BLACK});
	//wall 4
	wals.push_back({ {400, -525, 5, 150},1, 1, 0, BLACK });
	wals.push_back({ {405, -525, 5, 150},1, 0, 1, BLACK });
	//wals.push_back({{600, -525, 5, 150},1, 0, 1, BLACK});
	//wals.push_back({{595, -525, 5, 150},1, 1, 0, BLACK});
	//wall 5
	//wals.push_back({{400, -700, 5, 150},1, 1, 0, BLACK});
	//wals.push_back({{405, -700, 5, 150},1, 0, 1, BLACK});
	wals.push_back({ {600, -700, 5, 150},1, 0, 1, BLACK });
	wals.push_back({ {595, -700, 5, 150},1, 1, 0, BLACK });
	//wall 6
	wals.push_back({ {400, -875, 5, 150},1, 1, 0, BLACK });
	wals.push_back({ {405, -875, 5, 150},1, 0, 1, BLACK });
	wals.push_back({ {600, -875, 5, 150},1, 0, 1, BLACK });
	wals.push_back({ {595, -875, 5, 150},1, 1, 0, BLACK });
	//wall 7
	wals.push_back({ {400, -1050, 5, 150},1, 1, 0, BLACK });
	wals.push_back({ {405, -1050, 5, 150},1, 0, 1, BLACK });
	wals.push_back({ {600, -1050, 5, 150},1, 0, 1, BLACK });
	wals.push_back({ {595, -1050, 5, 150},1, 1, 0, BLACK });

		

	int envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

	Camera2D camera = { 0 };
	camera.target = player.position;
	camera.offset = Vector2{ ScreenWidth / 2.0f, ScreenHeight / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
		UpdateCameraCenter,
	};

	int cameraOption = 0;
	int cameraUpdatersLength = sizeof(cameraUpdaters) / sizeof(cameraUpdaters[0]);

	while (!WindowShouldClose()) {

		float deltaTime = GetFrameTime();

		UpdatePlayer(&player, envItems, wals, envItemsLength, deltaTime);

		BeginDrawing();
		ClearBackground(DARKBLUE);

		cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, ScreenWidth, ScreenHeight);

		BeginMode2D(camera);
		for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
		for (int i = 0; i < wals.size(); i++) DrawRectangleRec(wals[i].rect, wals[i].color);

		Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
		DrawRectangleRec(playerRect, RED);

		EndMode2D();

		EndDrawing();
	}

	return 0;
}
