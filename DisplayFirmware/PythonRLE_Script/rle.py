from pathlib import Path
import re

# Read the uploaded bikeData.txt file
file_path = Path("./warningData.txt")
text = file_path.read_text()

# Extract hex byte values from the C array
hex_values = re.findall(r'0x([0-9A-Fa-f]{2})', text)
byte_array = bytearray(int(h, 16) for h in hex_values)

# RLE compression
def rle_compress(data):
    output = bytearray()
    i = 0
    while i < len(data):
        value = data[i]
        count = 1
        while i + count < len(data) and data[i + count] == value and count < 255:
            count += 1
        output.append(value)
        output.append(count)
        i += count
    return output

compressed = rle_compress(byte_array)

# Format compressed data as a C array
lines = []
lines.append("const uint8_t warningicon_rle[] = {")
for i in range(0, len(compressed), 12):
    chunk = ", ".join(f"0x{b:02X}" for b in compressed[i:i+12])
    lines.append("    " + chunk + ",")
lines[-1] = lines[-1].rstrip(",")  # Remove trailing comma from last line
lines.append("};")
lines.append(f"const size_t warningicon_rle_len = sizeof(warningicon_rle);")

# Save as .h file
header_path = Path("warningData_rle.h")
header_path.write_text("\n".join(lines))

header_path.name
