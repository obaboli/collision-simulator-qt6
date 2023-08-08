#include "animation.h"
#include "collision.h"
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

        m_balls.append(Ball(x, y, 1, 1, 0, 0, diameter));
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
    Collision collisionHandler;

    for(Ball &ball : m_balls) {
        ball.move();

        collisionHandler.handleBoxDetection(ball, width(), height());

        for(Ball &other_ball: m_balls) {
            // Skip if same ball
            if(&ball == &other_ball) continue;

            if (collisionHandler.isColliding(ball, other_ball)) {
                collisionHandler.handleResponseVelocity(ball, other_ball);
            }
        }

        update();  // Trigger paintEvent
    }
}