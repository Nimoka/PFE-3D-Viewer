#include "camera.h"

#define ORTHO_THRESOLD		1.e2f
#define CAMERASPEED			0.1f
#define MAX_ANGLE			((EIGEN_PI / 2.) - 1.2e-3)

Eigen::Matrix4f OrthographicProjection(float l, float r, float b, float t,
		float n, float f) {
	float rpl = r + l;
	float tpb = t + b;
	float fpn = f + n;

	float rml = r - l;
	float tmb = t - b;
	float fmn = f - n;

	Eigen::Matrix4f m;
	m << 	(2. / rml), 0., 0., ((-rpl) / rml),
			0., (2. / tmb), 0., ((-tpb) / tmb),
			0., 0., ((-2.) / fmn), (fpn / fmn),
			0., 0., 0., 1.;
	return m;
}

Eigen::Matrix4f PerspectiveProjection(float l, float r, float b, float t,
		float n, float f) {
	float rpl = r + l;
	float tpb = t + b;
	float fpn = f + n;

	float rml = r - l;
	float tmb = t - b;
	float fmn = f - n;

	Eigen::Matrix4f m;
	m << 	(2. / rml), 0, (rpl / rml), 0.,
			0., (2. / tmb), (tpb/tmb), 0.,
			0., 0., ((-fpn) / fmn), ((-2. * f * n) / fmn),
			0., 0., -1., 0.;
	return m;
}

Eigen::Matrix4f Perspective(float fovy, float aspect,
		float zNear, float zFar) {
	float tanHalfFovy = tan(fovy / 2.f);

	Eigen::Matrix4f m;
	m << 	(1. / (aspect * tanHalfFovy)), 0., 0., 0.,
			0., (1. / (tanHalfFovy)), 0., 0.,
			0., 0., (-(zFar + zNear) / (zFar - zNear)),
					((-2. * zFar * zNear) / (zFar - zNear)),
			0., 0., -1., 0.;
	return m;
}

Eigen::Matrix4f LookAt(const Eigen::Vector3f& position,
		const Eigen::Vector3f& target, const Eigen::Vector3f& up) {
	Eigen::Matrix3f R;
	R.col(2) = (position - target).normalized();
	R.col(0) = up.cross(R.col(2)).normalized();
	R.col(1) = R.col(2).cross(R.col(0));

	Eigen::Matrix4f m(Eigen::Matrix4f::Identity());
	m.topLeftCorner<3,3>() = R.transpose();
	m.topRightCorner<3,1>() = -m.topLeftCorner<3,3>() * position;
	return m;
}

Eigen::Vector3f PolarToCartesian(Eigen::Vector2f coordinates,
		Eigen::Vector3f center, float distance) {
	float x = center.x()
			+ (distance * cos(coordinates.x()) * cos(coordinates.y()));
	float y = center.y()
			+ (distance * sin(coordinates.y()));
	float z = center.z()
			+ (distance * sin(coordinates.x()) * cos(coordinates.y()));

	return Eigen::Vector3f(x, y, z);
}

Camera::Camera()
		: cameraPolarCoordinates(Eigen::Vector2f(0., 0.))
		, camera3DCoordinates(Eigen::Vector3f(0.5,0.5,3.5))
		, cameraFront(Eigen::Vector3f(0.,0.,-1))
		, sceneCenter(Eigen::Vector3f(0., 0., 0.))
		, up(Eigen::Vector3f(0., 1., 0.)) {}

void Camera::MoveCameraPolar(Eigen::Vector2f coordinates) {
	this->cameraPolarCoordinates += coordinates;
	if (this->cameraPolarCoordinates.x() > EIGEN_PI)
		this->cameraPolarCoordinates.x() -= (2 * EIGEN_PI);
	else if (this->cameraPolarCoordinates.x() < (-EIGEN_PI))
		this->cameraPolarCoordinates.x() += (2 * EIGEN_PI);
	if (this->cameraPolarCoordinates.y() > MAX_ANGLE)
		this->cameraPolarCoordinates.y() = MAX_ANGLE;
	else if (this->cameraPolarCoordinates.y() < -MAX_ANGLE)
		this->cameraPolarCoordinates.y() = -MAX_ANGLE;
}

void Camera::ZoomCameraPolar(float intensity) {
	this->sceneRadius += intensity;

	if (this->sceneRadius < .0001)
		this->sceneRadius -= intensity;
}

Eigen::Matrix4f Camera::ComputeViewMatrix() const {
	return LookAt(
			PolarToCartesian(this->cameraPolarCoordinates,
					this->sceneCenter, this->sceneDistance),
			this->sceneCenter, this->up);
}

Eigen::Matrix4f Camera::Compute3DViewMatrix() const {
	return LookAt(
			this->camera3DCoordinates,
			this->camera3DCoordinates+cameraFront, this->up);
}


Eigen::Matrix4f Camera::ComputeProjectionMatrix() const {
	float screenMinSize = MinScreenViewportSize();
	float r = this->sceneRadius * this->screenViewport.sizes().x()
			/ (screenMinSize);
	float t = this->sceneRadius * this->screenViewport.sizes().y()
			/ (screenMinSize);

	if (this->IsPerspective()) {
		r /= this->sceneDistance;
		t /= this->sceneDistance;
		return PerspectiveProjection(
			-r, r, -t, t,
			std::max(this->minNear, this->sceneDistance + this->nearOffset),
			this->sceneDistance + this->farOffset);
	} else {
		return OrthographicProjection(-r, r, -t, t, this->nearOffset,
				this->farOffset);
	}
}

float Camera::MinScreenViewportSize() const {
	return this->screenViewport.sizes().minCoeff();
}

bool Camera::IsOrthographic() const {
	return (this->sceneDistance / this->sceneRadius) > ORTHO_THRESOLD;
}

bool Camera::IsPerspective() const {
	return !IsOrthographic();
}

float Camera::GetSceneRadius() const {
	return this->sceneRadius;
}

void Camera::SetSceneCenter(const Eigen::Vector3f& sceneCenter) {
	this->sceneCenter = sceneCenter;
}

void Camera::SetSceneDistance(float sceneDistance) {
	this->sceneDistance = sceneDistance;
}

void Camera::SetSceneRadius(float sceneRadius) {
	this->sceneRadius = sceneRadius;
}

void Camera::SetScreenViewport(const Eigen::AlignedBox2f& screenViewport) {
	this->screenViewport = screenViewport;
}

void Camera::SetMinNear(float minNear) {
	this->minNear = minNear;
}

void Camera::SetNearFarOffsets(float nearOffset, float farOffset) {
	this->nearOffset = nearOffset;
	this->farOffset = farOffset;
}
