#ifndef COLLISION_H
#define COLLISION_H

#include "ball.h"
#include <cmath>

class Collision {
public:
    Collision() {}
    
    void handleBoxDetection(Ball &ball, double boxWidth, double boxHeight) {
        if (ball.getPositionX() <= (0 + ball.getDiameter()) || ball.getPositionX() >= (boxWidth - ball.getDiameter())) {
            double currentVx = ball.getVelocityX();
            ball.setVelocityX(-currentVx); // Reverse X velocity
        }
        if (ball.getPositionY() <= (0 + ball.getDiameter()) || ball.getPositionY() >= (boxHeight - ball.getDiameter())) {
            double currentVy = ball.getVelocityY();
            ball.setVelocityY(-currentVy); // Reverse Y velocity
        }
    }

    bool isColliding(Ball &ball1, Ball &ball2) {
        double dx = ball1.getPositionX() - ball2.getPositionX();
        double dy = ball1.getPositionY() - ball2.getPositionY();
        double distance = std::sqrt(dx * dx + dy * dy);

        return (distance <= ball1.getDiameter() / 2 + ball2.getDiameter() / 2);
    }

    void handleResponseVelocity(Ball &ball1, Ball &ball2) {
        double dx = ball1.getPositionX() - ball2.getPositionX();
        double dy = ball1.getPositionY() - ball2.getPositionY();

        // Compute the unit normal vector
        double magnitude = std::sqrt(dx * dx + dy * dy);
        double nx = dx / magnitude;
        double ny = dy /magnitude;

        // Resolve velocities along the normal direction
        double v1n = ball1.getVelocityX() * nx + ball1.getVelocityY() * ny;
        double v2n = ball2.getVelocityX() * nx + ball2.getVelocityY() * ny;

        // Compute new velocities after the collision
        double v1n_final = ((ball1.getMass() - ball2.getMass()) / (ball1.getMass() + ball2.getMass())) * v1n
                        + ((2 * ball2.getMass()) / (ball1.getMass() + ball2.getMass())) * v2n;

        double v2n_final = ((2 * ball1.getMass()) / (ball1.getMass() + ball2.getMass())) * v1n
                        + ((ball2.getMass() - ball1.getMass()) / (ball1.getMass() + ball2.getMass())) * v2n;

        // Convert the scalar normal velocities into vector form
        ball1.setVelocityX(ball1.getVelocityX() + (v1n_final - v1n) * nx);
        ball1.setVelocityY(ball1.getVelocityY() + (v1n_final - v1n) * ny);

        // Convert the scalar normal velocities into vector form
        ball2.setVelocityX(ball2.getVelocityX() + (v2n_final - v2n) * nx);
        ball2.setVelocityY(ball2.getVelocityY() + (v2n_final - v2n) * ny);
    }
};

#endif