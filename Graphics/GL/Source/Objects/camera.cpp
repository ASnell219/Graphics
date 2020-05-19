#include "camera.h"
#include "core/scene.h"
#include "core/engine.h"


void Camera::Initialize()
{
	scene->m_engine->Get<Input>()->AddAction("camera_left", SDL_SCANCODE_A, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("camera_right", SDL_SCANCODE_D, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("camera_forward", SDL_SCANCODE_W, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("camera_backward", SDL_SCANCODE_S, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("camera_up", SDL_SCANCODE_E, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("camera_down", SDL_SCANCODE_Q, Input::eDevice::KEYBOARD);
	scene->m_engine->Get<Input>()->AddAction("left_action", SDL_BUTTON_LEFT, Input::eDevice::MOUSE);
	scene->m_engine->Get<Input>()->AddAction("middle_action", SDL_BUTTON_LEFT, Input::eDevice::MOUSE);
	scene->m_engine->Get<Input>()->AddAction("right_action", SDL_BUTTON_RIGHT, Input::eDevice::MOUSE);
	scene->m_engine->Get<Input>()->AddAction("x-axis", Input::eAxis::X, Input::eDevice::MOUSE);
	scene->m_engine->Get<Input>()->AddAction("y-axis", Input::eAxis::Y, Input::eDevice::MOUSE);

	SetProjection(45.0f, 0.01f, 1000.0f);
}

void Camera::Update()
{
	float dt = scene->m_engine->Get<Timer>()->DeltaTime();

	glm::vec3 translate(0.0f);
	glm::vec3 rotate(0.0f);

	switch (type)
	{
	case LOOK_AT:
		UpdateLookAt(translate, rotate);
		break;
	case EDITOR:
		UpdateEditor(translate, rotate);
		break;
	}

	transform.translation += (translate * transform.rotation) * dt;

	// update transform
	glm::mat4 mxt = glm::translate(glm::mat4(1.0f), -transform.translation);
	glm::mat4 mxr = glm::mat4_cast(transform.rotation);

	transform.matrix = mxr * mxt;
}

void Camera::SetView(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	transform.matrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
	transform.rotation = glm::quat_cast(transform.matrix);
	transform.translation = position;
}

void Camera::SetProjection(float fov, float nearClip, float farClip)
{
	projection = glm::perspective(glm::radians(fov), (float)scene->m_engine->Get<Renderer>()->GetWidth() / (float)scene->m_engine->Get<Renderer>()->GetHeight(), nearClip, farClip);
}

void Camera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
{
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
}

void Camera::UpdateLookAt(glm::vec3 & translate, glm::vec3 & rotate)
{
}

void Camera::UpdateEditor(glm::vec3 & translate, glm::vec3 & rotate)
{
	// update rotation
	if (scene->m_engine->Get<Input>()->GetActionButton("right_action"))
	{
		rotate.x = scene->m_engine->Get<Input>()->GetActionAxisRelative("y-axis") * 0.003f;
		rotate.y = scene->m_engine->Get<Input>()->GetActionAxisRelative("x-axis") * 0.003f;
		glm::quat qpitch = glm::angleAxis(rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qyaw = glm::angleAxis(rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform.rotation = qpitch * transform.rotation * qyaw;
		transform.rotation = glm::normalize(transform.rotation);
	}

	// update translate
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_left") == Input::eButtonState::HELD) translate.x -= m_rate;
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_right") == Input::eButtonState::HELD) translate.x += m_rate;
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_forward") == Input::eButtonState::HELD) translate.z -= m_rate;
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_backward") == Input::eButtonState::HELD) translate.z += m_rate;
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_up") == Input::eButtonState::HELD) translate.y += m_rate;
	if (scene->m_engine->Get<Input>()->GetActionButton("camera_down") == Input::eButtonState::HELD) translate.y -= m_rate;
}

//void Camera::Edit()
//{
	//Transform::Edit(&transform);
//}
