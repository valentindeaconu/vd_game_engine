#!/usr/bin/env python3

"""
    VDGE Updater is a cross-platform script used to automatically download and move in place all 
    the libraries and headers used by the engine.
    Keep in mind that the download links may become unavailable after a while.
    Last archive revision: 25.01.2021
"""

import os
import shutil
from sys import platform
import json
import urllib.request
import zipfile
import time

# Configs
PACKAGE_FILE    = 'archive.json'
CACHE_DIR       = '.cache_vdge'
INCLUDE_DIR     = 'include'
LIB_DIR         = 'lib'
WIN_LIB_SUBDIR  = 'win'
UNIX_LIB_SUBDIR = 'unix'

# Tools
def join_with_any(base, rel, create_if_not_exists=False):
    path = base
    if rel == '/':
        return path
    for d in rel.split('/'):
        if d == '*':
            any = os.listdir(path)[0]
            path = os.path.join(path, any)
        else:
            path = os.path.join(path, d)
        if not os.path.isdir(path):
            os.mkdir(path)
    return path

# Executable
if __name__ == '__main__':
    with open(PACKAGE_FILE, 'r') as f:
        # Read package file
        packageJson = f.read()
        packages = json.loads(packageJson)['data']

        # Remove cache directory if it already exists
        if os.path.isdir(CACHE_DIR):
            shutil.rmtree(CACHE_DIR)

        # Compute cachedir path
        cache_dir_path = os.path.join(os.getcwd(), CACHE_DIR)
        
        # Create a new cache directory
        os.mkdir(cache_dir_path)

        # Compute local lib dir path & create dir
        local_lib_dir_base_path = os.path.join(cache_dir_path, LIB_DIR)
        os.mkdir(local_lib_dir_base_path)
        local_lib_dir_path = os.path.join(local_lib_dir_base_path, WIN_LIB_SUBDIR if platform == 'win32' else UNIX_LIB_SUBDIR)
        os.mkdir(local_lib_dir_path)

        # Compute local include dir path & create dir
        local_include_dir_path = os.path.join(cache_dir_path, INCLUDE_DIR)
        os.mkdir(local_include_dir_path)

        # Total packages counter
        total_count = 0

        for package in packages:
            print ('[>] Installing {}'.format(package['LIB']))

            configuration = None

            error = False

            if 'ANY' in package['PLATFORM']:
                print ('\t[!] Cross-platform configuration found')
                configuration = package['PLATFORM']['ANY']
            elif (platform == 'linux' or platform == 'linux2' or platform == 'darwin') and ('UNIX' in package['PLATFORM']):
                print ('\t[!] UNIX configuration found')
                configuration = package['PLATFORM']['UNIX']
            elif (platform == 'win32') and ('WIN' in package['PLATFORM']):
                print ('\t[!] Windows configuration found')
                configuration = package['PLATFORM']['WIN']
            else:
                print ('\t[#] No configuration found for {}'.format(platform))
                error = True

            ctx = None

            # Run package methods
            for method in configuration['METHODS']:
                if error:
                    break

                if method.startswith('DOWNLOAD'):
                    fn = method.split(' ')[1]

                    print('\t[>] Downloading from {}:'.format(configuration['LINK']), flush=True, end=' ')
                    tfn, _ = urllib.request.urlretrieve(configuration['LINK'])
                    ctx = os.path.join(cache_dir_path, fn)
                    shutil.move(tfn, ctx)
                    print('Done!')             

                elif method.startswith('UNZIP'):
                    dn = method.split(' ')[1]
                    print ('\t[>] Unzipping {} to {}:'.format(ctx, dn), flush=True, end=' ')
                    with zipfile.ZipFile(ctx, 'r') as zr:
                        ctx = os.path.join(CACHE_DIR, dn)
                        zr.extractall(ctx)
                        zr.close()
                    print('Done!')

                elif method.startswith('HEADER') or method.startswith('LIBRARY'):
                    is_header = method.startswith('HEADER')
                    no_params = True if is_header and method == 'HEADER' else True if not is_header and method == 'LIBRARY' else False

                    if no_params:
                        bn = os.path.basename(ctx)
                        print ('\t[>] Moving {} {}:'.format('HEADER' if is_header else 'LIBRARY', bn), flush=True, end=' ')
                        shutil.move(ctx, os.path.join(local_include_dir_path if is_header else local_lib_dir_path, bn))
                        print ('Done!')                        
                    else:
                        tokens = method.split(' ')
                        src = tokens[1]
                        dst = tokens[2]
                        
                        print ('\t[>] Moving {} files from {}:'.format('HEADER' if is_header else 'LIBRARY', package['LIB']), flush=True, end=' ')

                        try:
                            dst = join_with_any(local_include_dir_path if is_header else local_lib_dir_path, tokens[2])
                        except:
                            print('\t[#] Could not locate DESTINATION in {} instruction'.format('HEADER' if is_header else 'LIBRARY'))
                            error = True
                            break

                        if src.endswith('/*'):
                            src = src[0:len(src) - 2]
                            wd = None
                            try:
                                wd = join_with_any(ctx, src)
                            except:
                                print('\t[#] Could not locate SOURCE in {} instruction!'.format('HEADER' if is_header else 'LIBRARY'))
                                error = True
                                break

                            if not os.path.isdir(wd):
                                print('\t[#] {} is not a directory'.format(wd))
                                error = True
                                break

                            for el in os.listdir(wd):
                                current = os.path.join(wd, el)
                                future = os.path.join(dst, el)
                                shutil.move(current, future)

                        print ('Done!')
                else:
                    print ('[#] Unknown method {}'.format(method))
                    error = True

            if error:
                print ('[\033[91mFAIL\033[0m] {} was not installed.'.format(package['LIB']))
                break
            else:
                print ('[\033[92mOK\033[0m] {} installed successfully.'.format(package['LIB']))
                total_count += 1

        if total_count < len(packages):
            print ('[\033[91mFAIL\033[0m] Updater failed after {}/{} packages.'.format(total_count, len(packages)))
        else:
            print ('[\033[92mOK\033[0m] Updater installed {}/{} packages.'.format(total_count, len(packages)))

        # Results
        # Library Directory
        if os.path.isdir(LIB_DIR):
            shutil.rmtree(LIB_DIR)
        shutil.move(local_lib_dir_base_path, LIB_DIR)

        # Include Directory
        if os.path.isdir(INCLUDE_DIR):
            shutil.rmtree(INCLUDE_DIR)
        shutil.move(local_include_dir_path, INCLUDE_DIR)

        # Cleaning up cache
        print ('[>] Cleaning up...', flush=True, end=' ')
        time.sleep(3)       
        shutil.rmtree(CACHE_DIR)
        print ('Done!')

        f.close()