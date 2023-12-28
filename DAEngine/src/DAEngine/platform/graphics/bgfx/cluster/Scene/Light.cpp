#include "dapch.h"
#include "Light.h"

#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/component_wise.hpp>
#include <bx/math.h>

float PointLight::calculateRadius() const
{
    // radius = where attenuation would lead to an intensity of 1W/m^2
    const float INTENSITY_CUTOFF = 1.0f;
    const float ATTENTUATION_CUTOFF = 0.05f;
    glm::vec3 intensity = flux / (4.0f * glm::pi<float>());

    if (intensity.x > 5000.f || intensity.y > 5000.f || intensity.z > 5000.f) {
        return bx::max(intensity.x, intensity.y, intensity.z);
    }

    float maxIntensity = glm::compMax(intensity);
    float attenuation = glm::max(INTENSITY_CUTOFF, ATTENTUATION_CUTOFF * maxIntensity) / maxIntensity;
    return 1.0f / sqrtf(attenuation);
}
