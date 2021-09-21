const http = require("http");

const params = JSON.stringify({
    x: 3,
    y: 4,
    s: 10
})
const options = {
    host: "localhost",
    port: 4000,
    path: `/task-3`,
    method: "POST",
    headers: {
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    }
}

const request = http.request(options,(response)=>
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
});

request.write(params);
request.end();