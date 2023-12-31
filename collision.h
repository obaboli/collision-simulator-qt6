#ifndef COLLISION_H
#define COLLISION_H

#include "ball.h"

class Collision {
public:
    Collision();
    
    // Narrow Phase
    bool isCollisionWithObject(const Ball &ball1, const Ball &ball2);
    bool isCollisionWithBox(const Ball &ball, double boxWidth, double boxHeight);

    // Solve Collision
    void resolveBoxCollision(Ball &ball, double boxWidth, double boxHeight);
    void handleResponseVelocity(Ball &ball1, Ball &ball2);
    void repositionAfterWallCollision(Ball &ball, double boxWidth, double boxHeight);
    void repositionAfterBallCollision(Ball &ball1, Ball &ball2);
};

#endif
