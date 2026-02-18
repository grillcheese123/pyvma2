from setuptools import setup

setup(
    cffi_modules=["src/pyvma2/_build_ffi.py:ffibuilder"],
)
