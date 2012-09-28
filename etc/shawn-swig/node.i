%{
	#include "../../src/sys/node.h"
%}

//%ignore Neighborhood;

// Ignoring Node::set_minimum_storage, because it is not implemented in Node.cpp
%ignore shawn::Node::set_minimum_storage(int);

%rename( equals )		shawn::Node::operator==;
%rename( isDifferent )	shawn::Node::operator!=;

%include "../../src/sys/node.h";
