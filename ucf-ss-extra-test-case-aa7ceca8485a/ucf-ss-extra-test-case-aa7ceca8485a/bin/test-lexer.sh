#!/bin/bash
if [ -f init.sh ]; then
  cd ..
fi
bash bin/init.sh;
if [ $? -ne 0 ]; then
  echo "failed when init"
  exit 1;
fi
exe=$(cat path.ini | grep bin_lexer | awk -F '=' '{print $2}')
if [ ! -f "$exe" ]; then
  echo "executable for lexer is not found"
  exit 1
fi
cd res && find -type f -name test.sh -exec echo cd \`dirname {}\` \&\& bash test.sh "../../$exe" \; | source /dev/stdin
