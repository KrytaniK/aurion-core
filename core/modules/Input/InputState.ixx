module;

#include <macros/AurionExport.h>

#include <cstdint>

export module Aurion.Input:State;

export namespace Aurion
{
	struct AURION_API InputStateBlock
	{
		uint8_t offset = 0; // Offset (in bytes) from the start of the parent state block
		uint8_t total_size = 0; // Total size (in bytes) of this state block
	};

	class AURION_API InputState
	{
	private:
		inline static const InputStateBlock s_invalid_block{0,0};

	public:
		InputState(const uint8_t& total_size = UINT8_MAX); // Default to max size of 255 bytes
		~InputState();

		InputStateBlock GetStateBlock(const uint8_t& offset, const uint8_t& size);

		void Write(const void* data, const uint8_t& offset, const uint8_t& size);

		bool IsValidBlock(const InputStateBlock& block) const;

	private:
		uint8_t* m_start_ptr = nullptr; // Pointer to the start of the state memory block
		uint8_t m_total_size = 0; // Total size (in bytes) of the state memory block
	};
}