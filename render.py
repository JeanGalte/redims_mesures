import csv
import matplotlib.pyplot as plt

# Listes pour stocker les résultats
n_values = []
temps_naif = []
alloc_naif = []
temps_hat = []
alloc_hat = []

# Lecture du CSV
with open("resultats.csv", newline='') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)  # sauter l'en-tête
    for row in reader:
        n = int(row[0])
        impl = row[1]
        temps = float(row[2])
        alloc = int(row[3])

        if impl == 'naif':
            n_values.append(n)
            temps_naif.append(temps)
            alloc_naif.append(alloc)
        elif impl == 'HAT':
            temps_hat.append(temps)
            alloc_hat.append(alloc)
        
# Courbe du temps
plt.figure(figsize=(10,5))
plt.plot(n_values, temps_naif, label='Naïf', marker='o')
plt.plot(n_values, temps_hat, label='HAT', marker='x')
plt.xlabel("n (nombre d'éléments)")
plt.ylabel("Temps (s)")
plt.title("Comparaison du temps d'exécution")
plt.legend()
plt.grid(True)
plt.savefig("temps.png")
plt.show()

# Courbe de la mémoire
plt.figure(figsize=(10,5))
plt.plot(n_values, alloc_naif, label='Naïf', marker='o')
plt.plot(n_values, alloc_hat, label='HAT', marker='x')
plt.xlabel("n (nombre d'éléments)")
plt.ylabel("Mémoire allouée (octets)")
plt.title("Comparaison de la mémoire utilisée")
plt.legend()
plt.grid(True)
plt.savefig("memoire.png")
plt.show()
