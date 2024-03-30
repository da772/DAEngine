#pragma once
#include "containers.h"

namespace da::core {

	using uint128_t = unsigned char[16];
	using guid_str = char[37];

	class CGuid {

	public:
		CGuid();
		CGuid(const uint128_t guid);
		CGuid(const CGuid& guid);
		CGuid(const char* guid);

		const unsigned char* const data() const;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		const char* c_str() const;
#else
		const char* c_str() const;
#endif
		void c_str(char* out) const;
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
		void genString(guid_str out) const;

	private:
		static unsigned int sm_seed;

	private:
		uint128_t m_uuid;
		char __padding = 0;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		guid_str m_debugName;
#endif

	};

}
