#!/bin/sh

TESTBED_RUNTIME=~/develop/testbed-runtime.git/

WSNAPP_PROTO_PATH=$TESTBED_RUNTIME/iwsn/runtime.wsn-app/src/main/resources/
MESSAGES_PROTO_PATH=$TESTBED_RUNTIME/iwsn/runtime/src/main/resources/

WSNAPP_PROTO_FILE=WSNAppMessages.proto
MESSAGES_PROTO_FILE=Messages.proto


cp $WSNAPP_PROTO_PATH/$WSNAPP_PROTO_FILE \
	 $MESSAGES_PROTO_PATH/$MESSAGES_PROTO_FILE

protoc --cpp_out=./ $WSNAPP_PROTO_FILE
protoc --cpp_out=./ $MESSAGES_PROTO_FILE

rm $WSNAPP_PROTO_FILE $MESSAGES_PROTO_FILE
