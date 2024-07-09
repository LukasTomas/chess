from ipywidgets import *

import pandas as pd

import seaborn as sns
import matplotlib.pyplot as plt

import scipy
import numpy as np



#convert board positions numbers (0-63) to chess notation (a1-h8)
def convert_to_chess_notation(pos):
    return chr(ord('a') + pos % 8) + str(int(pos / 8) + 1)

assert convert_to_chess_notation(0) == 'a1'
assert convert_to_chess_notation(8) == 'a2'
assert convert_to_chess_notation(63) == 'h8'
assert convert_to_chess_notation(9) == 'b2'

def get_prob_distribution(column, treshold_percent):
    value_counts = column.value_counts()
    value_counts = value_counts.sort_values(ascending=False)

    threshold_value = (treshold_percent/100) * value_counts.sum()
    negligible_values = value_counts[value_counts <= threshold_value].index
    print("Negligible values: " + str(negligible_values))

    value_counts = value_counts[value_counts > threshold_value]
    prob_distribution = value_counts / value_counts.sum()

    return prob_distribution

def plot_probability_distribution(prob_distribution, zoom=1):
    fig, axes = plt.subplots(nrows=1, ncols=1, figsize=(13, 5))
    axes.bar(prob_distribution.index, prob_distribution.values)
    plt.xlabel('Satisfied After')
    plt.ylabel('Probability')
    plt.title('Probability Distribution of Satisfied After')
    axes.set_ylim(0, zoom)
    plt.show()

def get_chess_notation(pos):
    if type(pos) != list:
        return convert_to_chess_notation(pos)
    else:
        return [convert_to_chess_notation(p) for p in pos]

def analyse_piece(df, column_name, treshold_percent):
    piece_column = df[column_name]

    #isOneDimensional = piece_column.apply(lambda x: len(x) == 1).all()
    #print('Is one dimensional: ' + str(isOneDimensional))
    #piece_column = piece_column.apply(lambda x: x[0]) if isOneDimensional else piece_column

    #display(piece_column.describe())
    #display('Median: ' + str(queen_column.median()))

    prob_distribution = get_prob_distribution(piece_column, treshold_percent)
    #print(f'\x1b[1mProbability\x1b[0m\x1b[1m distribution:\x1b[0m')
    #print(prob_distribution)

    #if isOneDimensional:
    #    interact(plot_probability_distribution, prob_distribution=fixed(prob_distribution), zoom=widgets.FloatSlider(value=1, min=0.01, max=1, step=0.05), prob_distrubtion=prob_distribution)

    value_counts_array = {}

    # result dataframe(win, lost, draw) for each queen position
    result_df = pd.DataFrame(columns=['position_int', 'position_str', 'reach_prob', 'win', 'lost', 'draw', 'avg_steps', 'median_steps', 'opening'])
    for index, value in prob_distribution.items():
        filtered_df = df[df[column_name].apply(lambda x:(index if type(index) == list else [index]) == x)]

        value_counts = filtered_df['result'].value_counts()
        value_counts = value_counts / value_counts.sum()

        avg_steps = filtered_df['satisfied_after'].mean()
        median_steps = filtered_df['satisfied_after'].median()

        value_counts_array[str(index)] = filtered_df['opening'].value_counts().sort_values(ascending=False)

        result_df.loc[len(result_df)] = [
            str(index),                                  # position in int: 0-63
            str(get_chess_notation(index)),              # square in chess notation: a1
            value,                                  # chance for selected piece to reach this position
            value_counts.get(1, default=0),         # chance of winning
            value_counts.get(0, default=0),         # chance of losing
            value_counts.get(1/2, default=0),       # chance of draw
            avg_steps,                              # average steps to reach this position
            median_steps,                           # median steps to reach this position
            value_counts_array[str(index)].index[0]
        ]

        # print(f'Queen pos: {get_chess_notation(pos)}:', value_counts / value_counts.sum(), '\n')

    return result_df, value_counts_array


def analyse_piece_over_all(dfs, column_name, threshold_percent):
    result_df = pd.DataFrame(columns=['position_int', 'position_str', 'reach_prob', 'win', 'lost', 'draw', 'avg_steps', 'median_steps', 'opening'])
    for df in dfs.values():
        if len(df[column_name][0]) == 0:
            continue

        piece_df, _ = analyse_piece(df, column_name, threshold_percent)

        result_df = pd.concat([result_df, piece_df], ignore_index=True)

    openings = result_df[['opening', 'position_int', 'position_str']].groupby(['position_int', 'position_str']).agg(lambda x:x.value_counts().index[0]).reset_index()
    #display(openings)
    result_df.drop(labels=['opening'], axis=1,inplace = True)

    # compute mean values for each position besides 'median_steps', for this one compute median
    result_df_median = result_df.groupby(['position_int', 'position_str'])['median_steps'].median().reset_index()
    result_df_mean = result_df.drop('median_steps', axis=1).groupby(['position_int', 'position_str']).mean().reset_index()


    #display(result_df_median.head())
    #display(result_df_mean.sort_values(by=['reach_prob'], ascending=False).head())

    result_df = pd.merge(result_df_mean, result_df_median, on=['position_int', 'position_str'])
    result_df = pd.merge(result_df, openings, on=['position_int', 'position_str'])

    result_df = result_df.sort_values(by=['reach_prob'], ascending=False)
    return result_df
