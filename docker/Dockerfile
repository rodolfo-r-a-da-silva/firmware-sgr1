FROM debian:13-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
     # Build tools
    build-essential \
    cmake \
    gcc \
    # Libraries
    ruby-dev \
    # Python
    python3 \
    # Utilities
    git \
    openssh-client \
    ruby

RUN gem install ceedling --no-document

ARG user_id=1001
ARG group_id=1001
ARG user_name=worker

RUN groupadd --gid $group_id appgroup && \
    useradd --uid $user_id --gid appgroup -m $user_name
