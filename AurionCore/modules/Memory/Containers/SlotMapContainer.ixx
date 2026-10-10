module;

#include <AurionExport.h>
#include <cstdint>
#include <cassert>
#include <stdexcept>

export module Aurion.Memory:SlotMap;

import Aurion.Types;

import :Vector;

export namespace Aurion
{
  template<typename T>
  class AURION_API SlotMap
  {
  public:
    struct Key
    {
      u32 index = 0;
      u32 generation = 0;
    };

  public:
    SlotMap();
    ~SlotMap() = default;

    [[nodiscard]] size_t Size() const;
    [[nodiscard]] size_t Capacity() const;
    [[nodiscard]] bool IsEmpty() const;
    [[nodiscard]] bool IsFull() const;

    [[nodiscard]] bool Contains(Key key) const;

    Key Insert(const T& value);
    Key Insert(T&& value);

    template<typename... Args>
    Key Emplace(Args&&... args);

    bool Erase(Key key);

    void Clear();

    T& At(Key key);
    const T& At(Key key) const;

    T& operator[](Key key);
    const T& operator[](Key key) const;

    // Iterates live values in dense storage order, which is not insertion order.
    // Lowercase by necessity: range-based for looks these names up
    [[nodiscard]] T* begin();
    [[nodiscard]] T* end();
    [[nodiscard]] const T* begin() const;
    [[nodiscard]] const T* end() const;

  private:
    Vector<Key> m_slots;
    Vector<T> m_data;
    Vector<u32> m_erase;
    u32 m_free_head;
  };

  template<typename T>
  SlotMap<T>::SlotMap()
    : m_free_head(UINT32_MAX)
  {  }

  template<typename T>
  size_t SlotMap<T>::Size() const
  {
    return m_data.Size();
  }

  template<typename T>
  size_t SlotMap<T>::Capacity() const
  {
    return m_data.Capacity();
  }

  template<typename T>
  bool SlotMap<T>::IsEmpty() const
  {
    return m_data.IsEmpty();
  }

  template<typename T>
  bool SlotMap<T>::IsFull() const
  {
    return m_data.IsFull();
  }

  template<typename T>
  bool SlotMap<T>::Contains(Key key) const
  {
    return key.index < m_slots.Size() && m_slots[key.index].generation == key.generation;
  }

  template<typename T>
  typename SlotMap<T>::Key SlotMap<T>::Insert(const T& value)
  {
    // Construct in-place via copy-constructor
    return this->Emplace(value);
  }

  template<typename T>
  typename SlotMap<T>::Key SlotMap<T>::Insert(T&& value)
  {
    // Construct in-place via move-constructor
    return this->Emplace(static_cast<T&&>(value));
  }

  template<typename T>
  template<typename... Args>
  typename SlotMap<T>::Key SlotMap<T>::Emplace(Args&&... args)
  {
    // If there are no more disjoint free slots, let vector logic
    //  amortize growth
    if (m_free_head == UINT32_MAX)
    {
      u32 insert_idx = static_cast<u32>(m_slots.Size());

      // Vectors will auto-resize when they are full
      m_slots.PushBack({ .index = insert_idx, .generation = 1 });
      m_data.EmplaceBack(static_cast<Args&&>(args)...);
      m_erase.PushBack(insert_idx);

      // Return a key, containing the index into the slot map
      return { .index = insert_idx, .generation = 1 };
    }

    // Get the next-available free slot
    u32 slot_idx = m_free_head;
    Key& slot = m_slots.At(slot_idx);

    // Construct the value in-place
    m_data.EmplaceBack(static_cast<Args&&>(args)...);

    // And add a corresponding erase id to point back to this slot
    m_erase.PushBack(slot_idx);

    // Then update the free-list head
    m_free_head = slot.index;

    // And update the slot index and increment generation counter
    slot.index = static_cast<u32>(m_data.Size() - 1);
    ++slot.generation;

    return { .index = slot_idx, .generation = slot.generation };
  }

  template<typename T>
  bool SlotMap<T>::Erase(Key key)
  {
    // Bounds check
    if (key.index >= m_slots.Size())
      return false;

    // Ensure matching generations
    Key& slot = m_slots[key.index];
    if (slot.generation != key.generation)
      return false;

    // Only trigger a move when the element to
    //  erase is not the last element
    if (slot.index != m_data.Size() - 1)
    {
      u32 erase_idx = slot.index;

      // Move the last element into this slot, and update the
      //  erase pointer for the last element
      m_data[erase_idx] = static_cast<T&&>(m_data.Back());
      m_erase[erase_idx] = m_erase.Back();

      // Then, update the slot index for the moved element,
      m_slots[m_erase[erase_idx]].index = erase_idx;
    }

    // Forward the erased slot pointer to an empty state,
    slot.index = m_free_head;
    ++slot.generation;

    // Pop the now stale data/erase elements
    m_data.PopBack();
    m_erase.PopBack();

    // And set the free-list to point to the erased slot pointer
    m_free_head = key.index;

    return true;
  }

  template<typename T>
  void SlotMap<T>::Clear()
  {
    // Forward all live slots to an 'empty' state
    for (size_t i = 0; i < m_erase.Size(); i++)
      ++m_slots[m_erase[i]].generation;

    // Then, clear all element data and erase pointers
    m_data.Clear();
    m_erase.Clear();

    if (m_slots.IsEmpty()) return;

    // If there were existing slots, reset the free-list head
    m_free_head = 0;

    // Then, reset the indices for all current slots
    for (size_t i = 0; i < m_slots.Size() - 1; i++)
      m_slots[i].index = static_cast<u32>(i + 1);

    // And force the last slot to point to an invalid slot
    m_slots[m_slots.Size() - 1].index = UINT32_MAX;
  }

  template<typename T>
  T& SlotMap<T>::At(Key key)
  {
    if (!this->Contains(key))
      throw std::runtime_error("[SlotMap] Invalid Key");

    return m_data[m_slots[key.index].index];
  }

  template<typename T>
  const T& SlotMap<T>::At(Key key) const
  {
    if (!this->Contains(key))
      throw std::runtime_error("[SlotMap] Invalid Key");

    return m_data[m_slots[key.index].index];
  }

  template<typename T>
  T* SlotMap<T>::begin()
  {
    return m_data.begin();
  }

  template<typename T>
  T* SlotMap<T>::end()
  {
    return m_data.end();
  }

  template<typename T>
  const T* SlotMap<T>::begin() const
  {
    return m_data.begin();
  }

  template<typename T>
  const T* SlotMap<T>::end() const
  {
    return m_data.end();
  }

  template<typename T>
  T& SlotMap<T>::operator[](Key key)
  {
    Key& slot = m_slots.At(key.index);
    return m_data[slot.index];
  }

  template<typename T>
  const T& SlotMap<T>::operator[](Key key) const
  {
    const Key& slot = m_slots.At(key.index);
    return m_data[slot.index];
  }
}
