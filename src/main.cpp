#include "raylib.h"

#include "agent.h"
#include <cstddef>

int main() {
  // Pencere genişlik ve yüksekliği
  int screenWidth = 1200; // Make this number can scale with fullscreen option and customazible with few options
  int screenHeight = 800; // Make this number can scale with fullscreen option and customazible with few options
  

  // Pencereyi başlat
  InitWindow(0, 0, "Drone Swarm Simulator - v0.1"); // Make this version number system increase with every github commit by 0.1 or do something better
  ToggleFullscreen();
  
  screenWidth = GetScreenWidth();
  screenHeight = GetScreenHeight();


  // Text Options
  float displayTime = 3.0f; // Ekranda textin kalma suresi
  float timer = 0.0f; // Gecen sureyi tutuyor
    
  // Hedeflenen FPS (Saniyedeki kare hızı)
  SetTargetFPS(60); // Maybe add 120 and other fps options or make dynamic ???

  Swarm mySwarm(screenWidth, screenHeight);

  // Ana Oyun Döngüsü (Pencere kapatılana kadar döner)
  while (!WindowShouldClose()) {
    // --- UPDATE ---

    mySwarm.update(screenWidth, screenHeight);

    // --- ÇİZİM (Draw) ---
    BeginDrawing();
    ClearBackground(RAYWHITE); // Arka planı beyaz yap // Maybe add options to user choose backgroung color

    if(timer < displayTime) {
      timer += GetFrameTime();
      DrawText("Swarm Simulator Baslatildi!", 400, 350, 30, DARKGRAY); // After few seconds this text must disapper
    }

    // Adds circles
    mySwarm.Draw(); // Maybe add options to user choose drones color

    // Ekrana yazı yaz
    
    DrawText("Press ESC to exit", 10, 10, 20, YELLOW);
    DrawFPS(15, 40);

    EndDrawing();
  }

  // Pencereyi kapat ve temizle
  CloseWindow();

  return 0;
}