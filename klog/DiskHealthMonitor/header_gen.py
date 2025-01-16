import sys


with open(sys.argv[1], "rb") as f:
    data = f.read()

with open("gen.h", "w") as out:
    out.write("#pragma once\n")

    out.write("const unsigned char embedded_exe[] = {")
    out.write(",".join(f"0x{byte:02x}" for byte in data))
    out.write("};")