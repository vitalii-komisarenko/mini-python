import base64

print(base64.decodebytes(b'===='))
print(base64.decodebytes(s=b'====\naaaa'))
print(base64.decodebytes(b'====\n\naaaa    '))
print(base64.decodebytes(b'== ==\n\naaaa    '))
print(base64.decodebytes(b'=\n\naaaa    '))
print(base64.decodebytes(b'aa==\n\naaaa    '))
print(base64.decodebytes(b'aa\n\naaaa==    '))
print(base64.decodebytes(b'a\ra\tb=='))
print(base64.decodebytes(b'a\ra\tb=!='))
print(base64.decodebytes(b'a\ra\tb=()[]{}-\\='))
print(base64.decodebytes(b'%^aaaa!@#$rtyu'))

print(base64.encodebytes(b''))
print(base64.encodebytes(b'='))
print(base64.encodebytes(b'=a'))
print(base64.encodebytes(b'%^&'))
print(base64.encodebytes(b'k;p8#'))

print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddgggg'))
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg')) # 76 characters after encoding
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggga'))

print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxddddddddddddggggg'))
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg')) # 2 X 76 characters after encoding
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggga'))

print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg'))
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg')) # 3 X 76 characters after encoding
print(base64.encodebytes(b'1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg1124rt1sfvdbrgwbetSDGSDGER3243133rt2gvxdddddddddddddggggg'))
