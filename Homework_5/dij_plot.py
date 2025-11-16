import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys

data_file = "dijkstra_performance.csv"

try:
    data = pd.read_csv(data_file)
except FileNotFoundError:
    print(f"Error: No file found '{data_file}'")
    sys.exit(1)

sns.set_theme(style="whitegrid")

# --- Gráfica: Tiempo Total vs. Complejidad Teórica (V * log V) ---
# Si la complejidad es O(V log V), la relación entre 
# el TiempoTotal (eje Y) y V_log_V (eje X) debe ser LINEAL.
plt.figure(figsize=(10, 6))

plot_lineal = sns.regplot(
    data=data,
    x="V_log_V",
    y="TotalTime",
    ci=None, 
    line_kws={"color": "red", "linestyle": "--"}
)

plot_lineal.set_title("Dijkstra time analysis O(V log V) for sparse graphs")
plot_lineal.set_xlabel("Theorethical complexity (V * log_2 V)")
plot_lineal.set_ylabel("Total exec time (s)")

plt.ticklabel_format(style='plain', axis='x')
plt.savefig("dijkstra_performance_graph.png")
