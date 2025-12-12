#ifndef AGENT_H
#define AGENT_H

#include "raylib.h"
#include "raymath.h"

struct Agent {
  Vector2 position;
  Vector2 velocity;
  Vector2 acceleration;
  float maxSpeed = 5.0f;
  float maxForce = 0.1f;




    void update(int screenWidth, int screenHeight) {
        acceleration.x = 0;
        acceleration.y = 0;

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePosition = GetMousePosition();
        Vector2 direction = Vector2Subtract(mousePosition, position);
        direction = Vector2Normalize(direction);
        
        Vector2 desired = Vector2Subtract(mousePosition, position);
        float dist = Vector2Length(desired);
        Vector2 desiredNormalized = Vector2Normalize(desired);

        if (dist < 100) {
            float speed = maxSpeed * (dist / 100.0f);
            desired = Vector2Scale(desiredNormalized, speed);
        } else {
            desired = Vector2Scale(desiredNormalized, maxSpeed);
        }
        
        Vector2 steer = Vector2Subtract(desired, velocity);
        steer = Vector2ClampValue(steer, 0.01, maxForce);
        
        acceleration = steer;
        
        }

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        position.x += velocity.x;
        position.y += velocity.y;

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

        // Friction values
        velocity.x *= 0.95f;
        velocity.y *= 0.95f;
    }

    void DrawCircle() {
        // Ortaya kırmızı bir nokta (Temsili Drone) koy
        DrawCircleV(position, 20, RED);
    }

};

#endif 