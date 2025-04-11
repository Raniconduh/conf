#!/bin/sh

sysctl dev.cpu | awk '
BEGIN {
	avrg = 0
	cnt = 0
}
/temperature/ {
	substr($NF, 1, length($0) - 1)
	avrg += $NF
	cnt += 1
}
END {
	print avrg / cnt
}'
