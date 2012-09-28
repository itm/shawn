/** \file
 *  Definitions of the simple TraCI data types.
 *
 *  \author Björn Hendriks
 */


#ifndef ATOMIC_DATATYPES_H
#define ATOMIC_DATATYPES_H

#include "_apps_enable_cmake.h"

#ifdef ENABLE_TRACICLIENT

#include "apps/tcpip/storage.h"
#include "tracitype.h"
#include "mobility_interface_constants.h"

namespace traci
{


/**
 *  TraCI MessageHandler with no value.
 *
 *  This MessageHandler does not read or write anything from or to the TraciMessage.
 */
struct MessageHandlerEmpty : public MessageHandler<int, id::EmptyType>
{
    virtual std::size_t getLength( const ValueType & value ) const
    {
        return 0;
    }

    virtual void print( const ValueType &value, std::ostream & out = std::cout ) const
    {
        out << "empty TraciType";
    }

    virtual void read( TraciMessage & storage, ValueType & value ) const
    {
    }

    virtual void write( TraciMessage & storage, const ValueType & value ) const
    {
    }
};
/// TraciType with no value
typedef FundamentalTraciType<MessageHandlerEmpty> EmptyType;



/**
 *  TraCI MessageHandler for all simple types.
 *
 *  This is a special message handler for messages of type tcpip::Storage. It can
 *  deal with all types for which special read/write functions exist in
 *  tcpip::Storage.
 *
 *  The read/write functions will be choosen by template arguments as pointer to
 *  member functions of tcpip::Storage.
 *
 *  \tparam T               value type passed to MessageHandler
 *  \tparam readType        return type of read function
 *  \tparam writeType       parameter type of write function
 *  \tparam TraciTypeId     TraCI type ID passed to MessageHandler
 *  \tparam readStorage     pointer to read function in tcpip::Storage
 *  \tparam writeStorage    pointer to write function in tcpip::Storage
 */
template < typename T, typename ReadType, typename WriteType, id::DataType TraciTypeId,
        ReadType (tcpip::Storage::* readStorage)(),
        void (tcpip::Storage::* writeStorage)(WriteType) >
struct MessageHandlerSimple : public MessageHandler<T, TraciTypeId>
{
    /**
     *  Print MessageHandler::ValueType into a std::ostream.
     */
    virtual void print( const T &value, std::ostream & out = std::cout ) const
    {
        // the static_cast converts char types to ints
        out << static_cast<ReadType>(value);
    }

    virtual void read( tcpip::Storage & storage, T & value ) const
    {
        value = (storage.*readStorage)();    // this implies a conversion ReadType -> T of value
    }

