# VTK 8.2.0 for RK3588 Buildroot (Release)

- Toolchain: `/opt/atk-dlrk3588-toolchain/share/buildroot/toolchainfile.cmake` (aarch64, GCC 10.4.0)
- Target Qt: 5.15.8; graphics: OpenGL ES 3.0 + EGL
- Source: `/home/alientek/Desktop/VTK/VTK-8.2.0.tar.gz`
- Source compatibility patch: [`scripts/rk3588/rk3588-vtk-8.2.0-gles.patch`](../../../scripts/rk3588/rk3588-vtk-8.2.0-gles.patch)
- GLES runtime patch: [`scripts/rk3588/vtk-8.2-glew-gles.patch`](../../../scripts/rk3588/vtk-8.2-glew-gles.patch)
- Build tree: `/home/alientek/Desktop/VTK/build-rk3588-release`
- Installed here: 55 AArch64 VTK shared libraries, headers, and `lib/cmake/vtk-8.2/VTKConfig.cmake`

The build enables FOSD's Qt, rendering, interaction, filter, and IO modules with their transitive dependencies. Desktop OpenGL export (`gl2ps`) and unused HDF5 modules are excluded because the target sysroot supplies GLES rather than desktop GL. Key cache settings include `CMAKE_BUILD_TYPE=Release`, `BUILD_SHARED_LIBS=ON`, `VTK_OPENGL_USE_GLES=ON`, `VTK_OPENGL_HAS_EGL=ON`, `VTK_USE_X=OFF`, `OPENGL_ES_VERSION=3.0`, and `GL_ES_VERSION_3_0=1` in C++ flags.

Rebuild and install from the configured tree:

```sh
source /opt/atk-dlrk3588-toolchain/environment-setup
cmake --build /home/alientek/Desktop/VTK/build-rk3588-release --parallel 4
cmake --install /home/alientek/Desktop/VTK/build-rk3588-release
```

Validation: all installed VTK `.so.1` files are AArch64 and their SONAME dependencies are present in this prefix or the toolchain sysroot. `/home/alientek/Desktop/VTK/smoke-rk3588/build/vtk_arm_smoke` links successfully against the installed VTK and target Qt. The installed `libvtkRenderingOpenGL2-8.2.so.1` includes the GLES runtime patch (SHA-256 `a442ce5449a65881d1ff1a4a211ba6d2f08e529abd6c75b3d8e5bcecc3fd0f2e`). FOSD's VTK page was exercised on the RK3588 board; deployment details are in [`doc/RK3588开发板部署与启动配置记录.md`](../../../doc/RK3588开发板部署与启动配置记录.md).

The bundled libpng in this VTK 8.2 source tree has no ARM NEON implementation files.
The saved patch therefore sets `PNG_ARM_NEON_OPT=0` for `vtkpng`, preventing an
unresolved `png_init_filter_functions_neon` symbol when FOSD links against VTK.
