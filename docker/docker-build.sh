#!/usr/bin/env bash

PROJECT_SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker build \
  --build-arg user_id=$(id -u) \
  --build-arg group_id=$(id -g) \
  -t "sgr1-worker" \
  "${PROJECT_SCRIPT_DIR}"
