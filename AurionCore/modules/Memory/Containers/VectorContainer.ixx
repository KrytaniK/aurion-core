module;

#include <AurionExport.h>
#include <cassert>
#include <cstring>
#include <type_traits>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

export module Aurion.Memory:Vector;

import :Interface;
import :LinearAllocator;

export namespace Aurion
{
  constexpr size_t k_vector_min_capacity = 2;

  template <typename T>
  class AURION_API Vector
  {
    static_assert(std::is_default_constructible_v<T>, "Value Type must be default constructible!");

  public:
    explicit Vector(size_t capacity = k_vector_min_capacity, IMemoryAllocator* allocator = nullptr);

    ~Vector();

    // Copying would leave two vectors owning the same buffer
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    [[nodiscard]] size_t Size() const;
    [[nodiscard]] size_t Capacity() const;
    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] bool IsFull() const;

    [[nodiscard]] T* Data();

    [[nodiscard]] T& At(const size_t& index);
    [[nodiscard]] const T& At(const size_t& index) const;
    [[nodiscard]] T& Front();
    [[nodiscard]] T& Back();

    T& Push(T& value);
    T& Push(T&& value);

    T& PushBack(const T& value);
    T& PushBack(T&& value);

    template <typename... Args>
    T& Emplace(size_t index, Args&&... args);

    template <typename... Args>
    T& EmplaceBack(Args&&... args);

    void Pop();

    void PopBack();

    void Clear();

    // Lowercase by necessity: range-based for looks these names up
    [[nodiscard]] T* begin();
    [[nodiscard]] T* end();
    [[nodiscard]] const T* begin() const;
    [[nodiscard]] const T* end() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

  private:
    // Allocates a buffer of twice the capacity. 'owner' receives the allocator
    //  the buffer came from, or nullptr if it came from the heap
    T* Resize(IMemoryAllocator*& owner);

    // Releases the current buffer with whatever owned it, then takes
    //  ownership of the new allocation, which may come from the Vector's
    //  own internal allocator, or heap memory with calloc().
    void Adopt(T* alloc, IMemoryAllocator* owner);

    // Moves 'count' elements from src into raw memory at dst, leaving src as
    //  raw memory. Ranges may overlap.
    void Relocate(T* dst, T* src, size_t count);

