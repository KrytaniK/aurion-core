#include <gtest/gtest.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
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

TEST(StaticArrayTest, RangeFor)
{
    Array<u8, 5> test_arr = { 1, 2, 3, 4, 5 };

    // By value, visiting every element in order
    u8 expected = 1;
    for (auto value : test_arr)
        EXPECT_EQ(value, expected++);
    EXPECT_EQ(expected, 6);

    // By reference, writing through to the array
    for (auto& value : test_arr)
        value *= 2;
    EXPECT_EQ(test_arr[0], 2);
    EXPECT_EQ(test_arr[4], 10);

    // Through a const array
    const Array<u8, 5>& const_arr = test_arr;
    u32 sum = 0;
    for (auto value : const_arr)
        sum += value;
    EXPECT_EQ(sum, 30);
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

TEST(DynamicArrayTest, RangeFor)
{
    Vector<u8> test;

    // An empty vector never enters the loop
    for (auto value : test)
        ADD_FAILURE() << "Iterated an empty vector, got " << static_cast<u32>(value);

    // Push past the initial capacity, so iteration covers a resized buffer
    for (u8 i = 1; i <= 5; i++)
        test.PushBack(i);

    // Iterate by value
    u8 expected = 1;
    for (auto value : test)
        EXPECT_EQ(value, expected++);

    EXPECT_EQ(expected, 6);

    // Iterate by reference, writing values
    for (auto& value : test)
        value *= 2;

    EXPECT_EQ(test[0], 2);
    EXPECT_EQ(test[4], 10);

    // Iterate through a const vector
    const Vector<u8>& const_test = test;
    u32 sum = 0;
    for (auto value : const_test)
        sum += value;

    EXPECT_EQ(sum, 30);
}

TEST(DynamicArrayTest, OutOfBoundsAccess)
{
    Vector<u8> test;

    EXPECT_THROW({ auto val = test.At(5); }, std::runtime_error);
}

// ---------------------------------------------------------------------------
// ----- Slot Map ------------------------------------------------------------
// ---------------------------------------------------------------------------

TEST(SlotMapTest, InitialState)
{
    SlotMap<u32> test;

    EXPECT_TRUE(test.IsEmpty());
    EXPECT_FALSE(test.IsFull());
    EXPECT_EQ(test.Size(), 0);
    EXPECT_GT(test.Capacity(), 0);
    EXPECT_EQ(test.begin(), test.end());
}

TEST(SlotMapTest, SizeAndCapacity)
{
    SlotMap<u32> test;
    const size_t initial_capacity = test.Capacity();

    // Fill to the initial capacity
    for (size_t i = 0; i < initial_capacity; i++)
    {
        EXPECT_FALSE(test.IsFull());
        test.Insert(static_cast<u32>(i));
        EXPECT_EQ(test.Size(), i + 1);
    }

    EXPECT_FALSE(test.IsEmpty());
    EXPECT_TRUE(test.IsFull());
    EXPECT_EQ(test.Capacity(), initial_capacity);

    // One more forces growth
    test.Insert(0);
    EXPECT_GT(test.Capacity(), initial_capacity);
    EXPECT_EQ(test.Size(), initial_capacity + 1);
    EXPECT_FALSE(test.IsFull());
}

TEST(SlotMapTest, InsertCopy)
{
    SlotMap<std::string> test;
    const std::string value = "a string long enough to avoid small-string storage";

    auto key = test.Insert(value);

    // The source is left untouched
    EXPECT_EQ(value, "a string long enough to avoid small-string storage");
    EXPECT_EQ(test.At(key), value);
    EXPECT_EQ(test.Size(), 1);
}

TEST(SlotMapTest, InsertMove)
{
    // Move-only types can only go through the rvalue overload
    SlotMap<std::unique_ptr<u32>> test;
    auto value = std::make_unique<u32>(42);
    u32* raw = value.get();

    auto key = test.Insert(std::move(value));

    EXPECT_EQ(value, nullptr);
    EXPECT_EQ(test.At(key).get(), raw);
    EXPECT_EQ(*test.At(key), 42);
}

TEST(SlotMapTest, Emplace)
{
    SlotMap<std::pair<u32, u32>> test;

    // Forwarded constructor arguments
    auto key_a = test.Emplace(3, 4);
    EXPECT_EQ(test.At(key_a).first, 3);
    EXPECT_EQ(test.At(key_a).second, 4);

    // No arguments default-constructs
    auto key_b = test.Emplace();
    EXPECT_EQ(test.At(key_b).first, 0);
    EXPECT_EQ(test.At(key_b).second, 0);

    EXPECT_EQ(test.Size(), 2);
}

TEST(SlotMapTest, KeysAreUnique)
{
    SlotMap<u32> test;

    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(3);

    EXPECT_NE(key_a.index, key_b.index);
    EXPECT_NE(key_a.index, key_c.index);
    EXPECT_NE(key_b.index, key_c.index);

    // Issued keys never carry the default generation
    EXPECT_NE(key_a.generation, 0);
    EXPECT_NE(key_b.generation, 0);
    EXPECT_NE(key_c.generation, 0);
}

TEST(SlotMapTest, ElementAccess)
{
    SlotMap<u32> test;
    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(3);

    EXPECT_EQ(test.At(key_a), 1);
    EXPECT_EQ(test.At(key_b), 2);
    EXPECT_EQ(test.At(key_c), 3);

    EXPECT_EQ(test[key_a], 1);
    EXPECT_EQ(test[key_b], 2);
    EXPECT_EQ(test[key_c], 3);

    // Both accessors refer to the same element, and are writable
    EXPECT_EQ(&test.At(key_b), &test[key_b]);
    test.At(key_a) = 10;
    test[key_b] = 20;
    EXPECT_EQ(test[key_a], 10);
    EXPECT_EQ(test.At(key_b), 20);

    // Const access
    const SlotMap<u32>& const_test = test;
    EXPECT_EQ(const_test.At(key_a), 10);
    EXPECT_EQ(const_test[key_b], 20);
    EXPECT_EQ(&const_test.At(key_c), &const_test[key_c]);
}

TEST(SlotMapTest, Contains)
{
    SlotMap<u32> test;

    // Nothing is contained in an empty slot map, including the default key
    EXPECT_FALSE(test.Contains({}));
    EXPECT_FALSE(test.Contains({ .index = 0, .generation = 1 }));

    auto key = test.Insert(1);
    EXPECT_TRUE(test.Contains(key));

    // Default key
    EXPECT_FALSE(test.Contains({}));
    // Index out of range
    EXPECT_FALSE(test.Contains({ .index = key.index + 1, .generation = key.generation }));
    EXPECT_FALSE(test.Contains({ .index = UINT32_MAX, .generation = key.generation }));
    // Right index, wrong generation
    EXPECT_FALSE(test.Contains({ .index = key.index, .generation = key.generation + 1 }));

    // Erased key
    EXPECT_TRUE(test.Erase(key));
    EXPECT_FALSE(test.Contains(key));
}

TEST(SlotMapTest, EraseLast)
{
    SlotMap<u32> test;
    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(3);

    // The most recent element sits at the back of the dense storage,
    //  so nothing needs to move
    EXPECT_TRUE(test.Erase(key_c));

    EXPECT_EQ(test.Size(), 2);
    EXPECT_FALSE(test.Contains(key_c));
    EXPECT_EQ(test.At(key_a), 1);
    EXPECT_EQ(test.At(key_b), 2);
}

TEST(SlotMapTest, EraseKeepsOtherKeysValid)
{
    SlotMap<std::string> test;
    auto key_a = test.Insert("a");
    auto key_b = test.Insert("b");
    auto key_c = test.Insert("c");
    auto key_d = test.Insert("d");

    // Erasing from the front and middle moves the back element into the
    //  hole. Every surviving key must still resolve to its own value
    EXPECT_TRUE(test.Erase(key_a));
    EXPECT_EQ(test.Size(), 3);
    EXPECT_EQ(test.At(key_b), "b");
    EXPECT_EQ(test.At(key_c), "c");
    EXPECT_EQ(test.At(key_d), "d");

    EXPECT_TRUE(test.Erase(key_c));
    EXPECT_EQ(test.Size(), 2);
    EXPECT_EQ(test.At(key_b), "b");
    EXPECT_EQ(test.At(key_d), "d");

    // Down to nothing
    EXPECT_TRUE(test.Erase(key_d));
    EXPECT_TRUE(test.Erase(key_b));
    EXPECT_TRUE(test.IsEmpty());
    EXPECT_EQ(test.begin(), test.end());
}

TEST(SlotMapTest, EraseInvalidKey)
{
    SlotMap<u32> test;

    // Empty slot map
    EXPECT_FALSE(test.Erase({}));
    EXPECT_FALSE(test.Erase({ .index = 0, .generation = 1 }));

    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);

    // Default key
    EXPECT_FALSE(test.Erase({}));
    // Index out of range
    EXPECT_FALSE(test.Erase({ .index = 2, .generation = 1 }));
    EXPECT_FALSE(test.Erase({ .index = UINT32_MAX, .generation = 1 }));
    // Right index, wrong generation
    EXPECT_FALSE(test.Erase({ .index = key_a.index, .generation = key_a.generation + 1 }));

    // Double erase
    EXPECT_TRUE(test.Erase(key_a));
    EXPECT_FALSE(test.Erase(key_a));

    // Failed erases changed nothing
    EXPECT_EQ(test.Size(), 1);
    EXPECT_EQ(test.At(key_b), 2);
}

