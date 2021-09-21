const fs = require("fs");
const qs = require("querystring");

const getFormParams = (request, response, args) => {
    const { viewsDirectory } = args;
    response.write(fs.readFileSync(`${viewsDirectory}/form.html`));
    response.end();
}

const postFormParams = (request, response) => {
    let obj = "";
    request.on("data", data => {
        obj += data;
    });
    request.on("end", () =>
    {
        const result = qs.parse(obj);
        response.writeHead(200, {
            "Content-Type" : "text/plain; charset=utf-8"
        });
        response.write(`text: ${result.text}\n`);
        response.write(`number: ${result.number}\n`);
        response.write(`date: ${result.date}\n`);
        response.write(`checkbox: ${result.checkbox}\n`);
        response.write(`radio: ${result.radio}\n`);
        response.write(`textarea: ${result.textarea}\n`);
        response.write(`submitOne: ${result.submitOne}\n`);
        response.write(`submitSecond: ${result.submitSecond}\n`);
        response.end();
    });
}

module.exports = {
    getFormParams,
    postFormParams
}