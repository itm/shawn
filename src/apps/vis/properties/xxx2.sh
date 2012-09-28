#!/bin/sh

sed -e 's/XXX/INT/g' \
	 -e 's/Xxx/Int/g' \
	 -e 's/xxx/int/g' \
	 -e 's/xxTxx/int/g'
