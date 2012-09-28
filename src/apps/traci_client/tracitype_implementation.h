/** \file
 *  Definitions of some template member functions of GenTraciType.
 *
 *  \author Björn Hendriks
 *
 *  This template "implementation" file is included _after_ the definition
 *  of the class template.
 *
 *  Although member functions of templates normally have to be inline and hence
 *  are defined within the header we have to separate them to resolve circular
 *  dependency. Functions GenTraciType::readType and GenTraciType::writeType
 *  depend on an atomic type which is derived from GenTraciType. So the definitions
 *  of these functions have to be after the class definition in atomic_datatypes.h which
 *  has to be after class definition of GenTraciType.
 */


#ifndef TRACITYPE_IMPLEMENTATION_H
#define TRACITYPE_IMPLEMENTATION_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "tracitype.h"
#include <iomanip>

namespace traci
{


namespace
{
    /** TraciType to use for TraCI type ID
     *
     *  Unfortunately we cannot put this typedef into MessageHandler, TraciType
     *  or GenTraciType due to circular dependency of types. To hide it a little
     *  we put it at least into an anomnymous namespace.
     */
    typedef UbyteType TypeIdType;
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
GenTraciType<MyMessageHandler, WithTypeId>::GenTraciType() :
        TraciType(),
        withTypeId( WithTypeId )
{
}


//--------------------------------------------------------------------
/**
 *  GenTraciType::withTypeId will be taken from the template argument
 *  \p WithTypeId of this instantiation.
 *
 *  Copy constructs base class and elements.
 *
 *  We cannot copy the value here because the actual implementation of
 *  getValueWritable in a derived class is not accessible from a c'tor like this.
 */
template<class MyMessageHandler, bool WithTypeId>
GenTraciType<MyMessageHandler, WithTypeId>::GenTraciType( const ThisGenTraciType& other ) :
        TraciType( other ),
        withTypeId( WithTypeId )
{
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
GenTraciType<MyMessageHandler, WithTypeId>::~GenTraciType()
{
}


//--------------------------------------------------------------------
/**
 * The own GenTraciType::withTypeId will not be changed. Only the value itself
 * will be assigned by calling GenTraciType::operator=(const ValueType &)
 * explicitly. So, whenever an appropriate cast of \p other's value type is
 * available this assignment is possible.
 *
 * Derived classes can use this assignment operator by applying the using
 * directive on it.
 */
template<class MyMessageHandler, bool WithTypeId>
template < class OtherMessageHandler, bool OtherWithTypeId >
GenTraciType<MyMessageHandler, WithTypeId> &
GenTraciType<MyMessageHandler, WithTypeId>::operator= ( const GenTraciType<OtherMessageHandler, OtherWithTypeId> & other )
{
    if ( dynamic_cast<const ThisGenTraciType *>(&other) != this )
    {
        TraciType::operator=( other ),
        operator=( other.getValue() );
    }
    return *this;
}


//--------------------------------------------------------------------
/**
 *  By applying the abstract getValueWritable() this function can do the job for derived
 *  classes too.
 */
template<class MyMessageHandler, bool WithTypeId>
GenTraciType<MyMessageHandler, WithTypeId> &
GenTraciType<MyMessageHandler, WithTypeId>::operator= ( const ValueType & aValue )
{
    getValueWritable() = aValue;
    return *this;
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::setValue( const ValueType & aValue )
{
    getValueWritable() = aValue;
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
bool GenTraciType<MyMessageHandler, WithTypeId>::operator==( const TraciType & other ) const
{
    // try to cast other to our type
    try
    {
        const ThisGenTraciType &myOther =
            dynamic_cast<const ThisGenTraciType &>(other);
        return myOther.getValue() == getValue();
    }
    catch( std::bad_cast & )    // casting failed?
    {
        return false;
    }
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
bool GenTraciType<MyMessageHandler, WithTypeId>::getWithTypeId() const
{
    return withTypeId;
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
bool GenTraciType<MyMessageHandler, WithTypeId>::setWithTypeId( bool aWithTypeId )
{
    const bool oldWithTypeId = withTypeId;
    withTypeId = aWithTypeId;
    return oldWithTypeId;
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
id::DataType GenTraciType<MyMessageHandler, WithTypeId>::getTraciTypeId() const
{
    return MyMessageHandler().getTraciTypeId( getValue() );
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
std::size_t GenTraciType<MyMessageHandler, WithTypeId>::getLength() const
{
    std::size_t length = 0;

    // add length of type id if included
    if ( getWithTypeId() )
        length += TypeIdType( getTraciTypeId() ).getLength();

    // take length of actual value
    length += MyMessageHandler().getLength( getValue() );

    return length;
}


//--------------------------------------------------------------------
/**
 *  If withTypeId is set the ID will be added. Then MessageHandler::print
 *  will be called to do the actual work.
 */
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::print( std::ostream & out ) const
{
    if ( getWithTypeId() )
    {
        // save state of out
        const std::ostream::fmtflags currFlags = out.flags();
        const std::streamsize currWidth = out.width();
        const std::ostream::char_type currFill = out.fill();

        out << "[type 0x" << std::hex << std::setfill('0') << std::setw(2)
            << static_cast<int>(getTraciTypeId()) << "] ";

        // reset state of out
        out.flags( currFlags );
        out.width( currWidth );
        out.fill( currFill );
    }
    MyMessageHandler().print( getValue(), out );
}


//--------------------------------------------------------------------
/**
 *  This function like all other member functions uses GenTraciType::getValueWritable to store
 *  the read value. That means the value will be copied times if not
 *  optimized by the compiler. But that will be safer in case of deriving
 *  from this class.
 *
 *  If this is a performance problem derive from this class and implement your
 *  own value access and read functions.
 */
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::read( TraciMessage & message )
{
    readType( message );
    MyMessageHandler().read( message, getValueWritable() );
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::write( TraciMessage & message ) const
{
    writeType( message );
    MyMessageHandler().write( message, getValue() );
}


//--------------------------------------------------------------------
/**
 *  Throws an exception if the type is actually read but not correct.
 */
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::readType( TraciMessage & message ) const
{
    if ( getWithTypeId() )
    {
        const TypeIdType typeId( message );
        if ( typeId != getTraciTypeId() )
        {
            std::ostringstream msg;
            msg << std::hex << std::setfill('0')
                << "traci::GenTraciType::readType: read wrong value type, "
                << "expected 0x" << std::setw(2) << static_cast<unsigned int>( getTraciTypeId() )
                << ", got 0x" << std::setw(2) << static_cast<unsigned int>( typeId );
            throw std::runtime_error( msg.str() );
        }
    }
}


//--------------------------------------------------------------------
template<class MyMessageHandler, bool WithTypeId>
void GenTraciType<MyMessageHandler, WithTypeId>::writeType( TraciMessage & message ) const
{
    if ( getWithTypeId() )
        TypeIdType( getTraciTypeId() ).write( message );
}


}   // namespace traci


#endif      // #ifdef ENABLE_TRACICLIENT
#endif      // #ifndef TRACITYPE_IMPLEMENTATION_H

