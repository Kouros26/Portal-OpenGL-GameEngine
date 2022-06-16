#pragma once
#include "Light.h"

class Shader;

class PointLight : public Light
{
    lm::vec3& position;

public:
    PointLight(lm::vec3 const& ambient, lm::vec3 const& diffuse, lm::vec3 const& specular, lm::vec3* pos, float constant, float linear, float quadratic);
    void UpdateLights(Shader* shader, int index) override;
    vec3& GetPos() { return position; }
    void SetPos(vec3 const& newPos) { position = newPos; }
};