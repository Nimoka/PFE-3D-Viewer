#ifndef CAMERA_H
#define CAMERA_H

/* ************************************************************
 * A part of this class (and his implementation) come from    *
 * files `trackball.h`, `trackball.cpp`, `camera.h` and       *
 * `camera.cpp`, from source code provided during assignments *
 * of Advanced Image Synthesis (for CS I&S students) at the   *
 * University of Bordeaux.                                    *
 *                                                            *
 * trackball: Copyright 2015 Simon Boyé.                      *
 ************************************************************ */

#include <Eigen/Geometry>

#define MAX_PITCH 89.0

/**
 * @brief Orthographic projection matrix.
 *
 * @param l Left, number farthest left on the x-axis.
 * @param r Right, number farthest right on the x-axis.
 * @param b Bottom, number farthest down on the y-axis.
 * @param t Top, number Farthest up on the y-axis.
 * @param n Near, number distance to the near clipping plane along the -z axis.
 * @param f Far, number distance to the far clipping plane along the -z axis.
 * @return Orthographic transformation matrix.
 */
Eigen::Matrix4f OrthographicProjection(float l, float r, float b, float t,
		float n, float f);

/**
 * @brief Perspective projection matrix.
 *
 * @param l Left, number farthest left on the x-axis.
 * @param r Right, number farthest right on the x-axis.
 * @param b Bottom, number farthest down on the y-axis.
 * @param t Top, number farthest up on the y-axis.
 * @param n Near, number distance to the near clipping plane along the -z axis.
 * @param f Far, number distance to the far clipping plane along the -z axis.
 * @return Perspective transformation matrix.
 */
Eigen::Matrix4f PerspectiveProjection(float l, float r, float b, float t,
		float n, float f);

/**
 * @brief Perspective matrix (for first person navigation).
 *
 * @param fovy Fovy of camera.
 * @param aspect Aspect of camera.
 * @param zNear Near of camera.
 * @param zFar Far of camera.
 * @return Eigen::Matrix4f the perspective matrix.
 */
static Eigen::Matrix4f Perspective(float fovy, float aspect,
		float zNear, float zFar);

/**
 * @brief Camera view.
 *
 * @param position Position of camera.
 * @param target Position of target.
 * @param up Angle of up of camera.
 * @return View matrix pointing to the target.
 */
static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
		const Eigen::Vector3f& target, const Eigen::Vector3f& up);

/**
 * @brief Get cartesian coordinates from polar coordinates
 *
 * @param coordinates Polar coordinate on the sphere.
 * @param center Center of the sphere.
 * @param distance Radius of the sphere.
 * @return Cartesian coordinates.
 */
Eigen::Vector3f PolarToCartesian(Eigen::Vector2f coordinates,
		Eigen::Vector3f center, float distance);

/**
 * \brief Camera class.
 *
 * Reprensent a camera in the 3D world.
 * Should be able to move in the scene around the objects.
 */
class Camera
{
public:
	/**
	 * @brief Constructor.
	 *
	 * `Camera` constructor.
	 */
	Camera();

	/**
	 * @brief Move camera in trackball mode.
	 *
	 * @param coordinates Coordinate on the trackball.
	 */
	void MoveCameraPolar(Eigen::Vector2f coordinates);

	/**
	 * @brief Zoom camera in trackball mode.
	 *
	 * @param intensity Change the intensity to zoom the camera.
	 */
	void ZoomCameraPolar(float intensity);

	/**
	 * @brief Compute the view matrix in trackball mode.
	 *
	 * @return View matrix.
	 */
	Eigen::Matrix4f ComputeViewMatrix() const;

	/**
	 * @brief Compute the view matrix in first person mode.
	 *
	 * @return View matrix of free mode…
	 */
	Eigen::Matrix4f Compute3DViewMatrix() const;

	/**
	 * @brief Compute the projection matrix in the 3D scene.
	 *
	 * @return Projection matrix.
	 */
	Eigen::Matrix4f ComputeProjectionMatrix() const;

	/**
	 * @brief Get the minimum screen viewport size.
	 *
	 * @return Minimum screen viewport size
	 */
	float MinScreenViewportSize() const;

	/**
	 * @brief Check if the camera is orthographic.
	 *
	 * @return Either if the camera is orthographic (`true`) or orthographic
	 *      (`false`).
	 */
	bool IsOrthographic() const;

