namespace da::core::memory
{
	class CArena {
	public:
		CArena(const size_t& size);
		~CArena();
		void* Allocate(const size_t& size);
		void Free();

	private:
		unsigned char* m_base = 0;
		size_t m_ptr;
		size_t m_size;
		size_t m_allocated;
	};

}