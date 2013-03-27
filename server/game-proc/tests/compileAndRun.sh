#!/bin/bash
echo "Cleaning..."
# If we don't clean we may get false fails if it fails to build.
# Clean makes sure this doesn't happen
make clean | sed 's/^/\t/'

echo "Compiling..."
# Pass output of make to sed to indent
make all | sed 's/^/\t/'

echo "Running Tests..."
# Pass output of running our tests to sed to indent
./allTests.out | sed 's/^/\t/'


