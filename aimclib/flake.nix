{
  description = "AIMCLib Cross compilation flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      target = pkgs.pkgsCross.aarch64-multiplatform;
    in {
      packages.x86_64-linux.default = target.stdenv.mkDerivation {
        pname = "example";
        version = "0.1";

        src = ./.;
        nativeBuildInputs = with pkgs; [ 
          pkg-config
          target.stdenv.cc
          target.libc.static
        ];

        CC="aarch64-linux-gnu-gcc";
        CXX="aarch64-linux-gnue-g++";
        PKG_CONFIG_SYSROOT_DIR="${pkgs.pkgsCross.aarch64-multiplatform.stdenv.cc.targetPrefix}";
        PKG_CONFIG_LIBDIR="${pkgs.pkgsCross.aarch64-multiplatform.stdenv.cc.targetPrefix}/lib/pkgconfig";

        buildPhase = ''
          ${target.stdenv.cc}/bin/aarch64-unknown-linux-gnu-g++ -static -o example example.cc
        '';
      };
      
      devShells.x86_64-linux.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          pkg-config
          target.stdenv.cc
          target.libc.static
        ];

        CC="aarch64-linux-gnu-gcc";
        CXX="aarch64-linux-gnue-g++";
        PKG_CONFIG_SYSROOT_DIR="${pkgs.pkgsCross.aarch64-multiplatform.stdenv.cc.targetPrefix}";
        PKG_CONFIG_LIBDIR="${pkgs.pkgsCross.aarch64-multiplatform.stdenv.cc.targetPrefix}/lib/pkgconfig";
        
        shellHook = ''
          echo -e "\e[1;37mAIMCLib cross compilation shell:\e[0m"
        '';
      };
    };
}
