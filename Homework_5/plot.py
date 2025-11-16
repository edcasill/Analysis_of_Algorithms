import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys

data_file = "skip_list_performance.csv"

try:
    data = pd.read_csv(data_file)
except FileNotFoundError:
    print(f"file not found '{data_file}'")
    sys.exit(1)
except pd.errors.EmptyDataError:
    print(f"file'{data_file}' is on blank")
    sys.exit(1)

sns.set_theme(style="whitegrid")

#average time vs N (O(log n))
# Debería mostrar líneas casi horizontales.
plt.figure(figsize=(10, 6))
plot_avg = sns.lineplot(data=data,
                        x="N",
                        y="AverageTime",
                        hue="Operation", #'Insert', 'Search', 'Delete'
                        marker="o") #add marker for each point

plot_avg.set(xscale="log") 
plot_avg.set_title("Average time behavior - O(log n)")
plot_avg.set_xlabel("N (# of elements) - logaritmic scale")
plot_avg.set_ylabel("Avergae time per operation (s)")
avg_filename = "skip_list_average_vs_N.png"
plt.savefig(avg_filename)
print(f"Graph save as: {avg_filename}")


#Total time vs. N - O(n log n)
# Debería ser una línea "casi" recta, un poco curva hacia arriba.
plt.figure(figsize=(10, 6))
plot_total = sns.lineplot(data=data,
                          x="N",
                          y="TotalTime",
                          hue="Operation",
                          marker="o")

# Para O(n log n), una escala log-log es muy útil.
# Si fuera O(n), sería una recta en log-log con pendiente 1.
# Siendo O(n log n), será un poco más empinada.
plot_total.set(xscale="log", yscale="log")
plot_total.set_title("Total time behavior - O(n log n)")
plot_total.set_xlabel("N (# of elements) - logaritmic scale")
plot_total.set_ylabel("Total time (s) - logaritmic scale")
total_filename = "skip_list_total_vs_N.png"
plt.savefig(total_filename)
print(f"Graph save as: {total_filename}")