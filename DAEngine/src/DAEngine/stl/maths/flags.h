#pragma once


namespace da::maths
{
	class CFlag
	{
	public:
		template<typename T>
		inline static bool hasFlag(T source, T flag) {
			return source & flag;
		}
		
		template<typename T>
		inline static void setFlag(T& source, T flag) {
			source |= flag;
		}

		template<typename T>
		inline static void removeFlag(T& source, T flag) {
			source &= ~flag;
		}
	};
}