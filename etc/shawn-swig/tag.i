// Suppress SWIG warning
#pragma SWIG nowarn=SWIGWARN_PARSE_NESTED_CLASS

// Redefine nested class in global scope in order for SWIG to 
// generate a proxy class. Only SWIG parses this definition.

%rename( equals )	IteratorData::operator==;

class IteratorData
{
public:
	virtual ~IteratorData();
    virtual void next( void ) throw() = 0;
    virtual IteratorData* clone( void ) const throw() = 0;
    virtual bool has_index( void ) const throw() = 0;
    virtual std::string index( void ) const throw() = 0;
    virtual std::string value( void ) const throw() = 0;
    virtual bool operator== ( const IteratorData& ) const throw() = 0;
};

//=================

%rename( increment )	const_entry_iterator::operator++;
%rename( assign )		const_entry_iterator::operator=;
%rename( equals )		const_entry_iterator::operator==;
%rename( isDifferent )	const_entry_iterator::operator!=;

class const_entry_iterator
{
public:
	const_entry_iterator();
	const_entry_iterator( const const_entry_iterator& );
	explicit const_entry_iterator( IteratorData* );
	~const_entry_iterator();
	
	inline std::string operator* ( void ) const throw() { return value(); }
	const_entry_iterator& operator++ ( void ) throw();
	const_entry_iterator& operator= ( const const_entry_iterator& ) throw();
	bool operator == ( const const_entry_iterator& ) const throw();
	bool operator != ( const const_entry_iterator& ) const throw();

	bool has_index( void ) const throw();
	std::string index( void ) const throw();
	std::string value( void ) const throw();

private:
	IteratorData* data_;
};

//=================


%{
	#include "../../src/sys/tag.h"
%}

%include "../../src/sys/tag.h";

%{
	// SWIG thinks that Inner is a global class, so we need to trick
	// the C++ compiler into understanding this so called global
	// type.
	typedef shawn::Tag::const_entry_iterator::IteratorData IteratorData;
	typedef shawn::Tag::const_entry_iterator const_entry_iterator;
%}
