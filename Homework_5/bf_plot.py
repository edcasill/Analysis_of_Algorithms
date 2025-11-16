import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import sys

# Nombre del archivo de datos
data_file = "bellman_ford_performance.csv"

# --- Cargar los datos ---
try:
    data = pd.read_csv(data_file)
except FileNotFoundError:
    print(f"Error: No file found '{data_file}'")
    sys.exit(1)
except pd.errors.EmptyDataError:
    print(f"Error: the file '{data_file}' is blank.")
    sys.exit(1)

# Configurar un estilo bonito
sns.set_theme(style="whitegrid")

# --- Gráfica: Tiempo Total vs. Complejidad (V * E) ---
# Esta es la gráfica clave. 
# Si la complejidad es O(V*E), la relación entre 
# el TiempoTotal (eje Y) y V_times_E (eje X) debe ser LINEAL.
plt.figure(figsize=(10, 6))

# Usamos regplot para trazar los puntos y una línea de regresión lineal
plot_lineal = sns.regplot(
    data=data,
    x="V_times_E",
    y="TotalTime",
    ci=None, # No mostrar intervalo de confianza
    line_kws={"color": "red", "linestyle": "--"} # Línea de regresión punteada
)

plot_lineal.set_title("Time analysis of Bellman-Ford O(V * E)")
plot_lineal.set_xlabel("Theoretical complex: V * E")
plot_lineal.set_ylabel("Total exec time (s)")

# Formatear el eje X para que sea más legible (sin notación científica)
plt.ticklabel_format(style='plain', axis='x')

# Guardar la gráfica
filename = "bellman_ford_lineal_vs_VE.png"
plt.savefig(filename)