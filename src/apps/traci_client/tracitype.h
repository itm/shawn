/** \file
 *  Definitions of the TraciType class hierarchy to handle TraCI data structures.
 *
 *  For an overview see \ref tracitypes.
 *
 *  \author Björn Hendriks
 */

#ifndef TRACITYPE_H
#define TRACITYPE_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <iostream>
#include "mobility_interface_constants.h"
#include "apps/tcpip/storage.h"

namespace traci
{

/** \page   tracitypes  The TraciType Class System

This text will give an overview over the TraciType class system. Detailed
information can be found in the documentation of the various classes and their
elements.

The design goal of the TraciType class system is to provide an easy to use
access to the data of the TraCI protocol. TraciTypes can transparently be used
like the contained value itself but add extra functionality to read/write the
value from/to a TraCI message with or without the TraCI type ID.

Examples:

\code
    IntegerType x( storage );   // read integer from TraCI message storage
    int y = x + 1;              // use x transparently
    x = 2 * y;                  // assign value to x
    x.write( storage2 );        // write new x value into another message
                                // storage
\endcode

Example with a class type:

\code
    Position2DTypeWithTypeId pos( storage );    // read 2DPosition inluding
                                                // type ID from storage
    float x = pos.x;                            // get x coordinate
    pos.setWithTypeId( false );                 // turn off type ID
    pos.write( storage2 );                      // write position without type
                                                // ID to anoterh storage
\endcode

The basic interface to those TraciTypes defines class TraciType. Derived from
that is template class GenTraciType which implements the general part of
TraciType. Derived from GenTraciType are the two templates FundamentalTraciType
and CompoundTraciType. FundamentalTraciType handles values of fundamental C++
types and CompoundTraciType handles class/struct values.

The main parameter of those templates is class MyMessageHandler which provides
the back end to handle the specific type. The concrete message handler should be
derived from template MessageHandler which defines the interface a
message handler should provide.

To add a new TraCI data type you only have to provide a MessageHandler derived
message handler implementing its abstract element functions. To make this
easier there are special message handler templates to build new TraciTypes out
of existing TraciTypes. See headers atomic_datatypes.h, composed_datatypes.h,
dynamic_length_datatypes.h, and complex_datatypes.h.

*/



/**
 *  Parameter type for all TraCI read/write functions.
 *
 *  A reference of this type is passed to all read and write functions.
 *
 *  This makes it easier to change the transfer medium in the future. Only the
 *  read/write functions in the basic message handlers have to be adapted.
 *
 *  To change the message class just adapt this typedef and let the compiler
 *  tell you where elements of TraciMessage are accessed and need to be adapted.
 */
typedef tcpip::Storage TraciMessage;


/**
 *  Abstract base of all message handlers for GenTraciType.
 *
 *  The message handler is the back end of TraciType. It knows the concrete type
 *  and how to read/write it from/to the TraCI message.
 *
 *  This class provides the value type itself, its TraCI ID and the interfaces
 *  to functions to read/write it from/to TraciMessage.
 *
 *  For an overview see \ref tracitypes.
 *
 *  \tparam T               type of the value to store
 *  \tparam TraciTypeId     TraCI ID of the type
 */
template <typename T, id::DataType TraciTypeId>
struct MessageHandler
{
    /// Make value type accessible.
    typedef T ValueType;

    /**
     *  Get TraCI type ID.
     *
     *  We pass a reference to a value to allow derived MessageHandlers to make the
     *  type ID dynamically dependent of the value.
     *
     *  \return     template argument \p TraciTypeId
     */
    virtual id::DataType getTraciTypeId( const ValueType & value ) const
    {
        return TraciTypeId;
    }

    /**
     *  Get length of MessageHandler::ValueType in TraciMessage.
     *
     *  We pass a reference to a value to allow derived MessageHandlers to make the
     *  length dynamically dependent of the actual value.
     *
     *  \attention  Check for each type if this implementation with sizeof() is
     *              sufficient. Otherwise override this function.
     *
     *  \return     sizeof( ValueType )
     */
    virtual std::size_t getLength( const ValueType & value ) const
    {
        return sizeof(ValueType);
    }

    /**
     *  Print MessageHandler::ValueType into a std::ostream.
     *
     *  This function is for debugging and error message purposes. There is no way
     *  to do any output formatting.
     *
     *  \attention  The stream's state should not be changed, i.e., if flags, fill,
     *              etc. are changed they should be reset before returning.
     *
     *  Used by TraciType::print.
     *
     *  \param  value   the value to print
     *  \param  out     [defaults to std::cout] std::ostream to print into
     */
    virtual void print( const ValueType &value, std::ostream & out = std::cout) const = 0;

