const net = require("net");
const socket = net.connect({port:5000,host:"localhost"});
socket.on("connect",()=>{
    socket.write("this is a test");
})
socket.on("data",(chunk)=>{
    const data = chunk.toString();
    console.log(data);
})
