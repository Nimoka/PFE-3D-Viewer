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

Eigen::Matrix4f OrthographicProjection(float l, float r, float b, float t,
		float n, float f);
Eigen::Matrix4f PerspectiveProjection(float l, float r, float b, float t,
		float n, float f);

static Eigen::Matrix4f Perspective(float fovy, float aspect,
		float zNear, float zFar);
static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
		const Eigen::Vector3f& target, const Eigen::Vector3f& up);

Eigen::Vector3f PolarToCartesian(Eigen::Vector2f coordinates, Eigen::Vector3f center, float distance);

class Camera
{
public:
	Camera();

	void MoveCameraPolar(Eigen::Vector2f coordinates);
	void ZoomCameraPolar(float intensity);

	Eigen::Matrix4f ComputeViewMatrix() const;
	Eigen::Matrix4f Compute3DViewMatrix() const;
	Eigen::Matrix4f ComputeProjectionMatrix() const;

	float MinScreenViewportSize() const;
	bool IsOrthographic() const;
	bool IsPerspective() const;
	float GetSceneRadius() const;

	void SetSceneCenter(const Eigen::Vector3f& sceneCenter);
	void SetSceneDistance(float sceneDistance);
	void SetSceneRadius(float sceneRadius);
	void SetScreenViewport(const Eigen::AlignedBox2f& screenViewport);
	void SetMinNear(float minNear);
	void SetNearFarOffsets(float nearOffset, float farOffset);

	bool navigation3DUpDown = true;
	Eigen::Vector3f camera3DCoordinates;
	Eigen::Vector3f cameraFront;
	Eigen::Vector3f up;
private:
	Eigen::Vector3f sceneCenter;
	float sceneDistance;
	Eigen::Quaternionf sceneOrientation;
	float sceneRadius;
	Eigen::AlignedBox2f screenViewport;
	float minNear;
	float nearOffset, farOffset;

	Eigen::Matrix4f cameraMatrix;
	Eigen::Vector2f cameraPolarCoordinates;
	
};

#endif // CAMERA_H
