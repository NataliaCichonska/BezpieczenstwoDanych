import os
import subprocess
import time
import matplotlib.pyplot as plt
import psutil  # Import psutil for resource usage checking

# Ścieżki do plików kodu C++
REFERENCE_CODE = "benchmark.cpp"
TESTED_CODE = "main.cpp"
REFERENCE_EXEC = "benchmark_exec"
TESTED_EXEC = "main_exec"
DATA_DIR = "data3"  # Folder z danymi wejściowymi
OUTPUT_FILE = "results_data3.txt"
REPEATS = 5  # Liczba powtórzeń wykonania każdego programu


def compile_cpp(source_file, output_file):
    """Kompiluje plik źródłowy C++."""
    try:
        subprocess.run(["g++", source_file, "-o", output_file, "-O2"], check=True)
        print(f"Skrypt {source_file} skompilowany do {output_file}.")
    except subprocess.CalledProcessError as e:
        print(f"Błąd kompilacji {source_file}: {e}")
        exit(1)

def check_resource_usage():
    """Sprawdza zajętość zasobów systemowych."""
    cpu_usage = psutil.cpu_percent(interval=1)
    memory_info = psutil.virtual_memory()
    return cpu_usage, memory_info.percent


def run_program(exec_file, input_file=None, repeats=1,reference=False):
    """
    Uruchamia program i zwraca czas wykonania.
    Jeśli input_file jest None, uruchamia program bez argumentów.
    """
    cmd = [f"./{exec_file}"]
    if input_file:
        cmd.append(input_file)
        cmd.append("false")

    times = []
    for i in range(repeats):
        start_time = time.perf_counter()
        try:
            subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)
        except subprocess.CalledProcessError as e:
            print(f"Błąd wykonania {exec_file} dla pliku {input_file}: {e}")
            exit(1)
        end_time = time.perf_counter()
        if reference:
            print(f"Czas {i}-tego wykonania programu referencyjnego: {end_time - start_time}")
        times.append(end_time - start_time)

        # Check resource usage during execution
        cpu_usage, memory_usage = check_resource_usage()
        print(f"Test execution: CPU Usage: {cpu_usage}%, Memory Usage: {memory_usage}%")

    avg_time = sum(times) / len(times)
    return avg_time

def plot_results(results):
    # Podział wyników na kategorie
    normal_data = [r for r in results if "normal" in r[0]]
    uniform_data = [r for r in results if "uniform" in r[0]]

    # Dane do wykresów
    normal_sizes = [int(r[0].split("_")[1].split(".")[0]) for r in normal_data]
    uniform_sizes = [int(r[0].split("_")[1].split(".")[0]) for r in uniform_data]

    normal_times = [r[3] for r in normal_data]
    uniform_times = [r[3] for r in uniform_data]
    # Wykresy czasu od rozmiaru danych
    plt.figure(figsize=(10, 5))

    plt.subplot(1, 2, 1)
    plt.plot(normal_sizes, normal_times, marker="o", label="Normal")
    plt.title("Czas wykonania (rozkład normalny)")
    plt.xlabel("Rozmiar danych")
    plt.ylabel("Czas wykonania (s)")
    plt.grid(True)
    plt.legend()

    plt.subplot(1, 2, 2)
    plt.plot(uniform_sizes, uniform_times, marker="o", label="Uniform")
    plt.title("Czas wykonania (rozkład równomierny)")
    plt.xlabel("Rozmiar danych")
    plt.ylabel("Czas wykonania (s)")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()
    plt.savefig("time_vs_size.png")
    print("Wykresy czasu od rozmiaru danych zapisano jako 'time_vs_size.png'.")

    # Wykres porównania czasów dla obu rozkładów
    plt.figure(figsize=(8, 5))
    bar_width = 0.35
    index = range(len(normal_sizes))

    plt.bar(index, normal_times, bar_width, label="Normal")
    plt.bar([i + bar_width for i in index], uniform_times, bar_width, label="Uniform")

    plt.title("Porównanie czasów wykonania dla rozkładów")
    plt.xlabel("Rozmiar danych")
    plt.ylabel("Czas wykonania (s)")
    plt.xticks([i + bar_width / 2 for i in index], normal_sizes)
    plt.legend()
    plt.grid(True)
    plt.savefig("time_comparison.png")
    print("Wykres porównania czasów wykonania zapisano jako 'time_comparison.png'.")

def main():
    # Check resource usage in idle state
    print("Sprawdzanie zajętości zasobów w stanie bezczynności...")
    idle_cpu_usage, idle_memory_usage = check_resource_usage()
    print(f"Idle CPU Usage: {idle_cpu_usage}%, Idle Memory Usage: {idle_memory_usage}%")

    # Kompilacja plików C++
    compile_cpp(REFERENCE_CODE, REFERENCE_EXEC)
    compile_cpp(TESTED_CODE, TESTED_EXEC)

    # Odczyt plików danych wejściowych
    data_files = sorted(
        [f for f in os.listdir(DATA_DIR) if f.endswith(".txt")],
        key=lambda x: int(x.split('_')[1].split('.')[0])  # Wyciągnięcie liczby z nazwy pliku
    )
    if not data_files:
        print(f"Brak plików danych w folderze {DATA_DIR}.")
        return

    results = []

    # Uruchamianie kodu referencyjnego bez plików wejściowych (jeden raz)
    print(f"Uruchamianie kodu referencyjnego {REPEATS} razy...")
    reference_time = run_program(REFERENCE_EXEC, repeats=REPEATS, reference=True)

    # Sprawdzanie błędu dla czasu referencyjnego
    if reference_time == 0:
        print(f"Błąd: czas referencyjny wynosi 0.")
        exit(1)

    # Uruchamianie kodu testowanego dla każdego zbioru danych
    for data_file in data_files:
        input_path = os.path.join(DATA_DIR, data_file)
        print(f"Przetwarzanie pliku danych: {data_file}")

        tested_time = run_program(TESTED_EXEC, input_file=input_path, repeats=REPEATS)
        ratio = tested_time / reference_time
        results.append((data_file, reference_time, tested_time, ratio))

    # Zapis wyników do pliku
    with open(OUTPUT_FILE, "w") as f:
        f.write("Plik danych, Średni czas referencyjny, Średni czas testowanego, Stosunek\n")
        for data_file, ref_time, test_time, ratio in results:
            f.write(f"{data_file}, {ref_time:.6f}, {test_time:.6f}, {ratio:.6f}\n")

    print(f"Wyniki zapisano w pliku {OUTPUT_FILE}.")

    # Generowanie wykresów
    plot_results(results)


if __name__ == "__main__":
    main()