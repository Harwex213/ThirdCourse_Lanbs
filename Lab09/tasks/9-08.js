const http = require("http");

const options = {
    host: "localhost",
    port: 4000,
    path: "/task-8",
    method: "GET"
}

http.request(options,(response)=>
{
    console.log("StatusCode: ", response.statusCode);
    console.log("StatusMessage: ", response.statusMessage);

    let data = "";
    response.on("data",(chunk) => {
        data += chunk;
    });
    response.on("end",() => {
        console.log(`Body response:\n ${data.substring(0, 1000)}.............`);
    });
}).end();