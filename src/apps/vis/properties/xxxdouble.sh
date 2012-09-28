#!/bin/sh

sed -e 's/XXX/DOUBLE/g' \
	 -e 's/Xxx/Double/g' \
	 -e 's/xxx/double/g' \
	 -e 's/xxTxx/double/g'
