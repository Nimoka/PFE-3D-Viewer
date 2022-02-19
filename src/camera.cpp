#include "camera.h"

#define ORTHO_THRESOLD		1.e2f

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

Eigen::Matrix4f Camera::Perspective(float fovy, float aspect,
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

Eigen::Matrix4f Camera::LookAt(const Eigen::Vector3f& position,
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

Eigen::Matrix4f Camera::ComputeViewMatrix() const {
	Eigen::Vector3f sceneCamera = this->sceneCenter;
	if (this->IsPerspective())
		sceneCamera += this->sceneOrientation * Eigen::Vector3f::UnitZ()
				* this->sceneDistance;

	return Eigen::Affine3f(this->sceneOrientation.inverse()
			* Eigen::Translation3f(-sceneCamera)).matrix();
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
