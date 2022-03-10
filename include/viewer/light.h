#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Geometry>

#include "shadersreader.h"

class Light
{
public:
	Light(const Eigen::Vector3f& intensity);
	Light(Light* light);
	virtual ~Light();

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

	Eigen::Vector3f GetDirection();

	void SetDirection(const Eigen::Vector3f& direction);

private:
	Eigen::Vector3f direction;
};

class PointLight: public Light
{
public:
	PointLight(const Eigen::Vector3f &color,
			const Eigen::Vector3f& position);
	PointLight(PointLight* light);
	PointLight(const Eigen::Vector3f &color);

	~PointLight();

	Eigen::Vector3f GetPosition();
	Eigen::Vector3f GetColor();

	void SetPosition(const Eigen::Vector3f& position);
	void SetColor(const Eigen::Vector3f &color);
	void SetRandomPosition(float radius); // on a surface of a sphere with radius r


private:
	Eigen::Vector3f position;
	Eigen::Vector3f color;
};

#endif // LIGHT_H
