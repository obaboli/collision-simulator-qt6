#include "animation.h"
#include "collision.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>

#include <iostream>

static const double TIME_INTERVAL_MS = (1000 / 24.0);  // For 60fps, approximately 16.67ms

static const int TOTAL_BALL_COUNT = 5;

static const int INITIAL_BALL_SPEED = 1;
static const int INITIAL_BALL_ACCELERATION = 0;

Animation::Animation(QWidget *parent) :
    QWidget(parent)
{
    m_programTimer.start();
    addBall(TOTAL_BALL_COUNT);  

    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Animation::updateImage);
    timer->start(TIME_INTERVAL_MS);
}

void Animation::addBall(int count) {
    for(int i = 0; i < count; ++i) {
        // Randomize Ball Diameter
        int diameter = QRandomGenerator::global()->bounded(20, 30);

        // Randomize Ball starting Position
        int x = QRandomGenerator::global()->bounded(0 + diameter, width() - diameter);
        int y = QRandomGenerator::global()->bounded(0 + diameter, height() - diameter);

        m_balls.append(Ball(x, y, INITIAL_BALL_SPEED, INITIAL_BALL_SPEED, INITIAL_BALL_ACCELERATION, INITIAL_BALL_ACCELERATION, diameter, i));
    }
}

void Animation::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for(Ball &ball : m_balls) {
        if (ball.hasActiveCollisionEffect()) {
            painter.setBrush(Qt::darkBlue); 
        } else {
            painter.setBrush(Qt::blue);
        }

        QRectF ballRect(ball.getPositionX(), ball.getPositionY(), ball.getDiameter(), ball.getDiameter());
        painter.drawEllipse(ballRect);

        // Draw number inside ball
        QString number = QString::number(ball.getNumber());
        painter.setPen(Qt::white);  
        painter.setFont(QFont("Arial", 8)); 
        painter.drawText(ballRect, Qt::AlignCenter, number);
    }
}

bool compareXPosition(const Ball& ball1, const Ball& ball2) {
    return ball1.getPositionX() < ball2.getPositionX();
}

void Animation::updateImage() {
    Collision collisionHandler;
    qint64 elapsedMilliseconds = 0;

    // Sweep and Prune Algorithm 
    std::sort(m_balls.begin(), m_balls.end(), compareXPosition);

    for (size_t i = 0; i < m_balls.size(); ++i) {
        Ball &ball = m_balls[i];
        ball.move();

        ball.decrementCollisionEffectCounter();
        
        if (collisionHandler.isCollisionWithBox(ball, width(), height())) {
            elapsedMilliseconds = m_programTimer.elapsed();
            std::cout << elapsedMilliseconds << "ms - Box Collision - Ball " << ball.getNumber() << std::endl; 

            collisionHandler.resolveBoxCollision(ball);
            collisionHandler.repositionAfterWallCollision(ball, width(), height());
            ball.triggerCollisionEffect();
        }

        for (size_t j = i + 1; j < m_balls.size(); ++j) {
            Ball &other_ball = m_balls[j];
            if(ball.getPositionX() + ball.getDiameter() < other_ball.getPositionX() - other_ball.getDiameter()) break;

            if (collisionHandler.isCollisionWithObject(ball, other_ball)) {

                elapsedMilliseconds = m_programTimer.elapsed();
                std::cout << elapsedMilliseconds << "ms - Object Collision - Ball " << ball.getNumber() << " Ball " << other_ball.getNumber() << std::endl; 

                collisionHandler.handleResponseVelocity(ball, other_ball);
                collisionHandler.repositionAfterBallCollision(ball, other_ball);
                ball.triggerCollisionEffect();
                other_ball.triggerCollisionEffect();
            }
        }

        update();  // Trigger paintEvent
    }
}