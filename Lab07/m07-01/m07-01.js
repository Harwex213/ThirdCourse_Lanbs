const fs = require("fs");
const path = require("path");

const contentTypes = {
    ".html": "text/html; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".js": "text/javascript; charset=utf-8",
    ".jpg": "image/jpg",
    ".docx": "application/msword",
    ".xml": "application/xml",
    ".json": "application/json",
    ".mp4": "video/mp4",
};

class FileManagerController {
    constructor(rootDirectory) {
        this._rootDirectory = rootDirectory;
    }

    sendFile(response, requestPath) {
        try {
            const file = fs.readFileSync(this._rootDirectory + requestPath);

            const contentType = contentTypes[path.extname(requestPath)];
            if (contentType) {
                response.writeHead(200, {
                    "Content-type": contentType
                })
            }
            else {
                this._sendError404(response);
                return;
            }

            response.end(file);
        } catch (e) {
            this._sendError404(response);
        }
    }

    _sendError404 (response) {
        response.writeHead(404);
        response.end("File not found");
    }
}

module.exports = (rootDirectory) => new FileManagerController(rootDirectory);