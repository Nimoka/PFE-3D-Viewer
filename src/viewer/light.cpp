#include "light.h"
#include <time.h>
#include <cmath>

Light::Light(const Eigen::Vector3f& intensity) {
	this->intensity = intensity;
}

Light::Light(Light* light) {
	this->intensity = light->GetIntensity();
}

Light::~Light() {}

Eigen::Vector3f Light::GetIntensity() {
	return this->intensity;
}

void Light::SetIntensity(const Eigen::Vector3f& intensity) {
	this->intensity = intensity;
}

DirectionalLight::DirectionalLight(const Eigen::Vector3f& intensity,
		const Eigen::Vector3f& direction)
		: Light(intensity) {
	this->SetDirection(direction);
}

DirectionalLight::DirectionalLight(DirectionalLight* light)
		: Light((Light*) light) {
	this->direction = light->GetDirection();
}

DirectionalLight::~DirectionalLight() {}

Eigen::Vector3f DirectionalLight::GetDirection() {
	return this->direction;
}

void DirectionalLight::SetDirection(const Eigen::Vector3f& direction) {
	this->direction = direction.normalized();
}

PointLight::PointLight(const Eigen::Vector3f& color,
		const Eigen::Vector3f& position)
		: Light(intensity) {
	this->SetPosition(position);
}

PointLight::PointLight(const Eigen::Vector3f &color): Light(intensity){
	this->color = color;
	this->SetRandomPosition(50);
}


PointLight::PointLight(PointLight* light)
		: Light((Light*) light) {
	this->position = light->GetPosition();
}

PointLight::~PointLight() {}

Eigen::Vector3f PointLight::GetPosition() {
	return this->position;
}

Eigen::Vector3f PointLight::GetColor(){
  return this->color;
}

void PointLight::SetPosition(const Eigen::Vector3f& position) {
	this->position = position;
}

void PointLight::SetRandomPosition(float radius) {
	srand(time(0));
	float rand1 = rand();
	float rand2 = rand();
	float latitude    = acos(2 * rand1 - 1) - EIGEN_PI/2;
	float longitude   = 2 * EIGEN_PI * rand2;
	this->position[0] = radius * cos(latitude) * cos(longitude);
	this->position[1] = radius * cos(latitude) * sin(longitude);
	this->position[2] = radius * sin(latitude);
}

void PointLight::SetColor(const Eigen::Vector3f &color) {
	this->color = color;
}