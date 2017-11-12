#include "Shader.h"
#include "Camera.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

Camera::Camera()
    : _view(new Transform(nullptr)), _proj(glm::mat4()), _dirty(false)
    , _fov(45.f), _aspect(4.f/3.f), _near(.1f), _far(100.f) {
}

void Camera::set_fov(float fov) {
    _fov = fov;
    _dirty = true;
}

void Camera::set_aspect(float aspect) {
    _aspect = aspect;
    _dirty = true;
}

void Camera::set_near(float near) {
    _near = near;
    _dirty = true;
}

void Camera::set_far(float far) {
    _far = far;
    _dirty = true;
}

void Camera::set_pos(glm::vec3 p) {
    _view->set_pos(p);
}

void Camera::set_rot(glm::vec3 look_dir, glm::vec3 up) {
    glm::vec3 pos = _view->pos();
    glm::mat4 look = glm::lookAt(pos, pos + glm::normalize(look_dir), up);
    _view->set_mat(look);
}

void Camera::move(glm::vec3 p) {
    _view->set_pos(p + _view->pos());
}

void Camera::rotate(glm::quat q) {
    //todo: move into Transform in a better fashion
    glm::mat4 rot = glm::toMat4(q);
    glm::vec3 pos = _view->pos();
    _view->set_pos(glm::vec3(0, 0, 0));
    _view->set_mat(rot * _view->mat());
    _view->set_pos(pos);
}

void Camera::zoom(float z) {
    //the clamp is needed for an FPS camera
    _fov = glm::clamp(glm::degrees(static_cast<float>(atan2(1, z))), 1.f, 90.f);
    _dirty = true;
}

void Camera::apply_proj(Shader& shader) {
    if (_dirty) {
        _proj = glm::perspective(glm::radians(_fov), _aspect, _near, _far);
        _dirty = false;
    }
    shader.use();
    shader.set("proj", _proj);
}

void Camera::apply_view(Shader& shader) {
    shader.use();
    shader.set("view", _view->global_mat());
}