TEST(SlotMapTest, SlotReuse)
{
    SlotMap<u32> test;
    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(3);

    EXPECT_TRUE(test.Erase(key_b));
    auto key_d = test.Insert(4);

    // The freed slot is reused under a new generation
    EXPECT_EQ(key_d.index, key_b.index);
    EXPECT_NE(key_d.generation, key_b.generation);

    // So the stale key stays invalid, and can't touch the new occupant
    EXPECT_FALSE(test.Contains(key_b));
    EXPECT_FALSE(test.Erase(key_b));
    EXPECT_THROW({ auto val = test.At(key_b); }, std::runtime_error);

    EXPECT_EQ(test.At(key_a), 1);
    EXPECT_EQ(test.At(key_c), 3);
    EXPECT_EQ(test.At(key_d), 4);
    EXPECT_EQ(test.Size(), 3);
}

TEST(SlotMapTest, FreeListOrder)
{
    SlotMap<u32> test;
    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(3);

    EXPECT_TRUE(test.Erase(key_a));
    EXPECT_TRUE(test.Erase(key_c));
    EXPECT_TRUE(test.Erase(key_b));

    // Freed slots are handed back most-recently-erased first
    auto key_d = test.Insert(4);
    auto key_e = test.Insert(5);
    auto key_f = test.Insert(6);
    EXPECT_EQ(key_d.index, key_b.index);
    EXPECT_EQ(key_e.index, key_c.index);
    EXPECT_EQ(key_f.index, key_a.index);

    // Once the free list is exhausted, new slots are appended
    auto key_g = test.Insert(7);
    EXPECT_EQ(key_g.index, 3);

    EXPECT_EQ(test.At(key_d), 4);
    EXPECT_EQ(test.At(key_e), 5);
    EXPECT_EQ(test.At(key_f), 6);
    EXPECT_EQ(test.At(key_g), 7);
    EXPECT_EQ(test.Size(), 4);
}

