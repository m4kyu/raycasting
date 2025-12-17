#include <raylib.h>
#include <math.h>



float pos_x = 110;
float pos_y = 110;  
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
  InitWindow(500 * 2, 500, "raycasting");
  SetTargetFPS(60);

    dy = sin(rotation * PI / 180);
    dx = cos(rotation * PI / 180);

  while (!WindowShouldClose()) {
    move();
    BeginDrawing();
    ClearBackground(BLACK);
    drawMap();
    drawPlayer();
    castRays(80);

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


  float step1 = (float)fov / 500;
  float start = -(float)fov / 2;
  for (int i = 0, j = 0; i < 500; i++, j++, start += step1) {
      int index_x = pos_x / 100;
  int index_y = pos_y / 100;

    float diry = sin((rotation + start) * PI / 180); 
    float dirx = cos((rotation + start) * PI / 180); 
    int cor_x = index_x, cor_y = index_y;

    Vector2 step = {sqrtf(1 + (diry / dirx) * (diry / dirx)), sqrtf(1 + (dirx / diry) * (dirx / diry))};
    Vector2 stepdir = {0};
    Vector2 rayLen = {0};

    if (dirx < 0) {
			stepdir.x = -1;
			rayLen.x = norm_x * step.x;
		} else {
			stepdir.x = 1;
			rayLen.x =  (1 - norm_x) * step.x;
		}


	  if (diry < 0) {
			stepdir.y = -1;
			rayLen.y = norm_y * step.y;
		} else {
			stepdir.y = 1;
		  rayLen.y = (1 - norm_y) * step.y;
		}


    int hit = 0;
		float distance = 0.0f;
		while (!hit)  {
			if (rayLen.x < rayLen.y) {
				index_x += stepdir.x;
				distance = rayLen.x;
				rayLen.x += step.x;
			} else {
				index_y += stepdir.y;
				distance = rayLen.y;
				rayLen.y += step.y;
			}

			if (index_x >= 0 && index_x < 5 && index_y >= 0 && index_y < 5) {
				if (map[index_y][index_x] == 1)
					hit = 1;
			}
		}

    float wall_size = 500 / (distance * 3); 
    distance *= 100;
    DrawLineEx((Vector2){pos_x, pos_y}, (Vector2){pos_x + distance * dirx, pos_y + distance * diry}, 1, GREEN);
    DrawLineEx((Vector2){500 + (j), 250 - (wall_size / 2)}, (Vector2){500 + (j), 250 + (wall_size / 2)}, 1, GREEN);
  }
}

void move() {
  if (IsKeyDown(KEY_W)) {
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


