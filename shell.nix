{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell rec {
  buildInputs = with pkgs; [
    python3
    cmake
    gcc
    clang
    pkg-config
    gnumake
    ninja
    git
  ];

}

