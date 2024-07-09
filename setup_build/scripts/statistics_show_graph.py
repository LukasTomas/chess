#/usr/bin/env python3.9

import matplotlib.pyplot as plt
import sys

STATISTICS_DIR = "../../statistics"

def number_length( line: str ) -> int:
    length = 0
    for char in line:
        if char.isdigit():
            length += 1
        else:
            break
    return length

def extract_values(line):
    values = []
    line = line.split(":")
    line = line[1].strip()
    while len(line) > 0:
        line = line.lstrip()
        value = ""
        # posibly: '9 (7)' OR '9 (7), '

        length = number_length(line)
        after_steps: int = int(line[:length])
        line = line[length+1:]

        line = line[1:] # skip '('
        length = number_length(line)
        number_occurences: int = int(line[:length])
        line = line[length+1:]  # skip ')'

        if len(line) > 0 and line[0] == ',':
            line = line[1:]
        values.append({
            "after_steps": after_steps,
            "number_occurences": number_occurences
        })

    return values


def main():
    if len(sys.argv) != 2:
        print("Usage: " + sys.argv[0] + " <statistics filename>")
        return 1

    filename = sys.argv[1];
    filepath = STATISTICS_DIR + "/" + filename

    with open(filepath, 'r') as statistics_file:
        value_line = ""

        values_keystring = "Sorted"
        for line in statistics_file.readlines():
            line = line.lstrip()

            if line.startswith(values_keystring):
                value_line = line
                break

        if value_line == "":
            print(f"'{values_keystring}' NOT found in file {filepath}")
            return 1

    values = extract_values(value_line)
    if len(values) == 0:
        raise Exception("No values found in line: " + value_line)

    for value in values:
        print(f"after {value['after_steps']} steps: {value['number_occurences']} occurences")

    x_values = [value['after_steps'] for value in values]
    y_values = [value['number_occurences'] for value in values]


    # plt.scatter(x_values, y_values, label='Scatter Plot', color='blue', marker='o')

    # # Adding labels and title
    # plt.xlabel('X-axis')
    # plt.ylabel('Y-axis')
    # plt.title('Scatter Plot Example')

    # # Adding a legend
    # plt.legend()

    # # Display the plot
    # plt.show()

    fig, axes = plt.subplots(nrows=1, ncols=1, figsize=(13, 5))
    axes.plot(x_values, y_values, marker='o')

    plt.xlabel('Satisfied After')
    plt.ylabel('Number of Orders')
    axes.set_xticks(x_values)
    plt.legend()
    plt.show()
    return 0

if __name__ == "__main__":
    main()
