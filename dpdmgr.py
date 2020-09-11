import os
from sys import platform
import urllib.request

# CONFIGS
URLS = { 
    'STB_IMAGE': 'https://raw.githubusercontent.com/nothings/stb/master/stb_image.h',
    'TINY_OBJ_LOADER': 'https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/master/tiny_obj_loader.h',
    'GLFW': 'https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip',
    'GLFW_SRC': 'https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.zip',
    'GLEW': 'https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download',
    'GLEW_SRC': 'https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download',
    'GLM': 'https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip',
}

# Only for Windows
# Possible options: lib-mingw-w64 lib-vc2012 lib-vc2013 lib-vc2015 lib-vc2017 lib-vc2019
GLFW_COMPILER_LIBS = 'lib-vc2019'


# HANDLERS
def stb_image_handler():
    if platform == "linux" or platform == "linux2":
        download(URLS['STB_IMAGE'], 'include/stb_image.h')
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        download(URLS['STB_IMAGE'], 'include\\stb_image.h')


def tiny_obj_loader_handler():
    if platform == "linux" or platform == "linux2":
        download(URLS['TINY_OBJ_LOADER'], 'include/tiny_obj_loader.h')
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        download(URLS['TINY_OBJ_LOADER'], 'include\\tiny_obj_loader.h')


def glfw_handler():
    commands = []

    if platform == "linux" or platform == "linux2":
        download(URLS['GLFW_SRC'], 'glfw.zip')
        commands = [
            'mkdir glfw',
            'unzip glfw.zip -d glfw',
            'cp -r glfw/glfw*/* ./glfw/',
            'cp -r glfw/include/* ./include/',
            'cmake -B./glfw/ -S./glfw/',
            'make -C ./glfw/ clean',
            'make -C ./glfw/ install',
            'cp /usr/local/lib/libglfw3.a ./lib/unix/libglfw3.a',
            'rm -rf glfw',
            'rm -rf glfw.zip'
        ]
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        download(URLS['GLFW'], 'glfw.zip')
        commands = [
            'md glfw',
            'tar -xf tar -xf glfw.zip -C glfw --strip-components 1',
            'copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3.dll .\\lib\\win\\',
            'copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3.lib .\\lib\\win\\',
            'copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3dll.lib .\\lib\\win\\',
            'Xcopy /E /Y glfw\\include\\* .\\include\\',
            'rmdir /s /q glfw',
            'del glfw.zip'
        ]

    for command in commands:
        print(command)
        os.system(command)

    
def glew_handler():
    commands = []

    if platform == "linux" or platform == "linux2":
        download(URLS['GLEW_SRC'], 'glew.zip')
        commands = [
            'mkdir glew',
            'unzip glew.zip -d glew',
            'cp -r glew/glew*/* ./glew/',
            'cp -r glew/include/* ./include/',
            'make -C ./glew/ clean',
            'make -C ./glew/ install',
            'cp -r glew/lib/* ./lib/unix/',
            'make -C ./glew/ uninstall',
            'rm -rf glew',
            'rm -rf glew.zip'
        ]
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        download(URLS['GLEW'], 'glew.zip')
        commands = [
            'md glew',
            'tar -xf glew.zip -C glew --strip-components 1',
            'copy /Y glew\\bin\\Release\\x64\\glew32.dll .\\lib\\win\\',
            'copy /Y glew\\lib\\Release\\x64\\glew32.lib .\\lib\\win\\',
            'copy /Y glew\\lib\\Release\\x64\\glew32s.lib .\\lib\\win\\',
            'Xcopy /E /Y glew\\include\\* .\\include\\',
            'rmdir /s /q glew',
            'del glew.zip'
        ]

    for command in commands:
        print(command)
        os.system(command)


def glm_handler():
    download(URLS['GLM'], 'glm.zip')

    commands = []
    if platform == "linux" or platform == "linux2":
        commands = [
            'mkdir glm',
            'unzip glm.zip -d glm',
            'cp -r glm/* ./',
            'mkdir include/glm',
            'cp -r glm/glm/* ./include/glm/',
            'rm -rf glm',
            'rm -rf glm.zip'
        ]
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        commands = [
            'md glm',
            'tar -xf glm.zip -C glm --strip-components 1',
            'Xcopy /E /Y glm\\glm\\* .\\include\\glm\\',
            'rmdir /s /q glm',
            'del glm.zip'
        ]

    for command in commands:
        print(command)
        os.system(command)


HANDLERS = [stb_image_handler, tiny_obj_loader_handler, glfw_handler, glew_handler, glm_handler]


# UTILS
def download(url, filename):
    print('Downloading from ' + url + '...')
    urllib.request.urlretrieve(url, filename)


# STEPS
def clean():
    commands = []

    if platform == "linux" or platform == "linux2":
        commands = [
            'rm -rf include',
            'rm -rf lib'
        ]
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        commands = [
            'rmdir /s /q include',
            'rmdir /s /q lib'
        ]
    
    for command in commands:
        print(command)
        os.system(command)


def setup():
    pre_commands = []
    post_commands = []

    if platform == "linux" or platform == "linux2":
        pre_commands = [
            'mkdir include',
            'mkdir lib',
            'mkdir lib/unix',
        ]
        post_commands = [
            'chmod -R 0777 include',
            'chmod -R 0777 lib'
        ]
    # TODO # elif platform == "darwin":
    elif platform == "win32":
        pre_commands = [
            'md include',
            'md lib\\win'
        ]

    for command in pre_commands:
        print(command)
        os.system(command)

    for handler in HANDLERS:
        handler()

    for command in post_commands:
        print(command)
        os.system(command)



# Main
print('Cleaning space before the download process begins...')
clean()
print('Workspace cleaned')

setup()
print('====================================')
print('Dependencies downloaded successfully')

