module;

#include <AurionExport.h>
#include <cassert>
#include <cstring>
#include <concepts>
#include <cstdlib>

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

    [[nodiscard]] size_t Size();
    [[nodiscard]] size_t Capacity();
    [[nodiscard]] bool IsEmpty();
    [[nodiscard]] bool IsFull();

    [[nodiscard]] T* Data();

    [[nodiscard]] T& At(const size_t& index);
    [[nodiscard]] T& Front();
    [[nodiscard]] T& Back();

    T& Push(T& value);
    T& Push(T&& value);

    T& PushBack(T& value);
    T& PushBack(T&& value);

    template <typename... Args>
    T& Emplace(size_t index, Args&&... args);

    template <typename... Args>
    T& EmplaceBack(Args&&... args);

    void Pop();

    void PopBack();

    void Clear();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

  private:
    T* Resize();

  private:
    IMemoryAllocator* m_allocator;
    T* m_data;
    size_t m_size;
    size_t m_capacity;
  };

  template <typename T>
  Vector<T>::Vector(size_t capacity, IMemoryAllocator* allocator)
    : m_allocator(allocator), m_data(nullptr), m_size(0), m_capacity(capacity)
  {
    if (allocator != nullptr)
      m_data = static_cast<T*>(allocator->Allocate(sizeof(T) * m_capacity, alignof(T)));
    else
      m_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));
  }

  template <typename T>
  Vector<T>::~Vector()
  {
    if (m_data == nullptr) return;

    if (m_allocator)
      m_allocator->Free(m_data);
    else
      free(m_data);
  }

  template <typename T>
  size_t Vector<T>::Size()
  {
    return m_size;
  }

  template <typename T>
  size_t Vector<T>::Capacity()
  {
    return m_capacity;
  }

  template <typename T>
  bool Vector<T>::IsEmpty()
  {
    return m_size == 0;
  }

  template <typename T>
  bool Vector<T>::IsFull()
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
    assert(index < m_capacity && "Index Out of Bounds!");
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
      T* alloc = Resize();

      // Then move all data to new memory to the adjusted position
      for (size_t i = 0; i < m_capacity; i++)
        alloc[i + 1] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }
    else // Shift all elements otherwise
    {
      for (size_t i = 0; i < m_size; i++)
        m_data[i + 1] = static_cast<T&&>(m_data[i]);
    }

    // Then, copy object into owned memory
    m_data[0] = value;
    m_size++;

    // and return a reference to it
    return m_data[0];
  }

  template <typename T>
  T& Vector<T>::Push(T&& value)
  {
    if (IsFull()) // Resize if full
    {
      T* alloc = Resize();

      // Then move all data to new memory to the adjusted position
      for (size_t i = 0; i < m_capacity; i++)
        alloc[i + 1] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }
    else // Shift all elements otherwise
    {
      for (size_t i = 0; i < m_size; i++)
        m_data[i + 1] = static_cast<T&&>(m_data[i]);
    }

    // Then, move object into owned memory
    m_data[0] = static_cast<T&&>(value);
    m_size++;

    // and return a reference to it
    return m_data[0];
  }

  template <typename T>
  T& Vector<T>::PushBack(T& value)
  {
    if (IsFull()) // Resize if full
    {
      T* alloc = Resize();

      // Then move all data to new memory
      for (size_t i = 0; i < m_capacity; i++)
        alloc[i] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }

    // Copy object into owned memory
    m_data[m_size++] = value;

    return m_data[m_size - 1];
  }

  template <typename T>
  T& Vector<T>::PushBack(T&& value)
  {
    if (IsFull()) // Resize if full
    {
      T* alloc = Resize();

      // Then move all data to new memory
      for (size_t i = 0; i < m_capacity; i++)
        alloc[i] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }

    // Move the object into owned memory
    m_data[m_size++] = static_cast<T&&>(value);

    return m_data[m_size - 1];
  }

  template <typename T>
  template <typename... Args>
  T& Vector<T>::Emplace(size_t index, Args&&... args)
  {
    if (IsFull()) // Resize if full
    {
      T* alloc = Resize();

      // Move all elements up to the desired insertion index
      for (size_t i = 0; i < index; i++)
        alloc[i] = static_cast<T&&>(m_data[i]);

      // Shift remaining data right by one index
      for (size_t i = index; i < m_size; i++)
        alloc[i + 1] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }

    // Construct the new object in-place at the insertion index
    T* value = new(m_data + index) T(static_cast<Args&&>(args)...);
    m_size++;

    return *value;
  }

  template <typename T>
  template <typename... Args>
  T& Vector<T>::EmplaceBack(Args&&... args)
  {
    if (IsFull()) // Resize if full
    {
      T* alloc = Resize();

      // Copy all elements into the new allocation
      for (size_t i = 0; i < m_size; i++)
        alloc[i] = static_cast<T&&>(m_data[i]);

      // Free the initial data
      if (m_allocator) m_allocator->Free(m_data);
      else delete[] m_data;

      // Then update to new allocation
      m_data = alloc;
    }

    // Construct the new object in-place at the end
    T* value = new(m_data + (m_size++)) T(static_cast<Args&&>(args)...);
    return *value;
  }

  template <typename T>
  void Vector<T>::Pop()
  {
    if (m_size == 0) return;

    // Destroy the first element
    m_data[0].~T();

    // Shift each successive element left by one
    for (size_t i = 0; i < m_size - 1; i++)
      m_data[i] = static_cast<T&&>(m_data[i + 1]);

    // Destroy the 'dead' last element
    m_data[--m_size].~T();
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
  T* Vector<T>::Resize()
  {
    // TODO: Integrate logarithmic scaling. For now, default to 2x capacity in all cases
    const size_t new_capacity = m_capacity * 2;
    T* alloc = nullptr;

    // Request a larger block from the allocator, if present
    alloc = m_allocator
              ? static_cast<T*>(m_allocator->Allocate(sizeof(T) * new_capacity, alignof(T)))
              : nullptr;

    // If the allocator didn't contain a large-enough block, or wasn't available,
    //  fallback to heap allocation
    if (!alloc)
      alloc = new T[new_capacity];

    m_capacity = new_capacity;

    return alloc;
  }
}
