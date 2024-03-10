#pragma once
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <stacktrace>
#include <functional>
#include <mutex>
#include <map>
#include <unordered_set>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <array>


#ifndef DA_DEBUG
#define NDEBUG
#endif
#include "DAEngine/core/core.h"
#include "DAEngine/core/containers.h"
#include "DAEngine/logger.h"