	/**
	 * @brief Check if the camera is perspective.
	 *
	 * @return Either if the camera is perspective (`true`) or orthographic
	 *      (`false`).
	 */
	bool IsPerspective() const;

	/**
	 * @brief Get the scene radius.
	 *
	 * @return Scene radius.
	 */
	float GetSceneRadius() const;

	/**
	 * @brief Get the yaw of camera.
	 *
	 * @return Yaw value.
	 */
	float GetYaw() const;

	/**
	 * @brief Get the pitch of camera.
	 *
	 * @return Pitch value.
	 */
	float GetPitch() const;

	/**
	 * @brief Get the last X coordinate of mouse.
	 *
	 * @return Last X value.
	 */
	float GetLastX() const;

	/**
	 * @brief Get the last Y coordinate of mouse.
	 *
	 * @return Last Y value.
	 */
	float GetLastY() const;

	/**
	 * @brief Set the last X coordinate of mouse.
	 *
	 * @param positionX Last X coordinate.
	 */
	void SetLastX(float positionX);

	/**
	 * @brief Set last Y coordinate of mouse.
	 *
	 * @param positionY Last Y coordinate.
	 */
	void SetLastY(float positionY);

	/**
	 * @brief Change the yaw of camera.
	 *
	 * @param degree Degree to change.
	 */
	void MoveYaw(float degree);

	/**
	 * @brief Change the pitch of camera.
	 *
	 * @param degree Degree to change.
	 */
	void MovePitch(float degree);

	/**
	 * @brief Set the scene center.
	 *
	 * @param sceneCenter Center position of scene.
	 */
	void SetSceneCenter(const Eigen::Vector3f& sceneCenter);

	/**
	 * @brief Set the distance of the scene.
	 *
	 * @param sceneDistance Value of scene distance.
	 */
	void SetSceneDistance(float sceneDistance);

	/**
	 * @brief Set the scene radius.
	 *
	 * @param sceneRadius Value of scene radius.
	 */
	void SetSceneRadius(float sceneRadius);

	/**
	 * @brief Set the screen viewport.
	 *
	 * @param screenViewport Vector of viewport.
	 */
	void SetScreenViewport(const Eigen::AlignedBox2f& screenViewport);

	/**
	 * @brief Set the min near of the camera.
	 *
	 * @param minNear Distance value.
	 */
	void SetMinNear(float minNear);

	/**
	 * @brief Set the near/far offsets values.
	 *
	 * @param nearOffset Value of near offset.
	 * @param farOffset Value of far offset.
	 */
	void SetNearFarOffsets(float nearOffset, float farOffset);

	/**
	 * @brief Navigation first person with movement up and down.
	 */
	bool navigation3DUpDown = true;

	/**
	 * @brief Coordinate of camera in the scene 3D.
	 */
	Eigen::Vector3f camera3DCoordinates;

	/**
	 * @brief Coordinate of camera front.
	 */
	Eigen::Vector3f cameraFront;

	/**
	 * @brief Coordinate of camera up.
	 */
	Eigen::Vector3f up;
private:
	/**
	 * @brief Coordinate of scene center.
	 */
	Eigen::Vector3f sceneCenter;

	/**
	 * @brief Value of scene distance.
	 */
	float sceneDistance;

	/**
	 * @brief Value of scene orientation.
	 */
	Eigen::Quaternionf sceneOrientation;

	/**
	 * @brief Value of scene radius.
	 */
	float sceneRadius;

	/**
	 * @brief Value of screen viewport.
	 *
	 */
	Eigen::AlignedBox2f screenViewport;

	/**
	 * @brief Value of min near.
	 */
	float minNear;

	/**
	 * @brief Value of near offset.
	 */
	float nearOffset;
	/**
	 * @brief Value of far offset.
	 */
	float farOffset;

	/**
	 * @brief Camera matrix.
	 */
	Eigen::Matrix4f cameraMatrix;

	/**
	 * @brief Polar coordinates of camera.
	 */
	Eigen::Vector2f cameraPolarCoordinates;

	/**
	 * @brief Yaw value of camera.
	 */
	float yaw   = -90.0f;

	/**
	 * @brief Pitch of camera.
	 */
	float pitch = 0.0f;

	/**
	 * @brief Last X of mouse.
	 */
	float lastX = 0.;

	/**
	 * @brief Last Y of mouse.
	 */
	float lastY = 0.;
	
};

#endif // CAMERA_H
