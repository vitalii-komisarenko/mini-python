import binascii

# hexlify / b2a_hex

print(binascii.hexlify(b"abcdef"))
print(binascii.hexlify(data=b"abcdef"))
print(binascii.hexlify(bytes_per_sep=2, data=b"abcdefh"))
print(binascii.hexlify(bytes_per_sep=2, sep=':', data=b"abcdefh"))
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

# unhexlify / a2b_hex

print(binascii.unhexlify(''))
print(binascii.unhexlify('0123456789abcdefABCDEF'))
print(binascii.unhexlify(b'0123456789abcdefABCDEF'))

print(binascii.a2b_hex(''))
print(binascii.a2b_hex('0123456789abcdefABCDEF'))
print(binascii.a2b_hex(b'0123456789abcdefABCDEF'))

# b2a_base64

print(binascii.b2a_base64(b''))
print(binascii.b2a_base64(b'1'))
print(binascii.b2a_base64(b'12'))
print(binascii.b2a_base64(b'123'))
print(binascii.b2a_base64(b'1234'))
print(binascii.b2a_base64(b'12345'))

print(binascii.b2a_base64(b'12345', newline=0))
print(binascii.b2a_base64(b'12345', newline=2))
print(binascii.b2a_base64(b'12345', newline=False))
print(binascii.b2a_base64(b'12345', newline=True))

# a2b_base64

print(binascii.a2b_base64(b''))
print(binascii.a2b_base64(b'ab=='))
print(binascii.a2b_base64(b'abc='))
print(binascii.a2b_base64(b'abcd'))

print(binascii.a2b_base64(''))
print(binascii.a2b_base64('ab=='))
print(binascii.a2b_base64('===='))
print(binascii.a2b_base64('=abcd'))
print(binascii.a2b_base64('AAAA'))
print(binascii.a2b_base64('AAAB'))


# b2a_base64
