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
    rustc
    nodejs
    yarn
  ];
  shellHook = ''
    export PATH=$PATH:${pkgs.nodejs}/bin:${pkgs.yarn}/bin
  '';

}