TEST(SlotMapTest, RepeatedReuseInvalidatesEveryOldKey)
{
    SlotMap<u32> test;
    std::vector<SlotMap<u32>::Key> stale;

    auto key = test.Insert(0);
    for (u32 i = 1; i <= 16; i++)
    {
        EXPECT_TRUE(test.Erase(key));
        stale.push_back(key);
        key = test.Insert(i);
    }

    // One slot, reused throughout
    EXPECT_EQ(key.index, 0);
    EXPECT_EQ(test.Size(), 1);
    EXPECT_EQ(test.At(key), 16);

    for (const auto& old_key : stale)
        EXPECT_FALSE(test.Contains(old_key));
}

TEST(SlotMapTest, InvalidKeyAccess)
{
    SlotMap<u32> test;

    // Empty slot map
    EXPECT_THROW({ auto val = test.At({}); }, std::runtime_error);
    EXPECT_THROW({ auto val = test.At({ .index = 0, .generation = 1 }); }, std::runtime_error);

    auto key = test.Insert(1);

    // Default key
    EXPECT_THROW({ auto val = test.At({}); }, std::runtime_error);
    // Index out of range
    EXPECT_THROW({ auto val = test.At({ .index = 1, .generation = 1 }); }, std::runtime_error);
    EXPECT_THROW({ auto val = test.At({ .index = UINT32_MAX, .generation = 1 }); }, std::runtime_error);
    // Right index, wrong generation
    EXPECT_THROW({ auto val = test.At({ .index = key.index, .generation = key.generation + 1 }); }, std::runtime_error);

    // Const access throws the same way
    const SlotMap<u32>& const_test = test;
    EXPECT_THROW({ auto val = const_test.At({}); }, std::runtime_error);
    EXPECT_THROW({ auto val = const_test.At({ .index = 1, .generation = 1 }); }, std::runtime_error);

    // Erased key
    EXPECT_TRUE(test.Erase(key));
    EXPECT_THROW({ auto val = test.At(key); }, std::runtime_error);
    EXPECT_THROW({ auto val = const_test.At(key); }, std::runtime_error);
}

