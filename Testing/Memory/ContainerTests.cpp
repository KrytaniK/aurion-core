#include <gtest/gtest.h>
#include <cstdint>

import Aurion.Types;
import Aurion.Memory;

using namespace Aurion;

// ---------------------------------------------------------------------------
// ----- Static Array --------------------------------------------------------
// ---------------------------------------------------------------------------

TEST(StaticArrayTest, ArrayIsNotEmpty)
{
    Array<u8, 5> test_arr{};

    EXPECT_FALSE(test_arr.IsEmpty());
    EXPECT_NE(test_arr.Data(), nullptr);
}

TEST(StaticArrayTest, ArrayInitializedWithCorrectSize)
{
    constexpr size_t size = 5;
    Array<u8, size> test_arr{};

    // Static array size/capacity should always be equal
    EXPECT_EQ(test_arr.Size(), size);
    EXPECT_EQ(test_arr.Capacity(), size);
    EXPECT_EQ(test_arr.Capacity(), test_arr.Size());
}

TEST(StaticArrayTest, ArrayElementAccess)
{
    Array<u8, 5> test_arr = { 1, 2, 3, 4, 5 };

    EXPECT_EQ(test_arr[0], 1);
    EXPECT_EQ(test_arr[1], 2);
    EXPECT_EQ(test_arr[2], 3);
    EXPECT_EQ(test_arr[3], 4);
    EXPECT_EQ(test_arr[4], 5);

    EXPECT_EQ(test_arr.At(0), 1);
    EXPECT_EQ(test_arr.At(1), 2);
    EXPECT_EQ(test_arr.At(2), 3);
    EXPECT_EQ(test_arr.At(3), 4);
    EXPECT_EQ(test_arr.At(4), 5);

    EXPECT_EQ(test_arr[0], test_arr.Front());
    EXPECT_EQ(test_arr[4], test_arr.Back());

    const u8 value = test_arr[1];
    EXPECT_EQ(value, test_arr[1]);
}

TEST(StaticArrayDeathTest, OutOfBoundsAccess)
{
    Array<u8, 5> test_arr{};

    EXPECT_DEATH({ auto val = test_arr.At(5); }, "Index Out of Bounds!");
}