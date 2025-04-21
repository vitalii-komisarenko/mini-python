import binascii

print(binascii.hexlify(b"abcdef"))
print(binascii.hexlify(b"abcdef", "-"))
print(binascii.hexlify(b"abcdef", "-", 2))
print(binascii.hexlify(b"abcdefd", "-", 2))
print(binascii.hexlify(b"abcdefd", "-", 3))
print(binascii.hexlify(b"abcdef", "-", -2))
print(binascii.hexlify(b"abcdefd", "-", -2))
print(binascii.hexlify(b"abcdefd", "-", -3))

print(binascii.hexlify(b"abcdefd", b"-"))
print(binascii.hexlify(b"abcdef", b"-", -2))

print(binascii.hexlify(b"abcdef", b"-", 0))

print(binascii.b2a_hex(b"abcdef"))
print(binascii.b2a_hex(b"abcdef", "-"))
print(binascii.b2a_hex(b"abcdef", "-", 2))
print(binascii.b2a_hex(b"abcdefd", "-", 2))
print(binascii.b2a_hex(b"abcdefd", "-", 3))
print(binascii.b2a_hex(b"abcdef", "-", -2))
print(binascii.b2a_hex(b"abcdefd", "-", -2))
print(binascii.b2a_hex(b"abcdefd", "-", -3))
