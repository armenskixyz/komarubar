{
  description = "Simple desktop bar for hyprland";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      packages.${system}.default = pkgs.stdenv.mkDerivation {
        pname = "komarubar";
        version = "0.1.0";
        src = ./.;

        nativeBuildInputs = with pkgs; [
          pkg-config
          gcc
        ];
        buildInputs = with pkgs; [
          gtk3
          gtk-layer-shell
        ];
        buildPhase = ''
          echo "--- Current Directory: $(pwd) ---"
          echo "--- Listing all files in source: ---"
          find . -maxdepth 2 -not -path '*/.*'

          # Manually finding and joining all .c files into a variable
          SOURCES=$(find . -name "*.c" -type f)

          if [ -z "$SOURCES" ]; then
            echo "ERROR: No .c files found in $(pwd)"
            exit 1
          fi

          echo "--- Compiling with sources: $SOURCES ---"

          # Standardizing the GCC call
          gcc -o komarubar $SOURCES \
            $(pkg-config --cflags --libs gtk+-3.0 gtk-layer-shell-0) \
            -lm
        '';

        installPhase = ''
          mkdir -p $out/bin
          cp komarubar $out/bin/

        '';
      };

      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          gcc
          gtk3
          gtk-layer-shell
          bear
          pkg-config
        ];
      };
    };
}
