const http = require("http");
const querystring = require("querystring");

const params = querystring.stringify({
    x: 3,
    y: 4
})
const options = {
    host: "localhost",
    port: 4000,
    path: `/task-2?${params}`,
    method: "GET",
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
        console.log("Body response: ", data);
    });
}).end();