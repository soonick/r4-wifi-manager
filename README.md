# R4-Wifi-Manager

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/soonick/r4-wifi-manager/tree/master.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/soonick/r4-wifi-manager/tree/master)

Starts AP for user to set up configuration information. This configuration can then used for whatever the developer needs (A common example is to connect to a WiFi network).

## Dependencies

This repo uses submodules for its dependencies. To fetch all submodules:

```
git submodule update --init --recursive
```

## Build

Builds, lints and runs tests

```
make verify
```

## Development environment

This library comes shipped with ready-to-use vim development environment with LSP ready to use. To access the development environment:

```
make vim
```
