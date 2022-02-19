const Sequelize = require('sequelize');
module.exports = function(sequelize, DataTypes) {
  return sequelize.define('Client', {
    Id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    FirstName: {
      type: DataTypes.STRING(30),
      allowNull: false
    },
    LastName: {
      type: DataTypes.STRING(45),
      allowNull: false
    },
    Passport: {
      type: DataTypes.CHAR(9),
      allowNull: false
    },
    Email: {
      type: DataTypes.STRING(256),
      allowNull: true
    }
  }, {
    sequelize,
    tableName: 'Client',
    schema: 'dbo',
    timestamps: false,
    indexes: [
      {
        name: "Client_PK",
        unique: true,
        fields: [
          { name: "Id" },
        ]
      },
    ]
  });
};
