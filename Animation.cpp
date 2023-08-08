#include "Animation.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>

static constexpr int X_SPEED = 1;
static constexpr int Y_SPEED = 2;

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

        m_balls.append(Ball(x, y, X_SPEED, Y_SPEED, 0, 0, diameter));
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

            if(distance <= ball.getDiameter() + other_ball.getDiameter()) {

                // Computer Resposne Velocity for Current Ball and Other Ball on Collision
                // Elastic Collision Formulat (Reducible)
    
                double dvx = ball.getVelocityX() - other_ball.getVelocityX();
                double dvy = ball.getVelocityY() - other_ball.getVelocityY();

                double norm_x_squared = dx * dx + dy * dy;

                double factor1 = (2 * other_ball.getMass() / (ball.getMass() + other_ball.getMass())) * (dvx * dx + dvy * dy) / norm_x_squared;
                double factor2 = (2 * ball.getMass() / (ball.getMass() + other_ball.getMass())) * (dvx * dx + dvy * dy) / norm_x_squared;

                double new_vx_ball = ball.getVelocityX() - factor1 * dx;
                double new_vy_ball = ball.getVelocityY() - factor1 * dy;

                double new_vx_other_ball = other_ball.getVelocityX() + factor2 * dx;
                double new_vy_other_ball = other_ball.getVelocityY() + factor2 * dy;

                ball.setVelocityX(new_vx_ball);
                ball.setVelocityY(new_vy_ball);

                other_ball.setVelocityX(new_vx_other_ball);
                other_ball.setVelocityY(new_vy_other_ball);
            }
        }

        update();  // Trigger paintEvent
    }
}