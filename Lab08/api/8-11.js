const fromXmlParser = require("fast-xml-parser");
const J2xParser = require("fast-xml-parser").j2xParser;

let id = 0;

const postXml = (request, response) => {
    let body;
    request.on("data", data => {
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

module.exports = {
    postXml
}