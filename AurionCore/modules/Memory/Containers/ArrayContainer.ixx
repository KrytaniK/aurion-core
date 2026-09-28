module;

#include <assert.h>
#include <AurionExport.h>
#include <memory>

export module Aurion.Memory:Array;

import Aurion.Types;

export namespace Aurion
{
  template<typename T, size_t N>
  class AURION_API Array
  {
    static_assert(std::is_trivially_constructible_v<T>, "Array type must be trivially constructible!");

  public:
    using Type = T;
    using Reference = Type&;
    using Pointer = Type*;

  public:
    T data[N];

    ~Array() = default;

    [[nodiscard]] size_t Size();
    [[nodiscard]] size_t Capacity();
    [[nodiscard]] bool IsEmpty();

    [[nodiscard]] Pointer Data();

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
  bool Array<T, N>::IsEmpty() { return data == nullptr; }

  template<typename T, size_t N>
  Array<T, N>::Pointer Array<T, N>::Data() { return data; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::At(const size_t &index)
  {
    assert(index < N && "Index Out of Bounds!");
    return data[index];
  }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::Front() { return data[0]; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::Back() { return data[N - 1]; }

  template<typename T, size_t N>
  typename Array<T, N>::Reference Array<T, N>::operator[](size_t index) { return data[index]; }

  template<typename T, size_t N>
  const typename Array<T, N>::Reference Array<T, N>::operator[](size_t index) const { return data[index]; }
}