    /// Read a value from a TraciMessage into \p value
    virtual void read( TraciMessage & message, ValueType & value ) const = 0;
    /// Write \p value to a TraciMessage.
    virtual void write( TraciMessage & message, const ValueType & value ) const = 0;
};


/**
 *  Interface for all TraCI type classes.
 *
 *  We need this interface to handle all specializations of GenTraciType
 *  in a polymorphic way.
 *
 *  For an overview see \ref tracitypes.
 */
class TraciType
{
public:
    /// Get TraCI type ID.
    virtual id::DataType getTraciTypeId() const = 0;
    /// Should type be read/written with type ID?
    virtual bool getWithTypeId() const = 0;
    /// Set if type should be read/written with type ID and return old setting
    virtual bool setWithTypeId( bool withTypeId ) = 0;

    /// Make a clone of this object on the heap (to be destroyed with delete)
    virtual TraciType *clone() const = 0;

    /// Compare values.
    virtual bool operator==( const TraciType & other ) const = 0;
    /// Compare values.
    virtual bool operator!=( const TraciType & other ) const
    {
        return !operator==(other);
    }

    /// Get length in TraciMessage.
    virtual std::size_t getLength() const = 0;
    /// Print value into stream.
    virtual void print( std::ostream & out = std::cout ) const = 0;
    /// Read value from TraciMessage.
    virtual void read( TraciMessage & message ) = 0;
    /// Write value to TraciMessage.
    virtual void write( TraciMessage & message ) const = 0;
};


/**
 * Smart pointer for TraciType
 *
 * All instances of this smart pointer keep their own clone of the TraciType
 * pointed to.
 *
 * TraciTypePtr::valuePtr always contains a valid pointer, hence, there is no
 * default c'tor.
 */
class TraciTypePtr
{
public:
    /// Store a pointer to a clone of \p other
    TraciTypePtr(const TraciTypePtr & other) :
        valuePtr(other->clone())
    {
    }

    /// Store a pointer to a clone of \p other
    TraciTypePtr(const TraciType & other) :
        valuePtr(other.clone())
    {
    }

    /// Destroy the object pointed to
    ~TraciTypePtr()
    {
        delete valuePtr;
    }

    /// Replace the stored TraciType by a clone of \p other
    TraciTypePtr & operator= (const TraciTypePtr & other)
    {
        if (&other != this)
        {
            delete valuePtr;
            valuePtr = other->clone();
        }
        return *this;
    }

    /// Access the stored TraciType
    TraciType & operator* () const
    {
        return *valuePtr;
    }

    /// Access the stored TraciType
    TraciType * operator-> () const
    {
        return valuePtr;
    }

    /// Compare stored TraciType with value in \p other
    bool operator== (const TraciTypePtr & other) const
    {
        return *other == *valuePtr;
    }

    /// Compare stored TraciType with \p other
    bool operator== (const TraciType & other) const
    {
        return other == *valuePtr;
    }

protected:
    /// Pointer to the stored TraciType
    TraciType * valuePtr;
};





/**
 *  This class represents a TraCI data type.
 *
 *  This template defines the front end to handle all TraCI data types as easy
 *  as possible by providing all functions needed to treat it as the underlying
 *  value type, i.e., assignments and type casts are transparent.
 *
 *  If the value is read/written with or without type ID is determined by element
 *  variable GenTraciType::withTypeId. The initial value of this member variable
 *  will be set by template argument \p WithTypeId but it could be changed
 *  afterwards.
 *
 *  The reason for \p WithTypeId as template parameter is that the type
 *  declaration is sufficient to make sure
 *  that GenTraciType::withTypeId has the right value even if you don't explicitly
 *  call its constructor or setWithTypeId() function. This makes it easier to
 *  implement aggregates of GenTraciType.
 *
 *  A downside of \p WithTypeId as template parameter is that different values
 *  lead to different C++ types even if the message handlers are the same. It might
 *  even be that GenTraciType::withTypeId of the current object is the same
 *  because of a later call to setWithTypeId() although the C++ types are different.
 *  Different C++ types means that on copy construction or assignment different
 *  c'tors or operator= will be called. To make the behavior independent of the
 *  template instantiation used the real copy constructor and assignment operator
 *  are defined such that they never copy the value of GenTraciType::withTypeId.
 *
 *  The template parameter \p MyMessageHandler contains the backend to handle
 *  the particular value and should be derived from an instantiation of template
 *  MessageHandler. The elements of that interface are made accessible here.
 *
 *  All member functions use getValue() and getValueWritable() functions to access
 *  the member variables. So you can easily provide
 *  your own variables by simply overwriting only those functions.
 *
 *  The definitions of the template member functions are moved to a separate file
 *  to resolve a circular dependancy. See documentation of file tracitype.cpp
 *  for details.
 *
 *  For an overview see \ref tracitypes.
 *
 *  \todo   Make GenTraciType::value a reference to avoid unnecessary copying of
 *          complex values but take care that it is not unsafe.
 *
 *  \tparam MyMessageHandler    backend derived from MessageHandler
 *  \tparam WithTypeId          initial value of GenTraciType::withTypeId
 */
template < class MyMessageHandler, bool WithTypeId = false >
class GenTraciType : public TraciType
{
protected:
    /// Type of this class.
    typedef GenTraciType<MyMessageHandler, WithTypeId> ThisGenTraciType;

public:
    /// Type of stored value.
    typedef typename MyMessageHandler::ValueType ValueType;

