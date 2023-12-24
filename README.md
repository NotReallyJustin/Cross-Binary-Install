# Cross-Binary-Install
An experiment to compile a C file into .exe, which the client will attempt to install and execute remotely. <br />
This serves as a demo/template for me to later build off of.

# Components
## EXE Conversion
Converts a C file into EXE. <br />
This EXE file should read the current directory and file data, and then chuck it into a .txt file. <br />
<br />

## EXE Server
A Node.js server (or more accurately, service) would be able to host the EXE file. <br />
Upon connection, the Node.js server should be able to send the .EXE back to the client.

## EXE Client
Sends a request to the EXE server, and executes the recieved EXE file.