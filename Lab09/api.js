const fromXmlParser = require("fast-xml-parser");
const J2xParser = require("fast-xml-parser").j2xParser;
const Multiparty = require('multiparty');
const fs = require("fs");

const handleTask1 = (_, response) => {
    response.writeHead(200, {
        "Content-Type": "text/html;charset=utf-8"
    })
    response.end("Task 1 completed")
};

const handleTask2 = (_, response) => {
    response.writeHead(200, {
        "Content-Type": "text/html;charset=utf-8"
    })
    response.end("Task 2 completed");
}

const handleTask3 = (request, response) => {
    let body;
    request.on("data", data => {
        body = JSON.parse(data);
    })
    request.on("end", () => {
        response.writeHead(200, {
            "Content-Type": "text/html;charset=utf-8"
        });
        const { x, y, s } = body;
        response.end(`sum: ${x + y + s}`);
    })
}

const handleTask4 = (request, response) => {
    let body;
    request.on("data", data => {
        body = JSON.parse(data);
    })
    request.on("end", () => {
        response.end(JSON.stringify({
            "__comment": "Ответ. Лабораторная работа 8/10",
            "x_plus_y": body.x + body.y,
            "Concatination_s_o": `${body.s}: ${body.o.surname}, ${body.o.name}`,
            "Length_m": body.m.length,
        }));
    })
}

let id = 0;
const handleTask5 = (request, response) => {
    let body;
    request.on("data", data => {
        console.log("Task5, received xml from client:\n", data.toString());
        body = fromXmlParser.parse(data.toString(), {
            ignoreAttributes: false,
            parseAttributeValue: true
        });
    })
    request.on("end", () => {
        const sum = body.request.x.reduce((accumulator, currentValue) => ({
            "@_element": "x",
            "@_result": accumulator["@_result"] + currentValue["@_value"],
        }), {
            "@_element": "x",
            "@_result": 0
        });
        const concat = body.request.m.reduce((accumulator, currentValue) => ({
            "@_element": "m",
            "@_result": accumulator["@_result"] + currentValue["@_value"],
        }), {
            "@_element": "m",
            "@_result": ""
        });

        const responseObj = {
            response: {
                "@_id": id++,
                "@_request": body.request.id,
                sum,
                concat
            }
        }
        const parser = new J2xParser({
            ignoreAttributes: false,
            format: true,
        });
        const xml = parser.parse(responseObj);
        response.end(xml);
    })
}

const handleTask6And7 = (request, response) => {
    const form = new Multiparty.Form({
        uploadDir: "./static"
    });

    form.parse(request, (err, _, files) => {
        if (err) {
            response.writeHead(400, {
                "Content-Type" : "text/plain; charset=utf-8"
            });
            response.end("Check your form input. Action must be multipart/form-data");
            return;
        }

        const file = fs.readFileSync(files.file[0].path);
        fs.unlinkSync(files.file[0].path);
        fs.writeFileSync(files.file[0].path, file.toString(), {
            encoding: "base64"
        });
        response.writeHead(200, {
            "Content-Type" : "text/plain; charset=utf-8"
        })
        response.end("File saved");
    })
}

const handleTask8 = (request, response) => {
    response.writeHead(200, {
        "Content-Type" : "text/plain; base64"
    })
    const boundary = "--divider";
    let body = `--${boundary}\r\n`;
    body += `Content-Disposition: form-data; name=\"file\"; Filename=\"task8.png\"\r\n`;
    body += `Content-Type: text/plain; base64\r\n\r\n`;
    const fileData = fs.readFileSync(`task8.png`, {
        encoding: "base64"
    });
    body += fileData;
    body += `\r\n--${boundary}--\r\n`;
    response.write(body);
    response.end();
}

module.exports = {
    handleTask1,
    handleTask2,
    handleTask3,
    handleTask4,
    handleTask5,
    handleTask6And7,
    handleTask8,
}