#include "camera.hpp"

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

glm::mat4 const &Camera::getViewMatrix() const {
  return m_viewMatrix;
}

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projectionMatrix = glm::perspective(glm::radians(70.0f), size.x / size.y, 0.1f, 100.0f);
}

glm::mat4 const &Camera::getProjectionMatrix() const {
  return m_projectionMatrix;
}

void Camera::pedestal(float speed) {
  m_eye += m_up * speed;
  m_at += m_up * speed;

  computeViewMatrix();
}

void Camera::dolly(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};

  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};
  auto const left{glm::normalize(glm::cross(m_up, forward))};

  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

void Camera::tilt(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};
  auto const left{glm::normalize(glm::cross(m_up, forward))};

  glm::mat4 transform{1.0f};

  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, left);
  transform = glm::translate(transform, -m_eye);

  auto const updatedM_at = transform * glm::vec4(m_at, 1.0f);

  if (std::abs(m_eye.z - updatedM_at.z) <= 0.1) {
    m_at = {updatedM_at.x, updatedM_at.z, m_at.z};
  } else {
    m_at = updatedM_at;
  }

  computeViewMatrix();
}

void Camera::reset() {
  m_eye = {0.0f, 10.0f, 5.0f};
  m_at = {0.0f, 0.0f, 0.0f};

  computeViewMatrix();
}