#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Eigen>

#include "shaderreader.h"

class Light
{
public:
	Light(const Eigen::Vector3f& intensity);
	Light(Light* light);
	virtual ~Light();

	virtual void Activate(ShaderReader* shader) = 0;

	Eigen::Vector3f GetIntensity();

	void SetIntensity(const Eigen::Vector3f& intensity);

protected:
	Eigen::Vector3f intensity;
};

class DirectionalLight: public Light
{
public:
	DirectionalLight(const Eigen::Vector3f& intensity,
			const Eigen::Vector3f& direction);
	DirectionalLight(DirectionalLight* light);
	~DirectionalLight();

	void Activate(ShaderReader* shader);

	Eigen::Vector3f GetDirection();

	void SetDirection(const Eigen::Vector3f& direction);

private:
	Eigen::Vector3f direction;
};

#endif // LIGHT_H
