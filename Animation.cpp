#include "animation.h"
#include "collision.h"
#include <QPainter>
#include <QTimer>
#include <QRandomGenerator>

static const double TIME_INTERVAL_MS = 1000 / 60;  // For 60fps, approximately 16.67ms

#if PERFORMANCE_MODE == 1
    static const int TOTAL_BALL_COUNT = 30;
#else
    static const int TOTAL_BALL_COUNT = 10;
#endif

Animation::Animation(QWidget *parent) :
    QWidget(parent)
{
    addBall(30);  

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Animation::updateImage);
    timer->start(TIME_INTERVAL_MS);
}

void Animation::addBall(int count) {
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
bool compareXPosition(const Ball& ball1, const Ball& ball2) {
    return ball1.getPositionX() < ball2.getPositionX();
}

void Animation::updateImage() {
    Collision collisionHandler;

    std::sort(m_balls.begin(), m_balls.end(), compareXPosition);

    for (size_t i = 0; i < m_balls.size(); ++i) {
        Ball &ball = m_balls[i];
        ball.move();
        
        if (collisionHandler.isCollisionWithBox(ball, width(), height())) {
            collisionHandler.resolveBoxCollision(ball);
            collisionHandler.repositionAfterWallCollision(ball, width(), height());
        }

        for (size_t j = i + 1; j < m_balls.size(); ++j) {
            Ball &other_ball = m_balls[j];
            if(ball.getPositionX() + ball.getDiameter() < other_ball.getPositionX() - other_ball.getDiameter()) break;

            if (collisionHandler.isCollisionWithObject(ball, other_ball)) {
                collisionHandler.handleResponseVelocity(ball, other_ball);
                collisionHandler.repositionAfterBallCollision(ball, other_ball);
            }
        }

        update();  // Trigger paintEvent
    }
}