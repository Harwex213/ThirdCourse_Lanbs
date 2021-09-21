const fs = require("fs");

const getFiles = (request, response, args) => {
    const showFilesCount = () => {
        fs.readdir(staticDirectory, (err, files) =>
        {
            response.writeHead(200, {
                "Content-Type" : "text/plain; charset=utf-8",
                "X-static-files-count": files.length,
            });
            response.end();
        });
    }

    const sendFile = () => {
        try {
            const file = fs.readFileSync(`${staticDirectory}/${routePaths[1]}`)
            response.writeHead(200);
            response.end(file);
        } catch (e) {
            response.writeHead(404, "Not found");
            response.end("File not found");
        }
    }

    const { routePaths, staticDirectory } = args;
    routePaths.length === 1 ? showFilesCount() : sendFile();
};

module.exports = {
    getFiles
};