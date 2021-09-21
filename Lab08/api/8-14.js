const fs = require("fs");
const Multiparty = require('multiparty');

const getUpload = (request, response, args) => {
    const { viewsDirectory } = args;
    response.write(fs.readFileSync(`${viewsDirectory}/uploadForm.html`));
    response.end();
}

const postUpload = (request, response, args) => {
    const { staticDirectory } = args;
    const form = new Multiparty.Form({
        uploadDir: staticDirectory
    });

    form.parse(request, (err) => {
        if (err) {
            response.writeHead(400, {
                "Content-Type" : "text/plain; charset=utf-8"
            });
            response.end("Check your form input. Action must be multipart/form-data");
            return;
        }

        response.writeHead(200, {
            "Content-Type" : "text/plain; charset=utf-8"
        })
        response.end("File saved");
    })
}

module.exports = {
    getUpload,
    postUpload
}