#ifndef __SHAWN_APPS_WISELIB_MESSAGE_H_
#define __SHAWN_APPS_WISELIB_MESSAGE_H_

#include "_apps_enable_cmake.h"
#ifdef ENABLE_WISELIB

#include "sys/message.h"
#include <cstring>

namespace wiselib
{

   template<typename NodeIdType,
            typename BlockDataType,
            typename SizeType>
   class WiselibMessage
       : public shawn::Message
   {
   public:
      typedef NodeIdType node_id_t;
      typedef BlockDataType block_data_t;
      typedef SizeType size_t;
      // --------------------------------------------------------------------
      WiselibMessage( node_id_t dest, size_t pay_size, block_data_t *pay );
      virtual ~WiselibMessage();
      // --------------------------------------------------------------------
      inline node_id_t destination(void) const throw()
      { return destination_; }
      // --------------------------------------------------------------------
      inline block_data_t* payload(void) const throw()
      { return payload_; }
      // --------------------------------------------------------------------
      inline size_t payload_size(void) const throw()
      { return payload_size_; }

   private:
      node_id_t destination_;
      block_data_t *payload_;
      size_t payload_size_;
   };
   // -----------------------------------------------------------------------
   template<typename NodeIdType,
            typename BlockDataType,
            typename SizeType>
   WiselibMessage<NodeIdType, BlockDataType, SizeType>::
   WiselibMessage( node_id_t dest, size_t pay_size, block_data_t *pay )
      : destination_  ( dest ),
         payload_size_( pay_size )
   {
      setSize(pay_size);
      payload_ = new block_data_t[pay_size];
      memcpy( payload_, pay, pay_size );
   }
   // -----------------------------------------------------------------------
   template<typename NodeIdType,
            typename BlockDataType,
            typename SizeType>
   WiselibMessage<NodeIdType, BlockDataType, SizeType>::
   ~WiselibMessage()
   {
      delete payload_;
   }

}

#endif
#endif
