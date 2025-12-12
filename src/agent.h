#ifndef AGENT_H
#define AGENT_H

#include "raylib.h"
#include "raymath.h"
#include "vector"

struct Swarm {
    std::vector<Vector2> positions;
    std::vector<Vector2> velocities;
    std::vector<Vector2> accelerations;
    int swarmSize = 10000; // Initial size is 10000
    float maxSpeed = 5.0f;
    float maxForce = 0.2f;
    float friction = 0.95f;

    Swarm() {
        positions.resize(swarmSize);
        velocities.resize(swarmSize);
        accelerations.resize(swarmSize);

        for (int i{0}; i < swarmSize; ++i) {
            positions[i].x = GetRandomValue(0, 1200);
            positions[i].y = GetRandomValue(0, 800);

            velocities[i] = {0.0f, 0.0f};
            accelerations[i] = {0.0f, 0.0f};
        }
    }

    void update(int screenWidth, int screenHeight) {
        bool isMouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        Vector2 mousePosition = GetMousePosition();

        for (int i{0}; i < swarmSize; ++i) {
            accelerations[i].x = 0;
            accelerations[i].y = 0;

            if (isMouseDown) {
                Vector2 desiredLocation = Vector2Subtract(mousePosition, positions[i]);
                float distanceToMouse = Vector2Length(desiredLocation);
                Vector2 desiredLocationNormalized = Vector2Normalize(desiredLocation);

            if (distanceToMouse < 100) {
                float speed = maxSpeed * (distanceToMouse / 100.0f);
                desiredLocation = Vector2Scale(desiredLocationNormalized, speed);
            } else {
                desiredLocation = Vector2Scale(desiredLocationNormalized, maxSpeed);
            }
            
                Vector2 steer = Vector2Subtract(desiredLocation, velocities[i]);
                steer = Vector2ClampValue(steer, 0.01, maxForce);
                accelerations[i] = steer;
            }
            
            velocities[i] = Vector2Add(velocities[i], accelerations[i]);
            positions[i] = Vector2Add(positions[i], velocities[i]);

            // Friction 
            velocities[i] = Vector2Scale(velocities[i], friction); 

            CheckBoundry(screenWidth, screenHeight, i);
            }
        }
        
    void CheckBoundry(int screenWidth, int screenHeight, int i) { 
        // Checking for boundry
        // XXX : This check may not work when screen resized
        if (positions[i].x > screenWidth) {
            positions[i].x = screenWidth;
            velocities[i].x *= -1;
        }
        if (positions[i].x < 0) {
            positions[i].x = 0;
            velocities[i].x *= -1;
        }
        if (positions[i].y > screenHeight) {
            positions[i].y = screenHeight;
            velocities[i].y *= -1;
        }
        if (positions[i].y < 0) {
            positions[i].y = 0;
            velocities[i].y *= -1;
        }
    }

    void Draw() {
        for(int i{0}; i < swarmSize; ++i) {
            DrawCircleV(positions[i], 4, RED);
        }
    }

};

#endif 