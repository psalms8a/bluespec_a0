# Bluespec Codex Environment

This repository provides a simple setup script for installing the open-source
Bluespec toolchain (BSC) in an environment that Codex can use for
building and testing Bluespec SystemVerilog (BSV) code.

## Requirements

- Ubuntu or Debian based Linux distribution
- `apt-get` package manager
- Internet access to fetch dependencies
- Root privileges (or sudo) to install packages

## Quick Start

Run the setup script to download the latest binary release and install BSC:

```bash
./scripts/setup_bsc.sh
```

The script will:

1. Install the runtime packages required by the Bluespec tools.
2. Detect your distribution and download the matching pre-built release
   from the BSC GitHub repository.
3. Extract the package under `/opt/bsc`.
4. Add `/opt/bsc/bin` to your `PATH`.
5. Run a simple `bsc -help` command to verify the installation.

After the script finishes, restart your shell or source your `.bashrc`
so that the updated `PATH` is active.

## Usage

Once installed, you can invoke `bsc` and related tools directly. Example:

```bash
bsc -help
```

## Example: FIFO

A simple FIFO implementation is provided under `src` with a testbench in
`test`. After installing BSC, build and run the simulation with:

```bash
bsc -p +:src -u -sim -g mkTb test/TestFifo.bsv
bsc -sim -e mkTb -o mkTb test/mkTb.ba
./mkTb
```

You should see output similar to:

```text
First: aa
First after deq: bb
Empty: 1
```

This environment is suitable for Codex automation or local development.
