#pragma once
#include "Light.h"

class Shader;

class SpotLight : public Light
{
    lm::vec3 position;
    lm::vec3 direction;
    float cutOff;
    float outerCutOff;

public:
    SpotLight(lm::vec3 const& ambient, lm::vec3 const& diffuse, lm::vec3 const& specular, lm::vec3 const& position, lm::vec3& direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
    void UpdateLights(Shader* shader, int index) override;
    void SetPositionDirection(Camera* cam);
    lm::vec3& GetDirection() { return direction; }
    float& GetCutOff() { return cutOff; }
    float& GetOuterCutOff() { return outerCutOff; }
};