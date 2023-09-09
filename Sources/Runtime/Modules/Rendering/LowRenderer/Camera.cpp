/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "cmath"

#include "Modules/Rendering/LowRenderer/Camera.h"
#include "Core/Maths/FMatrix4.h"

LittleEngine::Rendering::LowRenderer::Camera::Camera() :
    m_projectionMode(Settings::EProjectionMode::PERSPECTIVE),
	m_fov(45.0f),
    m_size(5.0f),
	m_near(0.1f),
	m_far(100.f),
	m_clearColor(0.f, 0.f, 0.f),
	m_frustumGeometryCulling(false),
	m_frustumLightCulling(false)
{
}

void LittleEngine::Rendering::LowRenderer::Camera::CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const LittleEngine::FVector3& p_position, const LittleEngine::FQuaternion& p_rotation)
{
	CacheProjectionMatrix(p_windowWidth, p_windowHeight);
	CacheViewMatrix(p_position, p_rotation);
	CacheFrustum(m_viewMatrix, m_projectionMatrix);
}

void LittleEngine::Rendering::LowRenderer::Camera::CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	m_projectionMatrix = CalculateProjectionMatrix(p_windowWidth, p_windowHeight);
}

void LittleEngine::Rendering::LowRenderer::Camera::CacheViewMatrix(const LittleEngine::FVector3& p_position, const LittleEngine::FQuaternion& p_rotation)
{
	m_viewMatrix = CalculateViewMatrix(p_position, p_rotation);
}

void LittleEngine::Rendering::LowRenderer::Camera::CacheFrustum(const LittleEngine::FMatrix4& p_view, const LittleEngine::FMatrix4& p_projection)
{
	m_frustum.CalculateFrustum(p_projection * p_view);
}

float LittleEngine::Rendering::LowRenderer::Camera::GetFov() const
{
	return m_fov;
}

float LittleEngine::Rendering::LowRenderer::Camera::GetSize() const
{
    return m_size;
}

float LittleEngine::Rendering::LowRenderer::Camera::GetNear() const
{
	return m_near;
}

float LittleEngine::Rendering::LowRenderer::Camera::GetFar() const
{
	return m_far;
}

const LittleEngine::FVector3 & LittleEngine::Rendering::LowRenderer::Camera::GetClearColor() const
{
	return m_clearColor;
}

const LittleEngine::FMatrix4& LittleEngine::Rendering::LowRenderer::Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const LittleEngine::FMatrix4& LittleEngine::Rendering::LowRenderer::Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const LittleEngine::Rendering::Data::Frustum& LittleEngine::Rendering::LowRenderer::Camera::GetFrustum() const
{
	return m_frustum;
}

bool LittleEngine::Rendering::LowRenderer::Camera::HasFrustumGeometryCulling() const
{
	return m_frustumGeometryCulling;
}

bool LittleEngine::Rendering::LowRenderer::Camera::HasFrustumLightCulling() const
{
	return m_frustumLightCulling;
}

LittleEngine::Rendering::Settings::EProjectionMode LittleEngine::Rendering::LowRenderer::Camera::GetProjectionMode() const
{
    return m_projectionMode;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetSize(float p_value)
{
    m_size = p_value;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetNear(float p_value)
{
	m_near = p_value;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetFar(float p_value)
{
	m_far = p_value;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetClearColor(const LittleEngine::FVector3 & p_clearColor)
{
	m_clearColor = p_clearColor;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetFrustumGeometryCulling(bool p_enable)
{
	m_frustumGeometryCulling = p_enable;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetFrustumLightCulling(bool p_enable)
{
	m_frustumLightCulling = p_enable;
}

void LittleEngine::Rendering::LowRenderer::Camera::SetProjectionMode(LittleEngine::Rendering::Settings::EProjectionMode p_projectionMode)
{
    m_projectionMode = p_projectionMode;
}

LittleEngine::FMatrix4 LittleEngine::Rendering::LowRenderer::Camera::CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const
{
    using namespace LittleEngine;
    using namespace LittleEngine::Rendering::Settings;

    const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);

    switch (m_projectionMode)
    {
    case EProjectionMode::ORTHOGRAPHIC:
        return FMatrix4::CreateOrthographic(m_size, ratio, m_near, m_far);

    case EProjectionMode::PERSPECTIVE: 
        return FMatrix4::CreatePerspective(m_fov, ratio, m_near, m_far);

    default:
        return FMatrix4::Identity;
    }
}

LittleEngine::FMatrix4 LittleEngine::Rendering::LowRenderer::Camera::CalculateViewMatrix(const LittleEngine::FVector3& p_position, const LittleEngine::FQuaternion& p_rotation) const
{
	const auto& up = p_rotation * LittleEngine::FVector3::Up;
	const auto& forward = p_rotation * LittleEngine::FVector3::Forward;

	return LittleEngine::FMatrix4::CreateView(p_position,forward,up);
}
