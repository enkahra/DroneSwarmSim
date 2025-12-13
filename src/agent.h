#ifndef AGENT_H
#define AGENT_H

#include "raylib.h"
#include "raymath.h"
#include <vector>

struct Swarm {
    std::vector<std::vector<int>> grid;
    std::vector<int> costField; // Map for flow field
    std::vector<int> agentCellIndices;
    int proximityMeter = 20; // Line 111 this is for collison system
    int cellSize = 20;
    int columns = 0;
    int rows = 0;
    
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<Vector2> accelerations;
    int swarmSize = 1000; // Initial size is 1000
    int distanceToMouseThreshold = 100; // Line 94 this is for moving
    float maxSpeed = 5.0f;
    float maxForce = 0.2f;
    float friction = 0.95f;

    Texture2D agentSprite;

    Swarm(int screenWidth, int screenHeight) {
        // Movement code
        positions.resize(swarmSize);
        velocities.resize(swarmSize);
        accelerations.resize(swarmSize);

        // Collision system code
        columns = screenWidth / cellSize; 
        rows = screenHeight / cellSize; 
        grid.resize(columns * rows);
        agentCellIndices.resize(swarmSize);

        // Flow field code
        costField.resize(columns * rows);
        fill(costField.begin(), costField.end(), 1);
        for(int i{0}; i < costField.size(); ++i) {
            int col = i % columns;
            int row = i / columns;
            if ((col == 0 || col == columns - 1) || (row == 0 || row == rows - 1)) {
                costField[i] = 255;
            }
        }

        // Image gen code
        Image droneImage = GenImageColor(10, 10, BLANK);
        ImageDrawCircle(&droneImage, 5, 5, 4, RED);
        agentSprite = LoadTextureFromImage(droneImage);
        UnloadImage(droneImage);

        // Spawn drones random locations and make them still
        for (int i{0}; i < swarmSize; ++i) {
            positions[i].x = GetRandomValue(cellSize, screenWidth - cellSize);
            positions[i].y = GetRandomValue(cellSize, screenHeight - cellSize);

            velocities[i] = {0.0f, 0.0f};
            accelerations[i] = {0.0f, 0.0f};
        }
    }

    void update(int screenWidth, int screenHeight) {
        bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        Vector2 mousePosition = GetMousePosition();

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

        for (int i{0}; i < swarmSize; ++i) {
            accelerations[i].x = 0;
            accelerations[i].y = 0;

            if (isMouseDown) {
                Vector2 desiredLocation = Vector2Subtract(mousePosition, positions[i]);
                float distanceToMouse = Vector2Length(desiredLocation);
                Vector2 desiredLocationNormalized = Vector2Normalize(desiredLocation);

            if (distanceToMouse < distanceToMouseThreshold) {
                float speed = maxSpeed * (distanceToMouse / 100.0f);
                desiredLocation = Vector2Scale(desiredLocationNormalized, speed);
            } 
                desiredLocation = Vector2Scale(desiredLocationNormalized, maxSpeed); // If its not close go in full speed
            
                Vector2 steer = Vector2Subtract(desiredLocation, velocities[i]);
                steer = Vector2ClampValue(steer, 0.01, maxForce); 
                accelerations[i] = steer;
            }
            
            int cellIndex = agentCellIndices[i]; // Takes from cached vector
            for(auto neighborId : grid[cellIndex]) {
                if(neighborId == i) continue;
                Vector2 prixomity = Vector2Subtract(positions[i], positions[neighborId]);
                float prixomityDistance = Vector2Length(prixomity);

                if(prixomityDistance < proximityMeter) {   
                    Vector2 prixomityNormalized = Vector2Normalize(prixomity);
                    prixomity = Vector2Scale(prixomityNormalized, 0.5f);
                    accelerations[i] = Vector2Add(accelerations[i], prixomity); 
                }
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
            if(costField[i] == 255) {
                DrawRectangle(pixelX, pixelY, cellSize, cellSize, BLACK);
            } else {
                DrawRectangleLines(pixelX, pixelY, cellSize, cellSize, GRAY);
            }
        }

        for(int i{0}; i < swarmSize; ++i) {
            DrawTexture(agentSprite, positions[i].x -5, positions[i].y -5, WHITE);
        }
    }
};

#endif 