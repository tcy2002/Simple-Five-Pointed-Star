def loadFile(file: str):
    with open(file, 'rb') as f:
        return f.read()


if __name__ == '__main__':
    simple_vert = f'{loadFile("./simple.vert")}'
    simple_frag = f'{loadFile("./simple.frag")}'
    particle_vert = f'{loadFile("./particle.vert")}'
    particle_frag = f'{loadFile("./particle.frag")}'
    illumination_vert = f'{loadFile("./illumination.vert")}'
    illumination_frag = f'{loadFile("./illumination.frag")}'

    with open('./shaderTextBuf.h', 'w') as f:
        f.write(f'#pragma once\n'
                f'\n'
                f'const char *simple_vert = "{simple_vert[2:-1]}";\n'
                f'\n'
                f'const char *simple_frag = "{simple_frag[2:-1]}";\n'
                f'\n'
                f'const char *particle_vert = "{particle_vert[2:-1]}";\n'
                f'\n'
                f'const char *particle_frag = "{particle_frag[2:-1]}";\n'
                f'\n'
                f'const char *illumination_vert = "{illumination_vert[2:-1]}";\n'
                f'\n'
                f'const char *illumination_frag = "{illumination_frag[2:-1]}";\n'
                f'')
