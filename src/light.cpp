#include "light.h"

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

void DirectionalLight::Activate(ShaderReader* shader) {
	if (shader == nullptr)
		return;
	glUniform3fv(shader->GetUniformLocation("light_intensity"), 1,
			this->intensity.data());
	glUniform3fv(shader->GetUniformLocation("light_direction"), 1,
			this->direction.data());
}

Eigen::Vector3f DirectionalLight::GetDirection() {
	return this->direction;
}

void DirectionalLight::SetDirection(const Eigen::Vector3f& direction) {
	this->direction = direction.normalized();
}
