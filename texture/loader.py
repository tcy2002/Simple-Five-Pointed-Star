def loadFile(file: str):
    with open(file, 'rb') as f:
        return f.read()


def addEscape(file: bytes):
    list = [f'\\x{hex(ch)[2:]}' if ch > 15 else f'\\x0{hex(ch)[2:]}' for ch in file]
    return "".join(list)


if __name__ == '__main__':
    pic1 = loadFile("../../hw1-ulti/texture/1.bmp")
    pic1 = addEscape(pic1)
    particle = loadFile("../../hw1-ulti/texture/star-3d.bmp")
    particle = addEscape(particle)

    with open('../../hw1-ulti/texture/textureTextBuf.h', 'w') as f:
        f.write(f'#pragma once\n'
                f'\n'
                f'const unsigned char pic1_bmp[] = "{pic1}";\n'
                f'\n'
                f'const unsigned char particle_bmp[] = "{particle}";\n'
                f'\n')
