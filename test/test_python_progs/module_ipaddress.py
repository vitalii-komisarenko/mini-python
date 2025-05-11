import ipaddress

print(ipaddress.v4_int_to_packed(0))
print(ipaddress.v4_int_to_packed(1234))
print(ipaddress.v4_int_to_packed(address=12345678))
print(ipaddress.v4_int_to_packed(0xFFFFFFFF))
