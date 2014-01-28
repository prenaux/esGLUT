#!/bin/sh
WORK=`pwd`"/../../../"
HAM_HOME=$WORK/ham
. $HAM_HOME/bin/ham-bash-setenv.sh
. hat python_26
python -m SimpleHTTPServer 8123
