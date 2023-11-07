#pragma once

#ifdef DA_GRAPHICS_BGFX

#ifndef INVALID_HANDLE
	#define INVALID_HANDLE (uint16_t)UINT16_MAX
#endif

#include <bgfx/bgfx.h>

namespace da::platform::bgfx {


	template<typename T>
	class CBgfxUniform {
	public:
		inline CBgfxUniform() {};
		inline CBgfxUniform(uint8_t type, const std::string& name, T data) : m_dataType(type), m_name(name), m_data(data)
		{
			m_handle = ::bgfx::createUniform(m_name.c_str(), (::bgfx::UniformType::Enum)m_dataType).idx;
			LOG_ASSERT((bool)(m_handle != INVALID_HANDLE), ELogChannel::Graphics, "Failed to create uniform");
			setData(m_data);
		}

		inline CBgfxUniform(uint8_t type, const std::string& name) : m_dataType(type), m_name(name)
		{
			m_handle = ::bgfx::createUniform(m_name.c_str(), (::bgfx::UniformType::Enum)m_dataType).idx;
			LOG_ASSERT((bool)(m_handle != INVALID_HANDLE), ELogChannel::Graphics, "Failed to create uniform");
		}


		inline void setData(const T& data) {
			m_data = data;

			if (m_handle == INVALID_HANDLE) return;

			::bgfx::UniformHandle handle{ m_handle };

			::bgfx::setUniform(handle, (const void*)getData());
		}

		inline const T* getData() const
		{
			return &m_data;
		}

		inline const uint8_t getDataType() const
		{
			return m_dataType;
		};

		inline const uint16_t getHandle()  const
		{
			return m_handle;
		};

		inline const std::string& getName()const
		{
			return m_name;
		}
		
		inline void destroy()
		{
			if (m_handle == INVALID_HANDLE) return;

			::bgfx::UniformHandle handle = { m_handle };
			::bgfx::destroy(handle);
		}


	private:
		uint8_t m_dataType;
		T m_data;
		std::string m_name;
		uint16_t m_handle = INVALID_HANDLE;
	};
}


#endif
