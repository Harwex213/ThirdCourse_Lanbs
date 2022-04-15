const { createClient } = require("webdav");
const express = require("express");
const config = require("./config.json");

const client = createClient(
    "https://webdav.yandex.ru",
    { ...config }
);

const app = express();

const generateError = (message = "Bad request", status = 400) => {
    const error = new Error(message);
    error.status = status;
    return error;
}
const generateRequestTimeout = (message = "Request timeout", status = 408) => generateError(message, status);
const generateNotFound = (message = "Not found", status = 404) => generateError(message, status);

app.post("/md/:directoryPath", async (request, response, next) => {
    const directoryPath = request.params?.directoryPath;
    if (!directoryPath) {
        next(generateError());
        return;
    }

    if (await client.exists(directoryPath)) {
        next(generateRequestTimeout("Such directory already exists"));
        return;
    }
    await client.createDirectory(directoryPath);

    response.end();
});

app.post("/rd/:directoryPath", async (request, response, next) => {
    const directoryPath = request.params?.directoryPath;
    if (!directoryPath) {
        next(generateError());
        return;
    }

    if (await client.exists(directoryPath) === false) {
        next(generateRequestTimeout("Such directory not exists"));
        return;
    }
    await client.deleteFile(directoryPath);

    response.end();
});

app.post("/up/:fileName", async (request, response, next) => {
    const filePath = request.query?.filePath ?? "";
    const fileName = request.params?.fileName;
    if (!fileName || fileName.split(".").length < 2) {
        next(generateError());
        return;
    }

    const writeStream = client.createWriteStream(filePath + "/" + fileName);
    request.pipe(writeStream);

    writeStream.on("finish", () => response.end());
    writeStream.on("error", () => next(generateRequestTimeout("Error while attempted to create file")));
});

app.post("/down/:fileName", async (request, response, next) => {
    const filePath = request.query?.filePath ?? "";
    const fileName = request.params?.fileName;
    if (!fileName || fileName.split(".").length < 2) {
        next(generateError());
        return;
    }

    if (await client.exists(filePath + "/" + fileName) === false) {
        next(generateNotFound());
        return;
    }
    const readStream = client.createReadStream(filePath + "/" + fileName);
    response.attachment(fileName);
    readStream.pipe(response);

    readStream.on("error", () => next(generateRequestTimeout("Error while attempted to download file")));
});

app.post("/del/:fileName", async (request, response, next) => {
    const filePath = request.query?.filePath ?? "";
    const fileName = request.params?.fileName;
    if (!fileName || fileName.split(".").length < 2) {
        next(generateError());
        return;
    }

    if (await client.exists(filePath + "/" + fileName) === false) {
        next(generateNotFound());
        return;
    }
    await client.deleteFile(filePath + "/" + fileName);

    response.end();
});

app.post("/copy/:originName/:newName", async (request, response, next) => {
    const filePathOrigin = request.query?.filePathOrigin ?? "";
    const filePathNew = request.query?.filePathNew ?? "";
    const fileNameOrigin = request.params?.originName;
    const fileNameNew = request.params?.newName;
    if (!fileNameOrigin || !fileNameNew ||
        fileNameOrigin.split(".").length < 2 ||
        fileNameNew.split(".").length < 2) {
        next(generateError());
        return;
    }

    if (await client.exists(filePathOrigin + "/" + fileNameOrigin) === false) {
        next(generateNotFound());
        return;
    }
    const readStream = client.createReadStream(filePathOrigin + "/" + fileNameOrigin);
    const writeStream = client.createWriteStream(filePathNew + "/" + fileNameNew);
    readStream.pipe(writeStream);

    writeStream.on("finish", () => response.end());
    writeStream.on("error", () => next(generateRequestTimeout("Error while attempted to create file")));
});

app.post("/move/:originName/:newName", async (request, response, next) => {
    const filePathOrigin = request.query?.filePathOrigin ?? "";
    const filePathNew = request.query?.filePathNew ?? "";
    const fileNameOrigin = request.params?.originName;
    const fileNameNew = request.params?.newName;
    if (!fileNameOrigin || !fileNameNew ||
        fileNameOrigin.split(".").length < 2 ||
        fileNameNew.split(".").length < 2) {
        next(generateError());
        return;
    }

    if (await client.exists(filePathOrigin + "/" + fileNameOrigin) === false) {
        next(generateNotFound());
        return;
    }
    const readStream = client.createReadStream(filePathOrigin + "/" + fileNameOrigin);
    const writeStream = client.createWriteStream(filePathNew + "/" + fileNameNew);
    readStream.pipe(writeStream);

    writeStream.on("finish", async () => {
        await client.deleteFile(filePathOrigin + "/" + fileNameOrigin);
        response.end();
    });
    writeStream.on("error", () => next(generateRequestTimeout("Error while attempted to create file")));
});

app.use((error, request, response, next) => {
    response.setHeader("Content-Type", "application/json");
    response.status(error.status || 500);
    response.json({
        error: {
            message: error.message
        }
    });
});

app.listen(3000);
