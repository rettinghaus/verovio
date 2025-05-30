#!/usr/bin/env sh

cd ..

output="./include/vrv/git_commit.h"
COMMIT=""

if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    SHA=$(git describe  --exclude '*' --abbrev=7 --always --dirty)
    if [ -z "$SHA" ]; then
        echo "Undefined git commit version"
    else
        COMMIT="-$SHA"
    fi
else
    echo "This is not a git directory and the git commit sha will remain undefined"
fi

echo "////////////////////////////////////////////////////////" > $output
echo "/// Git commit version file generated at compilation ///" >> $output
echo "/// Timestamp: $(date -u +"%Y-%m-%dT%H:%M:%SZ")                  ///" >> $output
echo "////////////////////////////////////////////////////////" >> $output
echo "" >> $output
echo "#define GIT_COMMIT \"$COMMIT\"" >> $output
echo "" >> $output
