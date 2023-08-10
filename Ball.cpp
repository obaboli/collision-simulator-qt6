#include "ball.h"
#include <cmath>

const double PI = 3.14159265358979323846;
const double AREA_DENSITY = 0.5; 

Ball::Ball(double position_x, double position_y, 
           double velocity_x, double velocity_y,
           double acceleration_x, double acceleration_y, 
           double diameter) 
    : m_position_x(position_x), m_position_y(position_y),
      m_velocity_x(velocity_x), m_velocity_y(velocity_y),
      m_acceleration_x(acceleration_x), m_acceleration_y(acceleration_y), 
      m_diameter(diameter), m_collisionEffectCounter(0) {

    // Calculate mass based on diameter
    
    // Calculate the radius
    double radius = m_diameter / 2.0;
    
    // Calculate the area
    double area = PI * std::pow(radius, 2);
    
    // Compute the mass using the area density
    m_mass = AREA_DENSITY * area;

}

double Ball::getPositionX() const { return m_position_x; }
double Ball::getPositionY() const { return m_position_y; }

void Ball::setPositionX(double position_x) { m_position_x = position_x; }
void Ball::setPositionY(double position_y) { m_position_y = position_y; }

double Ball::getVelocityX() const { return m_velocity_x; }
double Ball::getVelocityY() const { return m_velocity_y; }

void Ball::setVelocityX(double velocity_x) { m_velocity_x = velocity_x; }
void Ball::setVelocityY(double velocity_y) { m_velocity_y = velocity_y; }

double Ball::getAccelerationX() const { return m_acceleration_x; }
double Ball::getAccelerationY() const { return m_acceleration_y; }

void Ball::setAccelerationX(double acceleration_x) { m_acceleration_x = acceleration_x; }
void Ball::setAccelerationY(double acceleration_y) { m_acceleration_y = acceleration_y; }

double Ball::getMass() const { return m_mass; }

double Ball::getDiameter() const { return m_diameter; }

void Ball::move(double deltaTime) {
    m_velocity_x += m_acceleration_x * deltaTime;
    m_velocity_y += m_acceleration_y * deltaTime;
    m_position_x += m_velocity_x * deltaTime;
    m_position_y += m_velocity_y * deltaTime;
}

void Ball::triggerCollisionEffect() {
  m_collisionEffectCounter = 4;
}

bool Ball::hasActiveCollisionEffect() {
  return m_collisionEffectCounter > 0; 
}

void Ball::decrementCollisionEffectCounter(){
  if (m_collisionEffectCounter > 0) {
    m_collisionEffectCounter--;
  }
}