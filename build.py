#!/usr/bin/env python3

"""
    VDGE Build is a cross-platform script used to build the application
    It automatically runs the post-build script when build succeded
    Can be used to only update libraries, by using just the update param
    Commands available: help, build, update
"""

import os
import sys
import json
import subprocess
import shutil

from meta.resource_manager import resource_update
from meta.update_manager import update

CONFIG_PATH = os.path.join('meta', 'config.json')

def extract_params(argv):
    params = {}

    for arg in argv:
        if not (arg.startswith('-') or arg.startswith('--')):
            print ('[#] Ignoring unrecognized argument "{}"'.format(name))
            continue

        is_long = True if arg.startswith('--') else False

        t = arg.split('=')

        name = t[0][2:] if is_long else t[0][1:]
        value = None if len(t) < 2 else t[1]

        params[name] = value

    return params

def configure(build_type, output_dir, verbose = True, clean_configure = False):
    print ('[>] Building in {} mode. Binary will be created at {}.'.format(build_type, output_dir))

    if clean_configure and os.path.isdir(output_dir):
        shutil.rmtree(output_dir)

    cmd = 'cmake -DCMAKE_BUILD_TYPE={} -B{}'.format(build_type, output_dir)

    p = None
    if verbose:
        p = subprocess.run(cmd, shell=True)
    else:
        p = subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    return p.returncode


def run(command_name, value, default, config):
    if command_name == 'build':
        if value == None:
            value = default

        directory = config['BUILD_RELEASE_OUTPUT_DIR'] if value.startswith('r') else config['BUILD_DEBUG_OUTPUT_DIR']
        value = 'Release' if value.startswith('r') else 'Debug'

        if configure(value, directory) == 0:
            cwd = os.getcwd()
            os.chdir(directory)
            p = subprocess.run('make')
            os.chdir(cwd)
            if p.returncode == 0:
                print ('\n=====================================')
                print ('BUILD SUCCESSFUL')
                print ('=====================================\n')
            else:
                print ('\n=====================================')
                print ('BUILD FAILED')
                print ('=====================================\n')
        else:
            print ('\n=====================================')
            print ('BUILD FAILED')
            print ('=====================================\n')

    elif command_name == 'update':
        update(config['PACKAGE_FILE'], config['UPDATE_CACHE_DIR'], config['INCLUDE_DIR'], config['LIB_DIR'])

    elif command_name == 'help':
        print ('[!] VDGE Build System Help Menu')
        for cmd in config['COMMANDS']:
            print ('[=] --{}, -{} <=> {}'.format(cmd['NAME'], cmd['SHORT'], cmd['DESCRIPTION']))
            if len(cmd['VALUES']) > 0: 
                print ('\t[?] Possible values: {}'.format(cmd['VALUES']))

    elif command_name == 'resource-update':
        if value == None:
            resource_update(os.getcwd(), config['BUILD_RELEASE_OUTPUT_DIR'])
            resource_update(os.getcwd(), config['BUILD_DEBUG_OUTPUT_DIR'])
        elif value.startswith('r'):
            resource_update(os.getcwd(), config['BUILD_RELEASE_OUTPUT_DIR'])
        elif value.startswith('d'):
            resource_update(os.getcwd(), config['BUILD_DEBUG_OUTPUT_DIR'])

    elif command_name == 'clean':
        if value == None:
            value = default

        print ('[>] Cleaning workspace, mode: {}'.format(value))

        if os.path.isdir(config['BUILD_RELEASE_OUTPUT_DIR']):
            shutil.rmtree(config['BUILD_RELEASE_OUTPUT_DIR'])
        if os.path.isdir(config['BUILD_DEBUG_OUTPUT_DIR']):
            shutil.rmtree(config['BUILD_DEBUG_OUTPUT_DIR'])

        if value == 'all':
            if os.path.isdir(config['INCLUDE_DIR']):
                shutil.rmtree(config['INCLUDE_DIR'])
            if os.path.isdir(config['LIB_DIR']):
                shutil.rmtree(config['LIB_DIR'])

        print ('[>] Done!')

    elif command_name == 'configure':
        if value == None or value == 'release':
            btype = 'Release'
            print ('[>] Loading CMakeLists for build type {}...'.format(btype))
            configure(btype, config['BUILD_RELEASE_OUTPUT_DIR'], clean_configure=True)
            print ('[>] Done!')
        
        if value == None or value == 'debug':
            btype = 'Debug'
            print ('[>] Loading CMakeLists for build type {}...'.format(btype))
            configure(btype, config['BUILD_DEBUG_OUTPUT_DIR'], clean_configure=True)
            print ('[>] Done!')



if __name__ == '__main__':
    with open(CONFIG_PATH, 'r') as f:
        # Read config file
        configJson = f.read()
        config = json.loads(configJson)

        params = extract_params(sys.argv[1:])
        
        for param in params:
            for cmd in config['COMMANDS']:
                if not (cmd['NAME'] == param or cmd['SHORT'] == param):
                    continue

                name = cmd['NAME']
                value = params[param]

                if not value == None and value not in cmd['VALUES']:
                    print ('[#] Invalid parameter "{}" to argument "{}"'.format(value, name))
                    break

                run (name, value, None if len(cmd['VALUES']) == 0 else cmd['VALUES'][0], config)
