const Sequelize = require('sequelize');
module.exports = function(sequelize, DataTypes) {
  return sequelize.define('RentedVehicle', {
    Id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    ClientId: {
      type: DataTypes.INTEGER,
      allowNull: false,
      references: {
        model: 'Client',
        key: 'Id'
      }
    },
    VehicleId: {
      type: DataTypes.INTEGER,
      allowNull: false,
      references: {
        model: 'Vehicle',
        key: 'Id'
      }
    },
    DateFrom: {
      type: DataTypes.DATE,
      allowNull: false
    },
    DateTo: {
      type: DataTypes.DATE,
      allowNull: false
    }
  }, {
    sequelize,
    tableName: 'RentedVehicle',
    schema: 'dbo',
    timestamps: false,
    indexes: [
      {
        name: "DateProvidedServie",
        fields: [
          { name: "DateFrom" },
          { name: "DateTo" },
        ]
      },
      {
        name: "RentedVehicle_PK",
        unique: true,
        fields: [
          { name: "Id" },
        ]
      },
    ]
  });
};
