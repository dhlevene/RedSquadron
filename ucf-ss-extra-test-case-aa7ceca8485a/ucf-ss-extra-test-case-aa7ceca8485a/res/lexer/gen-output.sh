#!/bin/bash
exe=$1
if [ ! -f "$exe" ]; then
  echo "Error : $0 : executable $exe not found"
  echo "  Example : $0 ../../bin/lexer"
  exit 1
fi
if [ ! -d output ]; then
  mkdir output
fi
ls *.pl0 | sed 's/\.pl0//' | xargs -I {} echo "echo generating {}.pl0 '->' output/{}.$USER.log; $exe --source --clean {}.pl0 > output/{}.$USER.log" | source /dev/stdin
ls *.pl0 | sed 's/\.pl0//' | xargs -I {} echo "echo generating {}.pl0 '->' output/{}.$USER.token; $exe {}.pl0 | tail -n +4 > output/{}.$USER.token" | source /dev/stdin
