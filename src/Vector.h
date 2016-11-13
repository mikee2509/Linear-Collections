#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <math.h>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;

private:
    value_type *vec;
    size_type size; //Number of elements currently stored in vector
    size_type capacity; //Current capacity of allocated space
    const double INCREASE_FACTOR = 0.5; //Factor by which the capacity will be increased when reallocation is needed

public:
    Vector()
    {
        size = 0;
        capacity = 0;
        vec = new value_type[0];
    }

    Vector(std::initializer_list<Type> l)
    {
        vec = new value_type[l.size()];
        size = 0;
        for(value_type i : l)
        {
            vec[size++]=i;
        }
    }

    Vector(const Vector& other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    Vector(Vector&& other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    ~Vector()
    {}

    Vector& operator=(const Vector& other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    Vector& operator=(Vector&& other)
    {
        (void)other;
        throw std::runtime_error("TODO");
    }

    bool isEmpty() const
    {
        return size<=0;
    }

    size_type getSize() const
    {
        return size;
    }

    void append(const Type& item)
    {
        insert(end(), item);
    }

    void prepend(const Type& item)
    {
        (void)item;
        throw std::runtime_error("TODO");
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        iterator it;
        if(capacity>size)
        {

            for(it=end(); it!=insertPosition; --it)
                *it=*(it-1);
            *it = item;
        }
        else
        {
            capacity *= 1 + INCREASE_FACTOR;
            value_type *new_space = new value_type[capacity];
            size_type i=0;

            //Copy all the elements up to insertPosition
            for(it=begin(); it!=insertPosition; ++it, ++i)
                new_space[i] = vec[i];

            //Copy the item at inserPosition
            new_space[i++] = item;

            //Copy the rest of the vector
            for(it=insertPosition+1; it!=end()+1; ++it, ++i)
                new_space[i] = vec[i-1];

            delete[] vec;
            vec = new_space;
        }
        ++size;
    }

    Type popFirst()
    {
        throw std::runtime_error("TODO");
    }

    Type popLast()
    {
        throw std::runtime_error("TODO");
    }

    void erase(const const_iterator& possition)
    {
        (void)possition;
        throw std::runtime_error("TODO");
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        (void)firstIncluded;
        (void)lastExcluded;
        throw std::runtime_error("TODO");
    }

    iterator begin()
    {
        return iterator(const_iterator(this, vec));
    }

    iterator end()
    {
        return iterator(const_iterator(this, vec+size));
    }

    const_iterator cbegin() const
    {
        return const_iterator(this, vec);
    }

    const_iterator cend() const
    {
        return const_iterator(this, vec+size);
    }

    const_iterator begin() const
    {
        return cbegin();
    }

    const_iterator end() const
    {
        return cend();
    }
    };

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;

private:
    pointer element;
    const Vector<Type>* parent_vec;

public:

    explicit ConstIterator()
    {}

    ConstIterator(const Vector<Type>* parent, pointer ptr) : element(ptr), parent_vec(parent)
    {}

    ConstIterator(const ConstIterator &other) : element(other.element), parent_vec(other.parent_vec)
    {}

    ConstIterator& operator=(const ConstIterator &other)
    {
        element = other.element;
        parent_vec = other.parent_vec;
        return *this;
    }

    reference operator*() const
    {
        if(*this == parent_vec->end()) throw std::out_of_range("Iterator points at empty space after the last element");
        return *element;
    }

    ConstIterator& operator++()
    {
        if(*this==parent_vec->end())
        {
            throw std::out_of_range("Cannot increment iterator");
            return *this;
        }
        ++element;
        return *this;
    }

    ConstIterator operator++(int)
    {
        ConstIterator temp = *this;
        ++(*this);
        return temp;
    }

    ConstIterator& operator--()
    {
        if(*this==parent_vec->begin())
        {
            throw std::out_of_range("Cannot decrement iterator");
            return *this;
        }
        --element;
        return *this;
    }

    ConstIterator operator--(int)
    {
        ConstIterator temp = *this;
        --(*this);
        return temp;
    }

    ConstIterator operator+(difference_type d) const
    {
        return ConstIterator(parent_vec, element+d);
    }

    ConstIterator operator-(difference_type d) const
    {
        return ConstIterator(parent_vec, element-d);
    }

    bool operator==(const ConstIterator& other) const
    {
        return element==other.element;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return element!=other.element;
    }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;

    explicit Iterator()
    {}

    Iterator(const ConstIterator& other) : ConstIterator(other)
    {}

    Iterator& operator++()
    {
        ConstIterator::operator++();
        return *this;
    }

    Iterator operator++(int)
    {
        auto result = *this;
        ConstIterator::operator++();
        return result;
    }

    Iterator& operator--()
    {
        ConstIterator::operator--();
        return *this;
    }

    Iterator operator--(int)
    {
        auto result = *this;
        ConstIterator::operator--();
        return result;
    }

    Iterator operator+(difference_type d) const
    {
        return ConstIterator::operator+(d);
    }

    Iterator operator-(difference_type d) const
    {
        return ConstIterator::operator-(d);
    }

    reference operator*() const
    {
        // ugly cast, yet reduces code duplication.
        return const_cast<reference>(ConstIterator::operator*());
    }
};

}

#endif // AISDI_LINEAR_VECTOR_H
