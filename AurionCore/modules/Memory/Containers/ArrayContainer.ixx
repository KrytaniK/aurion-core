module;

#include <assert.h>
#include <AurionExport.h>
#include <memory>

export module Aurion.Memory:Array;

import Aurion.Types;

import :LinearAllocator;

export namespace Aurion
{
  template<typename T, size_t N>
  class AURION_API Array : public IMemoryContainer
  {
    static_assert(std::is_trivially_constructible_v<T>, "Array type must be trivially constructible!");

  public:
    using Type = T;
    using Reference = Type&;
    using Pointer = Type*;

  public:
    T m_start[N];

    ~Array() override = default;

    [[nodiscard]] size_t Size() override;
    [[nodiscard]] size_t Capacity() override;
    [[nodiscard]] bool IsEmpty() override;

    [[nodiscard]] MemoryAllocation Data() override;

    [[nodiscard]] Reference At(const size_t& index);
    [[nodiscard]] Reference Front();
    [[nodiscard]] Reference Back();

    Reference operator[](size_t index);
    const Reference operator[](size_t index) const;
  };

  template<typename T, size_t N>
  size_t Array<T, N>::Size() { return N; }

  template<typename T, size_t N>
  size_t Array<T, N>::Capacity() { return N; }

  template<typename T, size_t N>
  bool Array<T, N>::IsEmpty() { return m_start == nullptr; }

  template<typename T, size_t N>
  MemoryAllocation Array<T, N>::Data() { return m_start; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::At(const size_t &index)
  {
    assert(index < N && "Index Out of Bounds!");
    return m_start[index];
  }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::Front() { return m_start[0]; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::Back() { return m_start[N - 1]; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::operator[](size_t index) { return m_start[index]; }

  template<typename T, size_t N>
  const typename Array<T, N>::Reference Array<T, N>::operator[](size_t index) const { return m_start[index]; }
}
