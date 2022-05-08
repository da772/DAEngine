#pragma once

#include "core/containers/array.h"
#include "core/containers/enumerable.h"
#include "core/containers/enumerator.h"
#include "core/containers/list.h"
#include "core/containers/pair.h"

namespace da {

	template <typename T>
	using Array = da::core::containers::TArray<T>;
	template <typename T>
	using List = da::core::containers::TList<T>;
	template <typename T>
	using Enumerator = da::core::containers::TEnumerator<T>;
	template <typename T>
	using Enumerable = da::core::containers::IEnumerable<T>;
	template <typename K, typename V>
	using Pair = da::core::containers::TPair<K, V>;

}