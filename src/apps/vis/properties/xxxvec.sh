#!/bin/sh

sed -e 's/XXX/VEC/g' \
	 -e 's/Xxx/Vec/g' \
	 -e 's/xxx/vec/g' \
	 -e 's/xxTxx/shawn::Vec/g'
