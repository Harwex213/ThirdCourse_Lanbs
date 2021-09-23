const fs = require("fs");
const dateFormat = require("dateformat");
const { backupDestination, dataDestination } = require("./../const");

const getMethod = (connectionInfo) => {
    const { response } = connectionInfo;

    const fileNames = fs.readdirSync(backupDestination);

    response.writeHead(200, {
        "Content-Type": "application/json"
    })
    response.end(JSON.stringify(fileNames));
}

const postMethod = (connectionInfo) => {
    const { response } = connectionInfo;

    const prefix = dateFormat(new Date(), "yyyymmddHHMMss");
    const backupFileName = `${prefix}_StudentsList.json`;

    setTimeout(() => {
        fs.copyFileSync(dataDestination, `${backupDestination}/${backupFileName}`);

        response.writeHead(200);
        response.end();
    }, 2000);
}

const deleteMethod = (connectionInfo) => {
    const { response, requestUrl } = connectionInfo;

    const requestPath = requestUrl.pathname.match(/\/[0-9]{8}/gi);
    if (!requestPath) {
        throw new Error("Check your request pathname");
    }

    const dividerDate = Number(requestPath[0].substring(1));
    const fileNames = fs.readdirSync(backupDestination);

    fileNames.forEach(fileName => {
        const fileDate = fileName.substring(0, 8);

        if (dividerDate < Number(fileDate)) {
            fs.unlinkSync(`${backupDestination}/${fileName}`);
        }
    });

    response.writeHead(200);
    response.end();
}

const methods = {
    "GET": getMethod,
    "POST": postMethod,
    "DELETE": deleteMethod
};

module.exports = methods;