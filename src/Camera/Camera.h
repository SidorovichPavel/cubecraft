#pragma once

#include <src/glm/glm.hpp>

class Camera
{
	glm::vec3 mPosition;
	glm::vec3 mDirection;
	glm::vec3 mUp;
	glm::vec3 mRight;
	float mAspect{};
	float mFovy{};

	glm::mat4 mRotate;

	float mPitch;
	float mYaw;
	float mRoll;

	bool useQuaternions;

	void update_vertors() noexcept;
public:
	Camera(const glm::vec3 _Pos, const glm::vec3 _Targe, glm::vec3 _Up, const float _Aspect, const float _Fovy);
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;
	~Camera();

	Camera& operator+=(const glm::vec3& _Vec)noexcept;
	Camera& operator-=(const glm::vec3& _Vec)noexcept;

	glm::mat4 get_perspective() const noexcept;
	glm::mat4 get_view()const noexcept;
	glm::mat4 get_mat4() const noexcept;
	glm::vec3 get_direction() const noexcept;
	glm::vec3 get_right() const noexcept;
	glm::vec3 get_up() const noexcept;
	glm::vec3 get_position() const noexcept;

	void update_Fovy(uint16_t _Keys, int16_t _Delta, int32_t x, int32_t y) noexcept;
	void update_aspect(uint16_t x, uint16_t y)noexcept;
	void update_angles(float x, float y, float z);
	void use_quaternoins(bool _mode = true);
};

