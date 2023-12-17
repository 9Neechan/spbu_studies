import pandas as pd
import matplotlib.pyplot as plt

colors = ['b', 'g', 'r', 'c', 'm', 'lightcoral', 'k', 'gray', 'darkorange', 'gold', 'aquamarine', 'magenta']

def plots(df, cols, mode):
    label = ''
    c = 0
    if mode == 'boost':
        label = "Ускорение"
    else: 
        label = "Время выполнения"
    
    fig, ax = plt.subplots()
    for col in cols:
        if mode == 'boost':
            ax.plot(df['кол-во потоков'], df[col][0] / df[[col]], colors[c]);
        else:
            ax.plot(df['кол-во потоков'], df[[col]], colors[c]);
        c += 1

    ax.legend(cols)
    plt.title(label)
    plt.xlabel("Кол-во потоков")
    plt.ylabel(label);
    plt.show()


def read_df(table):
    df = pd.read_csv(f"~/spbu_studies/supercomputing_basics/parallel_OpenMP/tables/{table}")
    cols = df.columns.values.tolist()

    if 'Unnamed: 11' in cols:
        del df['Unnamed: 11']
    if 'Unnamed: 7' in cols:
        del df['Unnamed: 7']
    if 'Unnamed: 13' in cols:
        del df['Unnamed: 13']

    cols.pop(0)
    cols.pop()

    return df, cols


def draw(table):
    df, cols = read_df(table) 
    plots(df, cols, 'boost');
    plots(df, cols, "");

    return df

def draw2(df):
    cols = df.columns.values.tolist()
    cols.pop(0)
    plots(df, cols, 'boost');
    plots(df, cols, "");

    return df


