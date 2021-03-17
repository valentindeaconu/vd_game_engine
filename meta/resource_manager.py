"""
    VDGE
    Resource Manager is a cross-platform script used to automatically refresh resource folder on target build directory
"""

import os
import shutil

def resource_update(work_dir, build_dir_name):
    build_dir_location = os.path.join(work_dir, build_dir_name)
    build_resources_location = os.path.join(build_dir_location, 'resources')
    resources_location = os.path.join(work_dir, 'resources')

    print ('[>] Working on {}'.format(build_dir_name))
    if os.path.isdir(build_resources_location):
        print ('\t[>] Cleaning old resources folder:', end=' ', flush=True)
        shutil.rmtree(build_resources_location)
        print ('Done!')
    
    print ('\t[>] Copying new resources folder:', end=' ', flush=True)
    shutil.copytree(resources_location, build_resources_location)
    print ('Done!')

    return 0
    