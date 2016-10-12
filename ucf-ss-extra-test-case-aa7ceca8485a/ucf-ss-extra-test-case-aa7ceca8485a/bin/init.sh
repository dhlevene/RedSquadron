#!/bin/bash
if [ ! -d res ]; then
  cd ..
fi
if [ ! -d res ]; then
  echo "directory res is not found, please make sure you're running from the root of the repo"
  exit 1
fi
cd res && find -type f -name init.sh -exec echo cd \`dirname {}\` \&\& bash init.sh \; | source /dev/stdin
