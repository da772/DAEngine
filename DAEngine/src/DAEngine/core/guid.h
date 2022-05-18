#pragma once
#include "containers.h"

namespace da::core {

	using uint128_t = unsigned char[16];

	class CGuid {

	public:
		CGuid();
		CGuid(const uint128_t guid);
		CGuid(const CGuid& guid);
		CGuid(const CString& guid);

		const unsigned char* const data() const;
		CString string() const;
		bool isValid() const;

		bool operator==(const CGuid& rhs) const;
		bool operator!=(const CGuid& rhs) const;

	public:
		/// <summary>
		/// Version 4 (random)
		/// https://en.wikipedia.org/wiki/Universally_unique_identifier#Version_4_(random)
		/// </summary>
		/// <returns></returns>
		static CGuid Generate();

	private:
		static unsigned int sm_seed;

	private:
		uint128_t m_uuid;

	};

}
