#pragma once

#include "containers/array.h"
#include "containers/enumerable.h"
#include "containers/enumerator.h"
#include "containers/list.h"

namespace da {

	template <typename T>
	using Array = da::core::containers::TArray<T>;
	template <typename T>
	using List = da::core::containers::TList<T>;
	template <typename T>
	using Enumerator = da::core::containers::TEnumerator<T>;
	template <typename T>
	using Enumerable = da::core::containers::IEnumerable<T>;
}