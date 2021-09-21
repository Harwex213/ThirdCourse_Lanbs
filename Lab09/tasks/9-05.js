const http = require("http");
const J2xParser = require("fast-xml-parser").j2xParser;

const requestObj = {
    request: {
        "@_id": 28,
        x: [
            {
                "@_value": 1
            },
            {
                "@_value": 10
            },
            {
                "@_value": 3
            },
        ],
        m: [
            {
                "@_value": "a"
            },
            {
                "@_value": "b"
            },
            {
                "@_value": "c"
            },
        ]
    }
}
const parser = new J2xParser({
    ignoreAttributes: false,
    format: true,
});
const xml = parser.parse(requestObj);
console.log("Sent xml to server:\n", xml);
const options = {
    host: "localhost",
    port: 4000,
    path: `/task-5`,
    method: "POST",
    headers: {
        'Content-Type': 'application/xml',
        'Accept': 'application/xml'
    }
}

const request = http.request(options,(response)=>
{
    console.log("StatusCode: ", response.statusCode);
    console.log("StatusMessage: ", response.statusMessage);

    let body = "";
    response.on("data",(data) => {
        body += data;
    });
    response.on("end",() => {
        console.log("Received xml from server:\n", body);
    });
});

request.write(xml);
request.end();