module;

#include <assert.h>
#include <AurionExport.h>
#include <type_traits>
#include <cstddef>

export module Aurion.Memory:Array;

import Aurion.Types;

export namespace Aurion
{
  template <typename T, size_t N>
  class AURION_API Array
  {
    static_assert(std::is_default_constructible_v<T>, "Value Type must be default constructible!");

  public:
    T data[N];

    ~Array() = default;

    [[nodiscard]] size_t Size();
    [[nodiscard]] size_t Capacity();
    [[nodiscard]] bool IsEmpty();

    [[nodiscard]] T* Data();

    [[nodiscard]] T& At(const size_t& index);
    [[nodiscard]] T& Front();
    [[nodiscard]] T& Back();

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
  };

  template <typename T, size_t N>
  size_t Array<T, N>::Size() { return N; }

  template <typename T, size_t N>
  size_t Array<T, N>::Capacity() { return N; }

  template <typename T, size_t N>
  bool Array<T, N>::IsEmpty() { return data == nullptr; }

  template <typename T, size_t N>
  T* Array<T, N>::Data() { return data; }

  template <typename T, size_t N>
  T& Array<T, N>::At(const size_t& index)
  {
    assert(index < N && "Index Out of Bounds!");
    return data[index];
  }

  template <typename T, size_t N>
  T& Array<T, N>::Front() { return data[0]; }

  template <typename T, size_t N>
  T& Array<T, N>::Back() { return data[N - 1]; }

  template <typename T, size_t N>
  T& Array<T, N>::operator[](size_t index) { return data[index]; }

  template <typename T, size_t N>
  const T& Array<T, N>::operator[](size_t index) const { return data[index]; }
}
