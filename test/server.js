const net = require("net");
const server = net.createServer((socket)=>{
    socket.on("connect",()=>{
        socket.write(Buffer("test"));
    })
})
server.listen(5005,()=>{
    console.log("server now listening on port 5000");
})