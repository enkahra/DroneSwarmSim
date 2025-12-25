#ifndef AGENT_H
#define AGENT_H

#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <vector>
#include <queue>

struct Swarm {
    // UI Variables
    bool showGrid = true;
    bool showHeatmap = true;
    std::vector<std::vector<int>> grid;
    std::vector<int> costField; // Map for flow field
    std::vector<Vector2> flowField;
    std::vector<int> integrationField; // hedefe olan toplam uzakligi tutan vektor
    
    std::vector<int> agentCellIndices;
    int proximityMeter = 20; // Line 111 this is for collison system
    int cellSize = 20;
    int columns = 0;
    int rows = 0;
    
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<Vector2> accelerations;
    int targetIndex = -1; // Mouse ile secilen hedefin indexi
    int swarmSize = 5; // Initial size is 1000
    int distanceToMouseThreshold = 100; // Line 94 this is for moving
    float maxSpeed = 5.0f;
    float maxForce = 0.2f;
    float friction = 0.95f;

    Texture2D agentSprite;
    int wallBrushValue = 255;


    void GenerateSprite() {
        Image img = GenImageColor(10, 10, BLANK);
        ImageDrawCircle(&img, 5, 5, 5, RED);
        agentSprite = LoadTextureFromImage(img);
        UnloadImage(img);
    }


    void Reset(int screenWidth, int screenHeight) {
        columns = screenWidth / cellSize; 
        rows = screenHeight / cellSize; 

        grid.resize(rows * columns);
        costField.resize(rows * columns);
        integrationField.resize(rows * columns);
        flowField.resize(rows * columns);
        agentCellIndices.resize(swarmSize);
        std::fill(costField.begin(), costField.end(), 1); // Default cost 1
        // Add border walls
        for(int i{0}; i < costField.size(); ++i) {
            int col = i % columns;
            int row = i / columns;
            if ((col == 0 || col == columns - 1) || (row == 0 || row == rows - 1)) {
                costField[i] = 255;
            }
        }
        std::fill(integrationField.begin(), integrationField.end(), 9999);
        std::fill(flowField.begin(), flowField.end(), Vector2{0.0f, 0.0f});

        positions.resize(swarmSize);
        velocities.resize(swarmSize);
        accelerations.resize(swarmSize);

        // Spawn drones random locations and make them still
        for (int i{0}; i < swarmSize; ++i) {
            positions[i].x = GetRandomValue(cellSize, screenWidth - cellSize);
            positions[i].y = GetRandomValue(cellSize, screenHeight - cellSize);

            velocities[i] = {0.0f, 0.0f};
            accelerations[i] = {0.0f, 0.0f};
        }
        
        targetIndex = -1;
    }

    Swarm(int screenWidth, int screenHeight) {
        // Initial setup
        GenerateSprite();
        Reset(screenWidth, screenHeight);
    }
    
    ~Swarm() {
        UnloadTexture(agentSprite);
    }


