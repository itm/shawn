/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "spyglass_packet.h"
#ifdef ENABLE_SPYGLASS

#include "sys/simulation/simulation_controller.h"
#include "apps/spyglass/spyglass_packet.h"

#include <vector>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

namespace spyglass
{
    //
    SpyglassPacket::
        SpyglassPacket()
    {}
    // ----------------------------------------------------------------------
    SpyglassPacket::
        ~SpyglassPacket()
    {}
    // ----------------------------------------------------------------------

    int

        SpyglassPacket::

        readChar(std::vector<char> data, int start)
        throw()
    {
        int hb = (int) data[start];
        return hb < 0 ? hb + 256 : hb;
    }

    // ----------------------------------------------------------------------

    void

        SpyglassPacket::
        writeChar( int value, std::vector<char> *b, int start)
        throw()
    {
        //cerr << b->size() << " > " << start << endl;
        assert(int(b->size()) > start);
        (*b)[start] = value;
    }

    /**
    * Reads a string form the array
    * @param data	The array to read from
    * @param start	The array index start reading at
    * @return The read string
    */
    // -----------------------------------------------------------------------
    /*std::string
    SpyglassPacket::
    readString(std::vector<char> data, int start)
    throw()
    {
    int len = readChar(data, start);
    char[] s = new char[len];
    for (int i=0;i<len;i++) s[i] = (char) readChar(data, start+1+i);
    return new std::string( s );
    }
    */

    /**
    * Writes a string into the array;
    * @param s		The string to be written
    * @param b		The array to write into
    * @param start	The array index start writing at
    */
    void
        SpyglassPacket::
        writeString(std::string s, std::vector<char> *b, int start)
        throw()
    {
        assert(b->size() >= start+s.size());
        writeInt(s.size(), b, start);
        int i=0;
        for (string::iterator it=s.begin();it!=s.end();it++,i++)
            writeChar(*it, b, start+4+i);
    }

    // ----------------------------------------------------------------------

    /**
    * restores an integer, which was split up in two bytes according to the
    * specification, it must have been split by its row byte representation
    * with MSBF-order
    *
    * @param data the byte-array which contains the splitted integer
    * @param start starting position of the integer-bytes in this array
    * @return the unspoiled integer value
    */
    int
        SpyglassPacket::
        readShort(std::vector<char> data, int start)
        throw()
    {
        int hb = data[start];
        int lb = data[start + 1];
        int mult = ((int) (data[start] & 0x80)) > 0 ? -1 : 1;
        int num = ((hb << 8) & 0xFF00) | (lb & 0x00FF);

        if (mult < 0)
            num = ((int) ((~num) & 0xFFFF)) + 1;
        return mult * num;
    }

    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        writeShort( int value, std::vector<char> *b , int start)
        throw()
    {
        assert(int(b->size()) > start+1);
        (*b)[start] = (char) ((value >> 8 )& 0xFF);
        (*b)[start+1] = (char) (value & 0xFF);
    }

    // ----------------------------------------------------------------------

    /*
    * restores an integer, which was split up in four bytes acording to the
    * specification, it must have been split by its row byte representation
    * with MSBF-order
    *
    * @param data the byte-array which contains the splitted integer
    * @param start starting position of the integer-bytes in this array
    * @return the unspoiled integer value
    */
    int
        SpyglassPacket::
        readInt(std::vector<char> data, int start)
        throw()
    {
        int hb = data[start];
        int lb1 = data[start + 1];
        int lb2 = data[start + 2];
        int lb3 = data[start + 3];

        int mult = ((int) (data[start] & 0x80)) > 0 ? -1 : 1;
        int num = (hb << 24) | (lb1 << 16) | (lb2 << 8) | lb3;

        if (mult < 0)
            num = ((int) ((~num) & 0xFFFFFFFF)) + 1;
        return mult * num;
    }

    // ----------------------------------------------------------------------
    void
        SpyglassPacket::
        writeInt( int value, std::vector<char> *b , int start)
        throw()
    {
        assert(int(b->size()) > start+3);
        (*b)[start] = (char) ((value >> 24 )& 0xFF);
        (*b)[start+1] = (char) ((value >> 16 )& 0xFF);
        (*b)[start+2] = (char) ((value >> 8 )& 0xFF);
        (*b)[start+3] = (char) (value & 0xFF);
    }

    // ----------------------------------------------------------------------

