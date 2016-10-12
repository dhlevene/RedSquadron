#!/bin/bash
exe=$1
bash gen-output.sh "$exe"
ls *.pl0 | sed 's/\.pl0$//' | xargs -I {} echo echo checking {}.$USER.log\; diff output/{}.beenotung.log output/{}.$USER.log | source /dev/stdin
