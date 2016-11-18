#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Node;

template <typename Type>
class LinkedList
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
    using node = Node<Type>;

private:
    node *first;
    node *last;

public:
    //Create an empty list with one sentinel node
    LinkedList() : first(new node), last(first)
    {}

    LinkedList(std::initializer_list<Type> l) : LinkedList()
    {
        for(value_type i : l)
            append(i);
    }

    //Create an empty list and append *other*'s elements to it
    LinkedList(const LinkedList& other) : LinkedList()
    {
        appendList(other);
    }

    LinkedList(LinkedList&& other) : LinkedList()
    {
        moveList(other);
    }

    //Copy *other*'s nodes and append them to *this* list
    void appendList(const LinkedList& other)
    {
        for(const_iterator it = other.begin(); it!= other.end(); ++it)
            append(*it);
    }

private:
    //Move elements from *other* to *this* list
    void moveList(LinkedList& other)
    {
        //Remember the position of *this* list's sentinel
        node* sentinel = first;

        //Copy other's pointers
        first = other.first;
        last = other.last;

        //Make *other* an empty list with one sentinel node
        other.first = other.last = sentinel;
    }

public:
    //Delete all data nodes in the list
    void emptyList()
    {
        node *temp, *it = first;
        //Deleting all but one (last) elements
        while(it!=last)
        {
            temp = it;
            it = it->next;
            delete temp;
        }
        //Remembering sentinel position and deleting the last element
        temp = last->next;
        delete last;

        //Setting list to initial condition (empty with one sentinel node)
        first = last = temp;
    }

    ~LinkedList()
    {
        emptyList();
        delete first; //delete sentinel node
    }

    LinkedList& operator=(const LinkedList& other)
    {
        ///TODO: Idea: Swap the contents of existing nodes and create new or delete reduntant ones
        emptyList();
        appendList(other);
        return *this;
    }

    LinkedList& operator=(LinkedList&& other)
    {
        emptyList();
        moveList(other);
        return *this;
    }

    bool isEmpty() const
    {
        //Happens only when 'first' points at sentinel node
        return first->next == nullptr;
    }

    size_type getSize() const
    {
        size_type list_size = 0;
        for(const_iterator it = begin(); it!=end(); ++it)
            ++list_size;
        return list_size;
    }

private:
    void addFirstNode(node *nd)
    {
        nd->next = first;
        first->prev = nd;
        first = last = nd;
    }

public:
    void append(const Type& item)
    {
        node *nd = new node(item);
        if(isEmpty()) addFirstNode(nd);
        else
        {
            nd->next = last->next;
            nd->prev = last;
            last->next->prev = nd;
            last->next = nd;
            last = nd;
        }
    }

    void prepend(const Type& item)
    {
        node *nd = new node(item);
        if(isEmpty()) addFirstNode(nd);
        else
        {
            first->prev = nd;
            nd->next = first;
            first = nd;
        }
    }

    void insert(const const_iterator& insertPosition, const Type& item)
    {
        if(insertPosition == begin()) prepend(item);
        else if(insertPosition == end()) append(item);
        else if(isEmpty()) addFirstNode(new node(item));
        else
        {
            node *nd = new node(item);
            node *temp = insertPosition.element;
            nd->next = temp;
            nd->prev = temp->prev;
            nd->prev->next = nd;
            temp->prev = nd;
        }
    }

    value_type popFirst()
    {
        if(isEmpty()) throw std::logic_error("List is empty");
        value_type data = *(first->data);
        node *temp = first;
        first = first->next;
        first->prev = nullptr;
        if(first->next == nullptr) last = first;
        delete temp;
        return data;
    }

    value_type popLast()
    {
        if(isEmpty()) throw std::logic_error("List is empty");
        value_type data = *(last->data);
        node *temp = last;
        if(first!=last)
        {
            last->prev->next = last->next;
            last->next->prev = last->prev;
            last = last->prev;
        }
        else
        {
            first = last = last->next;
            first->prev = nullptr;
        }
        delete temp;
        return data;
    }

    void erase(const const_iterator& possition)
    {
        erase(possition, possition+1);
    }

    void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
    {
        if(isEmpty()) std::out_of_range("List is empty");
        if(firstIncluded == lastExcluded) return;
        const_iterator it = firstIncluded==begin() ? firstIncluded+1 : firstIncluded;
        if(lastExcluded==end()) last = it.element->prev;
        it.element->prev->next = lastExcluded.element;
        lastExcluded.element->prev = it.element->prev;
        node *temp;
        while(it!=lastExcluded)
        {
            temp = it.element;
            ++it;
            delete temp;
        }
        if(firstIncluded==begin()) popFirst();
    }

    iterator begin()
    {
        return iterator(const_iterator(this, first));
    }

    iterator end()
    {
        return iterator(const_iterator(this, last->next == nullptr ? last : last->next));
    }

    const_iterator cbegin() const
    {
        return const_iterator(this, first);
    }

    const_iterator cend() const
    {
        return const_iterator(this, last->next == nullptr ? last : last->next);
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
class Node
{
public:
    friend class LinkedList<Type>;
    //using difference_type = std::ptrdiff_t;
    //using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;
    using node = Node;

private:
    pointer data;
    node *next;
    node *prev;

public:
    Node() : data(nullptr), next(nullptr), prev(nullptr)
    {}

    Node(const_reference d) : data(new value_type(d)), next(nullptr), prev(nullptr)
    {}

    ~Node()
    {
        delete data;
        data = nullptr;
        next = prev = nullptr;
    }
};


template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
    friend class LinkedList<Type>;
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename LinkedList::value_type;
    using difference_type = typename LinkedList::difference_type;
    using pointer = typename LinkedList::const_pointer;
    using reference = typename LinkedList::const_reference;

private:
    node* element;
    const LinkedList<Type>* parent_list;

public:
    explicit ConstIterator()
    {}

    ConstIterator(const LinkedList<Type>* parent, node* ptr) : element(ptr), parent_list(parent)
    {}

    ConstIterator(const ConstIterator &other) : element(other.element), parent_list(other.parent_list)
    {}

    ConstIterator& operator=(const ConstIterator &other)
    {
        element = other.element;
        parent_list = other.parent_list;
        return *this;
    }

    reference operator*() const
    {
        if(*this == parent_list->end()) throw std::out_of_range("Iterator points at empty space after the last element");
        return *(element->data);
    }

    ConstIterator& operator++()
    {
        if(*this==parent_list->end())
        {
            throw std::out_of_range("Cannot increment iterator");
            return *this;
        }
        element = element->next;
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
        if(*this==parent_list->begin())
        {
            throw std::out_of_range("Cannot decrement iterator");
            return *this;
        }
        element = element->prev;
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
        ConstIterator temp(*this);
        for(difference_type i = 0; i<d; ++i)
            ++temp;
        return temp;

    }

    ConstIterator operator-(difference_type d) const
    {
        ConstIterator temp(*this);
        for(difference_type i = 0; i<d; ++i)
            --temp;
        return temp;
    }

    bool operator==(const ConstIterator& other) const
    {
        return element == other.element;
    }

    bool operator!=(const ConstIterator& other) const
    {
        return element != other.element;
    }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
    using pointer = typename LinkedList::pointer;
    using reference = typename LinkedList::reference;

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

#endif // AISDI_LINEAR_LINKEDLIST_H
