#!/usr/bin/env bash
set -euo pipefail

# builds the MAR1D flatpak with all byproducts contained to flatpak-cruft folder
# requires flatpak-builder and flatpak installed

# run in a subshell so `export`s don't leak
(

FLATPAK_DIR=$PWD/flatpak-cruft

mkdir $FLATPAK_DIR

# keep all the state contained with this plus --user
export FLATPAK_USER_DIR=$FLATPAK_DIR/user
export FLATPAK_SYSTEM_DIR=$FLATPAK_DIR/system
export FLATPAK_CONFIG_DIR=$FLATPAK_DIR/config
export FLATPAK_RUN_DIR=$FLATPAK_DIR/runtime

flatpak --user remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

flatpak --user install --noninteractive org.freedesktop.Sdk//20.08
flatpak --user install --noninteractive org.freedesktop.Platform//20.08

flatpak-builder --user --repo=$FLATPAK_DIR/repo --state-dir=$FLATPAK_DIR/state --force-clean $FLATPAK_DIR/build com.mar1d.MAR1D.yml
flatpak build-bundle $FLATPAK_DIR/repo/ MAR1D.flatpak com.mar1d.MAR1D --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
)
