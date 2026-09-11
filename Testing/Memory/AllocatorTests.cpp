#include <gtest/gtest.h>
#include <cstdint>

import Aurion.Types;
import Aurion.Memory;

using namespace Aurion;

namespace
{
	constexpr u16 kAlignments[] = { 1, 2, 4, 8, 16, 32, 64 };
}

// ---------------------------------------------------------------------------
// LinearAllocator
// ---------------------------------------------------------------------------

TEST(LinearAllocatorTest, AllocateProducesAlignedNonOverlappingBlocks)
{
	constexpr u32 kSize = 16;

	for (u16 alignment : kAlignments)
	{
		LinearAllocator allocator(256, alignment);

		void* first = allocator.Allocate(kSize, alignment);
		void* second = allocator.Allocate(kSize, alignment);

		ASSERT_NE(first, nullptr);
		ASSERT_NE(second, nullptr);
		EXPECT_EQ(reinterpret_cast<uintptr_t>(first) % alignment, 0u);
		EXPECT_EQ(reinterpret_cast<uintptr_t>(second) % alignment, 0u);
		EXPECT_GE(reinterpret_cast<uintptr_t>(second), reinterpret_cast<uintptr_t>(first) + kSize);
	}
}

TEST(LinearAllocatorTest, FreeIsNoOpAndDoesNotReclaimMemory)
{
	LinearAllocator allocator(256, 8);

	void* first = allocator.Allocate(16, 8);
	void* second = allocator.Allocate(16, 8);

	allocator.Free(first);

	void* third = allocator.Allocate(16, 8);

	EXPECT_NE(third, first);
	EXPECT_GE(reinterpret_cast<uintptr_t>(third), reinterpret_cast<uintptr_t>(second) + 16);
}

TEST(LinearAllocatorTest, ResetReclaimsAllMemory)
{
	LinearAllocator allocator(256, 8);

	void* first = allocator.Allocate(16, 8);
	(void)allocator.Allocate(16, 8);

	allocator.Reset();

	void* afterReset = allocator.Allocate(16, 8);

	EXPECT_EQ(afterReset, first);
}

// ---------------------------------------------------------------------------
// StackAllocator
// ---------------------------------------------------------------------------

TEST(StackAllocatorTest, AllocateProducesAlignedNonOverlappingBlocks)
{
	constexpr u32 kSize = 16;

	for (u16 alignment : kAlignments)
	{
		StackAllocator allocator(256, alignment);

		void* first = allocator.Allocate(kSize, alignment);
		void* second = allocator.Allocate(kSize, alignment);

		ASSERT_NE(first, nullptr);
		ASSERT_NE(second, nullptr);
		EXPECT_EQ(reinterpret_cast<uintptr_t>(first) % alignment, 0u);
		EXPECT_EQ(reinterpret_cast<uintptr_t>(second) % alignment, 0u);
		EXPECT_GE(reinterpret_cast<uintptr_t>(second), reinterpret_cast<uintptr_t>(first) + kSize);
	}
}

TEST(StackAllocatorTest, FreeIsNoOpAndDoesNotReclaimMemory)
{
	StackAllocator allocator(256, 8);

	void* first = allocator.Allocate(16, 8);
	void* second = allocator.Allocate(16, 8);

	allocator.Free(first);

	void* third = allocator.Allocate(16, 8);

	EXPECT_NE(third, first);
	EXPECT_GE(reinterpret_cast<uintptr_t>(third), reinterpret_cast<uintptr_t>(second) + 16);
}

TEST(StackAllocatorTest, ResetReclaimsAllMemory)
{
	StackAllocator allocator(256, 8);

	void* first = allocator.Allocate(16, 8);
	(void)allocator.Allocate(16, 8);

	allocator.Reset();

	void* afterReset = allocator.Allocate(16, 8);

	EXPECT_EQ(afterReset, first);
}

TEST(StackAllocatorTest, MarkerRewindsToPriorState)
{
	StackAllocator allocator(256, 8);

	(void)allocator.Allocate(16, 8);

	MemoryAllocationMarker marker = allocator.GetMarker();

	void* afterMarker = allocator.Allocate(16, 8);
	(void)allocator.Allocate(16, 8);

	allocator.FreeToMarker(marker);

	void* reallocated = allocator.Allocate(16, 8);

	EXPECT_EQ(reallocated, afterMarker);
}

// ---------------------------------------------------------------------------
// PoolAllocator
// ---------------------------------------------------------------------------

TEST(PoolAllocatorTest, ConstructorPadsChunkSizeToMinimum)
{
	PoolAllocator pool(3, 1, 1);

	void* first = pool.Allocate();
	void* second = pool.Allocate();

	ASSERT_NE(first, nullptr);
	ASSERT_NE(second, nullptr);
	EXPECT_EQ(reinterpret_cast<uintptr_t>(second) - reinterpret_cast<uintptr_t>(first), 4u);
}

TEST(PoolAllocatorTest, AllocateExhaustsPoolThenReturnsNull)
{
	constexpr u32 kChunkSize = 8;
	PoolAllocator pool(3, kChunkSize, 64);

	void* a = pool.Allocate();
	void* b = pool.Allocate(kChunkSize, 64);
	void* c = pool.Allocate();

	ASSERT_NE(a, nullptr);
	ASSERT_NE(b, nullptr);
	ASSERT_NE(c, nullptr);
	EXPECT_NE(a, b);
	EXPECT_NE(b, c);
	EXPECT_NE(a, c);

	EXPECT_EQ(pool.Allocate(), nullptr);
}

TEST(PoolAllocatorTest, FreeIgnoresOutOfBoundsPointer)
{
	PoolAllocator pool(2, 8, 1);

	int outOfBounds = 0;
	pool.Free(&outOfBounds);

	EXPECT_NE(pool.Allocate(), nullptr);
	EXPECT_NE(pool.Allocate(), nullptr);
	EXPECT_EQ(pool.Allocate(), nullptr);
}

TEST(PoolAllocatorTest, FreedChunksAreAllReusableInLifoOrder)
{
	PoolAllocator pool(3, 8, 1);

	void* a = pool.Allocate();
	void* b = pool.Allocate();
	void* c = pool.Allocate();

	pool.Free(a);
	pool.Free(b);

	void* reusedFirst = pool.Allocate();
	void* reusedSecond = pool.Allocate();

	EXPECT_EQ(reusedFirst, b);
	EXPECT_EQ(reusedSecond, a);
	EXPECT_EQ(pool.Allocate(), nullptr);
}

TEST(PoolAllocatorTest, ResetRestoresFullCapacity)
{
	PoolAllocator pool(3, 8, 1);

	void* first = pool.Allocate();
	(void)pool.Allocate();
	(void)pool.Allocate();

	ASSERT_EQ(pool.Allocate(), nullptr);

	pool.Reset();

	void* afterReset = pool.Allocate();

	EXPECT_EQ(afterReset, first);
	EXPECT_NE(pool.Allocate(), nullptr);
	EXPECT_NE(pool.Allocate(), nullptr);
	EXPECT_EQ(pool.Allocate(), nullptr);
}
