#ifndef ANIMATION_H
#define ANIMATION_H

#include <QWidget>
#include <QList>
#include "ball.h"

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

    void addBall(int count); 
};


#endif // ANIMATION_H
