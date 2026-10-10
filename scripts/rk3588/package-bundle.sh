#!/bin/sh
# qmake post-link step: keep the ARM runtime and project resources beside FOSD-Pro.
set -eu

if [ "$#" -ne 5 ]; then
    echo "usage: $0 TARGET CROSS_CC VTK_LIB_DIR LAUNCHER_SOURCE PROJECT_DIR" >&2
    exit 2
fi

target=$1
cross_cc=$2
vtk_lib_dir=$3
launcher_source=$4
project_dir=$5
bundle_dir=$(dirname "$target")
sysroot=$("$cross_cc" -print-sysroot)
sysroot_lib=$sysroot/usr/lib
runtime_dir=$bundle_dir/lib/runtime
vtk_dir=$bundle_dir/lib/VTK
launcher_tmp=$target.launcher.tmp

[ -f "$target" ] || { echo "missing linked executable: $target" >&2; exit 1; }
[ -f "$launcher_source" ] || { echo "missing launcher source: $launcher_source" >&2; exit 1; }
[ -d "$vtk_lib_dir" ] || { echo "missing VTK libraries: $vtk_lib_dir" >&2; exit 1; }
[ -d "$sysroot_lib" ] || { echo "missing toolchain sysroot: $sysroot_lib" >&2; exit 1; }
for name in data res; do
    [ -d "$project_dir/$name" ] || {
        echo "missing project resources: $project_dir/$name" >&2
        exit 1
    }
    [ ! -L "$bundle_dir/$name" ] || {
        echo "resource destination must be a real directory: $bundle_dir/$name" >&2
        exit 1
    }
    [ -z "$(find "$project_dir/$name" -type l -print -quit)" ] || {
        echo "project resources contain symbolic links: $project_dir/$name" >&2
        exit 1
    }
done

"$cross_cc" -std=c11 -static -Os -s -Wall -Wextra \
    -o "$launcher_tmp" "$launcher_source"
trap 'rm -f "$launcher_tmp"' EXIT HUP INT TERM

mkdir -p "$runtime_dir" "$vtk_dir"
for name in ld-linux-aarch64.so.1 libc.so.6 libm.so.6 \
            libdl.so.2 libpthread.so.0 librt.so.1 libresolv.so.2 \
            libnss_files.so.2 libnss_dns.so.2 libutil.so.1 libanl.so.1; do
    [ -f "$sysroot_lib/$name" ] || {
        echo "missing toolchain runtime library: $sysroot_lib/$name" >&2
        exit 1
    }
    cp -L "$sysroot_lib/$name" "$runtime_dir/$name"
done

found_vtk=0
for library in "$vtk_lib_dir"/libvtk*.so*; do
    [ -e "$library" ] || continue
    case "$library" in *.bak|*.pre-*) continue ;; esac
    cp -a "$library" "$vtk_dir/"
    found_vtk=1
done
[ "$found_vtk" -eq 1 ] || { echo "no VTK shared libraries found" >&2; exit 1; }

for name in data res; do
    mkdir -p "$bundle_dir/$name"
    cp -a "$project_dir/$name/." "$bundle_dir/$name/"
done

mv -f "$target" "$target.bin"
mv -f "$launcher_tmp" "$target"
trap - EXIT HUP INT TERM
echo "FOSD bundle ready: $bundle_dir"
