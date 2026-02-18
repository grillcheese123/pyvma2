"""
Smoke tests for pyvma2 — verify the CFFI extension loads and exposes the expected API.
"""

import pytest


def test_import():
    """pyvma2 imports without error."""
    import pyvma2

    assert hasattr(pyvma2, "ffi")
    assert hasattr(pyvma2, "lib")
    assert hasattr(pyvma2, "vma")
    assert pyvma2.vma is pyvma2.lib


def test_ffi_available():
    """ffi object supports the operations grilly needs."""
    from pyvma2 import ffi

    assert hasattr(ffi, "cast")
    assert hasattr(ffi, "new")
    assert hasattr(ffi, "memmove")
    assert hasattr(ffi, "from_buffer")
    assert hasattr(ffi, "buffer")
    assert hasattr(ffi, "NULL")


def test_lib_has_vma_functions():
    """lib exposes all VMA functions used by grilly."""
    from pyvma2 import lib

    required = [
        "vmaCreateAllocator",
        "vmaDestroyAllocator",
        "vmaCreateBuffer",
        "vmaDestroyBuffer",
        "vmaMapMemory",
        "vmaUnmapMemory",
        "vmaFlushAllocation",
        "vmaInvalidateAllocation",
        "vmaGetAllocationInfo",
    ]
    for fn_name in required:
        assert hasattr(lib, fn_name), f"Missing: {fn_name}"


def test_ffi_new_structs():
    """ffi.new can allocate all struct types grilly uses."""
    from pyvma2 import ffi

    # VmaVulkanFunctions
    vf = ffi.new("VmaVulkanFunctions*")
    assert vf is not None

    # VmaAllocatorCreateInfo
    ci = ffi.new("VmaAllocatorCreateInfo*")
    assert ci is not None

    # VmaAllocationCreateInfo
    aci = ffi.new("VmaAllocationCreateInfo*")
    assert aci is not None

    # VmaAllocationInfo
    ai = ffi.new("VmaAllocationInfo*")
    assert ai is not None
    # pMappedData must be accessible
    assert ai.pMappedData == ffi.NULL

    # VkBufferCreateInfo
    bci = ffi.new("VkBufferCreateInfo*")
    assert bci is not None

    # VmaAllocator*
    pa = ffi.new("VmaAllocator*")
    assert pa is not None

    # VkBuffer*
    pb = ffi.new("VkBuffer*")
    assert pb is not None

    # VmaAllocation*
    pal = ffi.new("VmaAllocation*")
    assert pal is not None

    # void**
    ppd = ffi.new("void**")
    assert ppd is not None


def test_ffi_cast_operations():
    """ffi.cast works for types grilly uses."""
    from pyvma2 import ffi

    # Cast to void*
    v = ffi.cast("void*", 0)
    assert v == ffi.NULL

    # Cast to uintptr_t
    u = ffi.cast("uintptr_t", ffi.NULL)
    assert int(u) == 0


def test_struct_init_with_dict():
    """Structs can be initialized with keyword dicts (grilly pattern)."""
    from pyvma2 import ffi

    bci = ffi.new(
        "VkBufferCreateInfo*",
        {
            "sType": 12,  # VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO
            "size": 65536,
            "usage": 0x00000080,  # VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
            "sharingMode": 0,
        },
    )
    assert bci.sType == 12
    assert bci.size == 65536
    assert bci.sharingMode == 0

    aci = ffi.new(
        "VmaAllocationCreateInfo*",
        {
            "usage": 3,  # VMA_MEMORY_USAGE_CPU_TO_GPU (deprecated but valid)
            "flags": 4,  # VMA_ALLOCATION_CREATE_MAPPED_BIT
        },
    )
    assert aci.usage == 3
    assert aci.flags == 4


def test_numpy_from_buffer():
    """ffi.from_buffer works with numpy arrays."""
    import numpy as np

    from pyvma2 import ffi

    arr = np.array([1.0, 2.0, 3.0], dtype=np.float32)
    buf = ffi.from_buffer(arr)
    assert buf is not None
