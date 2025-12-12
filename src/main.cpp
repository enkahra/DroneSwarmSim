#include "raylib.h"
#include "raymath.h"

struct Agent {
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
};

int main() {
  // Pencere genişlik ve yüksekliği
  const int screenWidth = 1200;
  const int screenHeight = 800;

  // Pencereyi başlat
  InitWindow(screenWidth, screenHeight, "Drone Swarm Simulator - v0.1");

  // Create a agent variable and initialize x and y positons to the middle of
  // the screen
  Agent myAgent;
  myAgent.position = {screenWidth / 2.0f, screenHeight / 2.0f};
  myAgent.velocity = {2.0f, 2.0f};
  myAgent.acceleration = {0.1f, 0.1f};

  // Hedeflenen FPS (Saniyedeki kare hızı)
  SetTargetFPS(60);

  // Ana Oyun Döngüsü (Pencere kapatılana kadar döner)
  while (!WindowShouldClose()) {
    // --- UPDATE ---
    myAgent.acceleration.x = 0;
    myAgent.acceleration.y = 0;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePosition = GetMousePosition();
      Vector2 temp = Vector2Subtract(mousePosition, myAgent.position);
      temp = Vector2Normalize(temp);
      myAgent.acceleration = Vector2Scale(temp, 0.5);
    }

    myAgent.velocity.x += myAgent.acceleration.x;
    myAgent.velocity.y += myAgent.acceleration.y;
    myAgent.position.x += myAgent.velocity.x;
    myAgent.position.y += myAgent.velocity.y;

    // Checking for boundry
    // XXX : This check may not work when screen resized
    if (myAgent.position.x > screenWidth) {
      myAgent.position.x = screenWidth;
      myAgent.velocity.x *= -1;
    }
    if (myAgent.position.x < 0) {
      myAgent.position.x = 0;
      myAgent.velocity.x *= -1;
    }
    if (myAgent.position.y > screenHeight) {
      myAgent.position.y = screenHeight;
      myAgent.velocity.y *= -1;
    }
    if (myAgent.position.y < 0) {
      myAgent.position.y = 0;
      myAgent.velocity.y *= -1;
    }

    // Friction values
    myAgent.velocity.x *= 0.95f;
    myAgent.velocity.y *= 0.95f;

    // Buraya matematiksel hesaplamalar gelecek

    // --- ÇİZİM (Draw) ---
    BeginDrawing();
    ClearBackground(RAYWHITE); // Arka planı beyaz yap

    // Ekrana yazı yaz
    DrawText("Swarm Simulator Baslatildi!", 400, 350, 30, DARKGRAY);
    DrawText("Press ESC to exit", 10, 10, 20, LIGHTGRAY);

    // Ortaya kırmızı bir nokta (Temsili Drone) koy
    DrawCircleV(myAgent.position, 20, RED);

    EndDrawing();
  }

  // Pencereyi kapat ve temizle
  CloseWindow();

  return 0;
}