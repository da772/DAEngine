#pragma once
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <stacktrace>
#include <functional>
#include <mutex>
#include <map>
#include <glm/glm.hpp>

#ifndef DA_DEBUG
#define NDEBUG
#endif
#include "DAEngine/core/containers.h"
#include "DAEngine/logger.h"