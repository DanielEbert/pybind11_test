
from dataclasses import dataclass
import dataclasses

from cpp.build import cppmain

@dataclass
class Echo:
    dst: int

@dataclass
class Meas:
    sendingSensor: int = 1
    de: list[Echo] = dataclasses.field(default_factory=list)
    ce: list[Echo] = dataclasses.field(default_factory=list)

@dataclass
class EFIOut:
    sendingMask: int = 1
    meas: list[Meas] = dataclasses.field(default_factory=list)

# TODO: seg_sim_creator should output directly these structures

efi_out_cpp = cppmain.EFIOut()

meas_cpp = cppmain.Meas()
meas_cpp.de = cppmain.EchoTFixedVector([cppmain.Echo(2), cppmain.Echo(3)])

efi_out_cpp.measurements = cppmain.MeasTFixedVector([meas_cpp])
