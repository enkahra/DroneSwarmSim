#include "raylib.h"

struct Agent {
  Vector2 position;
};

int main() {
  // Pencere genişlik ve yüksekliği
  const int screenWidth = 1200;
  const int screenHeight = 800;

  // Pencereyi başlat
  InitWindow(screenWidth, screenHeight, "Drone Swarm Simulator - v0.1");

  Agent myAgent;
  myAgent.position = {screenWidth / 2.0f, screenHeight / 2.0f};

  // Hedeflenen FPS (Saniyedeki kare hızı)
  SetTargetFPS(60);

  // Ana Oyun Döngüsü (Pencere kapatılana kadar döner)
  while (!WindowShouldClose()) {
    // --- GÜNCELLEME (Update) ---
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