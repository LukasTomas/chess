#!/usr/bin/env xonsh

import os

# ANSI escape codes for colors
BLACK = '\033[30m'
RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
BLUE = '\033[34m'
MAGENTA = '\033[35m'
CYAN = '\033[36m'
WHITE = '\033[37m'
ENDC = '\033[0m'

def printColor(text, color):
    print(f'{color}{text}{ENDC}')

def printNotice(text):
    printColor(text, GREEN)

def printInfo(text):
    printColor(text, BLUE)

def printWarning(text):
    printColor(text, YELLOW)

def printError(text):
    printColor(text, RED)

BUILD_NAME = 'build'
LINK_NAME = 'train_nn'

def printHelp():
    print(f"{$ARGS[0]} [clean|help]")

# ----------------------------------------------------------------------

if len($ARGS) > 1:
    if len($ARGS) > 2:
        printError(f'Invalid number of arguments: {len($ARGS) - 1}')
        printHelp()
        exit(1)

    argument = $ARGS[1]
    if argument == 'clean':
        printInfo('First cleaning ...')
        rm -rf ./@(BUILD_NAME)
        rm ./@(LINK_NAME)
    elif argument == 'help':
        printHelp()
        exit(0)
    else:
        printError(f'Invalid argument: {argument}')
        printHelp()
        exit(2)


if not os.path.exists(f'./{BUILD_NAME}'):
    printNotice(f"Creating build directory called '{BUILD_NAME}'.")
    mkdir @(BUILD_NAME)

project_path = os.path.abspath('../')
cd @(BUILD_NAME)

if os.path.exists(f'./Makefile'):
    printInfo("First cmake command already done when run previous time, continuing with other ...")
else:
    libtorch_path = f'{project_path}/libs/libtorch'
    printInfo(f'Libtorch path is {libtorch_path}')

    cmake -DCMAKE_PREFIX_PATH=@(libtorch_path) -DCAFFE2_USE_CUDNN=ON -DCAFFE2_USE_CUSPARSELT=ON ..
    print('\n\n\n\n')

cmake --build . --config Release

cd ..

# Symlink if not exists
if not os.path.exists(f'./{LINK_NAME}'):
    printNotice(f"Creating symlink called '{LINK_NAME}'.")
    ln -s @(BUILD_NAME)/neural @(LINK_NAME)
