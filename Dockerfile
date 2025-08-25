FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3 \
    ruby

RUN gem install ceedling

ARG user_id=1001
ARG group_id=1001
ARG user_name=WORKER

RUN groupadd --gid ${group_id} appgroup && \
    useradd --uid ${user_id} --gid appgroup -m ${user_name}
