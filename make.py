import subprocess
import os
from pathlib import Path

# Carpeta de entrada
input_dir = "inputs"

# Archivos fuente
source_files = [
    "main.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp"
]

# Compilar
print("Compilando...")
#compile_cmd = ["g++"] + source_files
#result = subprocess.run(compile_cmd)

#if result.returncode != 0:
#    print("Error de compilación.")
#    exit(1)


carpeta = Path('input')

for archivo in carpeta.glob("*.rs"):
    print(f"Compilando {archivo}...")
    path =  str(archivo)
    output_path = "out/" + str(archivo.name)[:-3]
    compile_cmd = ["rustc", path, "-o",output_path] 
    result = subprocess.run(compile_cmd, capture_output=True, text=True)

    # Exito
    if result.returncode == 0:
        print(f"Compilación exitosa de {archivo.name}. Ejecutando...")
        ejec_cmd = ["./" + output_path]

        ejec_result = subprocess.run(ejec_cmd, capture_output=True, text=True)
        if ejec_result.returncode == 0:
            print(f"Salida de {archivo.name}:\n{ejec_result.stdout}")
        else:  
            print(f"Error al ejecutar {archivo.name}:")
    else:
        print(f"Error de compilación en {archivo.name}:")
        print(result.stderr)
        exit(1)

print("Compilación exitosa.\n")

#for i in range(1, 2):
"""    input_file = os.path.join(input_dir, f"input{i}.txt")

    if not os.path.exists(input_file):
        print(f"{input_file} no existe. Se omite.")
        continue

    print(f"\nEjecutando con {input_file}")
    subprocess.run(["./a.exe", input_file])"""