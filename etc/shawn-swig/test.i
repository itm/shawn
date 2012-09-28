 %module PI

%include "std_string.i"
%include "std_vector.i"

%{
 #include "palminterface.h"
 #include "DateTime.h"
 #include "DateBookEvent.h"
%}


namespace std {
%template(DateBookEventList) vector<FarbergDateBookEvent>;
%template(DateBookEventListIt) vector<FarbergDateBookEvent>::iterator;
}

/*
%include "palminterface.h"
%include "DateTime.h"
%include "DateBookEvent.h"
*/
