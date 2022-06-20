#pragma once

#include "daengine/core/containers/array.h"
#include "daengine/core/containers/enumerable.h"
#include "daengine/core/containers/enumerator.h"
#include "daengine/core/containers/list.h"
#include "daengine/core/containers/string.h"
#include "daengine/core/containers/pair.h"
#include "daengine/core/containers/hashstring.h"

using namespace da::core::containers;

using CString = da::core::containers::CBasicString<da::memory::CGlobalAllocator>;
using CHashString = da::core::containers::CBasicHashString<da::memory::CGlobalAllocator>;