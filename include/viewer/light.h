#ifndef LIGHT_H
#define LIGHT_H

#include <Eigen/Geometry>

#include "shadersreader.h"

/**
 * \brief Light class.
 * 
 * Reprensent a basic light with only an intensity.
 * Should be derivated to add direction (directional light), position (point
 * light), etc.
 */
class Light
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `Light` constructor.
	 * 
	 * \param intensity Intensity (color without normalization) of the light.
	 */
	Light(const Eigen::Vector3f& intensity);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `Light` constructor using an existing object that would be duplicated.
	 * 
	 * \param light Light to duplicate.
	 */
	Light(Light* light);
	/**
	 * \brief Destructor.
	 * 
	 * `Light` destructor.
	 */
	virtual ~Light();

	/**
	 * \brief Getter of `intensity`.
	 * 
	 * Return the value of the `intensity` field, corresponding to the intensity
	 * (color without normalization) of the light.
	 * 
	 * \return Value of the `intensity` field.
	 */
	Eigen::Vector3f GetIntensity();

	/**
	 * \brief Setter of `intensity`.
	 * 
	 * Set a new intensity to the light.
	 * 
	 * \param intensity New intensity (color without normalization) of the
	 *      light.
	 */
	void SetIntensity(const Eigen::Vector3f& intensity);

protected:
	/**
	 * \brief Intensity of the light.
	 * 
	 * Intensity (color without normalization) of the light.
	 * Should theorically always be positive.
	 */
	Eigen::Vector3f intensity;
};

/**
 * \brief Directional light class.
 * 
 * Represent a directional light with an intensity and a direction.
 * The intensity is saved in the Light object.
 */
class DirectionalLight: public Light
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `DirectionalLight` constructor.
	 * 
	 * \param intensity Intensity (color without normalization) of the light.
	 * \param direction Direction of the light.
	 */
	DirectionalLight(const Eigen::Vector3f& intensity,
			const Eigen::Vector3f& direction);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `DirectionalLight` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param light Light to duplicate.
	 */
	DirectionalLight(DirectionalLight* light);
	/**
	 * \brief Destructor.
	 * 
	 * `DirectionalLight` destructor.
	 */
	~DirectionalLight();

	/**
	 * \brief Getter of `direction`.
	 * 
	 * Return the value of the `direction` field, corresponding to the
	 * direction of the light.
	 * 
	 * \return Value of the `direction` field.
	 */
	Eigen::Vector3f GetDirection();

	/**
	 * \brief Setter of `direction`.
	 * 
	 * Set a new direction to the light.
	 * Will be automatically normalized.
	 * 
	 * \param direction New direction of the light.
	 */
	void SetDirection(const Eigen::Vector3f& direction);

private:
	/**
	 * \brief Direction of the light.
	 * 
	 * Direciton of the light.
	 * Should always be normalized.
	 */
	Eigen::Vector3f direction;
};

/**
 * \brief Point light class.
 * 
 * Represent a point light with an intensity and a position.
 * The intensity is saved in the Light object.
 */
class PointLight: public Light
{
public:
	/**
	 * \brief Constructor.
	 * 
	 * `PointLight` constructor.
	 * 
	 * \param intensity Intensity (color without normalization) of the light.
	 * \param position Position of the light.
	 */
	PointLight(const Eigen::Vector3f &intensity,
			const Eigen::Vector3f& position);
	/**
	 * \brief Constructor by duplication.
	 * 
	 * `PointLight` constructor using an existing object that would be
	 * duplicated.
	 * 
	 * \param light Light to duplicate.
	 */
	PointLight(PointLight* light);
	/**
	 * \brief Destructor.
	 * 
	 * `PointLight` destructor.
	 */
	~PointLight();

	/**
	 * \brief Getter of `position`.
	 * 
	 * Return the value of the `position` field, corresponding to the position
	 * of the light.
	 * 
	 * \return Value of the `position` field.
	 */
	Eigen::Vector3f GetPosition();

	/**
	 * \brief Setter of `position`.
	 * 
	 * Set a new position to the light.
	 * 
	 * \param position New position of the light.
	 */
	void SetPosition(const Eigen::Vector3f& position);

private:
	/**
	 * \brief Position of the light.
	 * 
	 * Position of the light.
	 */
	Eigen::Vector3f position;
};

#endif // LIGHT_H