TEST(SlotMapTest, Clear)
{
    SlotMap<u32> test;
    std::vector<SlotMap<u32>::Key> keys;
    for (u32 i = 0; i < 5; i++)
        keys.push_back(test.Insert(i));

    // Leave a hole, so Clear sees both live and already-free slots
    EXPECT_TRUE(test.Erase(keys[2]));

    const size_t capacity = test.Capacity();
    test.Clear();

    EXPECT_TRUE(test.IsEmpty());
    EXPECT_EQ(test.Size(), 0);
    EXPECT_EQ(test.Capacity(), capacity);
    EXPECT_EQ(test.begin(), test.end());

    // Every key from before the clear is invalid
    for (const auto& key : keys)
    {
        EXPECT_FALSE(test.Contains(key));
        EXPECT_FALSE(test.Erase(key));
        EXPECT_THROW({ auto val = test.At(key); }, std::runtime_error);
    }
}

TEST(SlotMapTest, ReuseAfterClear)
{
    SlotMap<u32> test;
    std::vector<SlotMap<u32>::Key> old_keys;
    for (u32 i = 0; i < 4; i++)
        old_keys.push_back(test.Insert(i));

    test.Clear();

    // Existing slots are reused in order, each under a new generation
    std::vector<SlotMap<u32>::Key> new_keys;
    for (u32 i = 0; i < 4; i++)
    {
        new_keys.push_back(test.Insert(i + 10));
        EXPECT_EQ(new_keys[i].index, i);
        EXPECT_NE(new_keys[i].generation, old_keys[i].generation);
    }

    // Then new slots are appended once those run out
    auto appended = test.Insert(99);
    EXPECT_EQ(appended.index, 4);

    for (u32 i = 0; i < 4; i++)
    {
        EXPECT_EQ(test.At(new_keys[i]), i + 10);
        EXPECT_FALSE(test.Contains(old_keys[i]));
    }
    EXPECT_EQ(test.At(appended), 99);
    EXPECT_EQ(test.Size(), 5);
}

TEST(SlotMapTest, ClearEmpty)
{
    SlotMap<u32> test;

    // Clearing a slot map that never held anything is a no-op
    test.Clear();
    EXPECT_TRUE(test.IsEmpty());

    auto key = test.Insert(1);
    EXPECT_EQ(key.index, 0);
    EXPECT_EQ(test.At(key), 1);

    // Clearing twice in a row is equally harmless
    test.Clear();
    test.Clear();
    EXPECT_TRUE(test.IsEmpty());
    EXPECT_FALSE(test.Contains(key));

    key = test.Insert(2);
    EXPECT_EQ(test.At(key), 2);
    EXPECT_EQ(test.Size(), 1);
}

TEST(SlotMapTest, GrowthPreservesElements)
{
    SlotMap<std::string> test;
    std::vector<SlotMap<std::string>::Key> keys;

    // Far past the initial capacity, forcing several reallocations
    //  of a non-trivially-copyable type
    for (u32 i = 0; i < 100; i++)
        keys.push_back(test.Insert("element number " + std::to_string(i)));

    EXPECT_EQ(test.Size(), 100);
    EXPECT_GE(test.Capacity(), 100);

    for (u32 i = 0; i < 100; i++)
        EXPECT_EQ(test.At(keys[i]), "element number " + std::to_string(i));
}

TEST(SlotMapTest, InsertFromOwnElement)
{
    SlotMap<std::string> test;
    auto key = test.Insert("a string long enough to avoid small-string storage");

    // Each insert copies from an element of the slot map itself,
    //  including across the reallocations that would invalidate it
    for (u32 i = 0; i < 16; i++)
        test.Insert(test.At(key));

    EXPECT_EQ(test.Size(), 17);
    for (const auto& value : test)
        EXPECT_EQ(value, "a string long enough to avoid small-string storage");
}

