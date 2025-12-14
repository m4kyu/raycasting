#include <raylib.h>
#include <stdio.h>
#include <math.h>



float pos_x = 100;
float pos_y = 100;  
int rotation = 0;
float dx = 0;
float dy = 1;

int map[][5] = {
  {1, 1, 1, 1, 1},
  {1, 0, 1, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 0, 1, 0, 1},
  {1, 1, 1, 1, 1},
};


void drawPlayer();
void move();
void castRays(int fov);
void drawMap();



int main() {
  InitWindow(500, 500, "raycasting");
  SetTargetFPS(60);

    dy = sin(rotation * PI / 180);
    dx = cos(rotation * PI / 180);

  while (!WindowShouldClose()) {
    move();
    BeginDrawing();
    ClearBackground(BLACK);
    drawMap();
    drawPlayer();
    castRays(70);

    DrawFPS(0, 0);
    EndDrawing();

  }

  CloseWindow();
}


void drawPlayer() {
  DrawCircle(pos_x, pos_y, 10, YELLOW);
  DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x + 15 * dx, pos_y + 15 * dy}, 3, RED);
}

void drawMap() {
  int x = 0, y = 0;
  Color color;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      color = map[i][j] == 0 ? BLACK : BLUE;

      DrawRectangle(x, y, 100, 100, color);
      x += 100;
    }
    y += 100;
    x = 0;
  }
}

void castRays(int fov) {
  float norm_x = (float)((int)pos_x % 100) / 100; 
  float norm_y = (float)((int)pos_y % 100) / 100;
  int index_x = pos_x / 100;
  int index_y = pos_y / 100;
 

  for (int i = -fov/2; i < fov / 2; i++) {
    float diry = sin((rotation + i) * PI / 180); 
    float dirx = cos((rotation + i) * PI / 180); 
    int cor_x = index_x, cor_y = index_y;

    Vector2 step = {sqrtf(1 + (diry / dirx) * (diry / dirx)), sqrtf(1 + (dirx / diry) * (dirx / diry))};
    Vector2 vstep = {0};
       


    DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x + 200 *dirx, pos_y + 200 * diry}, 1, GREEN);
  }
}

void move() {
  if (IsKeyDown(KEY_W)) {
    printf("DX: %f. DY: %f\n", dx, dy);
    pos_y += 1 * dy;
    pos_x += 1 * dx;
  } else if (IsKeyDown(KEY_S)) {
    pos_y -= 1 * dy;
    pos_x -= 1 * dx;
  }

  if (IsKeyDown(KEY_A)) {
    pos_x += 1 * dy;
    pos_y += 1 * dx;
  } else if (IsKeyDown(KEY_D)) {
    pos_x -= 1 * dy;
    pos_y -= 1 * dx;
  }

  if (IsKeyDown(KEY_Q)) {
    rotation--;
    dy = sin(rotation * PI / 180);
    dx = cos(rotation * PI / 180);
  } else if (IsKeyDown(KEY_E)) {
    rotation++;
    dy = sin(rotation * PI / 180);
    dx = cos(rotation * PI / 180);
  }
}


