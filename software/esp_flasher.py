import tkinter
import customtkinter
import esptool
import os
import platformio.

print(pio.EnvironmentProcessor().config)


# baudRate = 921600
# port = '--port /dev/ttyUSB0'
# port = ''
# bundle_dir = os.path.dirname(os.path.abspath(__file__))

# customtkinter.set_appearance_mode("dark") 
# customtkinter.set_default_color_theme("dark-blue")
# app = customtkinter.CTk()
# app.geometry("400x240")
# app.title("RayonX Uploader")

# frame_1 = customtkinter.CTkFrame(master=app)
# frame_1.place(anchor=tkinter.CENTER, relx=0.5, rely=0.5)

# def button_callback():
#     if (os.name=='nt'):
#         cmd = (bundle_dir+'/esptool.exe {0} --baud {1}\
#                             --before default_reset --after hard_reset write_flash\
#                              --flash_mod dio {2}'.format(port, baudRate, bundle_dir+'firmware.bin'))
#         os.system(bundle_dir+'/esptool.exe {0} --baud {1}\
#                             --before default_reset --after hard_reset write_flash\
#                              --flash_mod dio {2}'.format(port, baudRate, bundle_dir+'firmware.bin'))
#     else:
#         os.system('sudo python3 '+bundle_dir+'/esptool.py {0} --baud {1}\
#                             --before default_reset --after hard_reset write_flash\
#                              --flash_mod dio {2}'.format(port, baudRate, bundle_dir+'firmware.bin'))

# radiobutton_var = tkinter.IntVar(value=1)

# radiobutton_1 = customtkinter.CTkRadioButton(master=frame_1, variable=radiobutton_var, value=1)
# radiobutton_1.pack(pady=10, padx=10)

# radiobutton_2 = customtkinter.CTkRadioButton(master=frame_1, variable=radiobutton_var, value=2)
# radiobutton_2.pack(pady=10, padx=10)

# button_1 = customtkinter.CTkButton(master=frame_1, command=button_callback)
# button_1.pack(pady=10, padx=10)

# progressbar_1 = customtkinter.CTkProgressBar(master=frame_1)
# progressbar_1.pack(pady=10, padx=10)


# app.mainloop()