#include "light.h"
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

PointLight::PointLight(const Eigen::Vector3f& intensity,
		const Eigen::Vector3f& position)
		: Light(intensity) {
	this->SetPosition(position);
}

PointLight::PointLight(PointLight* light)
		: Light((Light*) light) {
	this->position = light->GetPosition();
}

PointLight::~PointLight() {}

Eigen::Vector3f PointLight::GetPosition() {
	return this->position;
}

void PointLight::SetPosition(const Eigen::Vector3f& position) {
	this->position = position;
}
