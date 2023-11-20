#pragma once
#include "containers.h"

namespace da::core {

	using uint128_t = unsigned char[16];

	class CGuid {

	public:
		CGuid();
		CGuid(const uint128_t guid);
		CGuid(const CGuid& guid);
		CGuid(const char* guid);

		const unsigned char* const data() const;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		const char* c_str() const;
#endif
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
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		void genString();
#endif

	private:
		static unsigned int sm_seed;

	private:
		uint128_t m_uuid;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		char m_debugName[37];
#endif

	};

}
