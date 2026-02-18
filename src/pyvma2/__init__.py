"""
pyvma2 — Modern Python CFFI wrapper for Vulkan Memory Allocator 3.x

Drop-in compatible with pyvma1 API as used by grilly.
Exports: ffi, lib, vma (alias for lib)
"""

from pyvma2._vma import ffi, lib

# Backward-compatible alias:
# grilly does: pyvma_lib = getattr(pyvma, "vma", None) or getattr(pyvma, "lib", None)
vma = lib

__version__ = "3.4.0"
__all__ = ["ffi", "lib", "vma"]