  private:
    IMemoryAllocator* m_allocator;
    T* m_data;
    size_t m_size;
    size_t m_capacity;
  };

  template <typename T>
  Vector<T>::Vector(size_t capacity, IMemoryAllocator* allocator)
    : m_allocator(allocator), m_data(nullptr), m_size(0), m_capacity(std::max(capacity, k_vector_min_capacity))
  {
    if (m_allocator != nullptr)
      m_data = static_cast<T*>(m_allocator->Allocate(sizeof(T) * m_capacity, alignof(T)));

    // If the allocator didn't contain a large-enough block, or wasn't available,
    //  fallback to heap allocation
    if (m_data == nullptr)
    {
      m_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));
      m_allocator = nullptr;
    }
  }

  template <typename T>
  Vector<T>::~Vector()
  {
    if (m_data == nullptr) return;

    // Destroy all live elements
    for (size_t i = 0; i < m_size; ++i)
      m_data[i].~T();

    if (m_allocator)
      m_allocator->Free(m_data);
    else
      free(m_data);
  }

  template <typename T>
  size_t Vector<T>::Size() const
  {
    return m_size;
  }

  template <typename T>
  size_t Vector<T>::Capacity() const
  {
    return m_capacity;
  }

  template <typename T>
  bool Vector<T>::IsEmpty() const
  {
    return m_size == 0;
  }

  template <typename T>
  bool Vector<T>::IsFull() const
  {
    return m_size == m_capacity;
  }

  template <typename T>
  T* Vector<T>::Data()
  {
    return m_data;
  }

  template <typename T>
  T& Vector<T>::At(const size_t& index)
  {
    if (index >= m_size)
      throw std::runtime_error("[Vector] Index out of bounds!");

    return m_data[index];
  }

  template <typename T>
  const T& Vector<T>::At(const size_t& index) const
  {
    if (index >= m_size)
      throw std::runtime_error("[Vector] Index out of bounds!");

    return m_data[index];
  }

  template <typename T>
  T& Vector<T>::Front()
  {
    return m_data[0];
  }

  template <typename T>
  T& Vector<T>::Back()
  {
    return m_data[m_size == 0 ? 0 : m_size - 1];
  }

  template <typename T>
  T& Vector<T>::Push(T& value)
  {
    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Copy the object into the new memory first, since 'value'
      //  may refer to an element of this vector
      new (alloc) T(value);

      // Then move all data to new memory to the adjusted position
      Relocate(alloc + 1, m_data, m_size);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else
    {
      // Copy the object before shifting, since 'value' may refer
      //  to an element of this vector
      T temp(value);

      // Shift all elements to the right
      Relocate(m_data + 1, m_data, m_size);

      // Then, move the copy into owned memory
      new (m_data) T(static_cast<T&&>(temp));
    }

    m_size++;

    // and return a reference to it
    return m_data[0];
  }

  template <typename T>
  T& Vector<T>::Push(T&& value)
  {
    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Move the object into the new memory first, since 'value'
      //  may refer to an element of this vector
      new (alloc) T(static_cast<T&&>(value));

      // Then move all data to new memory to the adjusted position
      Relocate(alloc + 1, m_data, m_size);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else
    {
      // Move the object out before shifting, since 'value' may refer
      //  to an element of this vector
      T temp(static_cast<T&&>(value));

      // Shift all elements to the right
      Relocate(m_data + 1, m_data, m_size);

      // Then, move the object into owned memory
      new (m_data) T(static_cast<T&&>(temp));
    }

    m_size++;

    // and return a reference to it
    return m_data[0];
  }

  template <typename T>
  T& Vector<T>::PushBack(const T& value)
  {
    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Copy the object into the new memory first, since 'value'
      //  may refer to an element of this vector
      new (alloc + m_size) T(value);

      // Then move all data to new memory
      Relocate(alloc, m_data, m_size);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else // Copy the object into owned memory
      new (m_data + m_size) T(value);

    return m_data[m_size++];
  }

  template <typename T>
  T& Vector<T>::PushBack(T&& value)
  {
    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Move the object into the new memory first, since 'value'
      //  may refer to an element of this vector
      new (alloc + m_size) T(static_cast<T&&>(value));

      // Then move all data to new memory
      Relocate(alloc, m_data, m_size);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else // Move the object into owned memory
      new (m_data + m_size) T(static_cast<T&&>(value));

    return m_data[m_size++];
  }

  template <typename T>
  template <typename... Args>
  T& Vector<T>::Emplace(size_t index, Args&&... args)
  {
    assert(index <= m_size && "Index Out of Bounds!");

    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Construct the new object in the new memory first, since 'args'
      //  may refer to an element of this vector
      new (alloc + index) T(static_cast<Args&&>(args)...);

      // Move all elements up to the desired insertion index
      Relocate(alloc, m_data, index);

      // Shift elements to the right to make room
      Relocate(alloc + index + 1, m_data + index, m_size - index);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else
    {
      // Construct the new object before shifting, since 'args' may refer
      //  to an element of this vector
      T temp(static_cast<Args&&>(args)...);

      // Shift elements to the right to make room
      Relocate(m_data + index + 1, m_data + index, m_size - index);

      // Then, move the object into the insertion index
      new (m_data + index) T(static_cast<T&&>(temp));
    }

    m_size++;

    return m_data[index];
  }

  template <typename T>
  template <typename... Args>
  T& Vector<T>::EmplaceBack(Args&&... args)
  {
    if (IsFull()) // Resize if full
    {
      IMemoryAllocator* owner = nullptr;
      T* alloc = Resize(owner);

      // Construct the new object in the new memory first, since 'args'
      //  may refer to an element of this vector
      new (alloc + m_size) T(static_cast<Args&&>(args)...);

      // Move all elements into the new allocation
      Relocate(alloc, m_data, m_size);

      // Free the initial data and update to the new allocation
      Adopt(alloc, owner);
    }
    else // Construct the new object in-place at the end
      new (m_data + m_size) T(static_cast<Args&&>(args)...);

    return m_data[m_size++];
  }

  template <typename T>
  void Vector<T>::Pop()
  {
    if (m_size == 0) return;

    // Destroy the first element
    m_data[0].~T();

    // Shift each successive element left by one. The vacated
    //  last slot is left as raw memory
    Relocate(m_data, m_data + 1, m_size - 1);
    --m_size;
  }

  template <typename T>
  void Vector<T>::PopBack()
  {
    if (m_size == 0) return;

    // Call destructor on last element
    m_data[--m_size].~T();
  }

  template <typename T>
  void Vector<T>::Clear()
  {
    // Destroy all elements in 'reverse' order
    for (size_t i = 0; i < m_size; i++)
      m_data[m_size - 1 - i].~T();

    m_size = 0;
  }

  template <typename T>
  T* Vector<T>::begin()
  {
    return m_data;
  }

  template <typename T>
  T* Vector<T>::end()
  {
    return m_data + m_size;
  }

  template <typename T>
  const T* Vector<T>::begin() const
  {
    return m_data;
  }

  template <typename T>
  const T* Vector<T>::end() const
  {
    return m_data + m_size;
  }

  template <typename T>
  T& Vector<T>::operator[](size_t index)
  {
    return m_data[index];
  }

  template <typename T>
  const T& Vector<T>::operator[](size_t index) const
  {
    return m_data[index];
  }

  template <typename T>
  T* Vector<T>::Resize(IMemoryAllocator*& owner)
  {
    // TODO: Integrate logarithmic scaling. For now, default to 2x capacity in all cases
    const size_t new_capacity = m_capacity * 2;
    T* alloc = nullptr;

    // Request a larger block from the allocator, if present
    alloc = m_allocator
              ? static_cast<T*>(m_allocator->Allocate(sizeof(T) * new_capacity, alignof(T)))
              : nullptr;

    // If the allocator didn't contain a large-enough block, or wasn't available,
    //  fallback to heap allocation. The allocator can no longer be used once
    //  this buffer is adopted
    owner = alloc ? m_allocator : nullptr;
    if (!alloc)
      alloc = static_cast<T*>(calloc(new_capacity, sizeof(T)));

    m_capacity = new_capacity;

    return alloc;
  }

  template <typename T>
  void Vector<T>::Adopt(T* alloc, IMemoryAllocator* owner)
  {
    // Free the current data buffer
    if (m_allocator) m_allocator->Free(m_data);
    else free(m_data);

    // Then update it to be the provided allocation.
    // The allocator gets reassigned to either itself,
    //  or nullptr when there is no more room.
    m_data = alloc;
    m_allocator = owner;
  }

  template <typename T>
  void Vector<T>::Relocate(T* dst, T* src, size_t count)
  {
    // Trivial types can be moved bytewise, no constructors needed
    if constexpr (std::is_trivially_copyable_v<T>)
      memmove(dst, src, count * sizeof(T));
    else if (dst < src) // Shifting left, so iterate forwards
    {
      for (size_t i = 0; i < count; i++)
      {
        new (dst + i) T(static_cast<T&&>(src[i]));
        src[i].~T();
      }
    }
    else if (dst > src) // Shifting right, so iterate backwards
    {
      for (size_t i = count; i > 0; i--)
      {
        new (dst + i - 1) T(static_cast<T&&>(src[i - 1]));
        src[i - 1].~T();
      }
    }
  }
}
