#include "raylib.h"
#include "raymath.h"

#include "agent.h"
#include <vector>

int main() {
  // Pencere genişlik ve yüksekliği
  const int screenWidth = 1200;
  const int screenHeight = 800;

  // Pencereyi başlat
  InitWindow(screenWidth, screenHeight, "Drone Swarm Simulator - v0.1");

  // Hedeflenen FPS (Saniyedeki kare hızı)
  SetTargetFPS(60);

  Swarm mySwarm;

  // Ana Oyun Döngüsü (Pencere kapatılana kadar döner)
  while (!WindowShouldClose()) {
    // --- UPDATE ---
    
    mySwarm.update(screenWidth, screenHeight);

    // --- ÇİZİM (Draw) ---
    BeginDrawing();
    ClearBackground(RAYWHITE); // Arka planı beyaz yap

    mySwarm.Draw();

    // Ekrana yazı yaz
    DrawText("Swarm Simulator Baslatildi!", 400, 350, 30, DARKGRAY);
    DrawText("Press ESC to exit", 10, 10, 20, LIGHTGRAY);

    EndDrawing();
  }

  // Pencereyi kapat ve temizle
  CloseWindow();

  return 0;
}