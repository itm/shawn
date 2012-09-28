%{
	#include "../../src/sys/tag.h"
	#include "../../src/sys/util/refcnt_pointer.h"
%}

%ignore						shawn::RefcntPointer::operator shawn::RefcntPointer::ConstSelfType;
//%rename( selfTypeOperator )	shawn::RefcntPointer::operator shawn::RefcntPointer::ConstSelfType;
%rename( assign ) 			shawn::RefcntPointer::operator=;
%rename( equals ) 			shawn::RefcntPointer::operator==;
%rename( isDifferent ) 		shawn::RefcntPointer::operator!=;
%rename( lesserThan ) 		shawn::RefcntPointer::operator<;
%rename( lesserEqual ) 		shawn::RefcntPointer::operator<=;
%rename( greaterThan ) 		shawn::RefcntPointer::operator>;
%rename( greaterEqual )		shawn::RefcntPointer::operator>=;

%include "../../src/sys/util/refcnt_pointer.h";

// Needed by tag_container.h
%template( RefcntPointer_Tag ) shawn::RefcntPointer< shawn::Tag >;
%template( RefcntPointer_ConstTag ) shawn::RefcntPointer< const shawn::Tag >;
