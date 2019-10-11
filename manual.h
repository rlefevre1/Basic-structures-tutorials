#ifndef MANUAL_H
#define MANUAL_H

/*!
 * \file manual.h
 * \brief All-in-one header for the manual namespace
 * \author Raphaël Lefèvre
 */

#include "linkedlist.h"
#include "doublylinkedlist.h"

/*!
 * \namespace manual
 * \brief Namespace which contains manual implementations of basic structures.
 */
namespace manual
{
    // Convenience typedefs
    template <typename T> using List = LinkedList<T>;        /*!< Convenience `typedef` of LinkedList */
    template <typename T> using DList = DoublyLinkedList<T>; /*!< Convenience `typedef` of DoublyLinkedList */
}

/*!
 * \mainpage Manual implementation of basic structures (Tutorial-use)
 *
 * - The purpose of this library is to provide an implementation of common basic structures in order to show the mechanisms behind.<br/>
 * It is not meant to be used outside of tutorials/learning purposes (There are already existing implementations which are more complete and more efficient, especially in the standard library).
 *
 * - The library lives in the namespace ::manual.
 *
 * - To include the whole library, it is possible to include the all-in-one header manual.h.<br/>
 * Moreover it provides some convenience `typedef`.
 */

#endif // MANUAL_H
