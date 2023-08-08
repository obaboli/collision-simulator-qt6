#include "Animation.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>

static const double TIME_INTERVAL_MS = 1000 / 60;  // For 60fps, approximately 16.67ms

Animation::Animation(QWidget *parent) :
    QWidget(parent)
{
    create_ball(10);  

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Animation::updateImage);
    timer->start(TIME_INTERVAL_MS);
}

void Animation::create_ball(int count) {
    for(int i = 0; i < count; ++i) {
        // Randomize Ball Diameter
        int diameter = QRandomGenerator::global()->bounded(10, 30);

        // Randomize Ball starting Position
        int x = QRandomGenerator::global()->bounded(0 + diameter, width() - diameter);
        int y = QRandomGenerator::global()->bounded(0 + diameter, height() - diameter);

        m_balls.append(Ball(x, y, 3, 3, 0, 0, diameter));
    }
}

void Animation::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for(Ball &ball : m_balls) {
        painter.setBrush(Qt::red);

        QRectF ballRect(ball.getPositionX(), ball.getPositionY(), ball.getDiameter(), ball.getDiameter());
        painter.drawEllipse(ballRect);
    }
}

void Animation::updateImage() {
    for(Ball &ball : m_balls) {

        ball.move();

        // Box Detection Check
        if (ball.getPositionX() <= (0 + ball.getDiameter()) || ball.getPositionX() >= (width() - ball.getDiameter())) {
            int currentVx = ball.getVelocityX();
            ball.setVelocityX(-currentVx); // Reverse X velocity
        }
        if (ball.getPositionY() <= (0 + ball.getDiameter()) || ball.getPositionY() >= (height() - ball.getDiameter())) {
            int currentVy = ball.getVelocityY();
            ball.setVelocityY(-currentVy); // Reverse Y velocity
        }

        for(Ball &other_ball: m_balls) {
            // Skip if same ball
            if(&ball == &other_ball) continue;

            // Collision Detection Check
            double dx = ball.getPositionX() - other_ball.getPositionX();
            double dy = ball.getPositionY() - other_ball.getPositionY();
            double distance = std::sqrt(dx * dx + dy * dy);

            if(distance <= ball.getDiameter() / 2 + other_ball.getDiameter() / 2) {

                // Compute Resposne Velocity for Current Ball and Other Ball using Elastic Collision

                // Compute the unit normal vector
                double magnitude = std::sqrt(dx * dx + dy * dy);
                double nx = dx / magnitude;
                double ny = dy /magnitude;

                // Resolve velocities along the normal direction
                double v1n = ball.getVelocityX() * nx + ball.getVelocityY() * ny;
                double v2n = other_ball.getVelocityX() * nx + other_ball.getVelocityY() * ny;

                // Compute new velocities after the collision
                double v1n_final = ((ball.getMass() - other_ball.getMass()) / (ball.getMass() + other_ball.getMass())) * v1n
                                + ((2 * other_ball.getMass()) / (ball.getMass() + other_ball.getMass())) * v2n;

                double v2n_final = ((2 * ball.getMass()) / (ball.getMass() + other_ball.getMass())) * v1n
                                + ((other_ball.getMass() - ball.getMass()) / (ball.getMass() + other_ball.getMass())) * v2n;

                // Convert the scalar normal velocities into vector form
                ball.setVelocityX(ball.getVelocityX() + (v1n_final - v1n) * nx);
                ball.setVelocityY(ball.getVelocityY() + (v1n_final - v1n) * ny);

                // Convert the scalar normal velocities into vector form
                other_ball.setVelocityX(other_ball.getVelocityX() + (v2n_final - v2n) * nx);
                other_ball.setVelocityY(other_ball.getVelocityY() + (v2n_final - v2n) * ny);
            }
        }

        update();  // Trigger paintEvent
    }
}