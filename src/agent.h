#ifndef AGENT_H
#define AGENT_H

#include "raylib.h"
#include "raymath.h"

struct Agent {
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
  float maxSpeed = 5.0f;
  float maxForce = 0.2f;
  float friction = 0.95f;

    void update(int screenWidth, int screenHeight) {
        acceleration.x = 0;
        acceleration.y = 0;

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();
            
            Vector2 desiredLocation = Vector2Subtract(mousePosition, position);
            float distanceToMouse = Vector2Length(desiredLocation);
            Vector2 desiredLocationNormalized = Vector2Normalize(desiredLocation);

        if (distanceToMouse < 100) {
            float speed = maxSpeed * (distanceToMouse / 100.0f);
            desiredLocation = Vector2Scale(desiredLocationNormalized, speed);
        } else {
            desiredLocation = Vector2Scale(desiredLocationNormalized, maxSpeed);
        }
        
            Vector2 steer = Vector2Subtract(desiredLocation, velocity);
            steer = Vector2ClampValue(steer, 0.01, maxForce);
            acceleration = steer;
        }
        
        velocity = Vector2Add(velocity, acceleration);
        position = Vector2Add(position, velocity);

        // Friction 
        velocity = Vector2Scale(velocity, friction); 

        CheckBoundry(screenWidth, screenHeight);
    }
    void CheckBoundry(int screenWidth, int screenHeight) { 
        // Checking for boundry
        // XXX : This check may not work when screen resized
        if (position.x > screenWidth) {
            position.x = screenWidth;
            velocity.x *= -1;
        }
        if (position.x < 0) {
            position.x = 0;
            velocity.x *= -1;
        }
        if (position.y > screenHeight) {
            position.y = screenHeight;
            velocity.y *= -1;
        }
        if (position.y < 0) {
            position.y = 0;
            velocity.y *= -1;
        }
    }

    void Draw() {
        // Ortaya kırmızı bir nokta (Temsili Drone) koy
        DrawCircleV(position, 20, RED);
    }

};

#endif 