#!/bin/bash
set -ex

if [ $# -ne 1 ]; then
    echo "Usage: $0 [zenith]" >&2
    exit 1
fi

zenith="$(readlink -f "$1")"
cd "$(dirname "$0")"
i=1
echo -n "1.."
find -mindepth 1 -maxdepth 1 -type d | wc -l
for dir in $(find -mindepth 1 -maxdepth 1 -type d | cut -f2 -d/); do
    rm -f stdout.txt stderr.txt
    "$zenith" "$dir/main.zen" > stdout.txt 2> stderr.txt
    if [ $? -eq 0 ]; then
        if md5sum stderr.txt stdout.txt | bash -c "cd hello-world && md5sum -c -" > /dev/null 2> /dev/null; then
            echo "ok $i - $dir"
        else
            echo "not ok $i - $dir"
        fi
    else
        echo "$dir failed with exit code $?"
    fi
    i=$((i+1))
done
