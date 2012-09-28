%{
	#include "../../src/sys/vec.h"
%}

%rename( assign ) 			shawn::Vec::operator=;
%rename( equals ) 			shawn::Vec::operator==;
%rename( isDifferent ) 		shawn::Vec::operator!=;
%rename( subtract ) 		shawn::Vec::operator-;
%rename( add ) 				shawn::Vec::operator+;
%rename( subtractAssign )	shawn::Vec::operator-=;
%rename( addAssign )		shawn::Vec::operator+=;
%rename( mult )				shawn::Vec::operator*;
%rename( multAssign )		shawn::Vec::operator*=;
%rename( div )				shawn::Vec::operator/;
%rename( divAssign )		shawn::Vec::operator/=;
%rename( divAssign )		shawn::Vec::operator/=;

%ignore operator <<;

%include "../../src/sys/vec.h";
