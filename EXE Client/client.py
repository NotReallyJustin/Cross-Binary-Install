# Had thoughts about writing this in Node, but we are executing the exe file so
import socket
import subprocess

# Step 1) Connect to server and get EXE file content

# The parameters sound wonky, but it just tells Python to use IPv4 and TCP (which is what we programmed our Node to be)
client_connection_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

client_connection_socket.connect(("127.0.0.1", 8081))

# A list of byte objects. Think of this handling this like Node binary data
chunks = []
while True:             # Imagine not having async code can't relate
    data = client_connection_socket.recv(2048)
    
    if data == b"":        # Break out if there's no more items to receive
        break
    else:
        chunks.append(data)

data_str = b"".join(chunks)

# print(data_str)

# Step 2) Write the EXE file content to an actual exe

with open("ls_read.exe", "wb") as exe_file:         # Use b flag for bytes
    exe_file.write(data_str)

# Step 3) Run syscall to execute the EXE
process = subprocess.Popen("./ls_read.exe", stdout=subprocess.PIPE, stderr=subprocess.PIPE)

stdout, stderr = process.communicate()

print(stdout.decode(errors="ignore"))

if stderr:
    print(stderr)