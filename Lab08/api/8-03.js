const getParameters = (request, response, args) => {
    const getValuesFromRoute = () => {
        const matches = requestUrl.pathname.match(/\/[0-9]+/gi);
        return matches ? matches.map(value => Number(value.substring(1))) : [undefined, undefined];
    };

    const getValuesFromQuery = () => (
        [
            Number(requestUrl.searchParams.get("x")),
            Number(requestUrl.searchParams.get("y"))
        ]
    );

    const analyzeValues = (callback) => {
        if (valueX && valueY) {
            callback();
        }
        else {
            response.writeHead(400);
            response.write(isCalcByRoute ? requestUrl.href : "x & y must be strings");
        }
    }

    const { requestUrl } = args;
    const isCalcByRoute = requestUrl.pathname.match(/\//gi).length > 1;
    const [valueX, valueY] = isCalcByRoute ? getValuesFromRoute() : getValuesFromQuery();
    analyzeValues(() => {
        response.write(`Sum: ${valueX + valueY}; Diff: ${valueX - valueY}; Mult: ${valueX * valueY}; Div: ${valueX / valueY};   `)
    })

    response.end();
};

module.exports = {
    getParameters
};