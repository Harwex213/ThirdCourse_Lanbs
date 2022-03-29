const generateNotFound = () => {
    const error = new Error("Not found");
    error.status = 404;
    return error;
}

const generateForbidden = () => {
    const error = new Error("Not permitted");
    error.status = 403;
    return error;
}

module.exports = {
    generateNotFound,
    generateForbidden,
}