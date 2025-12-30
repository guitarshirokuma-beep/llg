import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def output_graph(
    fontsize,
    title_text,
    xlim,
    xlabel,
    x_data,
    ylim,
    ylabel,
    y_data,
    legend_text,
    figsize,
):
    fig, ax = plt.subplots(figsize=figsize)
    ax.grid()
    ax.set_title(label=title_text,fontsize=fontsize)
    ax.tick_params(labelsize=fontsize)
    ax.set_xlim(0,xlim)
    ax.set_xlabel(xlabel, fontsize=fontsize)
    ax.set_ylabel(ylabel, fontsize=fontsize)
    ax.set_ylim(-ylim, ylim)
    ax.plot(x_data, y_data, label=legend_text)
    ax.legend(fontsize=fontsize)
    plt.show()
