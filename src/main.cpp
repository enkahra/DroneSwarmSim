#include "raylib.h"
#include "raymath.h"

#include "agent.h"

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
    myAgent.update(screenWidth, screenHeight);
    myAgent.CheckBoundry(screenWidth, screenHeight);
    
    // Buraya matematiksel hesaplamalar gelecek

    // --- ÇİZİM (Draw) ---
    BeginDrawing();
    ClearBackground(RAYWHITE); // Arka planı beyaz yap

    // Ekrana yazı yaz
    DrawText("Swarm Simulator Baslatildi!", 400, 350, 30, DARKGRAY);
    DrawText("Press ESC to exit", 10, 10, 20, LIGHTGRAY);

    // Ortaya kırmızı bir nokta (Temsili Drone) koy
    myAgent.Draw();

    EndDrawing();
  }

  // Pencereyi kapat ve temizle
  CloseWindow();

  return 0;
}