const http = require("http");

const options = {
    host: "localhost",
    port: 4000,
    path: "/task-1",
    method: "GET"
}

http.request(options,(response)=>
{
    console.log("StatusCode: ", response.statusCode);
    console.log("StatusMessage: ", response.statusMessage);
    console.log("RemoteAddress: ", response.socket.remoteAddress);
    console.log("RemotePort: ", response.socket.remotePort);

    let data = "";
    response.on("data",(chunk) => {
        data += chunk;
    });
    response.on("end",() => {
        console.log("Body response: ", data);
    });
}).end();