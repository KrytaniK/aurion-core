module;

#include <AurionExport.h>
#include <AurionLog.h>

#include <cstddef>
#include <assert.h>
#include <cstdlib>

export module Aurion.Memory:Vector;

import Aurion.Types;

export namespace Aurion
{
    template<typename T>
    class AURION_API Vector
    {
    public:
        Vector();
        Vector(size_t size);

        // Move-only
        Vector(Vector&& other);
        Vector& operator=(Vector&& other);

        // No Copies
        Vector(const Vector& other) = delete;
        Vector& operator=(const Vector& other) = delete;

        ~Vector();

        void Reserve(size_t size);
        void ShrinkToFit();

        T& PushFront(const T& value);
        T& PushBack(const T& value);
        T& Insert(size_t index, const T& value);

        template<typename... Args>
        T& EmplaceFront(Args&&... args);

        template<typename... Args>
        T& EmplaceBack(Args&&... args);

        template<typename... Args>
        T& Emplace(size_t index, Args&&... args);

        void PopBack();

        bool IsEmpty();

        void Erase(size_t index);
        void Clear();

        T& Front();
        T& Back();
        T& At(size_t index);
        T& operator[](const size_t& index);

        const size_t& Size() const;

    private:
        void Reallocate(size_t new_size);

    private:
        T* m_data;
        size_t m_size;
        size_t m_capacity;
    };

    template <typename T>
    class vector : public Vector<T>
    {
    public:
    };

