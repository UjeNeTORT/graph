#!/usr/bin/bash

DIRECTORY="${1:-.}"   # use $1 if given, otherwise current directory

cd "$DIRECTORY" || exit 1

for f in *.dot; do
  dot -Tsvg "$f" > "${f%.dot}.svg"
done
