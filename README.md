# Cross-Binary-Install
An experiment to compile a C file into .exe, which the client will attempt to install and execute remotely. <br />
This serves as a demo/template for me to later build off of with a USB/Arduino code injection. <br /> <br />

The endgoal is for us to be able to:
* Write malicious code and compile it such that Win64/32 could execute it
* Host it on a remote server
* Through powershell, send a request to the remote server, download the malicious EXE script, and execute it

# Components
## EXE Conversion
Converts a C file into EXE using MingW32 (32 bits for compatibility reasons) <br />
Our dummy code here (`ls_read.exe`) essentially just enumerates the current directory and all of its file contents, and then converts it to a JSON-like object.
This EXE file should read the current directory and file data, and then chuck it into a .txt file. <br />

## EXE Server
A Node.js server (or more accurately, service) would be able to host the EXE file over TCP. <br />
I think because of the way the `net` module works in Node, this handles both public and private IP calls.
Upon connection, the Node.js server should be able to send the .EXE back to the client. <br />

## EXE Client
We will not be using anything in EXE Client - but I just wanted to map out everything in Python and ensure that everything's working before I try fiddling with Powershell. <br />
But yeah! This sends a request to the EXE server, and executes the recieved EXE file. <br />
It is a Python file and it's overly commented (on purpose) so 🤷‍♂️

## EXE Powershell
Demo powershell script we will be putting on the USB when the time comes. <br />
The code doesn't require privilege escalation for now.

# Future Bucket List Ideas:
When we actually do implement this outside of the Demo proof-of-concept: <br />
* We're going to end up exfiltrating this data - so we'll probably not want to use TCP because I was able to catch `ls_read` sending goofy requests on my home lab pretty easily via Wireshark --> So probably do it over HTTP to make it less sus
* Encode the JSON in Base64 because dealing with Binary Data is a pain
* Add more scripts we could possibly inject
* Do we want to encrypt this data so it doesn't get spotted as easily? Prolly symmetric encryption would do.
* Find a way to wipe Powershell history
* Find a way to execute the EXE without writing it to a file - because SIEMs could prolly log that