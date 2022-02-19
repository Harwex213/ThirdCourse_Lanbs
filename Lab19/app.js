const express = require("express");
const bodyParser = require("body-parser");
const { engine } = require("express-handlebars");
const clientController = require("./controllers/ClientController");
const rentedVehicleController = require("./controllers/RentedVehicleController");
const vehicleController = require("./controllers/VehicleController");

const app = express();

app.engine(".hbs", engine({ extname: ".hbs" }));
app.set("view engine", ".hbs");
app.set('views', './views');

app.use(bodyParser.json());

app.use("/client", clientController);
app.use("/rented-vehicle", rentedVehicleController);
app.use("/vehicle", vehicleController);

app.use((request, response, next) => {
    const error = new Error("Bad request");
    error.status = 400;
    error.stack = null;
    next(error);
});

app.use((error, request, response, next) => {
    response.status(error.status || 500);
    response.json({
        error: {
            message: error.original?.message ?? error.message
        }
    });
});


app.listen(3000, "localhost", () => {
    console.log("server successfully started");
});