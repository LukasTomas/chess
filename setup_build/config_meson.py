#!/usr/bin/env python3

import os
import glob
import pathlib


def get_sources():
    sources = []

    globbed = glob.glob('./src/**/*.cpp', recursive=True)

    last_dir = ''
    for source in globbed:
        source_path = pathlib.Path(source)
        if source_path.parent != last_dir:
            last_dir = source_path.parent
            sources.append('\n')

        source = source.replace('./', '')
        sources.append(source)

    return sources


def main():
    meson_input = 'meson.build.template'
    meson_output = 'meson.build'

    if not os.path.exists(meson_input):
        raise RuntimeError('No meson build file')

    output_buffer = []

    sources_line = 'chess_engine_sources = [\n'
    with open(meson_input, 'r') as f:
        line_found = False

        for current_line in f.readlines():
            output_buffer.append(current_line)

            if current_line == sources_line:
                line_found = True

                sources = get_sources()
                for source in sources:
                    if source == '\n':
                        output_buffer.append(source)
                    else:
                        output_buffer.append(f"\t'{source}',\n")

        if not line_found:
            raise RuntimeError('No sources line found')

    with open(meson_output, 'w') as f:
        f.writelines(output_buffer)


if __name__ == '__main__':
    main()
