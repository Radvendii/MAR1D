#!/usr/bin/env bash
set -euo pipefail

mkdir -p flatpak-cruft/flatpak-data

# keep all the state contained (hopefully)
export FLATPAK_USER_DIR=$PWD/flatpak-cruft/flatpak-user
export FLATPAK_SYSTEM_DIR=$PWD/flatpak-cruft/flatpak-system
export FLATPAK_CONFIG_DIR=$PWD/flatpak-cruft/flatpak-config
export FLATPAK_RUN_DIR=$PWD/flatpak-cruft/flatpak-runtime

flatpak --user remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

flatpak --user install --noninteractive org.freedesktop.Sdk//20.08
flatpak --user install --noninteractive org.freedesktop.Platform//20.08

flatpak-builder --user --disable-cache --repo=flatpak-cruft/flatpak-repo --state-dir=flatpak-cruft/flatpak-state --force-clean flatpak-cruft/flatpak-build com.mar1d.MAR1D.yml
flatpak build-bundle flatpak-cruft/flatpak-repo/ MAR1D.flatpak com.mar1d.MAR1D --runtime-repo=https://flathub.org/repo/flathub.flatpakrepo
