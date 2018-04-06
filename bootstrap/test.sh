#!/bin/bash
set -e

if [ -e "$(dirname "$0")/test/test.sh" ]; then
    "$(dirname "$0")/test/test.sh" src/zenith-bootstrap
else
    "$(dirname "$0")/test/test/test.sh" src/zenith-bootstrap
fi
