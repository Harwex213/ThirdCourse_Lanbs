const analyzeRequest = (request, response) => {
    request.on("data", chunk =>
    {
        console.log(chunk)
    });

    request.on("end", () =>
    {
        console.log("All chunks received");
        response.end("done");
    });
};

module.exports = {
    analyzeRequest
}