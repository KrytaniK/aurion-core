#include <gtest/gtest.h>
#include <cstdint>
#include <vector>

import Aurion.Types;
import Aurion.Memory;

using namespace Aurion;

// ---------------------------------------------------------------------------
// ----- Static Array --------------------------------------------------------
// ---------------------------------------------------------------------------

TEST(StaticArrayTest, IsNotEmpty)
{
    Array<u8, 5> test_arr{};

    EXPECT_FALSE(test_arr.IsEmpty());
    EXPECT_NE(test_arr.Data(), nullptr);
}

TEST(StaticArrayTest, InitializedWithCorrectSize)
{
    constexpr size_t size = 5;
    Array<u8, size> test_arr{};

    // Static array size/capacity should always be equal
    EXPECT_EQ(test_arr.Size(), size);
    EXPECT_EQ(test_arr.Capacity(), size);
    EXPECT_EQ(test_arr.Capacity(), test_arr.Size());
}

TEST(StaticArrayTest, ElementAccess)
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
}

TEST(StaticArrayDeathTest, OutOfBoundsAccess)
{
    Array<u8, 5> test_arr{};

    EXPECT_DEATH({ auto val = test_arr.At(5); }, "Index Out of Bounds!");
}

// ---------------------------------------------------------------------------
// ----- Dynamic Array (Vector) ----------------------------------------------
// ---------------------------------------------------------------------------

TEST(DynamicArrayTest, IsEmpty)
{
    Vector<u8> test;

    EXPECT_NE(test.Data(), nullptr);
    EXPECT_TRUE(test.IsEmpty());
}

TEST(DynamicArrayTest, IsFull)
{
    Vector<u8> test;

    test.Push(10);
    test.Push(5);
    EXPECT_EQ(test.Size(), test.Capacity());
    EXPECT_TRUE(test.IsFull());
}

TEST(DynamicArrayTest, InitializedWithCorrectSizeAndCapacity)
{
    constexpr size_t capacity = 5;
    Vector<u8> test(capacity);

    EXPECT_EQ(test.Size(), 0);
    EXPECT_EQ(test.Capacity(), capacity);
}

TEST(DynamicArrayTest, ElementAccess)
{
    Vector<u8> test;
    test.PushBack(1);
    test.PushBack(2);
    test.PushBack(3);
    test.PushBack(4);
    test.PushBack(5);

    u8 val0 = test[0];
    u8 val1 = test[1];
    u8 val2 = test[2];
    u8 val3 = test[3];
    u8 val4 = test[4];

    EXPECT_EQ(val0, 1);
    EXPECT_EQ(val1, 2);
    EXPECT_EQ(val2, 3);
    EXPECT_EQ(val3, 4);
    EXPECT_EQ(val4, 5);

    EXPECT_EQ(test[0], test.Front());
    EXPECT_EQ(test[4], test.Back());
}

TEST(DynamicArrayTest, PushPop)
{
    Vector<u8> test{};
    u8 val = 0;

    test.Push(5);
    // 5 at front: { 5 }
    val = test[0];
    EXPECT_EQ(val, 5);
    EXPECT_EQ(test.At(0), val);
    EXPECT_EQ(test.Front(), val);
    test.Push(7);
    // 7 at front: { 7, 5 }
    val = test[0];
    EXPECT_EQ(val, 7);
    EXPECT_EQ(test.At(0), val);
    EXPECT_EQ(test.Front(), val);
    test.PushBack(8);
    // 8 at back: { 7, 5, 8 }
    val = test[2];
    EXPECT_EQ(val, 8);
    EXPECT_EQ(test.At(2), val);
    EXPECT_EQ(test.Back(), val);
    EXPECT_EQ(test.Size(), 3);
    test.Pop();
    // 7 Removed: { 5, 8 }
    EXPECT_EQ(test[0], 5);
    EXPECT_EQ(test.At(0), 5);
    EXPECT_EQ(test[1], 8);
    EXPECT_EQ(test.At(1), 8);
    EXPECT_EQ(test.Size(), 2);
    test.PopBack();
    // 8 Removed: { 5 }
    EXPECT_EQ(test.Size(), 1);
    EXPECT_EQ(test.At(0), 5);
    EXPECT_EQ(test[0], 5);
}

TEST(DynamicArrayTest, Emplace)
{
    Vector<u8> test;

    test.EmplaceBack(10);
    test.EmplaceBack(9);
    test.EmplaceBack(8);
    test.EmplaceBack(7);

    EXPECT_EQ(test[0], 10);
    EXPECT_EQ(test[1], 9);
    EXPECT_EQ(test[2], 8);
    EXPECT_EQ(test[3], 7);

    test.Emplace(1, 4);
    EXPECT_EQ(test[0], 10);
    EXPECT_EQ(test[1], 4);
    EXPECT_EQ(test[2], 9);
    EXPECT_EQ(test[3], 8);
    EXPECT_EQ(test[4], 7);
}

TEST(DynamicArrayTest, Clear)
{
    Vector<u8> test;

    test.Push(10);
    test.Push(10);
    test.Push(10);

    EXPECT_FALSE(test.IsEmpty());

    test.Clear();

    EXPECT_TRUE(test.IsEmpty());
}

TEST(DynamicArrayDeathTest, OutOfBoundsAccess)
{
    Vector<u8> test;

    EXPECT_DEATH({ auto val = test.At(5); }, "Index Out of Bounds!");
}