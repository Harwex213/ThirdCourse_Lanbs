var DataTypes = require("sequelize").DataTypes;
var _Client = require("./Client");
var _RentedVehicle = require("./RentedVehicle");
var _Vehicle = require("./Vehicle");
var _VehicleType = require("./VehicleType");

function initModels(sequelize) {
  var Client = _Client(sequelize, DataTypes);
  var RentedVehicle = _RentedVehicle(sequelize, DataTypes);
  var Vehicle = _Vehicle(sequelize, DataTypes);
  var VehicleType = _VehicleType(sequelize, DataTypes);

  RentedVehicle.belongsTo(Client, { as: "Client", foreignKey: "ClientId"});
  Client.hasMany(RentedVehicle, { as: "RentedVehicles", foreignKey: "ClientId"});
  RentedVehicle.belongsTo(Vehicle, { as: "Vehicle", foreignKey: "VehicleId"});
  Vehicle.hasMany(RentedVehicle, { as: "RentedVehicles", foreignKey: "VehicleId"});
  Vehicle.belongsTo(VehicleType, { as: "VehicleType", foreignKey: "VehicleTypeId"});
  VehicleType.hasMany(Vehicle, { as: "Vehicles", foreignKey: "VehicleTypeId"});

  return {
    Client,
    RentedVehicle,
    Vehicle,
    VehicleType,
  };
}
module.exports = initModels;
module.exports.initModels = initModels;
module.exports.default = initModels;
