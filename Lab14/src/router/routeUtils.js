const controllers = {};

const findController = (resolverEndpoint) => {
    for (const [key, value] of Object.entries(controllers)) {
        if (resolverEndpoint.includes(key)) {
            const controllerEndpoint = resolverEndpoint.replace(key, "");
            return {
                controller: value,
                controllerEndpoint: controllerEndpoint === "" ? "/" : controllerEndpoint
            };
        }
    }

    throw new Error();
}

const findHandler = (handlers, controllerEndpoint) => {
    let requestParams = {};
    let isRouteRight = true;
    const requestEndpointRoutes = controllerEndpoint.split("/");
    requestEndpointRoutes.shift();

    for (const [endpoint, handler] of Object.entries(handlers)) {
        const endpointRoutes = endpoint.split("/");
        endpointRoutes.shift();

        for (let i = 0; i < endpointRoutes.length; i++) {
            if (requestEndpointRoutes.length < i + 1) {
                isRouteRight = false;
                break;
            }
            if (endpointRoutes[i].startsWith(":")) {
                requestParams[endpointRoutes[i].substring(1)] = requestEndpointRoutes[i];
                continue;
            }
            if (endpointRoutes[i] !== requestEndpointRoutes[i]) {
                isRouteRight = false;
                break;
            }
        }

        if (isRouteRight) {
            return {
                handler,
                requestParams
            }
        }
        requestParams = {};
        isRouteRight = true;
    }

    throw new Error();
}

const useRoute = (endpoint, router) => {
    controllers[endpoint.toLowerCase()] = {
        "GET": router._getHandlers,
        "POST": router._postHandlers,
        "PUT": router._putHandlers,
        "DELETE": router._deleteHandlers
    }
}

const resolveRoute = (request, endpoint) => {
    try {
        const { controller, controllerEndpoint } = findController(endpoint.toLowerCase());
        const handlers = controller[request.method];
        const { handler, requestParams } = findHandler(handlers, controllerEndpoint);

        request.params = requestParams;
        return handler;
    }
    catch (e) {
        const error = new Error("Api does not have such endpoint");
        error.status = 400;
        throw error;
    }
}

module.exports = {
    useRoute,
    resolveRoute
};