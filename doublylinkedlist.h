#ifndef MANUAL_DOUBLYLINKEDLIST_H
#define MANUAL_DOUBLYLINKEDLIST_H

/*!
 * \file doublylinkedlist.h
 * \brief A doubly linked list implementation (proposal).
 * \author Raphaël Lefèvre
 */

//#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace manual
{
    /*!
     * \class DoublyLinkedList
     * \brief A doubly linked list implementation.
     */
    template <typename T>
    class DoublyLinkedList
    {
        protected:
            /*!
             * \struct Node
             * \brief Internal representation of a node.
             */
            struct Node final
            {
                T value;         /*!< The value */
                Node * next;     /*!< Link the the next node */
                Node * previous; /*!< Link to the previous node */
            };

            // data members
            Node * head_; /*!< Pointer to the head */
            Node * tail_; /*!< Pointer to the tail */
            size_t size_; /*!< The size */

        public:
            // Constructors
            /*!
             * \brief Default constructor.
             *
             * Creates an empty list.
             */
            DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0)
            {}
            /*!
             * \brief Copy constructor.
             * \param[in] other The DoublyLinkedList to copy
             */
            DoublyLinkedList(const DoublyLinkedList<T> & other) : head_(nullptr), tail_(nullptr), size_(other.size_)
            {
                if(size_)
                {
                    head_ = new Node;
                    head_->value = other.head_->value;
                    head_->previous = nullptr;
                    head_->next = nullptr;

                    Node * current = head_;
                    Node * other_current = other.head_;
                    while(other_current->next)
                    {
                        current->next = new Node;
                        current->next->value = other_current->next->value;
                        current->next->previous = current;
                        current->next->next = nullptr;

                        current = current->next;
                        other_current = other_current->next;
                    }
                    tail_ = current;
                }
            }
            /*!
             * \brief Move constructor.
             * \param[in,out] other The DoublyLinkedList to move from
             *
             * \note The moved DoublyLinkedList will be left empty but still valid.
             */
            DoublyLinkedList(DoublyLinkedList<T> && other) noexcept : head_{std::exchange(other.head_, nullptr)},
                                                                      tail_{std::exchange(other.tail_, nullptr)},
                                                                      size_{std::exchange(other.size_, 0)}
            {}
            /*!
             * \brief Initialization constructor.
             * \param[in] init_list An initializer list to copy
             */
            DoublyLinkedList(const std::initializer_list<T> & init_list) : head_(nullptr), tail_(nullptr), size_(init_list.size())
            {
                if(size_)
                {
                    typename std::initializer_list<T>::iterator it = init_list.begin();

                    head_ = new Node;
                    head_->value = *it;
                    head_->previous = nullptr;
                    head_->next = nullptr;

                    Node * current = head_;

                    for(++it; it != init_list.end(); ++it)
                    {
                        current->next = new Node;
                        current->next->value = *it;
                        current->next->previous = current;
                        current->next->next = nullptr;

                        current = current->next;
                    }
                    tail_ = current;
                }
            }
            ~DoublyLinkedList()
            {
                if(size_)
                {
                    Node * current = head_;
                    Node * tmp = nullptr;
                    for(size_t i = 0; i < size_; ++i)
                    {
                        tmp = current->next;
                        delete current;
                        current = tmp;
                    }
                }
            }

            // Capacity
            /*!
             * \brief Get the size of the container.
             * \return The size
             */
            size_t size() const
            {
                return size_;
            }
            /*!
             * \brief Check if the container is empty.
             * \return `true` if the list is empty, `false` otherwise
             */
            bool empty() const
            {
                return !head_;
            }

            // Element Access
            /*!
             * \brief Get the first element.
             * \return A direct reference to the head value
             *
             * \warning Never call this function on an empty container (Undefined Behaviour).
             */
            T & front()
            {
                return head_->value;
            }
            /*!
             * \brief Get the first element.
             * \return A direct `const` reference to the head value
             *
             * \warning Never call this function on an empty container (Undefined Behaviour).
             */
            const T & front() const
            {
                return head_->value;
            }
            /*!
             * \brief Get the last element.
             * \return A direct reference to the tail value
             *
             * \warning Never call this function on an empty container (Undefined Behaviour).
             */
            T & back()
            {
                return tail_->value;
            }
            /*!
             * \brief Get the last element.
             * \return A direct `const` reference to the tail value
             *
             * \warning Never call this function on an empty container (Undefined Behaviour).
             */
            const T & back() const
            {
                return tail_->value;
            }
            /*!
             * \brief Access to an element by index.
             * \param[in] index The position of the element to access
             * \return A `const` reference to the element value at the given index
             *
             * \warning Never call this function with an out-of-range index (Undefined Behaviour).
             * \note Iterates over the container (from the closest end) until the index is reached. No direct access.
             */
            const T & operator[](size_t index) const
            {
                if((size_-1 - index) < index) // closer to the end
                {
                    Node * current = tail_;
                    for(size_t i = size_-1; i > index; --i)
                    {
                        current = current->previous;
                    }
                    return current->value;
                }
                else // closer to the beginning
                {
                    Node * current = head_;
                    for(size_t i = 0; i < index; ++i)
                    {
                        current = current->next;
                    }
                    return current->value;
                }
            }
            /*!
             * \brief Access to an element by index.
             * \param[in] index The position of the element to access
             * \return A reference to the element value at the given index
             *
             * \warning Never call this function with an out-of-range index (Undefined Behaviour).
             * \note Iterates over the container (from the closest end) until the index is reached. No direct access.
             */
            T & operator[](size_t index)
            {
                return const_cast<T&>(const_cast<const DoublyLinkedList<T> &>(*this)[index]);
            }
            /*!
             * \brief Safely access to an element by index.
             * \param[in] index The position of the element to access
             * \return A `const` reference to the element value at the given index
             *
             * \throws std::out_of_range Automatically checks whether the index is within the bounds of the container.
             * \note Iterates over the container (from the closest end) until the index is reached. No direct access.
             */
            const T & at(size_t index) const
            {
                if(index >= size_)
                    throw std::out_of_range(std::string("[Out of range error] - manual::DoublyLinkedList::at() - (index: ") + std::to_string(index) + ", size: " + std::to_string(size_) + ").");

                return const_cast<const DoublyLinkedList<T> &>(*this)[index];
            }
            /*!
             * \brief Safely access to an element by index.
             * \param[in] index The position of the element to access
             * \return A reference to the element value at the given index
             *
             * \throws std::out_of_range Automatically checks whether the index is within the bounds of the container.
             * \note Iterates over the container (from the closest end) until the index is reached. No direct access.
             */
            T & at(size_t index)
            {
                return const_cast<T&>(const_cast<const DoublyLinkedList<T> &>(*this).at(index));
            }

            // Modifiers
            /*!
             * \brief Clear the container.
             */
            void clear()
            {
                if(size_)
                {
                    Node * current = head_;
                    Node * tmp = nullptr;
                    for(size_t i = 0; i < size_; ++i)
                    {
                        tmp = current->next;
                        delete current;
                        current = tmp;
                    }
                    head_ = nullptr;
                    tail_ = nullptr;
                    size_ = 0;
                }
            }
            /*!
             * \brief Add a value at the end of the container.
             * \param[in] val The value to append
             */
            void push_back(const T & val)
            {
                Node * tmp = new Node;
                tmp->value = val;
                tmp->next = nullptr;
                tmp->previous = tail_;
                if(size_)
                    tail_->next = tmp;
                else
                    head_ = tmp;

                tail_ = tmp;
                ++size_;
            }
            /*!
             * \brief Add a value at the beginning of the container.
             * \param[in] val The value to prepend
             */
            void push_front(const T & val)
            {
                Node * tmp = new Node;
                tmp->value = val;
                tmp->previous = nullptr;
                tmp->next = head_;
                if(size_)
                    head_->previous = tmp;
                else
                    tail_ = tmp;

                head_ = tmp;
                ++size_;
            }
            /*!
             * \brief Remove the last value of the container (if any).
             */
            void pop_back()
            {
                if(size_)
                {
                    if(size_ == 1)
                    {
                        delete tail_;
                        tail_ = nullptr;
                        head_ = nullptr;
                    }
                    else
                    {
                        Node * tmp = tail_->previous;
                        tmp->next = nullptr;
                        delete tail_;
                        tail_ = tmp;
                    }
                    --size_;
                }
            }
            /*!
             * \brief Remove the first value of the container (if any).
             */
            void pop_front()
            {
                if(size_)
                {
                    if(size_ == 1)
                    {
                        delete head_;
                        head_ = nullptr;
                        tail_ = nullptr;
                    }
                    else
                    {
                        Node * tmp = head_->next;
                        tmp->previous = nullptr;
                        delete head_;
                        head_ = tmp;
                    }
                    --size_;
                }
            }
            /*!
             * \brief Insert a value at the specified index.
             * \param[in] index The position where to insert the element
             * \param[in] val The element to be inserted
             *
             * \note Does nothing if the index exceeds the size value.
             */
            void insert(size_t index, const T & val)
            {
                if(index <= size_)
                {
                    if(index == 0)
                    {
                        push_front(val);
                    }
                    else if(index == size_)
                    {
                        push_back(val);
                    }
                    else
                    {
                        Node * tmp = new Node;
                        tmp->value = val;

                        Node * current = nullptr;
                        if((size_-1 - index) < index) // closer to the end
                        {
                            current = tail_;
                            for(size_t i = size_-1; i > index; --i)
                                current = current->previous;
                        }
                        else // closer to the beginning
                        {
                            current = head_;
                            for(size_t i = 0; i < index; ++i)
                                current = current->next;
                        }

                        current->previous->next = tmp;
                        tmp->previous = current->previous;
                        tmp->next = current;
                        current->previous = tmp;
                        ++size_;
                    }
                }
            }
            /*!
             * \brief Remove the element at the given index.
             * \param[in] index The position of the element to remove
             *
             * \note Does nothing if the index is out-of-range or if the container is empty.
             */
            void remove(size_t index)
            {
                if(index < size_)
                {
                    if(index == 0)
                    {
                        pop_front();
                    }
                    else if(index == size_-1)
                    {
                        pop_back();
                    }
                    else
                    {
                        Node * current = nullptr;
                        if((size_-1 - index) < index) // closer to the end
                        {
                            current = tail_;
                            for(size_t i = size_-1; i > index; --i)
                                current = current->previous;
                        }
                        else // closer to the beginning
                        {
                            current = head_;
                            for(size_t i = 0; i < index; ++i)
                                current = current->next;
                        }
                        current->previous->next = current->next;
                        current->next->previous = current->previous;
                        delete current;
                        --size_;
                    }
                }
            }

            // Operators
            /*!
             * \brief Copy assign new contents to the container (replacing the current contents).
             * \param other A DoublyLinkedList of the same type (to copy)
             * \return A reference to `*this`
             */
            DoublyLinkedList<T> & operator=(const DoublyLinkedList<T> & other)
            {
                if(this != &other)
                {
                    clear();
                    size_ = other.size_;
                    if(size_)
                    {
                        head_ = new Node;
                        head_->value = other.head_->value;
                        head_->previous = nullptr;
                        head_->next = nullptr;

                        Node * current = head_;
                        Node * other_current = other.head_;
                        while(other_current->next)
                        {
                            current->next = new Node;
                            current->next->value = other_current->next->value;
                            current->next->previous = current;
                            current->next->next = nullptr;

                            current = current->next;
                            other_current = other_current->next;
                        }
                        tail_ = current;
                    }
                }
                return *this;
            }
            /*!
             * \brief Move assign new contents to the container (replacing the current contents).
             * \param[in,out] other A DoublyLinkedList of the same type (to move from)
             * \return A reference to `*this`
             *
             * \note The moved DoublyLinkedList will be left empty but still valid.
             */
            DoublyLinkedList<T> & operator=(DoublyLinkedList<T> && other) noexcept
            {
                if(this != &other)
                {
                    clear();
                    head_ = std::exchange(other.head_, nullptr);
                    tail_ = std::exchange(other.tail_, nullptr);
                    size_ = std::exchange(other.size_, 0);
                }
                return *this;
            }

            // Iterator
            /*!
             * \class Iterator
             * \brief An iterator implementation.
             */
            class Iterator final
            {
                friend class DoublyLinkedList;

                private:
                    Node * node;

                public:
                    /*!
                     * \brief Default constructor.
                     *
                     * \warning Should not be dereferenced (Undefined Behaviour).
                     */
                    Iterator() : node(nullptr)
                    {}
                    /*!
                     * \brief Copy constructor.
                     * \param[in] it The Iterator to copy
                     */
                    Iterator(const Iterator & it) : node(it.node)
                    {}
                    /*!
                     * \brief Assignment operator.
                     * \param[in] other The Iterator to copy
                     * \return A reference to `*this`
                     */
                    Iterator & operator=(const Iterator & other)
                    {
                        if(this != &other)
                        {
                            node = other.node;
                        }
                        return *this;
                    }
                    /*!
                     * \brief Equality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if equality, `false` otherwise
                     */
                    friend bool operator==(const Iterator & lhs, const Iterator & rhs)
                    {
                        return lhs.node == rhs.node;
                    }
                    /*!
                     * \brief Inequality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if inequality, `false` otherwise
                     */
                    friend bool operator!=(const Iterator & lhs, const Iterator & rhs)
                    {
                        return !(lhs == rhs);
                    }
                    /*!
                     * \brief Indirection (dereference) operator.
                     * \return A reference to the pointed value
                     *
                     * \note Dereference an out-of-range Iterator is Undefined Behaviour.
                     */
                    T & operator*() const
                    {
                        return node->value;
                    }
                    /*!
                     * \brief Member of pointer (dereference) operator.
                     * \return The address of the pointed value
                     *
                     * \note Dereference an out-of-range Iterator is Undefined Behaviour.
                     */
                    T * operator->() const
                    {
                        return &(node->value);
                    }
                    /*!
                     * \brief Prefix increment operator.
                     * \return A reference to the incremented `*this`
                     *
                     * Shift to the next element.
                     */
                    Iterator & operator++() //prefix
                    {
                        if(node)
                            node = node->next;
                        return *this;
                    }
                    /*!
                     * \brief Postfix increment operator.
                     * \return The before-increment Iterator
                     *
                     * Shift to the next element.
                     */
                    Iterator operator++(int) //postfix
                    {
                        Iterator tmp(*this);
                        ++(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Addition assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Increment the Iterator `rhs` times.
                     */
                    Iterator & operator+=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            ++(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting Iterator
                     *
                     * Increment `lhs` by `rhs`.
                     */
                    friend Iterator operator+(Iterator lhs, size_t rhs)
                    {
                        lhs += rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting Iterator
                     *
                     * Increment `rhs` by `lhs`.
                     */
                    friend Iterator operator+(size_t lhs, Iterator rhs)
                    {
                        return rhs + lhs;
                    }
                    /*!
                     * \brief Prefix decrement operator.
                     * \return A reference to the decremented `*this`
                     *
                     * Shift to the previous element.
                     */
                    Iterator & operator--() //prefix
                    {
                        if(node)
                            node = node->previous;
                        return *this;
                    }
                    /*!
                     * \brief Postfix decrement operator.
                     * \return The before-decrement Iterator
                     *
                     * Shift to the previous element.
                     */
                    Iterator operator--(int) //postfix
                    {
                        Iterator tmp(*this);
                        --(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Subtraction assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Decrement the Iterator `rhs` times.
                     */
                    Iterator & operator-=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            --(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting Iterator
                     *
                     * Decrement `lhs` by `rhs`.
                     */
                    friend Iterator operator-(Iterator lhs, size_t rhs)
                    {
                        lhs -= rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting Iterator
                     *
                     * Decrement `rhs` by `lhs`.
                     */
                    friend Iterator operator-(size_t lhs, Iterator rhs)
                    {
                        return rhs - lhs;
                    }
            };
            /*!
             * \class ConstIterator
             * \brief A `const` iterator implementation.
             */
            class ConstIterator final
            {
                friend class DoublyLinkedList;

                private:
                    Node * node;

                public:
                    /*!
                     * \brief Default constructor.
                     *
                     * \warning Should not be dereferenced (Undefined Behaviour).
                     */
                    ConstIterator() : node(nullptr)
                    {}
                    /*!
                     * \brief Copy constructor.
                     * \param[in] cit The ConstIterator to copy
                     */
                    ConstIterator(const ConstIterator & cit) : node(cit.node)
                    {}
                    /*!
                     * \brief Assignment operator.
                     * \param[in] other The ConstIterator to copy
                     * \return A reference to `*this`
                     */
                    ConstIterator & operator=(const ConstIterator & other)
                    {
                        if(this != &other)
                        {
                            node = other.node;
                        }
                        return *this;
                    }
                    /*!
                     * \brief Equality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The reight-hand side
                     * \return `true` if equality, `false` otherwise
                     */
                    friend bool operator==(const ConstIterator & lhs, const ConstIterator & rhs)
                    {
                        return lhs.node == rhs.node;
                    }
                    /*!
                     * \brief Inequality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if inequality, `false` otherwise
                     */
                    friend bool operator!=(const ConstIterator & lhs, const ConstIterator & rhs)
                    {
                        return !(lhs == rhs);
                    }
                    /*!
                     * \brief Indirection (dereference) operator.
                     * \return A `const` reference to the pointed value
                     *
                     * \note Dereference an out-of-range ConstIterator is Undefined Behaviour.
                     */
                    const T & operator*() const
                    {
                        return node->value;
                    }
                    /*!
                     * \brief Member of pointer (dereference) operator.
                     * \return The address of the pointed value as `const`
                     *
                     * \note Dereference an out-of-range ConstIterator is Undefined Behaviour.
                     */
                    const T * operator->() const
                    {
                        return &(node->value);
                    }
                    /*!
                     * \brief Prefix increment operator.
                     * \return A reference to the incremented `*this`
                     *
                     * Shift to the next element.
                     */
                    ConstIterator & operator++() //prefix
                    {
                        if(node)
                            node = node->next;
                        return *this;
                    }
                    /*!
                     * \brief Postfix increment operator.
                     * \return The before-increment ConstIterator
                     *
                     * Shift to the next element.
                     */
                    ConstIterator operator++(int) //postfix
                    {
                        ConstIterator tmp(*this);
                        ++(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Addition assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Increment the ConstIterator `rhs` times.
                     */
                    ConstIterator & operator+=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            ++(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstIterator
                     *
                     * Increment `lhs` by `rhs`.
                     */
                    friend ConstIterator operator+(ConstIterator lhs, size_t rhs)
                    {
                        lhs += rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstIterator
                     *
                     * Increment `rhs` by `lhs`.
                     */
                    friend ConstIterator operator+(size_t lhs, ConstIterator rhs)
                    {
                        return rhs + lhs;
                    }
                    /*!
                     * \brief Prefix decrement operator.
                     * \return A reference to the decremented `*this`
                     *
                     * Shift to the previous element.
                     */
                    ConstIterator & operator--() //prefix
                    {
                        if(node)
                            node = node->previous;
                        return *this;
                    }
                    /*!
                     * \brief Postfix decrement operator.
                     * \return The before-decrement ConstIterator
                     *
                     * Shift to the previous element.
                     */
                    ConstIterator operator--(int) //postfix
                    {
                        ConstIterator tmp(*this);
                        --(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Subtraction assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Decrement the ConstIterator `rhs` times.
                     */
                    ConstIterator & operator-=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            --(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstIterator
                     *
                     * Decrement `lhs` by `rhs`.
                     */
                    friend ConstIterator operator-(ConstIterator lhs, size_t rhs)
                    {
                        lhs -= rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstIterator
                     *
                     * Decrement `rhs` by `lhs`.
                     */
                    friend ConstIterator operator-(size_t lhs, ConstIterator rhs)
                    {
                        return rhs - lhs;
                    }
            };
            /*!
             * \class ReverseIterator
             * \brief A reverse iterator implementation.
             */
            class ReverseIterator final
            {
                friend class DoublyLinkedList;

                private:
                    Node * node;

                public:
                    /*!
                     * \brief default constructor.
                     *
                     * \warning Should not be dereferenced (Undefined Behaviour).
                     */
                    ReverseIterator() : node(nullptr)
                    {}
                    /*!
                     * \brief Copy constructor.
                     * \param[in] rit The ReverseIterator to copy
                     */
                    ReverseIterator(const ReverseIterator & rit) : node(rit.node)
                    {}
                    /*!
                     * \brief Assignment operator.
                     * \param[in] other The ReverseIterator to copy
                     * \return A reference to `*this`
                     */
                    ReverseIterator & operator=(const ReverseIterator & other)
                    {
                        if(this != &other)
                        {
                            node = other.node;
                        }
                        return *this;
                    }
                    /*!
                     * \brief Equality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if equality, `false` otherwise
                     */
                    friend bool operator==(const ReverseIterator & lhs, const ReverseIterator & rhs)
                    {
                        return lhs.node == rhs.node;
                    }
                    /*!
                     * \brief Inequality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if inequality, `false` otherwise
                     */
                    friend bool operator!=(const ReverseIterator & lhs, const ReverseIterator & rhs)
                    {
                        return !(lhs == rhs);
                    }
                    /*!
                     * \brief Indirection (dereference) operator.
                     * \return A reference to the pointed value
                     *
                     * \note Dereference an out-of-range ReverseIterator is Undefined Behaviour.
                     */
                    T & operator*() const
                    {
                        return node->value;
                    }
                    /*!
                     * \brief Member of pointer (dereference) operator.
                     * \return The address of the pointed value
                     *
                     * \note Dereference an out-of-range ReverseIterator is Undefined Behaviour.
                     */
                    T * operator->() const
                    {
                        return &(node->value);
                    }
                    /*!
                     * \brief Prefix increment operator.
                     * \return A reference to the incremented `*this`
                     *
                     * Shift to the previous element.
                     */
                    ReverseIterator & operator++() //prefix
                    {
                        if(node)
                            node = node->previous;
                        return *this;
                    }
                    /*!
                     * \brief Postfix increment operator.
                     * \return The before-increment ReverseIterator
                     *
                     * Shift to the previous element.
                     */
                    ReverseIterator operator++(int) //postfix
                    {
                        ReverseIterator tmp(*this);
                        ++(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Addition assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Increment the ReverseIterator `rhs` times.
                     */
                    ReverseIterator & operator+=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            ++(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ReverseIterator
                     *
                     * Increment `lhs` by `rhs`.
                     */
                    friend ReverseIterator operator+(ReverseIterator lhs, size_t rhs)
                    {
                        lhs += rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ReverseIterator
                     *
                     * Increment `rhs` by `lhs`.
                     */
                    friend ReverseIterator operator+(size_t lhs, ReverseIterator rhs)
                    {
                        return rhs + lhs;
                    }
                    /*!
                     * \brief Prefix decrement operator.
                     * \return A reference to the decremented `*this`
                     *
                     * Shift to the next element.
                     */
                    ReverseIterator & operator--() //prefix
                    {
                        if(node)
                            node = node->next;
                        return *this;
                    }
                    /*!
                     * \brief Postfix decrement operator.
                     * \return The before-decrement ReverseIterator
                     *
                     * Shift to the next element.
                     */
                    ReverseIterator operator--(int) //postfix
                    {
                        ReverseIterator tmp(*this);
                        --(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Subtraction assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Decrement the ReverseIterator `rhs` times.
                     */
                    ReverseIterator & operator-=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            --(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ReverseIterator
                     *
                     * Decrement `lhs` by `rhs`.
                     */
                    friend ReverseIterator operator-(ReverseIterator lhs, size_t rhs)
                    {
                        lhs -= rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ReverseIterator
                     *
                     * Decrement `rhs` by `lhs`.
                     */
                    friend ReverseIterator operator-(size_t lhs, ReverseIterator rhs)
                    {
                        return rhs - lhs;
                    }
            };
            /*!
             * \class ConstReverseIterator
             * \brief A `const` reverse iterator implementation.
             */
            class ConstReverseIterator final
            {
                friend class DoublyLinkedList;

                private:
                    Node * node;

                public:
                    /*!
                     * \brief Default constructor.
                     *
                     * \warning Should not be dereferenced (Undefined Behaviour).
                     */
                    ConstReverseIterator() : node(nullptr)
                    {}
                    /*!
                     * \brief Copy constructor.
                     * \param[in] crit The ConstReverseIterator to copy
                     */
                    ConstReverseIterator(const ConstReverseIterator & crit) : node(crit.node)
                    {}
                    /*!
                     * \brief Assignment operator.
                     * \param[in] other The ConstReverseIterator to copy
                     * \return A reference to `*this`
                     */
                    ConstReverseIterator & operator=(const ConstReverseIterator & other)
                    {
                        if(this != &other)
                        {
                            node = other.node;
                        }
                        return *this;
                    }
                    /*!
                     * \brief Equality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if equality, `false` otherwise
                     */
                    friend bool operator==(const ConstReverseIterator & lhs, const ConstReverseIterator & rhs)
                    {
                        return lhs.node == rhs.node;
                    }
                    /*!
                     * \brief Inequality operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return `true` if inequality, `false` otherwise
                     */
                    friend bool operator!=(const ConstReverseIterator & lhs, const ConstReverseIterator & rhs)
                    {
                        return !(lhs == rhs);
                    }
                    /*!
                     * \brief Indirection (dereference) operator.
                     * \return A `const` reference to the pointed value
                     *
                     * \note Dereference an out-of-range ConstReverseIterator is Undefined Behaviour.
                     */
                    const T & operator*() const
                    {
                        return node->value;
                    }
                    /*!
                     * \brief Member of pointer (dereference) operator.
                     * \return The address of the pointed value as `const`
                     *
                     * \note Dereference an out-of-range ConstReverseIterator is undefined Behaviour.
                     */
                    const T * operator->() const
                    {
                        return &(node->value);
                    }
                    /*!
                     * \brief Prefix increment operator.
                     * \return A reference to the incremented `*this`
                     *
                     * Shift to the previous element.
                     */
                    ConstReverseIterator & operator++() //prefix
                    {
                        if(node)
                            node = node->previous;
                        return *this;
                    }
                    /*!
                     * \brief Postfix increment operator.
                     * \return The before-increment ConstReverseIterator
                     *
                     * Shift to the previous element.
                     */
                    ConstReverseIterator operator++(int) //postfix
                    {
                        ConstReverseIterator tmp(*this);
                        ++(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Addition assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Increment the ConstReverseIterator `rhs` times.
                     */
                    ConstReverseIterator & operator+=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            ++(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstReverseIterator
                     *
                     * Increment `lhs` by `rhs`.
                     */
                    friend ConstReverseIterator operator+(ConstReverseIterator lhs, size_t rhs)
                    {
                        lhs += rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Addition operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstReverseIterator
                     *
                     * Increment `rhs` by `lhs`.
                     */
                    friend ConstReverseIterator operator+(size_t lhs, ConstReverseIterator rhs)
                    {
                        return rhs + lhs;
                    }
                    /*!
                     * \brief Prefix decrement operator.
                     * \return A reference to the decremented `*this`
                     *
                     * Shift to the next element.
                     */
                    ConstReverseIterator & operator--() //prefix
                    {
                        if(node)
                            node = node->next;
                        return *this;
                    }
                    /*!
                     * \brief Postfix decrement operator.
                     * \return The before-decrement ConstReverseIterator
                     *
                     * Shift to the next element.
                     */
                    ConstReverseIterator operator--(int) //postfix
                    {
                        ConstReverseIterator tmp(*this);
                        --(*this);
                        return tmp;
                    }
                    /*!
                     * \brief Subtraction assignment operator.
                     * \param[in] rhs The right-hand side
                     * \return A reference to `*this`
                     *
                     * Decrement the ConstReverseIterator `rhs` times.
                     */
                    ConstReverseIterator & operator-=(size_t rhs)
                    {
                        for(size_t i = 0; i < rhs; ++i)
                        {
                            --(*this);
                        }
                        return *this;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstReverseIterator
                     *
                     * Decrement `lhs` by `rhs`.
                     */
                    friend ConstReverseIterator operator-(ConstReverseIterator lhs, size_t rhs)
                    {
                        lhs -= rhs;
                        return lhs;
                    }
                    /*!
                     * \brief Subtraction operator.
                     * \param[in] lhs The left-hand side
                     * \param[in] rhs The right-hand side
                     * \return The resulting ConstReverseIterator
                     *
                     * Decrement `rhs` by `lhs`.
                     */
                    friend ConstReverseIterator operator-(size_t lhs, ConstReverseIterator rhs)
                    {
                        return rhs - lhs;
                    }
            };

            /*!
             * \brief Get an iterator referring to the first element.
             * \return An iterator
             */
            Iterator begin()
            {
                Iterator it;
                it.node = head_;
                return it;
            }
            /*!
             * \brief Get an iterator referring to the _past-the-end_ element.
             * \return An iterator
             */
            Iterator end()
            {
                Iterator it;
                it.node = tail_->next; //nullptr
                return it;
            }
            /*!
             * \brief Get a `const` iterator referring to the first element.
             * \return A `const` iterator
             */
            ConstIterator cbegin() const
            {
                ConstIterator cit;
                cit.node = head_;
                return cit;
            }
            /*!
             * \brief Get a `const` iterator referring to the _past-the-end_ element.
             * \return A `const` iterator
             */
            ConstIterator cend() const
            {
                ConstIterator cit;
                cit.node = tail_->next; //nullptr
                return cit;
            }
            /*!
             * \brief Get a reverse iterator referring to the last element.
             * \return A reverse iterator
             */
            ReverseIterator rbegin()
            {
                ReverseIterator rit;
                rit.node = tail_;
                return rit;
            }
            /*!
             * \brief Get a reverse iterator referring to the _preceding-the-beginning_ element.
             * \return A reverse iterator
             */
            ReverseIterator rend()
            {
                ReverseIterator rit;
                rit.node = head_->previous; //nullptr
                return rit;
            }
            /*!
             * \brief Get a `const` reverse iterator referring to the last element.
             * \return A `const` reverse iterator
             */
            ConstReverseIterator crbegin() const
            {
                ConstReverseIterator crit;
                crit.node = tail_;
                return crit;
            }
            /*!
             * \brief Get a `const` reverse iterator referring to the _preceding-the-beginning_ element.
             * \return A `const` reverse iterator
             */
            ConstReverseIterator crend() const
            {
                ConstReverseIterator crit;
                crit.node = head_->previous; //nullptr
                return crit;
            }
            //extras
            /*!
             * \brief Get a `const` iterator referring to the first element.
             * \return A `const` iterator
             */
            ConstIterator begin() const
            {
                return cbegin();
            }
            /*!
             * \brief Get a `const` iterator referring to the _past-the-end_ element.
             * \return A `const` iterator
             */
            ConstIterator end() const
            {
                return cend();
            }
            /*!
             * \brief Get a `const` reverse iterator referring to the last element.
             * \return A `const` reverse iterator
             */
            ConstReverseIterator rbegin() const
            {
                return crbegin();
            }
            /*!
             * \brief Get a `const` reverse iterator referring to the _preceding-the-beginning_ element.
             * \return A `const` reverse iterator
             */
            ConstReverseIterator rend() const
            {
                return crend();
            }

            // Iterator conversions
            /*!
             * \brief Iterator conversion.
             * \param[in] it An Iterator to convert
             * \return The converted iterator
             *
             * \note An Iterator can only be casted into an Iterator, a ConstIterator, a ReverseIterator or a ConstReverseIterator.
             */
            template <typename IT_type>
            static IT_type iterator_cast(const Iterator & it)
            {
                static_assert((std::is_same<IT_type, Iterator>::value || std::is_same<IT_type, ConstIterator>::value || std::is_same<IT_type, ReverseIterator>::value || std::is_same<IT_type, ConstReverseIterator>::value), "manual::DoublyLinkedList::iterator_cast - Forbidden template type provided.");

                IT_type res;
                res.node = it.node;
                return res;
            }
            /*!
             * \brief ReverseIterator conversion.
             * \param[in] rit A ReverseIterator to convert
             * \return The converted iterator
             *
             * \note A ReverseIterator can only be casted into a ReverseIterator, a ConstReverseIterator, an Iterator or a ConstIterator.
             */
            template <typename IT_type>
            static IT_type iterator_cast(const ReverseIterator & rit)
            {
                static_assert((std::is_same<IT_type, ReverseIterator>::value || std::is_same<IT_type, ConstReverseIterator>::value || std::is_same<IT_type, Iterator>::value || std::is_same<IT_type, ConstIterator>::value), "manual::DoublyLinkedList::iterator_cast - Forbidden template type provided.");

                IT_type res;
                res.node = rit.node;
                return res;
            }
            /*!
             * \brief ConstIterator conversion.
             * \param[in] cit A ConstIterator to convert
             * \return The converted iterator
             *
             * \note A ConstIterator can only be casted into a ConstIterator or a ConstReverseIterator.
             */
            template <typename IT_type>
            static IT_type iterator_cast(const ConstIterator & cit)
            {
                static_assert((std::is_same<IT_type, ConstIterator>::value || std::is_same<IT_type, ConstReverseIterator>::value), "manual::DoublyLinkedList::iterator_cast - Forbidden template type provided.");

                IT_type res;
                res.node = cit.node;
                return res;
            }
            /*!
             * \brief ConstReverseIterator conversion.
             * \param[in] crit A ConstReverseIterator to convert
             * \return The converted iterator
             *
             * \note A ConstReverseIterator can only be casted into a ConstReverseIterator or a ConstIterator.
             */
            template <typename IT_type>
            static IT_type iterator_cast(const ConstReverseIterator & crit)
            {
                static_assert((std::is_same<IT_type, ConstReverseIterator>::value || std::is_same<IT_type, ConstIterator>::value), "manual::DoublyLinkedList::iterator_cast - Forbidden template type provided.");

                IT_type res;
                res.node = crit.node;
                return res;
            }
    };
}

#endif // MANUAL_DOUBLYLINKEDLIST_H
