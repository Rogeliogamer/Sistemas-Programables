import tkinter as tk
from tkinter import messagebox
import serial
import json
from datetime import datetime

import serial
from tkinter import messagebox

# Configuración del puerto
bluetooth_port = 'COM10'  # Asegúrate de usar el puerto correcto
baud_rate = 9600

try:
    arduino = serial.Serial(bluetooth_port, baud_rate, timeout=1)
    print("Conexión exitosa al módulo HC-05")
except serial.SerialException as e:
    messagebox.showerror("Error", f"No se puede conectar al puerto {bluetooth_port}.\n{e}")
    exit()


def send_command(command):
    try:
        if arduino.is_open:
            arduino.write(command.encode())
            print(f"Comando enviado: {command}")
        else:
            print("El puerto serial no está abierto.")
    except Exception as e:
        print(f"Error al enviar comando: {e}")

    
# Función para leer el estado del motor y la configuración de Arduino
def read_status():
    if arduino.in_waiting > 0:
        return arduino.readline().decode('utf-8').strip()
    return ""

# Función para actualizar la interfaz con los datos del motor
def update_status():
    status = read_status()
    if status:
        status_label.config(text=status)
    root.after(500, update_status)  # Actualiza cada 500 ms

# Función para guardar la configuración actual en un archivo JSON
def save_configuration():
    config = {
        "username": username_entry.get(),
        "rotation_direction": rotation_direction.get(),
        "step_mode": step_mode.get(),
        "voltage": voltage_entry.get(),
        "timestamp": datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    }
    with open("config.json", "w") as file:
        json.dump(config, file, indent=4)
    messagebox.showinfo("Guardado", "La configuración ha sido guardada exitosamente.")

# Función para cargar la configuración desde un archivo JSON
def load_configuration():
    try:
        with open("config.json", "r") as file:
            config = json.load(file)
            username_entry.insert(0, config["username"])
            voltage_entry.insert(0, config["voltage"])
            rotation_direction.set(config["rotation_direction"])
            step_mode.set(config["step_mode"])
            messagebox.showinfo("Configuración Cargada", "La configuración ha sido cargada exitosamente.")
            send_command("8")  # Continuar con el último comando
    except FileNotFoundError:
        messagebox.showerror("Error", "No se encontró el archivo de configuración.")

# Función para enviar el comando basado en la entrada del usuario
def execute_command():
    command = ""
    if rotation_direction.get() == "Horario":
        if step_mode.get() == "Paso Completo":
            command = "1"
        elif step_mode.get() == "Torque Aumentado":
            command = "3"
        elif step_mode.get() == "Combinado":
            command = "5"
    elif rotation_direction.get() == "Antihorario":
        if step_mode.get() == "Paso Completo":
            command = "2"
        elif step_mode.get() == "Torque Aumentado":
            command = "4"
        elif step_mode.get() == "Combinado":
            command = "6"
    
    if command:
        send_command(command)
        messagebox.showinfo("Comando Enviado", f"Comando '{command}' enviado a Arduino.")

# Configuración de la interfaz gráfica con tkinter
root = tk.Tk()
root.title("Control Motor Paso a Paso")

# Campos de entrada
tk.Label(root, text="Usuario:").grid(row=0, column=0)
username_entry = tk.Entry(root)
username_entry.grid(row=0, column=1)

tk.Label(root, text="Voltaje:").grid(row=1, column=0)
voltage_entry = tk.Entry(root)
voltage_entry.grid(row=1, column=1)

tk.Label(root, text="Dirección de Rotación:").grid(row=2, column=0)
rotation_direction = tk.StringVar(value="Horario")
tk.Radiobutton(root, text="Horario", variable=rotation_direction, value="Horario").grid(row=2, column=1)
tk.Radiobutton(root, text="Antihorario", variable=rotation_direction, value="Antihorario").grid(row=2, column=2)

tk.Label(root, text="Modo de Paso:").grid(row=3, column=0)
step_mode = tk.StringVar(value="Paso Completo")
tk.Radiobutton(root, text="Paso Completo", variable=step_mode, value="Paso Completo").grid(row=3, column=1)
tk.Radiobutton(root, text="Torque Aumentado", variable=step_mode, value="Torque Aumentado").grid(row=3, column=2)
tk.Radiobutton(root, text="Combinado", variable=step_mode, value="Combinado").grid(row=3, column=3)

# Botones
tk.Button(root, text="Ejecutar Comando", command=execute_command).grid(row=4, column=0, columnspan=2)
tk.Button(root, text="Guardar Configuración", command=save_configuration).grid(row=5, column=0, columnspan=2)
tk.Button(root, text="Cargar Configuración", command=load_configuration).grid(row=5, column=2, columnspan=2)

# Etiqueta para mostrar el estado
status_label = tk.Label(root, text="Estado del Motor: Esperando...", width=40, height=4, relief="sunken")
status_label.grid(row=6, column=0, columnspan=4)

# Iniciar la actualización del estado
root.after(500, update_status)  # Llama a la función cada 500 ms

# Iniciar la interfaz gráfica
root.mainloop()