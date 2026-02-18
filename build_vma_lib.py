#!/usr/bin/env python3
"""
Build the VMA 3.4.0 static library from vma_impl.cpp.

Windows: produces vendor/vk_mem_alloc.lib (MSVC)
Linux:   produces vendor/libvk_mem_alloc.a  (GCC)

Requires:
 - Vulkan SDK headers (auto-detected from VULKAN_SDK or common paths)
 - MSVC (Windows) or GCC (Linux)
"""

import os
import platform
import shutil
import subprocess
import sys
from pathlib import Path

VENDOR_DIR = Path(__file__).parent / "src" / "pyvma2" / "vendor"
SRC_FILE = VENDOR_DIR / "vma_impl.cpp"

# Common Vulkan SDK locations
VULKAN_SDK_SEARCH = [
    os.environ.get("VULKAN_SDK", ""),
    os.environ.get("VK_SDK_PATH", ""),
    "C:/Users/grill/VulkanSDK",
    "C:/VulkanSDK",
    "/usr",
    "/usr/local",
]


def find_vulkan_include():
    """Find the Vulkan SDK include directory."""
    for base in VULKAN_SDK_SEARCH:
        if not base:
            continue
        inc = Path(base) / "Include"
        if (inc / "vulkan" / "vulkan.h").exists():
            return str(inc)
        inc = Path(base) / "include"
        if (inc / "vulkan" / "vulkan.h").exists():
            return str(inc)
    raise RuntimeError(
        "Vulkan SDK not found. Set VULKAN_SDK environment variable "
        "or install the Vulkan SDK."
    )


def find_msvc():
    """Find MSVC cl.exe and lib.exe via vswhere or PATH."""
    # Check PATH first
    cl = shutil.which("cl")
    if cl:
        lib_exe = shutil.which("lib")
        return cl, lib_exe

    # Try vswhere
    vswhere = Path(
        "C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"
    )
    if vswhere.exists():
        result = subprocess.run(
            [str(vswhere), "-latest", "-property", "installationPath"],
            capture_output=True,
            text=True,
        )
        vs_path = result.stdout.strip()
        if vs_path:
            # Set up MSVC environment
            vcvars = Path(vs_path) / "VC" / "Auxiliary" / "Build" / "vcvarsall.bat"
            if vcvars.exists():
                return str(vcvars), None  # Signal to use vcvars wrapper

    raise RuntimeError("MSVC not found. Install Visual Studio with C++ workload.")


def build_windows():
    """Build vk_mem_alloc.lib on Windows."""
    vulkan_inc = find_vulkan_include()
    vendor_str = str(VENDOR_DIR)
    obj_file = VENDOR_DIR / "vma_impl.obj"
    lib_file = VENDOR_DIR / "vk_mem_alloc.lib"

    cl = shutil.which("cl")
    if cl:
        # cl.exe is on PATH (MSVC environment already set up)
        print(f"[pyvma2] Using cl.exe: {cl}")
        print(f"[pyvma2] Vulkan SDK include: {vulkan_inc}")

        subprocess.run(
            [
                "cl",
                "/c",
                "/nologo",
                "/W3",
                "/Ox",
                "/EHsc",
                "/MD",
                "/TP",
                "/DVMA_IMPLEMENTATION",
                "/DVMA_STATIC_VULKAN_FUNCTIONS=0",
                "/DVMA_DYNAMIC_VULKAN_FUNCTIONS=0",
                f"/I{vulkan_inc}",
                f"/I{vendor_str}",
                str(SRC_FILE),
                f"/Fo:{obj_file}",
            ],
            check=True,
            cwd=str(VENDOR_DIR),
        )

        subprocess.run(
            ["lib", "/nologo", f"/OUT:{lib_file}", str(obj_file)],
            check=True,
            cwd=str(VENDOR_DIR),
        )
    else:
        # Need to set up MSVC environment via vcvarsall.bat
        vswhere = Path(
            "C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe"
        )
        # Query for installations with C++ tools, prefer BuildTools over Community
        result = subprocess.run(
            [
                str(vswhere),
                "-latest",
                "-products",
                "*",
                "-requires",
                "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
                "-property",
                "installationPath",
            ],
            capture_output=True,
            text=True,
        )
        vs_path = result.stdout.strip()
        if not vs_path:
            # Fallback to any VS installation
            result = subprocess.run(
                [str(vswhere), "-latest", "-property", "installationPath"],
                capture_output=True,
                text=True,
            )
            vs_path = result.stdout.strip()
        vcvars = Path(vs_path) / "VC" / "Auxiliary" / "Build" / "vcvarsall.bat"

        cmd = (
            f'call "{vcvars}" x64 && '
            f'cl /c /nologo /W3 /Ox /EHsc /MD /TP '
            f'/DVMA_IMPLEMENTATION /DVMA_STATIC_VULKAN_FUNCTIONS=0 /DVMA_DYNAMIC_VULKAN_FUNCTIONS=0 '
            f'/I"{vulkan_inc}" /I"{vendor_str}" '
            f'"{SRC_FILE}" /Fo:"{obj_file}" && '
            f'lib /nologo /OUT:"{lib_file}" "{obj_file}"'
        )
        subprocess.run(["cmd", "/c", cmd], check=True, cwd=str(VENDOR_DIR))

    # Clean up obj file
    if obj_file.exists():
        obj_file.unlink()

    print(f"[pyvma2] Built: {lib_file}")
    return lib_file


def build_linux():
    """Build libvk_mem_alloc.a on Linux."""
    vulkan_inc = find_vulkan_include()
    vendor_str = str(VENDOR_DIR)
    obj_file = VENDOR_DIR / "vma_impl.o"
    lib_file = VENDOR_DIR / "libvk_mem_alloc.a"

    print(f"[pyvma2] Using g++")
    print(f"[pyvma2] Vulkan SDK include: {vulkan_inc}")

    subprocess.run(
        [
            "g++",
            "-c",
            "-std=c++14",
            "-fPIC",
            "-O2",
            "-DVMA_IMPLEMENTATION",
            "-DVMA_STATIC_VULKAN_FUNCTIONS=0",
            "-DVMA_DYNAMIC_VULKAN_FUNCTIONS=0",
            f"-I{vulkan_inc}",
            f"-I{vendor_str}",
            str(SRC_FILE),
            "-o",
            str(obj_file),
        ],
        check=True,
    )

    subprocess.run(
        ["ar", "rcs", str(lib_file), str(obj_file)],
        check=True,
    )

    if obj_file.exists():
        obj_file.unlink()

    print(f"[pyvma2] Built: {lib_file}")
    return lib_file


def main():
    if not SRC_FILE.exists():
        print(f"Error: {SRC_FILE} not found", file=sys.stderr)
        sys.exit(1)

    system = platform.system()
    if system == "Windows":
        build_windows()
    elif system == "Linux":
        build_linux()
    else:
        print(f"Unsupported platform: {system}", file=sys.stderr)
        sys.exit(1)

    print("[pyvma2] Static library build complete.")


if __name__ == "__main__":
    main()
