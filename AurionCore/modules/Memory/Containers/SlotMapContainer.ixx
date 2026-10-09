module;


export module Aurion.Memory:SlotMap;

import Aurion.Types;

import :Vector;

export namespace Aurion
{
  template<typename T>
  class SlotMap
  {
  public:
    union Key {
      u64 value;
      struct { u32 index; u32 generation; };
    };


  public:
    SlotMap();
    ~SlotMap() = default;

    Key Insert(T& value);
    Key Insert(T&& value);

    template<typename... Args>
    Key Emplace(Args&& args);

    bool Erase(Key key);

    void Clear();

    T& At(Key key);
    const T& At(Key key) const;

    T& operator[](Key key);
    const T& operator[](Key key) const;


  private:
    Vector<Key> m_slots;
    Vector<T> m_data;
    Vector<u32> m_erase;
    u32 m_free_head;
  };

  template<typename T>
  SlotMap<T>::SlotMap()
    : m_free_head(0)
  {
    for (size_t i = 0; i < m_slots.Size(); i++)
    {
      m_slots[i] = { .index = i, .generation = 0 };
      m_erase[i] = i;
    }
  }

  template<typename T>
  typename SlotMap<T>::Key SlotMap<T>::Insert(T& value)
  {


  }

  template<typename T>
  typename SlotMap<T>::Key SlotMap<T>::Insert(T&& value)
  {

  }

  template<typename T>
  template<typename ... Args>
  typename SlotMap<T>::Key SlotMap<T>::Emplace(Args &&args)
  {

  }

  template<typename T>
  bool SlotMap<T>::Erase(Key key)
  {

  }

  template<typename T>
  void SlotMap<T>::Clear()
  {

  }

  template<typename T>
  T & SlotMap<T>::At(Key key)
  {

  }

  template<typename T>
  const T & SlotMap<T>::At(Key key) const
  {

  }

  template<typename T>
  T & SlotMap<T>::operator[](Key key)
  {

  }

  template<typename T>
  const T & SlotMap<T>::operator[](Key key) const
  {

  }
}
