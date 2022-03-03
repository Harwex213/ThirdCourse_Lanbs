const Sequelize = require("sequelize");
module.exports = function(sequelize, DataTypes) {
  return sequelize.define("users", {
    id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    username: {
      type: DataTypes.STRING(16),
      allowNull: false
    },
    email: {
      type: DataTypes.STRING(255),
      allowNull: true
    },
    password: {
      type: DataTypes.STRING(32),
      allowNull: false
    },
    role: {
      type: DataTypes.STRING(5),
      allowNull: false
    }
  }, {
    sequelize,
    tableName: "users",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "PK__users__3213E83F121C89DA",
        unique: true,
        fields: [
          { name: "id" },
        ]
      },
    ]
  });
};
