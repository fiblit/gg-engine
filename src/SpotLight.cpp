#include "SpotLight.h"

SpotLight::SpotLight()
    : _dir(glm::vec3(0, 0, 1)), 
    _cutoff_100p(.35f),//roughly 20->30 degrees
    _cutoff_0p(.52f) {
}

void SpotLight::pass_to(Shader& shader, const std::string& id) {
    PointLight::pass_to(shader, id);
    shader.set(id + "dir", _dir);
    shader.set(id + "cutoff_100p", _cutoff_100p);
    shader.set(id + "cutoff_0p", _cutoff_0p);
}

glm::vec3 SpotLight::dir() {
    return _dir;
}
void SpotLight::dir(glm::vec3 dir) {
    _dir = dir;
}
float SpotLight::cutoff_100p() {
    return _cutoff_100p;
}
float SpotLight::cutoff_0p() {
    return _cutoff_0p;
}
void SpotLight::cutoff(float p0, float p100) {
    _cutoff_100p = p100;
    _cutoff_0p = p0;
}
