#ifndef BALL_H
#define BALL_H

class Ball {
public:
    Ball(double position_x, double position_y, 
         double velocity_x, double velocity_y,
         double acceleration_x, double acceleration_y, 
         double diameter);


    double getPositionX() const;
    double getPositionY() const;

    void setPositionX(double position_x);
    void setPositionY(double position_y);

    double getVelocityX() const;
    double getVelocityY() const;


    void setVelocityX(double velocity_x);
    void setVelocityY(double velocity_y);

    double getAccelerationX() const;
    double getAccelerationY() const;

    void setAccelerationX(double acceleration_x);
    void setAccelerationY(double acceleration_y);

    double getMass() const;
    double getDiameter() const;

    void move(double deltaTime = 1.0);

private:
    double m_position_x, m_position_y;
    double m_velocity_x,  m_velocity_y;
    double m_acceleration_x, m_acceleration_y;
    double m_mass;
    double m_diameter;
};

#endif // BALL_H
