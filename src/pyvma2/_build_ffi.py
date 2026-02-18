"""
CFFI out-of-line builder for pyvma2.

Links against the pre-built VMA static library (vk_mem_alloc.lib / libvk_mem_alloc.a).
The library must be compiled first via build_vma_lib.py.
"""

import os
import platform

from cffi import FFI

HERE = os.path.dirname(os.path.realpath(__file__))
VENDOR_DIR = os.path.join(HERE, "vendor")

ffibuilder = FFI()

# Load CFFI cdef declarations
cdef_path = os.path.join(HERE, "cdef", "vma.cdef.h")
with open(cdef_path) as f:
    ffibuilder.cdef(f.read())

# Libraries to link
libs = ["vk_mem_alloc"]
if platform.system() != "Windows":
    libs += ["stdc++"]

# Build include directories list
include_dirs = [VENDOR_DIR]

# Find Vulkan SDK include (VMA needs vulkan/vulkan.h)
_vulkan_sdk_candidates = [
    os.environ.get("VULKAN_SDK", ""),
    os.environ.get("VK_SDK_PATH", ""),
    "C:/Users/grill/VulkanSDK",
    "C:/VulkanSDK",
]
for base in _vulkan_sdk_candidates:
    if not base:
        continue
    for sub in ("Include", "include"):
        inc = os.path.join(base, sub)
        if os.path.isfile(os.path.join(inc, "vulkan", "vulkan.h")):
            include_dirs.append(inc)
            break

# C source that #includes the VMA header (NOT with VMA_IMPLEMENTATION —
# that's in the static lib we link against).
ffibuilder.set_source(
    "pyvma2._vma",
    """
    #define VMA_STATIC_VULKAN_FUNCTIONS  0
    #define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
    #include "vk_mem_alloc.h"
    """,
    libraries=libs,
    library_dirs=[VENDOR_DIR],
    include_dirs=include_dirs,
)

if __name__ == "__main__":
    ffibuilder.compile(verbose=True)
