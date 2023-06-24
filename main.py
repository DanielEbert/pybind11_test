
from dataclasses import dataclass
import dataclasses

from cpp.build import cppmain

@dataclass
class Echo:
    dst: int

@dataclass
class Meas:
    de: list[Echo] = dataclasses.field(default_factory=list)
    ce: list[Echo] = dataclasses.field(default_factory=list)
    sendingSensor: int = 1

@dataclass
class EFIOut:
    sendingMask: int = 1
    meas: list[Meas] = dataclasses.field(default_factory=list)

# TODO: seg_sim_creator should output directly these structures

def echo_py_to_cpp(echo: Echo) -> cppmain.Echo:
    return cppmain.Echo(echo.dst)

def meas_py_to_cpp(meas: Meas) -> cppmain.Meas:
    return cppmain.Meas(
        cppmain.EchoTFixedVector([echo_py_to_cpp(j) for j in meas.de]),
        cppmain.EchoTFixedVector([echo_py_to_cpp(j) for j in meas.ce]),
        meas.sendingSensor
    )

def efi_out_py_to_cpp(efi_out: EFIOut) -> cppmain.EFIOut:
    return cppmain.EFIOut(
        efi_out.sendingMask,
        cppmain.MeasTFixedVector([meas_py_to_cpp(i) for i in efi_out.meas])
    )

"""
def python_to_cpp(efi_out: EFIOut) -> cppmain.EFIOut:
    return cppmain.EFIOut(
        efi_out.sendingMask,
        cppmain.MeasTFixedVector([cppmain.Meas(
            cppmain.EchoTFixedVector([echo_py_to_cpp(j) for j in i.de]),
            cppmain.EchoTFixedVector([echo_py_to_cpp(j) for j in i.ce]),
            i.sendingSensor
        ) for i in efi_out.meas])
    )
"""

efi_out = EFIOut(1, [Meas([Echo(2), Echo(3)], [], 1)])

efi_out_cpp = efi_out_py_to_cpp(efi_out) #python_to_cpp(efi_out)

# efi_out_cpp = cppmain.EFIOut(
#     1,
#     cppmain.MeasTFixedVector([
#         cppmain.Meas(
#             cppmain.EchoTFixedVector([cppmain.Echo(2), cppmain.Echo(3)]),
#             cppmain.EchoTFixedVector(),
#             1
#         )
#     ])
# )