    void update(int screenWidth, int screenHeight) {
        if (IsKeyPressed(KEY_R)) {
            Reset(screenWidth, screenHeight);
            return;
        }

        Vector2 mousePosition = GetMousePosition();
        
        // UI Koruması: Eğer mouse sol üst köşedeki panelin üzerindeyse haritaya tıklamayı engelle
        if (mousePosition.x < 240 && mousePosition.y < 300) return;

        bool needsUpdate = false;

        for(auto& cell : grid) {
            cell.clear(); // Clear early cell
        }

        for(int i{0}; i < swarmSize; ++i) {
            int col = positions[i].x / cellSize;
            col = Clamp(col, 0, columns - 1); // 0 ile 23 arasina sinirliyoruz(24 olup out of bound hatasi vermesin diye)
            int row = positions[i].y / cellSize;
            row = Clamp(row, 0, rows - 1); // 0 ile 23 arasina sinirliyoruz
            int cellIndex = row * columns + col; // Ajan hangi hucrede bunu tutuyor
            agentCellIndices[i] = cellIndex; // Caching
            grid[cellIndex].push_back(i);
        }

        // Sağ tıklama ile Duvar Ekle/Kaldır
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            int x = mousePosition.x / cellSize;
            int y = mousePosition.y / cellSize;

            // Protect borders: > 0 and < columns - 1
            if (x > 0 && x < columns - 1 && y > 0 && y < rows - 1) {
                int index = y * columns + x;
                // If hitting a wall, we are erasing (1). Else adding (255)
                wallBrushValue = (costField[index] == 255) ? 1 : 255;
                costField[index] = wallBrushValue;
                needsUpdate = true;
            }

        }
        // Fırça Modu (Basılı tutunca sadece EKLEME yapar -> Changed to smart brush)
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            int x = mousePosition.x / cellSize;
            int y = mousePosition.y / cellSize;
             // Protect borders here too
             if (x > 0 && x < columns - 1 && y > 0 && y < rows - 1) {
                int index = y * columns + x;
                // Apply the persistent brush value
                if (costField[index] != wallBrushValue) {
                    costField[index] = wallBrushValue;
                    needsUpdate = true;
                }
             }

        }


        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            int x = mousePosition.x / cellSize;
            int y = mousePosition.y / cellSize;
            // Sınır koruması ve kayıt
            if (x >= 0 && x < columns && y >= 0 && y < rows) {
                targetIndex = y * columns + x;
                needsUpdate = true;
            }
            
        }

        if(needsUpdate && targetIndex != -1) {
            std::queue<int> tempQ; // hedefe olan toplam uzakligi tutan vektor ile alakali

            std::fill(integrationField.begin(), integrationField.end(), 9999);

            integrationField[targetIndex] = 0;
            tempQ.push(targetIndex);

            while(!tempQ.empty()) {
                int temp3 = tempQ.front();
                tempQ.pop();

                int x = temp3 % columns;
                int y = temp3 / columns;

                // 8 Yönlü Hareket (Daire şeklinde yayılma için)
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue; // Kendisi

                        int nx = x + dx;
                        int ny = y + dy;

                        // Sınır kontrolü
                        if (nx >= 0 && nx < columns && ny >= 0 && ny < rows) {
                            int neighborIndex = ny * columns + nx;
                            
                            // Eğer duvar (255) ise geçme
                            if (costField[neighborIndex] == 255) continue;
                            if (dx != 0 && dy != 0) {
                                int horizontalIndex = y * columns + (x + dx);
                                int verticalIndex = (y + dy) * columns + x;
                                // Eğer yanımız veya üstümüz duvarsa, çapraz geçiş yasak
                                if (costField[horizontalIndex] == 255 || costField[verticalIndex] == 255) continue;
                            }

                            // Çapraz mı düz mü? (Düz = 10 birim, Çapraz = 14 birim / 1.41 katı)
                            int weight = (dx != 0 && dy != 0) ? 14 : 10;
                            int newDistance = integrationField[temp3] + weight;

                            if (newDistance < integrationField[neighborIndex]) {
                                integrationField[neighborIndex] = newDistance;
                                tempQ.push(neighborIndex);
                            }
                        }
                    }
                }
            }
            for(int i{0}; i < flowField.size(); ++i) {
                int x = i % columns;
                int y = i / columns;
                int bestNeighbourIndex = -1;
                int minVal = integrationField[i];

                
                for (int dy{-1}; dy <= 1; ++dy) {
                    for (int dx{-1}; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < columns && ny >= 0 && ny < rows) { 
                            int neighborIndex = ny * columns + nx;
                            if (minVal > integrationField[neighborIndex]) {
                                minVal = integrationField[neighborIndex];
                                bestNeighbourIndex = neighborIndex;
                            }
                            if (dx != 0 && dy != 0) {
                                int horizontalIndex = y * columns + (x + dx);
                                int verticalIndex = (y + dy) * columns + x;
                                // Eğer yanımız veya üstümüz duvarsa, çapraz geçiş yasak
                                if (costField[horizontalIndex] == 255 || costField[verticalIndex] == 255) continue;
                            }
                        }
                    }
                }
                if (bestNeighbourIndex != -1) {
                    // Komşunun koordinatlarını bul
                    int bx = bestNeighbourIndex % columns;
                    int by = bestNeighbourIndex / columns;
                    // Yön Vektörü: Hedef - Ben
                    Vector2 direction = { (float)(bx - x), (float)(by - y) };
                    
                    // Normalize et (Her adımda eşit hızda gitmek için)
                    flowField[i] = Vector2Normalize(direction);
                } else {
                    // Çıkmaz sokak veya hedef (hareket yok)
                    flowField[i] = { 0.0f, 0.0f };
                }
            }
        }

        for (int i{0}; i < swarmSize; ++i) {
            accelerations[i] = {0, 0};

            int cellIndex = agentCellIndices[i]; // Takes from cached vector
            Vector2 flow = flowField[cellIndex];
            // Eğer geçerli bir akış varsa (0,0 değilse)
            if (flow.x != 0 || flow.y != 0) {
                // Desired Velocity (Arzulanan Hız)
                Vector2 desired = Vector2Scale(flow, maxSpeed);
                
                // Steering Force (Dümenleme Gücü) = Desired - Velocity
                Vector2 steer = Vector2Subtract(desired, velocities[i]);
                
                // Gücü Sınırla (Max Force) - Çok keskin dönmesinler diye
                if (Vector2Length(steer) > maxForce) {
                    steer = Vector2Scale(Vector2Normalize(steer), maxForce);
                }
                
                // Kuvveti Uygula
                accelerations[i] = Vector2Add(accelerations[i], steer);
            }

            velocities[i] = Vector2Add(velocities[i], accelerations[i]);
            positions[i] = Vector2Add(positions[i], velocities[i]);
            
            //  Friction 
            velocities[i] = Vector2Scale(velocities[i], friction); 

            CheckBoundary(screenWidth, screenHeight, i);
            }
        }
        
    void CheckBoundary(int screenWidth, int screenHeight, int i) { 
        if (positions[i].x > screenWidth - cellSize) {
            positions[i].x = screenWidth - cellSize;
            velocities[i].x *= -1;
        }
        if (positions[i].x < cellSize) {
            positions[i].x = cellSize;
            velocities[i].x *= -1;
        }
        if (positions[i].y > screenHeight - cellSize) {
            positions[i].y = screenHeight - cellSize;
            velocities[i].y *= -1;
        }
        if (positions[i].y < cellSize) {
            positions[i].y = cellSize;
            velocities[i].y *= -1;
        }
    }

    void Draw() {
        for(int i{0}; i < costField.size(); ++i) {
            int x = i % columns;
            int y = i / columns;
            int pixelX = x * cellSize;
            int pixelY = y * cellSize;

            // 1. ADIM: Heatmap (Mesafeye göre daireler ve renkler)
            if (showHeatmap && integrationField[i] < 9999) { 
                float maxDist = 500.0f; // Değerleri 10 ile çarptığımız için burayı da arttırdık
                float t = (float)integrationField[i] / maxDist;
                
                // Uzakları çizme, sadece yakını boya
                if (t <= 1.0f) {
                    unsigned char r, g, b;
                    if (t < 0.5f) {
                        float localT = t * 2.0f;
                        r = 255;
                        g = (unsigned char)(50 + localT * 205);
                        b = 50;
                    } else {
                        float localT = (t - 0.5f) * 2.0f;
                        r = (unsigned char)(255 - localT * 75);
                        g = (unsigned char)(255 - localT * 75);
                        b = (unsigned char)(50 + localT * 130);
                    }
                    // Yarı saydam renk
                    Color cellColor = { r, g, b, 150 }; 
                    DrawCircle(pixelX + cellSize/2, pixelY + cellSize/2, cellSize/1.5f, cellColor);
                }
            }

            // 2. ADIM: Grid ve Duvarlar (Her zaman en üstte çizilsin)
            if(costField[i] == 255) {
                DrawRectangle(pixelX, pixelY, cellSize, cellSize, BLACK);
            } else if (showGrid) {
                DrawRectangleLines(pixelX, pixelY, cellSize, cellSize, Fade(LIGHTGRAY, 0.3f));
            }
        }

        // 3. ADIM: Ajanlar
        for(int i{0}; i < swarmSize; ++i) {
            DrawTexture(agentSprite, positions[i].x -5, positions[i].y -5, WHITE);
        }
        
        DrawUI();
    }
    
    // --- UI Helper Fonksiyonlari ---
    void DrawUI() {
        int startX = 20;
        int startY = 20;
        
        // Panel Arkaplani
        DrawRectangle(10, 10, 220, 300, Fade(BLACK, 0.7f));
        DrawRectangleLines(10, 10, 220, 300, WHITE);
        
        DrawText(TextFormat("Agents: %i", swarmSize), startX, startY, 20, GREEN);
        DrawText(TextFormat("FPS: %i", GetFPS()), startX, startY + 25, 20, GREEN);

        // Sliders
        maxSpeed = GuiSliderBar({(float)startX, (float)startY + 60, 150, 15}, "Max Speed", TextFormat("%.1f", maxSpeed), maxSpeed, 1.0f, 20.0f);
        maxForce = GuiSliderBar({(float)startX, (float)startY + 100, 150, 15}, "Steer Force", TextFormat("%.2f", maxForce), maxForce, 0.1f, 2.0f);
        friction = GuiSliderBar({(float)startX, (float)startY + 140, 150, 15}, "Friction", TextFormat("%.3f", friction), friction, 0.80f, 0.999f);

        // Toggles
        if (GuiButton({(float)startX, (float)startY + 180, 100, 25}, showGrid ? "Grid: ON" : "Grid: OFF")) showGrid = !showGrid;
        if (GuiButton({(float)startX, (float)startY + 220, 100, 25}, showHeatmap ? "Heat: ON" : "Heat: OFF")) showHeatmap = !showHeatmap;
        
        DrawText("Right Click: Wall Toggle", startX, startY + 255, 10, LIGHTGRAY);
        DrawText("R: Reset Simulation", startX, startY + 270, 10, LIGHTGRAY);
    }

    float GuiSliderBar(Rectangle bounds, const char* textLeft, const char* textRight, float value, float minValue, float maxValue) {
        DrawText(textLeft, bounds.x, bounds.y - 15, 10, WHITE);
        DrawText(textRight, bounds.x + bounds.width + 10, bounds.y, 10, WHITE);
        DrawRectangleRec(bounds, DARKGRAY);
        float ratio = (value - minValue) / (maxValue - minValue);
        DrawRectangle(bounds.x, bounds.y, bounds.width * ratio, bounds.height, RED);
        
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, {bounds.x - 5, bounds.y - 10, bounds.width + 10, bounds.height + 20})) {
            float newRatio = (mouse.x - bounds.x) / bounds.width;
            if (newRatio < 0) newRatio = 0;
            if (newRatio > 1) newRatio = 1;
            return minValue + newRatio * (maxValue - minValue);
        }
        return value;
    }

    bool GuiButton(Rectangle bounds, const char* text) {
        Vector2 mouse = GetMousePosition();
        bool hovered = CheckCollisionPointRec(mouse, bounds);
        DrawRectangleRec(bounds, hovered ? LIGHTGRAY : GRAY);
        DrawText(text, bounds.x + 10, bounds.y + 8, 10, BLACK);
        return (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
    }
};

#endif 