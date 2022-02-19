const Sequelize = require('sequelize');
module.exports = function(sequelize, DataTypes) {
  return sequelize.define('VehicleType', {
    Name: {
      type: DataTypes.STRING(50),
      allowNull: false,
      primaryKey: true
    }
  }, {
    sequelize,
    tableName: 'VehicleType',
    schema: 'dbo',
    timestamps: false,
    indexes: [
      {
        name: "VehicleType_PK",
        unique: true,
        fields: [
          { name: "Name" },
        ]
      },
    ]
  });
};
