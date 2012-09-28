#include "sys/node_movements/playback/tag_node_movement_creator.h"
#include "sys/node_movements/playback/movement_controller.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/jump_movement.h"
#include "sys/node_movements/linear_movement.h"
#include "sys/node_movement.h"
#include "sys/node.h"
#include "sys/world.h"
#include "sys/tag.h"
#include "sys/taggings/tag_factory.h"
#include "sys/taggings/group_tag.h"
#include "sys/taggings/basic_tags.h"
#include "sys/tag_container.h"
#include<string>
#include<limits>

using namespace std;

namespace shawn{

	TagNodeMovementCreator::TagNodeMovementCreator(SimulationController& sc)
{
	sc_ = &sc;
	movement_info_=NULL;
}

TagNodeMovementCreator::~TagNodeMovementCreator(void)
{
}


MovementInfo* TagNodeMovementCreator::next_movement()
	throw( std::runtime_error )
{
	double min=numeric_limits<double>::max();
	double start_time=0;
//	double arrival_time=0;
	double velocity=0.0;
	movement_info_ =NULL;
	DoubleTag* min_time_tag=NULL;


	for ( World::node_iterator current_node = sc_->world_w().begin_nodes_w();
               current_node != sc_->world_w().end_nodes_w(); ++current_node ){
	GroupTag* movement_tag = dynamic_cast<GroupTag* >
		( current_node->find_tag_w( "movement" ).get() );
	if( movement_tag != NULL ){
		for( TagContainer::tag_iterator current_tag = movement_tag->begin_tags();
			current_tag != movement_tag->end_tags(); ++current_tag){
				DoubleTag* time_tag = dynamic_cast<DoubleTag* >
					(current_tag->second.get());
				if( time_tag != NULL){

					const BoolTag* vis_tag = dynamic_cast<const BoolTag* >
						(time_tag->find_tag( "visited" ).get() );
					if(vis_tag == NULL) {

						if( (time_tag->value() < min ) ){

				start_time = time_tag->value();

				const GroupTag* movement_type_tag=dynamic_cast<const GroupTag* >
					( time_tag->find_tag( "linear" ).get());

				if( movement_type_tag != NULL ){
					getDestinationPosition( movement_type_tag );

					if(destination != NULL ){

						const DoubleTag* velocity_tag = dynamic_cast<const DoubleTag* >
							( movement_type_tag->find_tag( "velocity" ).get() );
						const DoubleTag* arrival_tag = dynamic_cast<const DoubleTag* >
							( movement_type_tag->find_tag( "arrival time").get() );
					  const DoubleTag* duration_tag = dynamic_cast<const DoubleTag* >
							( movement_type_tag->find_tag( "duration").get() );
						if( ( velocity_tag != NULL ) && ( arrival_tag == NULL ) && ( duration_tag == NULL)){
							velocity = velocity_tag->value();
							}
						else
							if( ( velocity_tag == NULL ) && ( arrival_tag != NULL )&& ( duration_tag == NULL) ){
								velocity = ( *(destination) - current_node->real_position() ).euclidean_norm()/
									(arrival_tag->value() - start_time);
							}
							else if( ( velocity_tag == NULL ) && ( arrival_tag == NULL )&& ( duration_tag != NULL)){
							  	velocity = ( *(destination) - current_node->real_position() ).euclidean_norm()/
							  	duration_tag->value();

							}
							else throw std::runtime_error( "Invalid value for either 'velocity'or 'arrival time' " );

							min_time_tag=time_tag;

						movement_info_ = new MovementInfo();
						LinearMovement* linear_movement = new LinearMovement();

						linear_movement->set_parameters(velocity, *destination, sc_->world_w());
						movement_info_->set_urgency(MovementInfo::Delayed);
						movement_info_->set_dispatch_time(start_time);
						movement_info_->set_node(&(*current_node));
						movement_info_->set_node_movement(linear_movement);

						min = start_time;

					} else continue;
				}
				else{
					movement_type_tag=dynamic_cast<const GroupTag* >( time_tag->find_tag( "jump" ).get());
					if( movement_type_tag!=NULL ){
						getDestinationPosition(movement_type_tag);
						if(destination != NULL ){
							min_time_tag=time_tag;
							movement_info_ = new MovementInfo();
							JumpMovement* jump_movement = new JumpMovement();

							jump_movement->set_dimensions(destination);
							movement_info_->set_urgency(MovementInfo::Delayed);
							movement_info_->set_dispatch_time(start_time);
							movement_info_->set_node(&(*current_node));
							movement_info_->set_node_movement(jump_movement);

							min = start_time;
						}
						else continue;
					}
					else{
						throw std::runtime_error( "Invalid value for movement-type in xml-file" );
						continue;
					}
				}
						}else continue;
				}
			}else throw std::runtime_error( "Invalid value for time in xml-file" );

		}
	}
	}
	if( movement_info_!= NULL ){
		min_time_tag->add_tag(new BoolTag("visited", true));
		if(( dynamic_cast< LinearMovement* >( movement_info_->node_movement()) ) != NULL){
		DEBUG(sc_->logger(), "linearmovement: " << "time: " << start_time << " , nodenumber: " <<
			movement_info_->node()->id() <<  " , xposition: " << destination->x() << " , yposition: "
							<< destination->y() << " , zposition: "<<destination->z()<<" , velocity: " << velocity);
		}
		else{
			DEBUG(sc_->logger(), "jumpmovement: " << "time: " << start_time <<
								" , nodenumber: " << movement_info_->node()->id() <<  " , xposition: " << destination->x() <<
								" , yposition: " << destination->y() << " , zposition: "<<destination->z()<<" , velocity: " << velocity);
		}
		return movement_info_;
	}
	else{
		return NULL;
	}
}

void TagNodeMovementCreator::reset()
{
	for ( World::node_iterator current_node = sc_->world_w().begin_nodes_w();
               current_node != sc_->world_w().end_nodes_w(); ++current_node ){
		GroupTag* movement_tag = dynamic_cast<GroupTag* >
		( current_node->find_tag_w( "movement" ).get() );
	if( movement_tag != NULL ){
		for( TagContainer::tag_iterator current_tag = movement_tag->begin_tags();
			current_tag != movement_tag->end_tags(); ++current_tag){
				DoubleTag* time_tag = dynamic_cast<DoubleTag* >
					(current_tag->second.get());
				if( time_tag != NULL){
					time_tag->remove_tag_by_name( "visited" );
				}
		}
	}
	}

}

void TagNodeMovementCreator::getDestinationPosition(const Tag* movement_type_tag){

	double x_dest=0.0;
	double y_dest=0.0;
	double z_dest=0.0;
	delete destination;

	const DoubleTag* dimension=dynamic_cast<const DoubleTag* >
		( movement_type_tag->find_tag( "x-pos" ).get() );
	if( dimension!= NULL ){
		x_dest=dimension->value();
		dimension=dynamic_cast<const DoubleTag* >
			( movement_type_tag->find_tag("y-pos" ).get() );
		if( dimension!= NULL ){
			y_dest=dimension->value();
			dimension=dynamic_cast<const DoubleTag* >
				( movement_type_tag->find_tag("z-pos" ).get() );
			if( dimension!= NULL ){
				z_dest=dimension->value();
				destination=new Vec(x_dest,y_dest,z_dest);
				}
			else throw std::runtime_error( "Invalid value for z-pos in xml-file" );
		}
		else throw std::runtime_error( "Invalid value for y-pos in xml-file" );
	}
	else throw std::runtime_error( "Invalid value for x-pos in xml-file" );
}

}
