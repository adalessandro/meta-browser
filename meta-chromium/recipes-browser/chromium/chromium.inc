DESCRIPTION = "Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web."
HOMEPAGE = "https://www.chromium.org/Home"

CVE_PRODUCT = "chromium:chromium google:chrome"

SRC_URI = "https://commondatastorage.googleapis.com/chromium-browser-official/chromium-${PV}.tar.xz"
SRC_URI[sha256sum] = "5d12ffc2a33027bde1b15998b28f48560647f1196ecca068ba8cde6bd68f8c9f"

S = "${WORKDIR}/chromium-${PV}"

# GCC is not tested or officially supported upstream, and supporting it here
# requires an ever-growing amount of backports and custom patches, without any
# guarantee that the generated code will work as expected.
TOOLCHAIN = "clang"

# By default, clang.bbclass sets TOOLCHAIN:class-native to "gcc", but we want
# to build the native recipes (e.g. GN) with clang too.
TOOLCHAIN:class-native = "clang"

# This makes the target build use libc++ and compiler_rt instead of the GNU
# runtime, just like upstream does. The native binaries compiled and run as
# part of the build still use libstdc++ and libgcc though (see
# https://github.com/kraj/meta-clang/issues/449).
RUNTIME = "llvm"
# TC_CXX_RUNTIME is same as RUNTIME but adopted by yocto project
# eventually RUNTIME will be removed
TC_CXX_RUNTIME = "llvm"

# meta-clang uses RUNTIME variable to add build dependencies on libcxx/compiler-rt
# but does not add compiler options to commandline. It expects compiler's own
# defaults to be used and compiler's buildtime defaults are chosen based on
# global RUNTIME variable setting when building clang compiler, which defaults to 'gnu'.
# For chromium libc++ is expected to be used therefore ensure that
# right build flags are set for chromium irrespective of
# distro defaults (which could be 'gnu' or 'llvm').
# This also ensures that it does not rely too much on meta-clang's defaults.
LIBCPLUSPLUS = "-stdlib=libc++"

# Explicitly add dependency on libcxx, because meta-clang/hardknott checks for
# --stdlib=libc++ in LIBCPLUSPLUS (with extra dash, which was removed in
# newer meta-clang in https://github.com/kraj/meta-clang/commit/cda2283e4cf3c90ea42b43dbc3a2268be6655670)
DEPENDS += "libcxx"

inherit mime-xdg python3native
DEPENDS += "python3-setuptools-native"

# Chromium itself is licensed under the 3-clause BSD license. However, it
# depends upon several other projects whose copyright files are listed in
# LIC_FILES_CHKSUM. The licenses in those files vary, but none of them should
# cause the main Chromium license to change. The list of copyright files was
# generated with the aid of Chromium's own tools/licenses.py module. As such,
# it contains more items than we actually use in our build (e.g. there are
# components specific to Android, OS X, iOS etc, or bundled dependencies that
# we replace with system-wide versions). For more information about each
# project, you can consult README.chromium in each directory listed below.
# LICENSE does not cover all the licenses in LIC_FILES_CHKSUM; instead, it
# currently contains Chromium's license (BSD-3-Clause) as well as Blink's
# (LGPL2.0, LGPL2.1 and BSD-2-Clause).
LICENSE = "BSD-3-Clause & LGPL-2.0-or-later & LGPL-2.1-or-later"

# The generate_license_list.py script can help generate and update this list.
LIC_FILES_CHKSUM = "\
    file://${S}/LICENSE;md5=c408a301e3407c3803499ce9290515d6 \
    "
