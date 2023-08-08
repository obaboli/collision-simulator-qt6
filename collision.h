#ifndef COLLISION_H
#define COLLISION_H

#include "ball.h"
#include <cmath>

class Collision {
public:
    Collision() {}
    
    void handleBoxDetection(Ball &ball, double boxWidth, double boxHeight) {
        double diameter = ball.getDiameter();
        if (ball.getPositionX() <= diameter || ball.getPositionX() >= (boxWidth - diameter)) {
            double currentVx = ball.getVelocityX();

            // Reverse Velocity
            ball.setVelocityX(-currentVx); 
        }
        if (ball.getPositionY() <= diameter || ball.getPositionY() >= (boxHeight - diameter)) {
            double currentVy = ball.getVelocityY();

            // Reverse Y velocity
            ball.setVelocityY(-currentVy); 
        }
    }

    bool isColliding(Ball &ball1, Ball &ball2) {
        double dx = ball1.getPositionX() - ball2.getPositionX();
        double dy = ball1.getPositionY() - ball2.getPositionY();
        double distance = std::sqrt(dx * dx + dy * dy);

        return (distance <= ball1.getDiameter() / 2 + ball2.getDiameter() / 2);
    }

    // Since we're using discrete collision detection we need to handle cases of overlap still
    // One way to handle that is to use slower velocities on the balls but was still seeing some overlap
    // These reposition functions will detect how far the overlap is and reset the position to remove the overlap otherwise the balls get stuck forever :(
    // TODO: CCD algorithm? 
    void repositionAfterWallCollision(Ball &ball, double boxWidth, double boxHeight) {
        double diameter = ball.getDiameter();
        if (ball.getPositionX() <= diameter) {
            ball.setPositionX(diameter); 
        } else if (ball.getPositionX() >= (boxWidth - diameter)) {
            ball.setPositionX(boxWidth - diameter); 
        }
        
        if (ball.getPositionY() <= diameter) {
            ball.setPositionY(diameter); 
        } else if (ball.getPositionY() >= (boxHeight - diameter)) {
            ball.setPositionY(boxHeight - diameter); 
        }
    }

    void repositionAfterBallCollision(Ball &ball1, Ball &ball2) {
        double dx = ball1.getPositionX() - ball2.getPositionX();
        double dy = ball1.getPositionY() - ball2.getPositionY();
        double distance = std::sqrt(dx * dx + dy * dy);

        double overlap = (ball1.getDiameter() / 2 + ball2.getDiameter() / 2) - distance;
        double nx = dx / distance;
        double ny = dy / distance;

        // Repositioning the balls based on their masses
        double totalMass = ball1.getMass() + ball2.getMass();
        double move1 = (overlap * (ball2.getMass() / totalMass));
        double move2 = (overlap * (ball1.getMass() / totalMass));

        ball1.setPositionX(ball1.getPositionX() + nx * move1);
        ball1.setPositionY(ball1.getPositionY() + ny * move1);
        ball2.setPositionX(ball2.getPositionX() - nx * move2);
        ball2.setPositionY(ball2.getPositionY() - ny * move2);
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