    template <typename T>
    Vector<T>::Vector()
        : m_size(0), m_capacity(4)
    {
        m_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));
    }

    template <typename T>
    Vector<T>::Vector(size_t size)
        : m_size(0), m_capacity(size)
    {
        assert(size > 0 && "Invalid vector size");
        m_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));
    }

    template <typename T>
    Vector<T>::Vector(Vector&& other)
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other)
    {
        if (this == &other) return *this;

        // Destroy current contents
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~T();

        free(m_data);

        // Now own other's data
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;

        return *this;
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        // cleanup
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~T();

        free(m_data);
    }

    template <typename T>
    void Vector<T>::Reserve(size_t size)
    {
        if (size <= m_capacity || size == 0)
            return;

        // Adjust capacity
        m_capacity = size;
        this->Reallocate(m_capacity);
    }

    template <typename T>
    void Vector<T>::ShrinkToFit()
    {
        if (m_size == 0) return;

        m_capacity = m_size;
        this->Reallocate(m_capacity);
    }

    template <typename T>
    T& Vector<T>::PushFront(const T& value)
    {
        // Case 1: Have no room; Allocate more room
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            T* new_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));

            // Push the value to the front of the array
            new (new_data) T(value);

            // Move the remaining values into the new array
            for (size_t i = 0; i < m_size; i++)
                new (new_data + i + 1) T(static_cast<T&&>(m_data[i]));

            // cleanup old data
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();

            free(m_data);

            m_size++;
            m_data = new_data;
            return m_data[0];
        }

        // Case 2: Vector Empty
        if (m_size == 0)
        {
            new (m_data) T(value);
            m_size++;
            return m_data[0];
        }

        // Case 3: Have room: simply move existing elements
        for (size_t i = m_size - 1; i > 0; i--)
            m_data[i] = static_cast<T&&>(m_data[i - 1]);
        m_data[0].~T();


        // Then copy the value into the array at the front
        new (m_data) T(value);
        m_size++;

        return m_data[0];
    }

    template <typename T>
    T& Vector<T>::PushBack(const T& value)
    {
        // Allocate more memory if we've hit capacity
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            this->Reallocate(m_capacity);
        }

        // Copy the value, increment size
        new (m_data + m_size++) T(value);

        return m_data[m_size - 1];
    }

    template <typename T>
    T& Vector<T>::Insert(size_t index, const T& value)
    {
        assert(index <= m_size);

        // Case 1: Resize needed
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            T* new_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));

            // Move the values up to the target index
            for (size_t i = 0; i < index; i++)
                new (new_data + i) T(static_cast<T&&>(m_data[i]));

            // Copy the value at the target index
            new (new_data + index) T(value);

            // Then move remaining elements, shifted right by one index
            for (size_t i = index; i < m_size; i++)
                new (new_data + i + 1) T(static_cast<T&&>(m_data[i]));

            // clean up old memory
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();

            free(m_data);

            m_size++;
            m_data = new_data;
            return m_data[index];
        }

        // Case 2: No resize: Shift elements after index by one (Move op)
        for (size_t i = m_size; i > index; i--)
            m_data[i] = static_cast<T&&>(m_data[i - 1]);

        // Copy value into index, increment size
        m_data[index].~T();
        new (m_data + index) T(value);
        m_size++;
        return m_data[index];
    }

    template <typename T>
    template <typename... Args>
    T& Vector<T>::EmplaceFront(Args&&... args)
    {
        // Case 1: Have no room; Allocate more room
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            T* new_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));

            // Construct the value in-place at the beginning of the new array
            new (new_data) T(static_cast<Args&&>(args)...);

            // Move the remaining values
            for (size_t i = 0; i < m_size; i++)
                new (new_data + i + 1) T(static_cast<T&&>(m_data[i]));

            // clean up old memory
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();
            free(m_data);

            m_size++;
            m_data = new_data;
            return m_data[0];
        }

        // Case 2: Vector Empty
        if (m_size == 0)
        {
            new (m_data) T(static_cast<Args&&>(args)...);
            m_size++;
            return m_data[0];
        }

        // Case 3: Have room: simply move existing elements
        for (size_t i = m_size; i > 0; i--)
            m_data[i] = static_cast<T&&>(m_data[i - 1]);

        // Then construct the value in-place at the front
        m_data[0].~T();
        new (m_data) T(static_cast<Args&&>(args)...);

        m_size++;
        return m_data[0];
    }

    template <typename T>
    template <typename... Args>
    T& Vector<T>::EmplaceBack(Args&&... args)
    {
        // Allocate more memory if we've hit capacity
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            this->Reallocate(m_capacity);
        }

        // Construct the element in-place at the back of the array
        new (m_data + m_size++) T(static_cast<Args&&>(args)...);
        return m_data[m_size - 1];
    }

    template <typename T>
    template <typename... Args>
    T& Vector<T>::Emplace(size_t index, Args&&... args)
    {
        assert(index <= m_size);

        // Case 1: Resize needed
        if (m_size == m_capacity)
        {
            m_capacity *= 2;
            T* new_data = static_cast<T*>(calloc(m_capacity, sizeof(T)));

            // Move the values up to the target index
            for (size_t i = 0; i < index; i++)
                new (new_data + i) T(static_cast<T&&>(m_data[i]));

            // Create the value in-place at the target index in new_data
            new (new_data + index) T(static_cast<Args&&>(args)...);

            // Then move remaining elements, shifted right by one index
            for (size_t i = index; i < m_size; i++)
                new (new_data + i + 1) T(static_cast<T&&>(m_data[i]));

            // clean up old memory
            for (size_t i = 0; i < m_size; i++)
                m_data[i].~T();
            free(m_data);

            m_size++;
            m_data = new_data;

            return m_data[index];
        }

        // Case 2: Shift elements after index by one (Move op)
        for (size_t i = m_size; i > index; i--)
            m_data[i] = static_cast<T&&>(m_data[i - 1]);

        // Construct value in-place at index, increment size
        m_data[index].~T();
        new (m_data + index) T(static_cast<Args&&>(args)...);
        m_size++;

        return m_data[index];
    }

    template <typename T>
    void Vector<T>::PopBack()
    {
        m_data[--m_size].~T();
    }

    template <typename T>
    bool Vector<T>::IsEmpty()
    {
        return m_size == 0;
    }

    template <typename T>
    void Vector<T>::Erase(size_t index)
    {
        assert(index < m_size && index < m_capacity && "Vector index out of range");

        // Explicitly destruct the object
        m_data[index].~T();

        // Then shift remaining elements to the left
        for (size_t i = index; i < m_size - 1; i++)
        {
            new (m_data + i) T(static_cast<T&&>(m_data[i + 1]));
            m_data[i + 1].~T();
        }

        // Adjust size
        m_size--;
    }

    template <typename T>
    void Vector<T>::Clear()
    {
        // Explicitly call destructors
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~T();

        // and reset size
        m_size = 0;
    }

    template <typename T>
    T& Vector<T>::Front()
    {
        return m_data[0];
    }

    template <typename T>
    T& Vector<T>::Back()
    {
        return m_data[m_size - 1];
    }

    template <typename T>
    T& Vector<T>::operator[](const size_t& index)
    {
        assert(index < m_size && index < m_capacity && "Vector index out of range");
        return m_data[index];
    }

    template <typename T>
    T& Vector<T>::At(size_t index)
    {
        assert(index < m_size && index < m_capacity && "Vector index out of range");
        return m_data[index];
    }

    template <typename T>
    const size_t& Vector<T>::Size() const
    {
        return m_size;
    }

    template <typename T>
    void Vector<T>::Reallocate(size_t new_size)
    {
        // Allocate new size
        T* new_data = static_cast<T*>(calloc(new_size, sizeof(T)));
        if (!new_data || new_data == nullptr)
            AURION_ERROR("Failed to allocate memory for new vector data");

        // Move data to new array
        for (size_t i = 0; i < m_size; i++)
            new (new_data + i) T(static_cast<T&&>(m_data[i]));

        // clean up old memory
        for (size_t i = 0; i < m_size; i++)
            m_data[i].~T();

        free(m_data);

        // Assign new memory
        m_data = new_data;
    }
}
