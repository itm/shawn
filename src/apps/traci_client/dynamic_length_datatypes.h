/** \file
 *  Definitions of the composed TraCI data types.
 *
 *  \author Björn Hendriks
 */

#ifndef DYNAMIC_LENGTH_DATATYPE_H
#define DYNAMIC_LENGTH_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "tracitype.h"
#include "atomic_datatypes.h"
#include "composed_datatypes.h"
#include "mobility_interface_constants.h"
#include <deque>


namespace traci
{


/**
 *  Base class for all TraCI types with a dynamic length list.
 *
 *  This template is a container to store the list elements and knows the
 *  particular traci::CompoundTraciType for the length value and its container type.
 *
 *  This template is designed such that its instantiation can be used as argument
 *  \p DynamicLengthValueType of MessageHandlerDynamicLength.
 *
 *  \tparam ElementType     type for element of list derived from TraciType
 *  \tparam TheLengthType   integer type derived from TraciType to store the length of the list
 *  \tparam Container       container template to store the elements itself, its only
 *                          requirement is to provide a resize() like the sequence containers of the STL
 */
template < class ElementType, class TheLengthType = IntegerType,
           template <class T, class = std::allocator<T> > class Container = std::vector >
struct DynamicLengthValue : public Container<ElementType>
{
    typedef TheLengthType LengthType;
};


/**
 *  Message handler for dynamic length lists of TraCI types.
 *
 *  The read/write functions read/write first the length and then the respective number
 *  of elements.
 *
 *  To do this the \p DynamicLengthValueType has to provide a container which
 *  can be accessed with STL iterators and has elements based on TraciType.
 *  Moreover \p DynamicLengthValueType has to provide functions to get/set the
 *  length of the list.
 *
 *  \todo   Put the iterations in a general private function and pass it a pointer
 *          to the read or write function to use.
 *          A problem to solve would be to find the proper pointer to memeber
 *          function pointer. (see similar todo at MessageHandlerComposed)
 *
 *  \tparam DynamicLengthValueType  TraCI value based on template DynamicLengthValue
 *  \tparam TraciTypeId             TraCI ID of the particular type
 */
template < class DynamicLengthValueType, id::DataType TraciTypeId = id::TypeCompound >
struct MessageHandlerDynamicLength : public MessageHandler<DynamicLengthValueType, TraciTypeId>
{
    virtual std::size_t getLength( const DynamicLengthValueType &dynamicLengthValue ) const
    {
        std::size_t length = 0;
        // we construct a complete LengthType to be sure to get the correct length
        // even if the type is somehow dynamic
        length += typename DynamicLengthValueType::LengthType( dynamicLengthValue.size() ).getLength();

        // we have to iterate through all elements because their actual length
        // might depend on their value (e.g. for StringType)
        for( typename DynamicLengthValueType::const_iterator it = dynamicLengthValue.begin();
                    it != dynamicLengthValue.end();
                    ++it )
        {
            length += it->getLength();
        }
        return length;
    }

    virtual void print( const DynamicLengthValueType & dynamicLengthValue, std::ostream & out = std::cout ) const
    {
        out << "(list of " << dynamicLengthValue.size() << " values: ";
        for( typename DynamicLengthValueType::const_iterator it = dynamicLengthValue.begin();
                    it != dynamicLengthValue.end();
                    ++it )
        {
            if ( it != dynamicLengthValue.begin() )
                out << ", ";
            it->print(out);
        }
        out << ')';
    }

    virtual void read( TraciMessage & message, DynamicLengthValueType & dynamicLengthValue ) const
    {
        typename DynamicLengthValueType::LengthType length(message);
        dynamicLengthValue.resize( length );
        for( typename DynamicLengthValueType::iterator it = dynamicLengthValue.begin();
                    it != dynamicLengthValue.end();
                    ++it )
        {
            it->read( message );
        }
    }

    virtual void write( TraciMessage & message, const DynamicLengthValueType & dynamicLengthValue ) const
    {
        typename DynamicLengthValueType::LengthType( dynamicLengthValue.size() ).write( message );
        for( typename DynamicLengthValueType::const_iterator it = dynamicLengthValue.begin();
                    it != dynamicLengthValue.end();
                    ++it )
        {
            it->write( message );
        }
    }
};


/**
 *  Composed value type for elements of TraCI type polygon.
 */
struct PolygonPoint
{
    DoubleType x;
    DoubleType y;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&x);
        queue.push_back(&y);
    }
};
/// TraciType for polygon elements
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<PolygonPoint> > > PolygonPointType;
/// Container for polygon points
typedef DynamicLengthValue<PolygonPointType, UbyteType> PolygonPointContainer;
/// Message handler for polygons
typedef MessageHandlerDynamicLength< PolygonPointContainer, id::TypePolygon > MessageHandlerPolygon;
/// TraciType for polygon without type info
typedef CompoundTraciType< MessageHandlerPolygon > PolygonType;
/// TraciType for polygon with type info
typedef CompoundTraciType< MessageHandlerPolygon, true > PolygonTypeWithTypeId;




}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // DYNAMIC_LENGTH_DATATYPE_H

