#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define DEGR2RAD PI / 180
#define FOV 60 
#define FPS 60
#define SPEED 500
#define ROTATION_SPEED 100

#define CELL_SIZE 100
#define MAP_WIDTH 10 
#define MAP_HIGHT 10

#define WIDTH 1980
#define HIGHT 1280


Vector2 player = {110, 110};
Vector2 direction;
float rotation = 0;

int map[MAP_HIGHT][MAP_HIGHT] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 1, 1, 1, 0, 1},
  {1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
};


void drawPlayer();
void move(float fram_time);
void castRays(int fov);
void drawMap();



int main() {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WIDTH, HIGHT, "raycasting");
//  SetTargetFPS(FPS);

  direction.y = sin(rotation * DEG2RAD);
  direction.x = cos(rotation * DEG2RAD);

  float frame_time = 0;
  while (!WindowShouldClose()) {
    move(frame_time);
    BeginDrawing();
    ClearBackground(BLACK);
//    drawMap();
    castRays(FOV);
 //   drawPlayer();


    DrawFPS(0, 0);
    frame_time = GetFrameTime();
    EndDrawing();

  }

  CloseWindow();
}


void drawPlayer() {
  DrawCircle(player.x, player.y, 10, YELLOW);
  DrawLineEx((Vector2){player.x, player.y}, (Vector2){player.x + 15 * direction.x, player.y + 15 * direction.y}, 3, RED);
}

void drawMap() {
  int x = 0, y = 0;
  Color color;
  for (int i = 0; i < MAP_HIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      color = map[i][j] == 0 ? BLACK : BLUE;

      DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, color);
      x += CELL_SIZE;
    }
    y += CELL_SIZE;
    x = 0;
  }
}

void castRays(int fov) {
  float norm_x = (float)((int)player.x % CELL_SIZE) / CELL_SIZE; 
  float norm_y = (float)((int)player.y % CELL_SIZE) / CELL_SIZE;


  float step = (float)fov / WIDTH;
  float deltaA = -(float)fov / 2;

  for (int i = 0, j = 0; i < WIDTH; i++, j++, deltaA += step) {
    int index_x = player.x / CELL_SIZE;
    int index_y = player.y / CELL_SIZE;
    float diry = sin((rotation + deltaA) * DEG2RAD); 
    float dirx = cos((rotation + deltaA) * DEG2RAD); 
    int cor_x = index_x, cor_y = index_y;

    Vector2 deltaStep = {sqrtf(1 + (diry / dirx) * (diry / dirx)), sqrtf(1 + (dirx / diry) * (dirx / diry))};
    Vector2 stepdir = {0};
    Vector2 rayLen = {0};

    if (dirx < 0) {
			stepdir.x = -1;
			rayLen.x = norm_x * deltaStep.x;
		} else {
			stepdir.x = 1;
			rayLen.x =  (1 - norm_x) * deltaStep.x;
		}


	  if (diry < 0) {
			stepdir.y = -1;
			rayLen.y = norm_y * deltaStep.y;
		} else {
			stepdir.y = 1;
		  rayLen.y = (1 - norm_y) * deltaStep.y;
		}


    int hit = 0;
		float distance = 0.0f;
    float maxd = 1000.0f;

    Color color;
    int side;
		while (!hit && distance < maxd)  {
			if (rayLen.x < rayLen.y) {
				index_x += stepdir.x;
				distance = rayLen.x;
				rayLen.x += deltaStep.x;
        color = GREEN;
        side = 0;
			} else {
				index_y += stepdir.y;
				distance = rayLen.y;
				rayLen.y += deltaStep.y;
        color = BLUE;
        side = 1;
			}

			if (index_x >= 0 && index_x < MAP_WIDTH && index_y >= 0 && index_y < MAP_HIGHT) {
				if (map[index_y][index_x] == 1)
					hit = 1;
			}
		}

    float perpWallDist;
    if(side == 0) perpWallDist = (rayLen.x - deltaStep.x);
    else          perpWallDist = (rayLen.y - deltaStep.y);


  //  DrawLineEx((Vector2){player.x, player.y}, (Vector2){player.x + distance * CELL_SIZE * dirx, player.y + distance * CELL_SIZE * diry}, 1, GREEN);

    float wall_size = HIGHT / (perpWallDist); 
    DrawLineEx((Vector2){(j), ((float)HIGHT / 2) - (wall_size / 2)}, (Vector2){(j), ((float)HIGHT / 2) + (wall_size / 2)}, 1, color);
  }
}

void move(float frame_time) {
  if (IsKeyDown(KEY_W)) {
    player.y += SPEED * direction.y * frame_time;
    player.x += SPEED * direction.x * frame_time;
  } else if (IsKeyDown(KEY_S)) {
    player.y -= SPEED * direction.y * frame_time;
    player.x -= SPEED * direction.x * frame_time;
  }

  if (IsKeyDown(KEY_A)) {
    player.x += SPEED * direction.y * frame_time;
    player.y += SPEED * -direction.x * frame_time;
  } else if (IsKeyDown(KEY_D)) {
    player.x += SPEED * -direction.y * frame_time;
    player.y += SPEED * direction.x * frame_time;
  }

  if (IsKeyDown(KEY_Q)) {
    rotation -= ROTATION_SPEED * frame_time;
    direction.y = sin(rotation * PI / 180);
    direction.x = cos(rotation * PI / 180);
  } else if (IsKeyDown(KEY_E)) {
    rotation += ROTATION_SPEED * frame_time;
    direction.y = sin(rotation * PI / 180);
    direction.x = cos(rotation * PI / 180);
  }
}


