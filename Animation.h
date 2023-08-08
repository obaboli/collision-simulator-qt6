#ifndef ANIMATION_H
#define ANIMATION_H

#include <QWidget>
#include <QList>
#include "Ball.h"

class Animation : public QWidget {
    Q_OBJECT

public:
    explicit Animation(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateImage();

private:
    QList<Ball> m_balls;

    void create_ball(int count); // Creates a number of balls with random properties
};


#endif // ANIMATION_H
