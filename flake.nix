{
  description = "template";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-26.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    # generates outputs for all common systems
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            xmake
            clang-tools
            pkg-config

            glfw
            cglm
            libclipboard
            libxrandr
            libxcursor
          ];

          shellHook = ''
            if [ ! -f compile_commands.json ]; then
              # generates compile_commands.json for clangd
              xmake project -k compile_commands
            fi
          '';
        };
      }
    );
}
