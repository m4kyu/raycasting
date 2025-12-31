#include <raylib.h>
#include <math.h>
#include <stdio.h>

#define DEGR2RAD PI / 180
#define FOV 60 
#define FPS 60
#define SPEED 300
#define ROTATION_SPEED 200

#define CELL_SIZE 100
#define MAP_WIDTH 10 
#define MAP_HIGHT 10

#define WIDTH 1980
#define HIGHT 1280


Vector2 player = {121, 121};
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
    castRays(FOV);
//    drawMap();

  //  drawPlayer();


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

    if (!hit) {
      continue;
    }

    float perpWallDist;
    if(side == 0) perpWallDist = (rayLen.x - deltaStep.x);
    else          perpWallDist = (rayLen.y - deltaStep.y);

    
    float wall_size = HIGHT / (perpWallDist * 2); 
    DrawLineEx((Vector2){(j), ((float)HIGHT / 2) - (wall_size / 2)}, (Vector2){(j), ((float)HIGHT / 2) + (wall_size / 2)}, 1, color);
  }
}

void move(float frame_time) {
  const float DIST = 10;
  

  float addx = 0, addy = 0;
  int ismove = 0;
 
  int index_x = player.x / CELL_SIZE;
  int index_y = player.y / CELL_SIZE;
  if (direction.x < 0) {
    addx = -DIST;
  } else if (direction.x > 0) {
    addx = DIST;
  }
  if (direction.y < 0) {
    addy = -DIST;
  } else if (direction.y > 0){
    addy = DIST;
  }

    int new_x = (int)((player.x + addx) / CELL_SIZE);
  int new_y = (int)((player.y + addy) / CELL_SIZE);
  int new_x_back = (int)((player.x - addx) / CELL_SIZE);
  int new_y_back = (int)((player.y - addy) / CELL_SIZE);


  if (IsKeyDown(KEY_W)) {
    if (map[index_y][new_x] == 0) {
      player.x += SPEED * direction.x * frame_time;
    }
    if (map[new_y][index_x] == 0) {
      player.y += SPEED * direction.y * frame_time;
    }
  } else if (IsKeyDown(KEY_S)) {
    if (map[index_y][new_x_back] == 0) {
      player.x -= SPEED * direction.x * frame_time;
    }
    if (map[new_y_back][index_x] == 0) {
      player.y -= SPEED * direction.y * frame_time;
    }
  }

  index_x = player.x / CELL_SIZE;
  index_y = player.y / CELL_SIZE;


  int snew_x = (int)((player.x + addy) / CELL_SIZE);
  int snew_y = (int)((player.y + addx) / CELL_SIZE);
  int snew_x_back = (int)((player.x - addy) / CELL_SIZE);
  int snew_y_back = (int)((player.y - addx) / CELL_SIZE);


  if (IsKeyDown(KEY_A)) {
    if (map[index_y][snew_x] == 0) {
      player.x += SPEED * direction.y * frame_time;
    }
    if (map[snew_y_back][index_x] == 0) {
      player.y -= SPEED * direction.x * frame_time;
    }

  } else if (IsKeyDown(KEY_D)) {
    if (map[index_y][snew_x_back] == 0) {
      printf("D\n");
      player.x -= SPEED * direction.y * frame_time;
    }
    if (map[snew_y][index_x] == 0) {
      player.y += SPEED * direction.x * frame_time;
    }
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

