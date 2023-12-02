#!/bin/sh
if [ `uname -m` = "x86_64" ]; then
		./tests/ic23int -v tests/generator_test.ifjc23 >  tests/generator_test.out 2>&1;
	else
		./tests/ic23intx86-64 -v tests/generator_test.ifjc23 >  tests/generator_test.out 2>&1
fi