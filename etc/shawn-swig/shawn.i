// processor.h:177: Warning(813): Warning for shawn::Processor proxy: Base shawn::TagSyncable ignored. Multiple inheritance is not supported in Java.
//#pragma SWIG nowarn=813

// node.h:54: Warning(401): Nothing known about base class 'std::set<Node * >'. Ignored.
//#pragma SWIG nowarn=401

/*
	Problematisch:
	
		refcnt_pointer.i:
			//%rename( selfTypeOperator )	shawn::RefcntPointer::operator shawn::RefcntPointer::ConstSelfType;
			(erstmal ignorieren)

		tag_container.h:
			Was mit "typedef TagMap::const_iterator tag_iterator;" machen?
			
			
		processor.h, node.h: Mehrfachvererbung! (bei node.h kritisch?)
*/

// =====================================================

%module Shawn


%include "std_string.i"

%include "defutils.i"
%include "refcnt_pointable.i"
%include "refcnt_pointer.i"

%include "tag_container.i"
%include "tag.i"
%include "tag_syncable.i"

%include "vec.i"							// first needed by node.h
%include "node_change_listener.i"			// first needed by world.h
%include "skipnull_iterator.i"				// first needed by world.h
%include "keeper_managed.i"					// first needed by simulation_task.h
%include "simulation_task.i"				// first needed by world.h
%include "node_movement.i"					// first needed by node.h

%include "message.i"
%include "processor.i"
%include "world.i"
%include "node.i"

%include "simulation_environment.i"			// first needed by simulation_controller.h
%include "simulation_controller.i"
%include "shawn_simulation_controller.i"
%include "event_scheduler.i"

%{
	extern shawn::ShawnSimulationController* master_init();
%}

extern shawn::ShawnSimulationController* master_init();