    virtual void write( tcpip::Storage & storage, const T & value ) const
    {
        (storage.*writeStorage)( value );   // this implies a conversion T -> WriteType of value
    }
};


/**
 *  Simplification of template arguments of traci::MessageHandlerSimple
 *
 *  \p T will be used for \p readType and \p writeType, too.
 *
 *  For description of template parameters see MessageHandlerSimple.
 */
template < typename T, id::DataType TraciTypeId,
        T (tcpip::Storage::* readStorage)(),
        void (tcpip::Storage::* writeStorage)(T) >
struct MessageHandlerSimpleSameType : MessageHandlerSimple<T, T, T, TraciTypeId, readStorage, writeStorage>
{
};


/** \name   Specializations of MessageHandlerSimple
 *
 *  \todo   Add compile time assertions to make sure that int, float and double
 *          have the right size defined by TraCI protocol.
 */
//@{
typedef MessageHandlerSimple<unsigned char, int, int, id::TypeUbyte,
    &tcpip::Storage::readUnsignedByte, &tcpip::Storage::writeUnsignedByte> MessageHandlerUbyte;

typedef MessageHandlerSimple<signed char, int, int, id::TypeByte,
    &tcpip::Storage::readByte, &tcpip::Storage::writeByte> MessageHandlerByte;

typedef MessageHandlerSimpleSameType<int, id::TypeInteger,
    &tcpip::Storage::readInt, &tcpip::Storage::writeInt> MessageHandlerInteger;

typedef MessageHandlerSimpleSameType<float, id::TypeFloat,
    &tcpip::Storage::readFloat, &tcpip::Storage::writeFloat> MessageHandlerFloat;

typedef MessageHandlerSimpleSameType<double, id::TypeDouble,
    &tcpip::Storage::readDouble, &tcpip::Storage::writeDouble> MessageHandlerDouble;
//@}



/** \name   Specializations of GenTraciType for some basic TraCI data types
 */
//@{
typedef FundamentalTraciType<MessageHandlerUbyte> UbyteType;
typedef FundamentalTraciType<MessageHandlerUbyte, true> UbyteTypeWithTypeId;

typedef FundamentalTraciType<MessageHandlerByte> ByteType;
typedef FundamentalTraciType<MessageHandlerByte, true> ByteTypeWithTypeId;

typedef FundamentalTraciType<MessageHandlerInteger> IntegerType;
typedef FundamentalTraciType<MessageHandlerInteger, true> IntegerTypeWithTypeId;

typedef FundamentalTraciType<MessageHandlerFloat> FloatType;
typedef FundamentalTraciType<MessageHandlerFloat, true> FloatTypeWithTypeId;

typedef FundamentalTraciType<MessageHandlerDouble> DoubleType;
typedef FundamentalTraciType<MessageHandlerDouble, true> DoubleTypeWithTypeId;
//@}



/**
 *  Special message handler for TraCI type string using tcpip::Storage messages.
 *
 *  We need this special message handler for strings because the TraCI length of
 *  a string cannot be computed with a simple sizeof().
 */
struct MessageHandlerString : public MessageHandlerSimple<std::string, std::string, const std::string &,
    id::TypeString, &tcpip::Storage::readString, &tcpip::Storage::writeString>
{
    virtual std::size_t getLength( const std::string &s ) const
    {
        std::size_t length = 0;
        length += IntegerType().getLength();
        length += s.size();
        return length;
    }

    virtual void print( const std::string &s, std::ostream & out = std::cout ) const
    {
        out << '"' << s << '"';
    }
};
typedef CompoundTraciType<MessageHandlerString> StringType;
typedef CompoundTraciType<MessageHandlerString, true> StringTypeWithTypeId;




/**
 *  Special message handler for TraCI type stringList.
 *
 *  We need this special message handler for stringList because getLength() and
 *  print() cannot be handled by MessageHandlerSimple.
 *
 *  \todo   Convert this StringListType to a dynamic length type if it has no unwanted
 *          complications.
 */
struct MessageHandlerStringList : public MessageHandler< std::vector<std::string>, id::TypeStringlist >
{
    typedef std::vector<std::string> ValueType;

    virtual std::size_t getLength( const ValueType &stringList ) const
    {
        std::size_t length = 0;
        length += IntegerType().getLength();
        MessageHandlerString messageHandlerStringDummy;     // dummy for computing lengths of TraCI strings
        for ( ValueType::const_iterator it = stringList.begin(); it != stringList.end(); ++it )
        {
            length += messageHandlerStringDummy.getLength(*it);
        }
        return length;
    }

    virtual void print( const ValueType &stringList, std::ostream & out = std::cout ) const
    {
        out << '(' << stringList.size() << " strings: ";
        MessageHandlerString messageHandlerStringDummy;     // dummy for printing of TraCI strings
        for ( ValueType::const_iterator it = stringList.begin(); it != stringList.end(); ++it )
        {
            if ( it != stringList.begin() )
                out << ", ";
            messageHandlerStringDummy.print(*it, out);
        }
        out << ')';
    }

    virtual void read( tcpip::Storage & storage, ValueType & value ) const
    {
        value = storage.readStringList();
    }

    virtual void write( tcpip::Storage & storage, const ValueType & value ) const
    {
        storage.writeStringList( value );
    }
};
typedef CompoundTraciType<MessageHandlerStringList> StringListType;
typedef CompoundTraciType<MessageHandlerStringList, true> StringListTypeWithTypeId;


}   // namespace traci


/// we need this after the above definition of UbyteType (see documentation of included file)
#include "tracitype_implementation.h"


#endif  // #ifdef ENABLE_TRACICLIENT
#endif // ATOMIC_DATATYPES_H

