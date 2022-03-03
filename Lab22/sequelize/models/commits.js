const Sequelize = require("sequelize");
module.exports = function(sequelize, DataTypes) {
  return sequelize.define("commits", {
    id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    repoId: {
      type: DataTypes.INTEGER,
      allowNull: true,
      references: {
        model: "repos",
        key: "id"
      }
    },
    message: {
      type: DataTypes.STRING(255),
      allowNull: true
    }
  }, {
    sequelize,
    tableName: "commits",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "PK__commits__3213E83F2A1C5131",
        unique: true,
        fields: [
          { name: "id" },
        ]
      },
    ]
  });
};
