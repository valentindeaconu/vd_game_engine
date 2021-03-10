#!/usr/bin/env python3

import os
import shutil

BUILD_DEBUG = 'cmake-build-debug'
BUILD_RELEASE = 'cmake-build-release'

RESOURCES_LOCATION = os.path.join(os.getcwd(), 'resources')
DEBUG_LOCATION = os.path.join(os.getcwd(), BUILD_DEBUG)
RELEASE_LOCATION = os.path.join(os.getcwd(), BUILD_RELEASE)
DEBUG_RESOURCES_LOCATION = os.path.join(DEBUG_LOCATION, 'resources')
RELEASE_RESOURCES_LOCATION = os.path.join(RELEASE_LOCATION, 'resources')

print ('[>] Working on {}'.format(BUILD_DEBUG))
if os.path.isdir(DEBUG_RESOURCES_LOCATION):
    print ('\t[>] Cleaning old resources folder:', end=' ', flush=True)
    shutil.rmtree(DEBUG_RESOURCES_LOCATION)
    print ('Done!')
print ('\t[>] Copying new resources folder:', end=' ', flush=True)
shutil.copytree(RESOURCES_LOCATION, DEBUG_RESOURCES_LOCATION)
print ('Done!')

print ('[>] Working on {}'.format(BUILD_RELEASE))
if os.path.isdir(RELEASE_RESOURCES_LOCATION):
    print ('\t[>] Cleaning old resources folder:', end=' ', flush=True)
    shutil.rmtree(RELEASE_RESOURCES_LOCATION)
    print ('Done!')
print ('\t[>] Copying new resources folder:', end=' ', flush=True)
shutil.copytree(RESOURCES_LOCATION, RELEASE_RESOURCES_LOCATION)
print ('Done!')