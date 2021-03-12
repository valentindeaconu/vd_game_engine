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
import tarfile
import time
import subprocess

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
def update(package_file_location, cache_dir_location, include_dir_location, lib_dir_location):
    with open(package_file_location, 'r') as f:
        # Read package file
        packageJson = f.read()
        packages = json.loads(packageJson)['data']

        # Remove cache directory if it already exists
        if os.path.isdir(cache_dir_location):
            shutil.rmtree(cache_dir_location)

        # Compute cachedir path
        cache_dir_path = os.path.join(os.getcwd(), cache_dir_location)
        
        # Create a new cache directory
        os.mkdir(cache_dir_path)

        # Library Directory
        if os.path.isdir(lib_dir_location):
            shutil.rmtree(lib_dir_location)

        # Compute local lib dir path & create dir
        local_lib_dir_path = lib_dir_location
        os.mkdir(local_lib_dir_path)

        # Include Directory
        if os.path.isdir(include_dir_location):
            shutil.rmtree(include_dir_location)

        # Compute local include dir path & create dir
        local_include_dir_path = include_dir_location
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

            base_ctx = None
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
                    base_ctx = ctx
                    shutil.move(tfn, ctx)
                    print('Done!')             

                elif method.startswith('UNZIP'):
                    dn = method.split(' ')[1]
                    print ('\t[>] Unzipping {} to {}:'.format(ctx, dn), flush=True, end=' ')
                    with zipfile.ZipFile(ctx, 'r') as zr:
                        ctx = os.path.join(cache_dir_location, dn)
                        base_ctx = ctx
                        zr.extractall(ctx)
                        zr.close()
                    print('Done!')

                elif method.startswith('UNTAR'):
                    dn = method.split(' ')[1]
                    print ('\t[>] Tar extract {} to {}:'.format(ctx, dn), flush=True, end=' ')
                    with tarfile.open(ctx) as tr:
                        ctx = os.path.join(cache_dir_location, dn)
                        base_ctx = ctx
                        tr.extractall(ctx)
                        tr.close()
                    print('Done!')

                elif method.startswith('CTX'):
                    dn = method.split(' ')[1]

                    print ('\t[>] Moving context from {}'.format(ctx), flush=True, end=' ')

                    if dn == '/':
                        ctx = base_ctx
                    elif dn.startswith('/'):
                        ctx = join_with_any(base_ctx, dn[1:])
                    else:
                        ctx = join_with_any(ctx, dn)

                    print ('to {}'.format(ctx))

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
                        
                        print ('\t[>] Moving {} files from {}...'.format('HEADER' if is_header else 'LIBRARY', package['LIB']))

                        try:
                            dst = join_with_any(local_include_dir_path if is_header else local_lib_dir_path, tokens[2])
                        except:
                            print('\t\t[#] Could not locate DESTINATION in {} instruction'.format('HEADER' if is_header else 'LIBRARY'))
                            error = True
                            break

                        print ('\t\t[>] DESTINATION found at {}'.format(dst))

                        is_file = False
                        file_basename = None
                        if src.endswith('/*'):
                            src = src[0:len(src) - 2]
                        elif src.endswith('/'):
                            src = src[0:len(src) - 1]
                        else:
                            is_file = True
                            file_basename = os.path.basename(src)
                            src = os.path.dirname(src)

                        wd = None
                        try:
                            wd = join_with_any(ctx, src)
                        except:
                            print('\t\t[#] Could not locate SOURCE in {} instruction!'.format('HEADER' if is_header else 'LIBRARY'))
                            error = True
                            break

                        print ('\t\t[>] SOURCE found at {}'.format(wd))

                        if os.path.isdir(wd):
                            units = [file_basename] if is_file else os.listdir(wd)

                            for el in units:
                                current = os.path.join(wd, el)

                                link_names = []
                                while (os.path.islink(current)):
                                    link_names.append(os.path.basename(current))
                                    current = os.readlink(current)
                                    current = os.path.join(wd, current)

                                future = os.path.join(dst, os.path.basename(current))
                                print ('\t\t[>] Moving {} to {}'.format(current, future))
                                shutil.move(current, future)

                                for ln in link_names:
                                    ln_path = os.path.join(dst, ln)
                                    f_name = os.path.basename(future)
                                    print ('\t\t[>] Creating symlink {} to {}'.format(ln, f_name))
                                    os.symlink(f_name, ln_path)

                        print ('\t[>] Done!')

                elif method.startswith('CMAKE') or method.startswith('MAKE'):
                    params = method.split(' ')
                    params[0] = params[0].lower()

                    print ('\t[>] Building using {}...'.format(params[0]))

                    cmd = ' '.join(params)

                    cwd = os.getcwd()

                    wd = join_with_any(cwd, ctx)

                    print ('\t\t[>] Changing working directory to {}:'.format(wd), flush=True, end=' ')
                    os.chdir(wd)
                    print ('Done!')

                    print ('\t\t[>] Running command "{}": '.format(cmd), flush=True, end=' ')
                    p = subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    print ('Done!')

                    print ('\t\t[>] Changing working directory to {}:'.format(cwd), flush=True, end=' ')
                    os.chdir(cwd)
                    print ('Done!')

                    if p.returncode != 0:
                        print ('\t\t[#] Failure')

                        out = p.stdout.decode('utf-8')
                        err = p.stderr.decode('utf-8')
                        print ('\t\t[#] Output:\n{}'.format(out))
                        print ('\t\t[#] Error:\n{}'.format(err))
                        error = True
                    else:
                        print('\t[>] Done!')

                else:
                    print ('\t[#] Unknown method {}'.format(method))
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

        # Cleaning up cache
        print ('[>] Cleaning up...', flush=True, end=' ')
        time.sleep(3)       
        shutil.rmtree(cache_dir_location)
        print ('Done!')

        f.close()