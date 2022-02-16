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

Eigen::Matrix4f OrthographicProjection(float l, float r, float b, float t, float n, float f);
Eigen::Matrix4f PerspectiveProjection(float l, float r, float b, float t, float n, float f);

class Camera
{
public:
	Camera() {}

	static Eigen::Matrix4f Perspective(float fovy, float aspect,
			float zNear, float zFar);
	static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
			const Eigen::Vector3f& target, const Eigen::Vector3f& up);

	Eigen::Matrix4f ComputeViewMatrix() const;
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

private:
	Eigen::Vector3f sceneCenter;
	float sceneDistance;
	Eigen::Quaternionf sceneOrientation;
	float sceneRadius;
	Eigen::AlignedBox2f screenViewport;
	float minNear;
	float nearOffset, farOffset;
};

#endif // CAMERA_H
