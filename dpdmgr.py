import os
import urllib.request

# CONFIGS
URLS = { 
    'STB_IMAGE': 'https://raw.githubusercontent.com/nothings/stb/master/stb_image.h',
    'TINY_OBJ_LOADER': 'https://raw.githubusercontent.com/tinyobjloader/tinyobjloader/master/tiny_obj_loader.h',
    'GLFW': 'https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip',
    'GLEW': 'https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0-win32.zip/download',
    'GLM': 'https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip'
}

# Possible options: lib-mingw-w64 lib-vc2012 lib-vc2013 lib-vc2015 lib-vc2017 lib-vc2019
GLFW_COMPILER_LIBS = 'lib-vc2019'


# HANDLERS
def stb_image_win_handler():
    download(URLS['STB_IMAGE'], 'include\\stb_image.h')
    
def tiny_obj_loader_win_handler():
    download(URLS['TINY_OBJ_LOADER'], 'include\\tiny_obj_loader.h')
    
def glfw_win_handler():
    download(URLS['GLFW'], 'glfw.zip')
    
    os.system('md glfw')
    
    os.system('tar -xf tar -xf glfw.zip -C glfw --strip-components 1')
    os.system('copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3.dll .\\lib\\win\\')
    os.system('copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3.lib .\\lib\\win\\')
    os.system('copy /Y glfw\\' + GLFW_COMPILER_LIBS + '\\glfw3dll.lib .\\lib\\win\\')
    os.system('Xcopy /E /Y glfw\\include\\* .\\include\\')
    
    os.system('rmdir /s /q glfw')
    os.system('del glfw.zip')
    
def glew_win_handler():
    download(URLS['GLEW'], 'glew.zip')
    
    os.system('md glew')
    
    os.system('tar -xf glew.zip -C glew --strip-components 1')
    os.system('copy /Y glew\\bin\\Release\\x64\\glew32.dll .\\lib\\win\\')
    os.system('copy /Y glew\\lib\\Release\\x64\\glew32.lib .\\lib\\win\\')
    os.system('copy /Y glew\\lib\\Release\\x64\\glew32s.lib .\\lib\\win\\')
    os.system('Xcopy /E /Y glew\\include\\* .\\include\\')
    
    os.system('rmdir /s /q glew')
    os.system('del glew.zip')

def glm_win_handler():
    download(URLS['GLM'], 'glm.zip')
    
    os.system('md glm')
    
    os.system('tar -xf glm.zip -C glm --strip-components 1')
    
    os.system('Xcopy /E /Y glm\\glm\\* .\\include\\glm\\')
    
    os.system('rmdir /s /q glm')
    os.system('del glm.zip')

WIN_HANDLERS = [stb_image_win_handler, tiny_obj_loader_win_handler, glfw_win_handler, glew_win_handler, glm_win_handler]

# UTILS
def download(url, filename):
    print('Downloading from ' + url + '...')
    urllib.request.urlretrieve(url, filename)


# STEPS
def clean():
    os.system('rmdir /s /q include')
    os.system('rmdir /s /q lib')

def setup():
    os.system('md include')
    os.system('md lib\\win')
    
    for handler in WIN_HANDLERS:
        handler()
    

# Main
print('Cleaning space before the download process begins...')
try:
    clean()
except OSError:
    print('There is nothing to clean')
else:
    print('Workspace cleaned')
    
try:
    setup()
except OSError:
    print('=================================')
    print('Dependencies could not be downloaded')
else:
    print('=================================')
    print('Dependencies downloaded successfully')