    // We cannot implement a c'tor GenTraciType( TraciMessage & message ) because
    // at construction time we have no access to getValueWritable() of derived classes.

    /// Default constructor.
    GenTraciType();
    /// Copy constructor (doesn't copy GenTraciType::withTypeId).
    GenTraciType( const ThisGenTraciType & other );
    /// Destructor.
    virtual ~GenTraciType();

    /** \name Value access functions
     *
     *  The GenTraciType::ValueType assignment and conversion operators and the
     *  GenTraciType::ValueType constructor allow to use this
     *  object like an object of type GenTraciType::ValueType.
     */
    //@{
    /// Assign another GenTraciType value
    template < class OtherMessageHandler, bool OtherWithTypeId >
    ThisGenTraciType & operator= ( const GenTraciType<OtherMessageHandler, OtherWithTypeId> & other );
    /// Assign another value.
    virtual ThisGenTraciType & operator= ( const ValueType & aValue );
    /// Get a const reference to stored value.
    virtual const ValueType & getValue() const = 0;
    /// Get a non-const reference to the stored value.
    virtual ValueType & getValueWritable() = 0;
    /// Set stored value.
    virtual void setValue( const ValueType & aValue );
    /// Compare with other value
    virtual bool operator==( const TraciType & other ) const;
    /// Get TraCI type ID.
    virtual id::DataType getTraciTypeId() const;
    /// Get GenTraciType::withTypeId.
    virtual bool getWithTypeId() const;
    /// Set GenTraciType::withTypeId and return old value
    virtual bool setWithTypeId( bool aWithTypeId );
    //@}

    /** \name Read/write message functions
     *
     *  \param  message     reference to TraciMessage
     */
    //@{
    /// Get length in TraciMessage.
    virtual std::size_t getLength() const;
    /// Print value into std::ostream.
    virtual void print( std::ostream & out = std::cout ) const;
    /// Read value from \p message.
    virtual void read( TraciMessage & message );
    /// Write value to \p message.
    virtual void write( TraciMessage & message ) const;
    //@}

protected:
    /// Read type from \p message and check correctness if getWithTypeId() is \c true.
    virtual void readType( TraciMessage & message ) const;
    /// Write type to \p message if getWithTypeId() is \c true.
    virtual void writeType( TraciMessage & message ) const;

private:
    /// Should GenTraciType::value be read/written from/to with type ID or not.
    bool withTypeId;
};


//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


/**
 *  Specialization of GenTraciType for fundamental C++ type values.
 *
 *  This class stores its value as private element and provides type conversion
 *  constructor and operator to access it besides getter and setter.
 */
template < class MyMessageHandler, bool WithTypeId = false >
class FundamentalTraciType : public GenTraciType< MyMessageHandler, WithTypeId >
{
protected:
    /// Type of this class.
    typedef FundamentalTraciType< MyMessageHandler, WithTypeId > ThisFundamentalTraciType;
    /// Type of one base class
    typedef GenTraciType< MyMessageHandler, WithTypeId > ThisGenTraciType;

public:
    /// Type of stored value.
    typedef typename MyMessageHandler::ValueType ValueType;

    /// Inherit assignment operator
    using ThisGenTraciType::operator=;

