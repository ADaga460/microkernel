# Copilot / AI agent instructions for the microkernel repo

Purpose: help an AI coding agent become productive quickly by summarizing architecture, developer workflows, and project-specific conventions.

1) Big picture
- This is a tiny x86_64 microkernel/boot project written in assembly under `src/impl/x86_64/boot` (files: `header.asm`, `main.asm`, `main64.asm`).
- Build produces a flat `kernel.bin` which is embedded into a GRUB ISO at `targets/x86_64/iso/boot/kernel.bin` and packaged as `dist/x86_64/kernel.iso`.
- The linker script is `targets/x86_64/linker.ld` which defines the memory layout and sections for the kernel image.

2) Key workflows (how to build / run / test locally)
- Primary build command: `make build-x86_64` (inspects `src/impl/x86_64/*.asm`, emits objects under `build/x86_64`, links with `x86_64-elf-ld`, and creates `dist/x86_64/kernel.iso`).
- Required toolchain (host): `nasm`, a cross `x86_64-elf-ld` (binutils for x86_64-elf), `grub-mkrescue`/`grub-pc-bin`, and `xorriso`.
- Containerized build (recommended on non-Linux hosts): use the provided `buildenv/Dockerfile`.
  - Build container: `docker build -t mykernel buildenv`
  - Run build inside container: `docker run --rm -it -v ${PWD}:/root/env mykernel` then run `make build-x86_64` inside container.
- Run ISO in QEMU: `qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`

3) Project-specific conventions and patterns
- Assembly layout: sources under `src/impl/x86_64/` are compiled with `nasm -f elf64` by the Makefile and transformed into `build/x86_64/*.o` using a `find` + `patsubst` pattern.
- The Makefile expects the final `kernel.bin` to be copied to `targets/x86_64/iso/boot/kernel.bin` before `grub-mkrescue` creates the ISO.
- Do not assume higher-level OS services — this repo's code is low-level boot/early kernel code; changes to entry/stack/segment setup require careful coordination between `header.asm`, `main.asm`, `main64.asm`, and `linker.ld`.

4) Important files to inspect when making changes
- Boot sources: `src/impl/x86_64/boot/header.asm`, `src/impl/x86_64/boot/main.asm`, `src/impl/x86_64/boot/main64.asm`.
- Linker: `targets/x86_64/linker.ld` (controls virtual addresses and section placement).
- GRUB config for the ISO: `targets/x86_64/iso/boot/grub/grub.cfg`.
- Makefile: top-level `Makefile` (shows compile/link/iso steps).
- Docker: `buildenv/Dockerfile` (containerized toolchain and example run commands).

5) Integration / cross-component notes
- The build is native artifacts -> `dist` -> ISO. Any change that affects symbol names, section names, or entry point must be mirrored in the linker script and copied path for `kernel.bin`.
- GRUB expects `targets/x86_64/iso/boot/kernel.bin` at ISO creation time. Keep that path intact unless updating `Makefile` and `grub.cfg` together.

6) Troubleshooting & debugging tips
- If linking fails, confirm `x86_64-elf-ld` is installed (cross-binutils). On Windows, use WSL or the container image from `buildenv`.
- If ISO creation fails, ensure `grub-mkrescue` and `xorriso` are installed and that `/usr/lib/grub/i386-pc` (GRUB platform files) exist where `grub-mkrescue` expects them.
- Run the ISO under QEMU to exercise boot paths quickly: `qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`.

7) What an AI agent should do first when changing code
- Update the smallest file that exercises behavior (e.g., `main64.asm`), then rebuild with `make build-x86_64`, and run under QEMU.
- When changing memory layout or symbols, update `targets/x86_64/linker.ld` and ensure `grub.cfg` still points to the correct entry, then rebuild.
- Preserve naming conventions and filename locations used by the Makefile (assembly under `src/impl/x86_64`, objects in `build/x86_64`, outputs in `dist/x86_64`).

8) Examples (copyable commands)
- Build locally (Linux):

  make build-x86_64

- Containerized (recommended on Windows):

  docker build -t mykernel buildenv
  docker run --rm -it -v ${PWD}:/root/env mykernel
  # inside container: make build-x86_64

- Run the ISO:

  qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso

9) When to ask for human help
- If a change requires altering the linker script or GRUB config, confirm intended addresses and boot protocol with a human reviewer before committing.

If any section is unclear or you'd like this to include more specific examples (e.g., annotated snippets from `main64.asm`), tell me which files to expand and I'll iterate.
