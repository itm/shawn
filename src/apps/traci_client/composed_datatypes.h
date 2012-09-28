/** \file
 *  Definitions of the composed TraCI data types.
 *
 *  \author Björn Hendriks
 */

#ifndef COMPOSED_DATATYPE_H
#define COMPOSED_DATATYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "tracitype.h"
#include "atomic_datatypes.h"
#include "mobility_interface_constants.h"
#include <deque>


namespace traci
{


/// Container type for composed TraCI types.
typedef std::deque<TraciType *> ComposedContainer;
/// Container type for composed constant TraCI types.
typedef std::deque<const TraciType *> ConstComposedContainer;


/**
 *  Message handler for composed types with constant values
 *
 *  \tparam ComposedValueType needs to be a container with (constant) pointers to TraciType
 *  as elements (or at least elements with an operator-> returning something derived
 *  from TraciType and an element function size()).
 *
 *  \tparam ComposedValueType   container with pointers to TraciType
 *  \tparam TraciTypeId         TraCI ID of the particular type
 */
template < class ComposedValueType, id::DataType TraciTypeId = id::TypeCompound >
struct ConstMessageHandlerComposed : public MessageHandler<ComposedValueType, TraciTypeId>
{
    virtual std::size_t getLength( const ComposedValueType &composedValue ) const
    {
        std::size_t length = 0;
        for( typename ComposedValueType::const_iterator it = composedValue.begin(); it != composedValue.end(); ++it )
        {
            checkIterator(it);
            length += (*it)->getLength();
        }
        return length;
    }

    virtual void print( const ComposedValueType &composedValue, std::ostream & out = std::cout ) const
    {
        out << "(composition of " << composedValue.size() << " values: ";
        for( typename ComposedValueType::const_iterator it = composedValue.begin(); it != composedValue.end(); ++it )
        {
            checkIterator(it);
            if ( it != composedValue.begin() )
                out << ", ";
            (*it)->print(out);
        }
        out << ')';
    }

    // TODO: Use template specializations to remove the definition of this function instead of throwing an exception
    virtual void read( TraciMessage & message, ComposedValueType & composedValue ) const
    {
        throw std::runtime_error( "traci::ConstMessageHandlerComposed: read called although we are const" );
    }

    virtual void write( TraciMessage & message, const ComposedValueType & composedValue ) const
    {
        for( typename ComposedValueType::const_iterator it = composedValue.begin(); it != composedValue.end(); ++it )
        {
            checkIterator(it);
            (*it)->write( message );
        }
    }

protected:
    void checkIterator(const typename ComposedValueType::const_iterator it) const
    {
        if ( NULL == *it )
            throw std::runtime_error( "traci::(Const)MessageHandlerComposed: null pointer in composedType" );
    }
};


/**
 * Extension of ConstMessageHandlerComposed for non-const values
 *
 * The template traci::ComposedValue is designed to provide an interface suitable
 * as \p ComposedValueType.
 */
template < class ComposedValueType, id::DataType TraciTypeId = id::TypeCompound >
struct MessageHandlerComposed : public ConstMessageHandlerComposed<ComposedValueType, TraciTypeId>
{
    // checkIterator is non-dependent, so, we need to tell the compiler to use it, anyway
    using ConstMessageHandlerComposed<ComposedValueType, TraciTypeId>::checkIterator;

    virtual void read( TraciMessage & message, ComposedValueType & composedValue ) const
    {
        for( typename ComposedValueType::iterator it = composedValue.begin(); it != composedValue.end(); ++it )
        {
            checkIterator(it);
            (*it)->read( message );
        }
    }
};


/**
 * This class is a TraciType containing pointers to arbitrary other TraciTypes.
 *
 * To fill it with data push TraciType pointers into the underlying traci::ComposedContainer
 * in the appropriate order.
 */
template < id::DataType TraciTypeId = id::TypeCompound, bool WithTypeId = false >
struct DynamicTraciType : public CompoundTraciType< MessageHandlerComposed< ComposedContainer, TraciTypeId >, WithTypeId >
{
};


/**
 * Variant of DynamicTraciType which accepts constant pointers.
 *
 * To fill it with data push TraciType pointers into the underlying traci::ConstComposedContainer
 * in the appropriate order.
 */
template < id::DataType TraciTypeId = id::TypeCompound, bool WithTypeId = false >
struct ConstDynamicTraciType : public CompoundTraciType< ConstMessageHandlerComposed< ConstComposedContainer, TraciTypeId >, WithTypeId >
{
};


/**
 *  This class combines the actual composed value with its traci::ComposedContainer.
 *
 *  The goal of the design of this class is to keep the actual value class
 *  as simple as possible. This is done by deriving from Value and traci::ComposedContainer
 *  and making sure that traci::ComposedContainer is properly initialised by defining
 *  the four default functions of a class.
 *
 *  To achieve this the class Value needs to define an accessible member function
 *  with the following declaration:
 *  \code
 *      void init( ComposedContainer &queue );
 *  \endcode
 *  This function should put pointers to its elements in the order defined by
 *  TraCI into the \p queue. (See traci::Position3D for an example.)
 *
 *  This template is designed such that its instantiations can be used as argument
 *  \p ComposedValueType of traci::MessageHandlerComposed.
 *
 *  \tparam Value   value class to derive from with elements of type TraciType
 */
template <class Value>
class ComposedValue : public ComposedContainer, public Value
{
public:
    ComposedValue() :
        ComposedContainer(),
        Value()
    {
        init();
    }

    ComposedValue(const Value & aValue) :
        ComposedContainer(),
        Value(aValue)
    {
        init();
    }

    ComposedValue(const ComposedValue &other) :
        ComposedContainer(),
        Value(other)
    {
        init();
    }

