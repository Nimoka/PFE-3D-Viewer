#ifndef CAMERA_H
#define CAMERA_H

#define MAX_PITCH 89.0
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

/**
 * @brief the orthographic projection matrix of camera
 *
 * @param l left, Number farthest left on the x-axis
 * @param r right, Number Farthest right on the x-axis
 * @param b bottom, Number Farthest down on the y-axis
 * @param t top, Number Farthest up on the y-axis
 * @param n near, Number distance to the near clipping plane along the -z axis
 * @param f far, Number distance to the far clipping plane along the -z axis
 * @return Eigen::Matrix4f, the orthographic transformation matrix
 */
Eigen::Matrix4f OrthographicProjection(float l, float r, float b, float t,
		float n, float f);

/**
 * @brief the perspective projection matrix of camera
 *
 * @param l left, Number farthest left on the x-axis
 * @param r right, Number Farthest right on the x-axis
 * @param b bottom, Number Farthest down on the y-axis
 * @param t top, Number Farthest up on the y-axis
 * @param n near, Number distance to the near clipping plane along the -z axis
 * @param f far, Number distance to the far clipping plane along the -z axis
 * @return Eigen::Matrix4f, the perspective transformation matrix
 */
Eigen::Matrix4f PerspectiveProjection(float l, float r, float b, float t,
		float n, float f);

/**
 * @brief the perspective matrix of camera (for first person navigation)
 *
 * @param fovy the fovy of camera
 * @param aspect the aspect of camera
 * @param zNear the near of camera
 * @param zFar  the far of camera
 * @return Eigen::Matrix4f the perspective matrix
 */
static Eigen::Matrix4f Perspective(float fovy, float aspect,
		float zNear, float zFar);

/**
 * @brief the camera view
 *
 * @param position the position of camera
 * @param target the position of target
 * @param up the angle of up of camera
 * @return Eigen::Matrix4f the view matrix
 */
static Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
		const Eigen::Vector3f& target, const Eigen::Vector3f& up);

/**
 * @brief computer the cartesian form polar
 *
 * @param coordinates  the coordinate
 * @param center  the center
 * @param distance  the distance
 * @return Eigen::Vector3f
 */
Eigen::Vector3f PolarToCartesian(Eigen::Vector2f coordinates,
		Eigen::Vector3f center, float distance);

