#!/usr/bin/env bash
set -e

# Install dependencies for building Bluespec Compiler
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y \
    git build-essential ghc cabal-install \
    libghc-regex-compat-dev libghc-syb-dev libghc-old-time-dev libghc-split-dev \
    tcl-dev pkg-config autoconf gperf flex bison iverilog

# Clone BSC source if not already present
if [ ! -d /tmp/bsc ]; then
    git clone --recursive https://github.com/B-Lang-org/bsc /tmp/bsc
fi

cd /tmp/bsc

# Build and install BSC into /opt/bsc
make install-src
mkdir -p /opt
mv inst /opt/bsc

# Update PATH for current session and future logins
export PATH=/opt/bsc/bin:$PATH
echo 'export PATH=/opt/bsc/bin:$PATH' >> ~/.bashrc

# Run a smoke test to verify the installation
make check-smoke

echo "BSC installed to /opt/bsc"
