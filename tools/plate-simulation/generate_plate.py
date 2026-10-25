"""Generate the fixed demo plate as a closed, consistently wound binary STL (mm)."""
import argparse
import csv
import math
from pathlib import Path
import struct

LENGTH, WIDTH, THICKNESS = 1000.0, 400.0, 4.0
NX, NY = 40, 16

def triangles():
    def p(i, j, z):
        return (LENGTH * i / NX, -WIDTH / 2 + WIDTH * j / NY, z)
    def quad(a, b, c, d):
        return [(a, b, c), (a, c, d)]
    top, bottom = THICKNESS / 2, -THICKNESS / 2
    for i in range(NX):
        for j in range(NY):
            yield from quad(p(i, j, top), p(i + 1, j, top), p(i + 1, j + 1, top), p(i, j + 1, top))
            yield from quad(p(i, j, bottom), p(i, j + 1, bottom), p(i + 1, j + 1, bottom), p(i + 1, j, bottom))
    for i in range(NX):
        yield from quad(p(i, 0, bottom), p(i + 1, 0, bottom), p(i + 1, 0, top), p(i, 0, top))
        yield from quad(p(i, NY, bottom), p(i, NY, top), p(i + 1, NY, top), p(i + 1, NY, bottom))
    for j in range(NY):
        yield from quad(p(0, j, bottom), p(0, j, top), p(0, j + 1, top), p(0, j + 1, bottom))
        yield from quad(p(NX, j, bottom), p(NX, j + 1, bottom), p(NX, j + 1, top), p(NX, j, top))

def write_model(path):
    faces = list(triangles())
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as output:
        output.write(b"FOSD demo plate 1000x400x4 mm; clamped x=0".ljust(80, b"\0"))
        output.write(struct.pack("<I", len(faces)))
        for a, b, c in faces:
            u, v = [b[k] - a[k] for k in range(3)], [c[k] - a[k] for k in range(3)]
            normal = (u[1]*v[2]-u[2]*v[1], u[2]*v[0]-u[0]*v[2], u[0]*v[1]-u[1]*v[0])
            norm = math.sqrt(sum(x*x for x in normal))
            output.write(struct.pack("<12fH", *(x/norm for x in normal), *a, *b, *c, 0))
    with path.with_name("sensors.csv").open("w", newline="", encoding="utf-8") as output:
        writer = csv.writer(output)
        writer.writerow(["index", "channel", "sensor", "x_mm", "y_mm", "z_mm", "direction", "reference_nm"])
        for i in range(12):
            eta = -0.9 + 1.8*i/11
            writer.writerow([i, i+1, 1, LENGTH*(0.25+0.6*(i%4)/3), WIDTH*eta/2, THICKNESS/2, "xx", 1550+0.5*i])
    print(f"{path}: {len(faces)} triangles")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path(__file__).parent / "models" / "plate.stl")
    write_model(parser.parse_args().output)
