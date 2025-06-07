# Bluespec Codex Environment

This repository provides a simple setup script for building the open-source
Bluespec toolchain (BSC) in an environment that Codex can use for
building and testing Bluespec SystemVerilog (BSV) code.

## Requirements

- Ubuntu or Debian based Linux distribution
- `apt-get` package manager
- Internet access to fetch dependencies
- Root privileges (or sudo) to install packages

## Quick Start

Run the setup script to install all dependencies and build BSC:

```bash
./scripts/setup_bsc.sh
```

The script will:

1. Install the packages required to build the Bluespec compiler.
2. Clone the BSC source repository.
3. Build the compiler and install it under `/opt/bsc`.
4. Add `/opt/bsc/bin` to your `PATH`.
5. Run a simple smoke test (`make check-smoke`) to verify the build.

After the script finishes, restart your shell or source your `.bashrc`
so that the updated `PATH` is active.

## Usage

Once installed, you can invoke `bsc` and related tools directly. Example:

```bash
bsc -help
```

This environment is suitable for Codex automation or local development.
