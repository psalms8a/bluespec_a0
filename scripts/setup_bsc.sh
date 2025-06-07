#!/usr/bin/env bash
set -e

# Install runtime dependencies
apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y curl tar iverilog

# Determine distribution ID and version
if [ -f /etc/os-release ]; then
    . /etc/os-release
    DIST_ID=$ID
    DIST_VERSION=$VERSION_ID
else
    echo "Unable to detect distribution" >&2
    exit 1
fi

# Query latest release tag
TAG=$(curl -s https://api.github.com/repos/B-Lang-org/bsc/releases/latest | grep tag_name | cut -d '"' -f4)

# Fetch list of assets for the latest release
ASSET_JSON=$(curl -s https://api.github.com/repos/B-Lang-org/bsc/releases/latest)

# Try to find a matching asset for the distro
ASSET_URL=$(echo "$ASSET_JSON" | grep browser_download_url | grep "bsc-${TAG}-${DIST_ID}-${DIST_VERSION}" | head -n1 | cut -d '"' -f4)

# Fallback: match without minor version
if [ -z "$ASSET_URL" ]; then
    SHORT_VERSION=${DIST_VERSION%%.*}
    ASSET_URL=$(echo "$ASSET_JSON" | grep browser_download_url | grep "bsc-${TAG}-${DIST_ID}-${SHORT_VERSION}" | head -n1 | cut -d '"' -f4)
fi

if [ -z "$ASSET_URL" ]; then
    echo "No binary release found for ${DIST_ID}-${DIST_VERSION}" >&2
    exit 1
fi

# Download and extract to /opt/bsc
TMP_TAR=/tmp/bsc.tar.gz
curl -L "$ASSET_URL" -o "$TMP_TAR"
mkdir -p /opt/bsc
tar -xzf "$TMP_TAR" -C /opt/bsc --strip-components=1

# Update PATH
export PATH=/opt/bsc/bin:$PATH
echo 'export PATH=/opt/bsc/bin:$PATH' >> ~/.bashrc

# Verify installation
bsc -help >/dev/null

echo "BSC installed to /opt/bsc"
