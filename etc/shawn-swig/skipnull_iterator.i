%{
	#include "../../src/sys/misc/skipnull_iterator.h"

	//typedef std::vector< shawn::Node* > vector_node_t;
%}


/*
	SkipNullIterator hat einen
	  inline value_type* operator-> ( void ) const
	Operator, warum wird dieser nicht als Warning angezeigt? Wird
	der irgendwie automatisch gewrappt?
*/

/*
%rename( getNext )	shawn::SkipNullIterator::operator++;

%ignore		shawn::SkipNullIterator::operator=;
%ignore		shawn::SkipNullIterator::operator--;
%ignore		shawn::SkipNullIterator::operator+=;
%ignore		shawn::SkipNullIterator::operator+;
%ignore		shawn::SkipNullIterator::operator-=;
%ignore		shawn::SkipNullIterator::operator-;
%ignore		shawn::SkipNullIterator::operator[];
*/

%include "../../src/sys/misc/skipnull_iterator.h";

/*
%include "std_vector.i";
%template( vector_node ) std::vector< shawn::Node* >;

typedef std::vector< shawn::Node* > vector_node_t;

%template( skipnull_iterator_world_const )
	shawn::SkipNullIterator< const shawn::Node,
							 vector_node_t,
							 vector_node_t::const_iterator >;

*/
