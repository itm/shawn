/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#ifndef __SHAWN_SYS_TAG_H
#define __SHAWN_SYS_TAG_H

#include "sys/tag_container.h"
#include <stdexcept>

namespace shawn 
{

	DECLARE_HANDLES(Tag);
	
	class Tag 
		: public TagContainer 
	{
	public:
	
		///@name Ctor, Dtor
		///@{
		///
			Tag(const std::string&, bool lock = false);
			///
			virtual ~Tag();
		///@}
	
	
		///@name Access
		///@{
		///
			const std::string& name(void) const throw();
		///@}
	
	
		///@name Locks
		///@{
		///
			virtual bool is_locked(void) const throw();
			///
			virtual void lock(void) throw();
			///
			virtual void unlock(void) throw();
		///@}
	
	
		class const_entry_iterator 
		{
		public:
			class IteratorData 
			{
			public:
				virtual ~IteratorData();
				virtual void next(void) throw() = 0;
				virtual IteratorData* clone(void) const throw() = 0;
				virtual bool has_index(void) const throw() = 0;
				virtual std::string index(void) const throw() = 0;
				virtual std::string value(void) const throw() = 0;
				virtual bool operator==(const IteratorData&) const throw() = 0;
			};
			
			const_entry_iterator();
			const_entry_iterator(const const_entry_iterator&);
			
			explicit const_entry_iterator(IteratorData*);
			~const_entry_iterator();
			
			const_entry_iterator& operator=(const const_entry_iterator&) throw();
			bool has_index(void) const throw();
			std::string index(void) const throw();
			std::string value(void) const throw();
			inline std::string operator*(void) const throw() 
			{
				return value();
			}
			const_entry_iterator& operator++(void) throw();
			bool operator ==(const const_entry_iterator&) const throw();
			bool operator !=(const const_entry_iterator&) const throw();
		private:
			IteratorData* data_;
		};
			
		///@name Persistency - saving-relating
		///@{
		///
		
			///
			virtual const std::string& type_identifier(void) const throw() = 0;
			///
			virtual std::string encoded_content(void) const throw(std::runtime_error) = 0;
			///
			virtual void set_persistency(bool) throw();
			///
			virtual bool is_persistent(void) const throw();
			///
			virtual const_entry_iterator begin_entries(void) const throw();
			///
			virtual const_entry_iterator end_entries(void) const throw();
			
		///@}
	
		///@name Persistency - loading-related
		///@{
		///
			virtual void add_indexed_entry(const std::string& index, const std::string& value) throw(std::runtime_error );
			///
			virtual void add_value_entry(const std::string& value) throw(std::runtime_error );
		///@}
	
	private:
		std::string name_;
		bool persistent_;
		int lock_count_;
	};

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/tag.h,v $
 * Version $Revision: 430 $
 * Date    $Date: 2010-06-15 12:16:52 +0200 (Tue, 15 Jun 2010) $
 *-----------------------------------------------------------------------
 * $Log: tag.h,v $
 *-----------------------------------------------------------------------*/
