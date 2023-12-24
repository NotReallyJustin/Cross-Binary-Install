const net = require("node:net");
const fs = require("fs");
const EXE_PATH = "./ls_read.exe";

// This is a TCP server. For UDP, just use a datagram
const malware_server = net.createServer({keepAlive: false}, connection => {
    // Note that we don't need an extra event listener to check for connection - since the callback happens *when* a connection event is fired

    console.log(`Connected to: ${connection.address().address}:${connection.address().port}.`);
    console.log(`Server running on ${connection.address().family}`);

    // Send the EXE file data upon connection
    fs.readFile(EXE_PATH, (err, data) => {
        if (err)
        {
            console.error(err);
        }
        else
        {
            // 🚨 socket.write is async
            connection.write(data, err => {
                console.error(err);
            }).then(() => {
                console.log(`${EXE_PATH} has been sent to client.`);
                console.log("");      // Just a new line
            })
        }
    })
});

malware_server.listen(8081);