const Sequelize = require('sequelize');
module.exports = function(sequelize, DataTypes) {
  return sequelize.define('Vehicle', {
    Id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    Name: {
      type: DataTypes.STRING(30),
      allowNull: false
    },
    Price: {
      type: DataTypes.DECIMAL(10,4),
      allowNull: false
    },
    Amount: {
      type: DataTypes.INTEGER,
      allowNull: false
    },
    VehicleTypeId: {
      type: DataTypes.STRING(50),
      allowNull: false,
      references: {
        model: 'VehicleType',
        key: 'Name'
      }
    }
  }, {
    sequelize,
    tableName: 'Vehicle',
    schema: 'dbo',
    timestamps: false,
    indexes: [
      {
        name: "Vehicle_PK",
        unique: true,
        fields: [
          { name: "Id" },
        ]
      },
    ]
  });
};