    ComposedValue &operator=(const ComposedValue &other)
    {
        if( &other != this )
        {
            Value::operator=( other );  // copy only values
        }
        return *this;
    }

    /// Make sure the d'tor is virtual
    virtual ~ComposedValue()
    {
    }

    /** Compare all elements.
     *
     *  We need our own compare operator because ComposedContainer contains
     *  pointers but we need to compare their values. Besides, if Value occasionally
     *  provides another operator== we would have an ambiguity the compiler
     *  couldn't resolve.
     */
    virtual bool operator==( const ComposedValue & other ) const
    {
        // compare all elements of the ComposedContainers
        for ( ComposedContainer::const_iterator myIt = begin(), otherIt = other.begin();
            (myIt != end()) && (otherIt != other.end());
            ++myIt, ++otherIt )
        {
            // ComposedContainer contains pointers so we have to dereference twice to get the values itself
            if ( **myIt != **otherIt )  // compare TraciType values
                return false;
        }
        return true;
    }

    virtual bool operator!=( const ComposedValue & other ) const
    {
        return !operator==(other);
    }

protected:
    void init()
    {
        ComposedContainer::clear(); // empty container first to be defensive
        Value::init( *this );
    }
};


/**
 *  Composed value type for TraCI type 3D Position.
 */
struct Position3D
{
    FloatType x;
    FloatType y;
    FloatType z;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&x);
        queue.push_back(&y);
        queue.push_back(&z);
    }
};
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<Position3D>, id::Position3d >, true > Position3DTypeWithTypeId;


/**
 *  Composed value type for TraCI type 3D Position.
 */
struct Position2D
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
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<Position2D>, id::Position2d >, true > Position2DTypeWithTypeId;


/**
 *  Composed value type for TraCI type Road Map Position.
 */
struct RoadMapPosition
{
    StringType road;
    FloatType pos;
    UbyteType lane;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&road);
        queue.push_back(&pos);
        queue.push_back(&lane);
    }
};
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<RoadMapPosition>, id::PositionRoadmap >, true > RoadMapPositionTypeWithTypeId;


/**
 *  Composed value type for TraCI type Boundary Box.
 */
struct BoundaryBox
{
    FloatType lowerLeftX;
    FloatType lowerLeftY;
    FloatType upperRightX;
    FloatType upperRightY;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&lowerLeftX);
        queue.push_back(&lowerLeftY);
        queue.push_back(&upperRightX);
        queue.push_back(&upperRightY);
    }
};
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<BoundaryBox>, id::TypeBoundingbox >, true > BoundaryBoxTypeWithTypeId;


/**
 *  Composed value type for TraCI type Color.
 */
struct Color
{
    UbyteType r;
    UbyteType g;
    UbyteType b;
    UbyteType a;

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&r);
        queue.push_back(&g);
        queue.push_back(&b);
        queue.push_back(&a);
    }
};
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<Color>, id::TypeColor >, true > ColorTypeWithTypeId;


/**
 *  Composed value type for status response
 */
struct StatusResponse
{
    enum
    {
        ResultSuccess           = 0x00,
        ResultNotImplemented    = 0x01,
        ResultFailure           = 0xff
    };

    UbyteType result;
    StringType description;
    bool isSuccess() const
    {
        return ResultSuccess == result;
    }

protected:
    /// set order of elements in TraCI type
    virtual void init( ComposedContainer &queue )
    {
        queue.push_back(&result);
        queue.push_back(&description);
    }
};
typedef CompoundTraciType< MessageHandlerComposed< ComposedValue<StatusResponse> > > StatusResponseType;




/**
 *  Message handler for composed types with leading number of elements
 *
 *  The read and write functions first read/write the number of elements and then
 *  call read/write of the base class.
 *
 *  After reading a message the number of elements of the passed ComposedValueType
 *  will be compared with the number contained in the mssage. If they do not match
 *  a std::runtime_error will be thrown.
 *
 *  The template traci::ComposedValue is designed to provide an interface suitable
 *  as \p ComposedValueType.
 *
 *  \tparam ComposedValueType   TraCI value derived from traci::ComposedContainer
 *  \tparam TraciTypeId         TraCI ID of the particular type
 */
template < class ComposedValueType, id::DataType TraciTypeId = id::TypeCompound >
struct MessageHandlerLeadingNum : public MessageHandlerComposed<ComposedValueType, TraciTypeId>
{
    typedef MessageHandlerComposed<ComposedValueType, TraciTypeId>  MyMessageHandlerComposed;
    typedef IntegerType NumberType;

    virtual std::size_t getLength( const ComposedValueType &composedValue ) const
    {
        const NumberType dummy;
        return dummy.getLength() + MyMessageHandlerComposed::getLength( composedValue );
    }

    virtual void read( TraciMessage & message, ComposedValueType & composedValue ) const
    {
        NumberType number;
        number.read( message );
        MyMessageHandlerComposed::read( message, composedValue );
        if (static_cast<NumberType::ValueType>(composedValue.size()) != number)
        {
            std::ostringstream msg;
            msg << "MessageHandlerLeadingNum: expected " << composedValue.size() << " elements in TraCI message, ";
            msg << "but TraCI message announced " << number << " elements";
            throw std::runtime_error(msg.str());
        }
    }

    virtual void write( TraciMessage & message, const ComposedValueType & composedValue ) const
    {
        NumberType number;
        number = composedValue.size();
        number.write( message );
        MyMessageHandlerComposed::write( message, composedValue );
    }
};




}   // namespace traci

#endif  // #ifdef ENABLE_TRACICLIENT
#endif // COMPOSED_DATATYPE_H