TEST(SlotMapTest, MixedOperations)
{
    SlotMap<u32> test;
    std::vector<SlotMap<u32>::Key> keys;
    std::vector<u32> values;

    // Interleave inserts and erases, tracking what should remain
    for (u32 i = 0; i < 200; i++)
    {
        keys.push_back(test.Insert(i));
        values.push_back(i);

        // Drop an older element on every third step
        if (i % 3 == 2)
        {
            const size_t victim = (i * 7) % keys.size();
            EXPECT_TRUE(test.Erase(keys[victim]));
            EXPECT_FALSE(test.Contains(keys[victim]));

            keys[victim] = keys.back();
            values[victim] = values.back();
            keys.pop_back();
            values.pop_back();
        }
    }

    ASSERT_EQ(test.Size(), keys.size());
    for (size_t i = 0; i < keys.size(); i++)
    {
        ASSERT_TRUE(test.Contains(keys[i]));
        EXPECT_EQ(test.At(keys[i]), values[i]);
    }

    // Iteration visits exactly the surviving elements
    u64 expected_sum = 0;
    for (u32 value : values)
        expected_sum += value;

    u64 sum = 0;
    for (auto value : test)
        sum += value;
    EXPECT_EQ(sum, expected_sum);
}

TEST(SlotMapTest, ElementLifetime)
{
    // Every stored copy of the token holds a reference, so the use count
    //  is always one (the token itself) plus the number of live elements
    auto token = std::make_shared<u32>(0);

    {
        SlotMap<std::shared_ptr<u32>> test;

        // Enough elements to relocate the storage a few times
        std::vector<SlotMap<std::shared_ptr<u32>>::Key> keys;
        for (u32 i = 0; i < 10; i++)
            keys.push_back(test.Insert(token));
        EXPECT_EQ(token.use_count(), 11);

        // Move inserts and emplaces each add exactly one live element
        test.Insert(std::shared_ptr<u32>(token));
        test.Emplace(token);
        EXPECT_EQ(token.use_count(), 13);

        // Erasing from the middle and the back destroys one element each
        EXPECT_TRUE(test.Erase(keys[3]));
        EXPECT_EQ(token.use_count(), 12);
        EXPECT_TRUE(test.Erase(keys[9]));
        EXPECT_EQ(token.use_count(), 11);

        // Failed erases destroy nothing
        EXPECT_FALSE(test.Erase(keys[3]));
        EXPECT_EQ(token.use_count(), 11);

        // Clear destroys everything stored
        test.Clear();
        EXPECT_EQ(token.use_count(), 1);

        // Leave elements behind for the destructor
        test.Insert(token);
        test.Insert(token);
        EXPECT_EQ(token.use_count(), 3);
    }

    EXPECT_EQ(token.use_count(), 1);
}

TEST(SlotMapTest, RangeFor)
{
    SlotMap<u32> test;

    // An empty slot map never enters the loop
    for (auto value : test)
        ADD_FAILURE() << "Iterated an empty slot map, got " << value;

    auto key_a = test.Insert(1);
    auto key_b = test.Insert(2);
    auto key_c = test.Insert(4);

    // Erase from the middle, then reuse the freed slot
    EXPECT_TRUE(test.Erase(key_b));
    auto key_d = test.Insert(8);

    // Iterate by value (insertion order is not preserved)
    u32 count = 0;
    u32 sum = 0;
    for (auto value : test)
    {
        ++count;
        sum += value;
    }

    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 13);

    // Iterate by reference, updating each value
    for (auto& value : test)
        value *= 2;

    EXPECT_EQ(test.At(key_a), 2);
    EXPECT_EQ(test.At(key_c), 8);
    EXPECT_EQ(test.At(key_d), 16);

    // Iterate through const slot map
    sum = 0;
    const SlotMap<u32>& const_test = test;
    for (auto value : const_test)
        sum += value;

    EXPECT_EQ(sum, 26);

    // Cleared slot maps iterate nothing
    test.Clear();
    for (auto value : test)
        ADD_FAILURE() << "Iterated a cleared slot map, got " << value;
}