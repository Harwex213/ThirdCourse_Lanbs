const Router = () => ({
    _getHandlers: {},
    _postHandlers: {},
    _putHandlers: {},
    _deleteHandlers: {},
    get: function (endpoint, handler) {
        this._getHandlers[endpoint] = handler
    },
    post: function (endpoint, handler) {
        this._postHandlers[endpoint] = handler
    },
    put: function (endpoint, handler) {
        this._putHandlers[endpoint] = handler
    },
    delete: function (endpoint, handler) {
        this._deleteHandlers[endpoint] = handler
    },
})

module.exports = Router;