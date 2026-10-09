#!/bin/sh
# Run Qt Creator's deployed executable with the board's FOSD runtime and data.
APP_DIR=/root/Qt_Project/Qt_Test_Compilation
PROGRAM=/opt/FOSD-Pro/bin/FOSD-Pro
RUNTIME=$APP_DIR/lib/runtime
VTK_LIB=$APP_DIR/lib/VTK

if [ ! -x "$PROGRAM" ] || [ ! -x "$RUNTIME/ld-linux-aarch64.so.1" ]; then
    echo "FOSD executable or private loader is missing" >&2
    exit 1
fi

cd "$APP_DIR" || exit 1
export XDG_RUNTIME_DIR=/run
export WAYLAND_DISPLAY=wayland-0
export QT_QPA_PLATFORM=wayland
export LD_LIBRARY_PATH="$RUNTIME:$VTK_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
exec "$RUNTIME/ld-linux-aarch64.so.1" \
    --library-path "$LD_LIBRARY_PATH:/lib:/usr/lib" "$PROGRAM" "$@"