/**
 * \brief camera class.
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
	 * @param corrdinates. the coordinate on the trackball.
	 *
	 */
	void MoveCameraPolar(Eigen::Vector2f coordinates);

	/**
	 * @brief Zoom camera in trackball mode.
	 *
	 * @param intensity. change the intensity to zoom the camera.
	 */
	void ZoomCameraPolar(float intensity);

	/**
	 * @brief compute the view matrix in trackball mode
	 *
	 * @return Eigen::Matrix4f the 4 * 4 matrix of view
	 */
	Eigen::Matrix4f ComputeViewMatrix() const;

	/**
	 * @brief compute the view matrix in first person mode
	 *
	 * @return Eigen::Matrix4f the 4 * 4 matrix of view
	 */
	Eigen::Matrix4f Compute3DViewMatrix() const;

	/**
	 * @brief compute the projection matrix in the 3d scene
	 *
	 * @return Eigen::Matrix4f  the 4*4 projection matrix
	 */
	Eigen::Matrix4f ComputeProjectionMatrix() const;

	/**
	 * @brief Get the minimum screen viewport size
	 *
	 * @return float, value of the size
	 */
	float MinScreenViewportSize() const;

	/**
	 * @brief check if the camera is orthographic
	 *
	 * @return true , the camera is orthographic
	 * @return false , the camera is perspective
	 */
	bool IsOrthographic() const;

	/**
	 * @brief check if the camera is perspective
	 *
	 * @return true, the camera is perspective
	 * @return false, the camera is orthographic
	 */
	bool IsPerspective() const;

	/**
	 * @brief Get the scene radius
	 *
	 * @return float, the value of radius
	 */
	float GetSceneRadius() const;

	/**
	 * @brief Get the yaw of camera
	 *
	 * @return float, the value of yaw
	 */
	float GetYaw() const;

	/**
	 * @brief Get the pitch of camera
	 *
	 * @return float, the value of pitch
	 */
	float GetPitch() const;

	/**
	 * @brief Get the last x coordinate of mouse
	 *
	 * @return float, the last x value
	 */
	float GetLastX() const;

	/**
	 * @brief Get the last y  coordinate of mouse
	 *
	 * @return float, the last y value
	 */
	float GetLastY() const;

	/**
	 * @brief Set the last x coordinate of mouse
	 *
	 * @param positionX , the last x coordinate
	 */
	void SetLastX(float positionX);

	/**
	 * @brief Set last y  coordinate of mouse
	 *
	 * @param positionY, the last y coordinate
	 */
	void SetLastY(float positionY);

	/**
	 * @brief change the yaw of camera
	 *
	 * @param degree, the degree to change
	 */
	void MoveYaw(float degree);

	/**
	 * @brief change the pitch of camera
	 *
	 * @param degree, the degree to change
	 */
	void MovePitch(float degree);

	/**
	 * @brief Set the scene center
	 *
	 * @param sceneCenter, the center position of scene
	 */
	void SetSceneCenter(const Eigen::Vector3f& sceneCenter);

	/**
	 * @brief Set the distance of the scene
	 *
	 * @param sceneDistance, the value of distance
	 */
	void SetSceneDistance(float sceneDistance);

	/**
	 * @brief Set the scene radius
	 *
	 * @param sceneRadius, the value of radius
	 */
	void SetSceneRadius(float sceneRadius);

	/**
	 * @brief Set the screen viewport
	 *
	 * @param screenViewport, the vector of viewport
	 */
	void SetScreenViewport(const Eigen::AlignedBox2f& screenViewport);

	/**
	 * @brief Set the min near of the camera
	 *
	 * @param minNear, the distance value
	 */
	void SetMinNear(float minNear);

	/**
	 * @brief Set the near/far Offsets values
	 *
	 * @param nearOffset, the value of nearoffset
	 * @param farOffset, the value of far offset
	 */
	void SetNearFarOffsets(float nearOffset, float farOffset);

	/**
	 * @brief navigation first person with movement up and down
	 *
	 */
	bool navigation3DUpDown = true;

	/**
	 * @brief the coordinate of camera in the scene 3D
	 *
	 */
	Eigen::Vector3f camera3DCoordinates;

	/**
	 * @brief the coordinate of camera front
	 *
	 */
	Eigen::Vector3f cameraFront;

	/**
	 * @brief the coordinate of camera up
	 *
	 */
	Eigen::Vector3f up;
private:
	/**
	 * @brief the coordinate of scenecenter
	 *
	 */
	Eigen::Vector3f sceneCenter;

	/**
	 * @brief the value of scene disntance
	 *
	 */
	float sceneDistance;

	/**
	 * @brief the value of scene orientation
	 *
	 */
	Eigen::Quaternionf sceneOrientation;

	/**
	 * @brief the value of scene radius
	 *
	 */
	float sceneRadius;

	/**
	 * @brief the value of scenen view port
	 *
	 */
	Eigen::AlignedBox2f screenViewport;

	/**
	 * @brief the value of min near
	 *
	 */
	float minNear;

	/**
	 * @brief the value of near offset
	 * @brief the value of far offset
	 */
	float nearOffset, farOffset;

	/**
	 * @brief the camera matrix
	 *
	 */
	Eigen::Matrix4f cameraMatrix;

	/**
	 * @brief the polar coordinates of camera
	 *
	 *
	 */
	Eigen::Vector2f cameraPolarCoordinates;

	/**
	 * @brief the initial yaw value of camera
	 *
	 */
	float yaw   = -90.0f;

	/**
	 * @brief the initial pitch of camera
	 *
	 */
	float pitch = 0.0f;

	/**
	 * @brief the initial last x of mouse
	 *
	 */
	float lastX = 1280.0 / 2.0;

	/**
	 * @brief the initial last y of mouse
	 *
	 */
	float lastY = 800 / 2.0;
	
};

#endif // CAMERA_H
