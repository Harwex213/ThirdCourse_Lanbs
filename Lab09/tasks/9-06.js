const http = require("http");
const fs = require("fs");

const filenameByTask = {
    "6": "myFile.txt",
    "7": "myFile.png",
}

// Content-Type: multipart/form-data; boundary=haha
// --haha
// Content-Disposition: form-data; name="field"
//
// text
// ---------------573cf973d5228
// Content-Disposition: form-data; name="file"; filename="sample.txt"
// Content-Type: text/plain
//
// Content file
// --haha--

const filename = filenameByTask[process.argv[2]];
const boundary = "--divider";
let body = `--${boundary}\r\n`;
body += `Content-Disposition: form-data; name=\"file\"; Filename=\"${filename}\"\r\n`;
body += `Content-Type: text/plain; base64\r\n\r\n`;
const fileData = fs.readFileSync(`./files/${filename}`, {
    encoding: "base64"
});
body += fileData;
body += `\r\n--${boundary}--\r\n`;

const options = {
    host: "localhost",
    port: 4000,
    path: `/task-6`,
    method: "POST",
    headers: {
        "Content-Type": `multipart/form-data; boundary=${boundary}`
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
        console.log("Body: ", body);
    });
});

request.write(body);
request.end();