    /*
    * restores a float , which was split up in four bytes acording to the
    * specification, it must have been split by its row byte representation
    * with MSBF-order
    *
    * @param data the byte-array which contains the splitted float
    * @param start starting position of the float-bytes in this array
    * @return the unspoiled float value
    */
    float
        SpyglassPacket::
        readFloat(vector<char> daten, int start)
        throw()
    {
        int back = 0;
        for (int i = 0; i < 4; i++) {
            back <<= 8;
            int n = (int) daten[start + i];
            if (n < 0)
                n += 256;
            back |= n;
        }
        //System.err.println("Packet.readFloat: I MAY YIELD INCORRECT RESULTS. CHECK ME!");
        //return Float.intBitsToFloat(back);
        float f=5.0;
        return f;
    }

    // ----------------------------------------------------------------------
    void
        SpyglassPacket::
        writeFloat( float value, std::vector<char> *b , int start)
        throw()
    {

        assert(int(b->size()) > start+3);
        int intvalue ;
        memcpy(&intvalue, &value, 4);
        (*b)[start] = (char) ((intvalue >> 24 )& 0xFF);
        (*b)[start+1] = (char) ((intvalue >> 16 )& 0xFF);
        (*b)[start+2] = (char) ((intvalue >> 8 )& 0xFF);
        (*b)[start+3] = (char) (intvalue & 0xFF);
    }
    // ----------------------------------------------------------------------
    std::vector<char>*
        SpyglassPacket::
        serialize( void )
        throw()
    {
        length_ = get_size();
        std::vector<char> *b = new std::vector<char>(length_+1);
        writeChar( length_, b, 0);
        writeChar( version_, b, 1);
        writeChar( type_, b, 2);
        writeInt( nodeId_, b, 3 );
        writeChar( hour_, b, 7);
        writeChar( minute_, b, 8);
        writeChar( second_, b, 9);
        writeFloat( x_, b, 10);
        writeFloat( y_, b, 14);
        writeFloat( z_, b, 18);
        return b;
    }

    // ----------------------------------------------------------------------
    void
        SpyglassPacket::
        deserialize(std::vector<char> b)
        throw()
    {
        length_ = readChar( b, 0);
        version_ = readChar( b, 1);
        type_ = readChar( b, 2);
        nodeId_ = readInt( b, 3 );
        hour_ = readChar( b, 7);
        minute_ = readChar( b, 8);
        second_ = readChar( b, 9);
        x_ = readFloat( b, 10);
        y_ = readFloat( b, 14);
        z_ = readFloat( b, 18);
    }

    // ----------------------------------------------------------------------
    int
        SpyglassPacket::
        get_size()
        throw()
    {
        return 21;
    }

    // ----------------------------------------------------------------------

    int
        SpyglassPacket::
        get_type()
        throw()
    {
        return type_;
    }
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_version( int v )
        throw()
    { version_ = v;	}
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_type( int t )
        throw()
    { type_ = t;	}
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_nodeId( int id )
        throw()
    { nodeId_ = id;	}
    // ----------------------------------------------------------------------
    void
        SpyglassPacket::
        set_hour( int h )
        throw()
    { hour_ = h; }
    // ----------------------------------------------------------------------
    void
        SpyglassPacket::
        set_minute( int m )
        throw()
    { minute_ = m; }
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_second( int s )
        throw()
    { second_ = s; }
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_x( float x )
        throw()
    { x_ = x; }
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_y( float y )
        throw()
    { y_ = y; }
    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_z( float z )
        throw()
    { z_ = z; }

    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_parameters(int version, int type, int nodeId, int hour, int minute, int second, float x, float y, float z)
        throw()
    {
        version_ = version;
        type_ = type;
        nodeId_ = nodeId;
        hour_ = hour;
        minute_ = minute;
        second_ = second;
        x_ = x;
        y_ = y;
        z_ = z;
    }

    // ----------------------------------------------------------------------

    void
        SpyglassPacket::
        set_parameters(int version, int type, int nodeId, double time, float x, float y, float z)
        throw()
    {
        version_ = version;
        type_ = type;
        nodeId_ = nodeId;
        if (time > 0) {
            int t = int(ceil(time));
            hour_ = int(t/3600);
            minute_ = int((t%3600)/60);
            second_ = t%60;
        }
        else {
            hour_ = 0;
            minute_ = 0;
            second_ = 0;
        }
        x_ = x;
        y_ = y;
        z_ = z;
    }

    // ----------------------------------------------------------------------




}
#endif

/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/apps/spyglass/spyglass_packet.cpp,v $
* Version $Revision: 182 $
* Date    $Date: 2008-04-07 15:14:38 +0200 (Mon, 07 Apr 2008) $
*-----------------------------------------------------------------------
* $Log: spyglass_packet.cpp,v $
 *-----------------------------------------------------------------------*/
