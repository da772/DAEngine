
namespace da {

	namespace memory
	{
		class Arena {
		public:
			Arena(const size_t& size);
			~Arena();
			void* Allocate(const size_t& size);

		private:
			unsigned char* m_base = 0;
			size_t m_ptr;
			size_t m_size;
			size_t m_allocated;
		};

	}
}