    /// Default constructor
    FundamentalTraciType() :
        ThisGenTraciType(),
        value()
    {
    }

    /// Copy constructor
    FundamentalTraciType( const ThisFundamentalTraciType & other ) :
        ThisGenTraciType( other ),
        value( other.getValue() )
    {
    }

    /// Initialize by value.
    FundamentalTraciType( const ValueType & aValue ) :
        ThisGenTraciType(),
        value(aValue)
    {
    }

    /// Initialize from message.
    FundamentalTraciType( TraciMessage & message ) :
        ThisGenTraciType(),
        value()
    {
        ThisGenTraciType::read( message );
    }

    /// Clone object to heap
    virtual TraciType *clone() const
    {
        return new ThisFundamentalTraciType( *this );
    }

    /// Get const reference to stored value.
    virtual const ValueType & getValue() const
    {
        return value;
    }

    /// Get non-const reference to stored value.
    virtual ValueType & getValueWritable()
    {
        return value;
    }

    /** Convert this object to its contained value.
     *
     *  This function allows to use this
     *  object like an object of type GenTraciType::ValueType.
     */
    virtual operator ValueType() const
    {
        return getValue();
    }

private:
    ValueType value;
};


//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


/**
 *  Specialization of GenTraciType for class and struct values.
 *
 *  The value is set as base class to make direct access possible. Also a
 *  disambiguation of compare operators is provided.
 *
 *  Normally both base classes have an operator= so we have to disambiguate
 *  which one to use although both would yield the same result. std::string
 *  as ValueType is an actual exception so we have to choose the operator=
 *  from GenTraciType. The same applies for operator== and operator!=.
 */
template < class MyMessageHandler, bool WithTypeId = false >
class CompoundTraciType :
    public GenTraciType< MyMessageHandler, WithTypeId >,
    public MyMessageHandler::ValueType
{
protected:
    /// Type of this class.
    typedef CompoundTraciType< MyMessageHandler, WithTypeId > ThisCompoundTraciType;
    /// Type of one base class
    typedef GenTraciType< MyMessageHandler, WithTypeId > ThisGenTraciType;

public:
    /// Type of stored value.
    typedef typename MyMessageHandler::ValueType ValueType;

    /// Default constructor
    CompoundTraciType() :
        ThisGenTraciType(),
        ValueType()
    {
    }

    /// Copy constructor
    CompoundTraciType( const ThisCompoundTraciType & other ) :
        ThisGenTraciType( other ),
        ValueType( other.getValue() )
    {
    }

    /// Initialize by value.
    CompoundTraciType( const ValueType & aValue ) :
        ThisGenTraciType(),
        ValueType(aValue)
    {
    }

    /// Initialize from message.
    CompoundTraciType( TraciMessage & message ) :
        ThisGenTraciType(),
        ValueType()
    {
        ThisGenTraciType::read( message );
    }

    /// Clone object to heap
    virtual TraciType *clone() const
    {
        return new ThisCompoundTraciType( *this );
    }

    /// Get reference to stored value.
    virtual const ValueType & getValue() const
    {
        return dynamic_cast< const ValueType & >(*this);
    }

    /// Get non-const reference to stored value.
    virtual ValueType & getValueWritable()
    {
        return dynamic_cast< ValueType & >(*this);
    }

    // see class documentation for explanation
    using ThisGenTraciType::operator=;
    using ThisGenTraciType::operator==;
    using ThisGenTraciType::operator!=;
};



/**
 *  Base class for all specializations of VariableTraciType
 *
 *  Makes the template parameters accessible at run time.
 */
template <unsigned char Command, unsigned char Variable, class TheTraciType>
struct VariableTraciTypeBase
{
    typedef TheTraciType Type;

    unsigned char getCommand() const
    {
        return Command;
    }

    unsigned char getVariable() const
    {
        return Variable;
    }
};



/**
 *  General template for mapping command/variable pairs to the corresponding TraCI
 *  data type.
 *
 *  All implemented specializations should pass the TraciType belonging to the
 *  special command/variable pair as \tparam TheTraciType to the base class.
 */
template <unsigned char Command, unsigned char Variable>
struct VariableTraciType : public VariableTraciTypeBase<Command, Variable, void>
{
};



}   // namespace traci


/// this includes tracitype_implementations.h at the right place (see its documentation)
#include "atomic_datatypes.h"


#endif  // #ifdef ENABLE_TRACICLIENT
#endif // TRACITYPE_H

