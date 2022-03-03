const Sequelize = require("sequelize");
module.exports = function(sequelize, DataTypes) {
  return sequelize.define("repos", {
    id: {
      autoIncrement: true,
      type: DataTypes.INTEGER,
      allowNull: false,
      primaryKey: true
    },
    name: {
      type: DataTypes.STRING(255),
      allowNull: true
    },
    authorId: {
      type: DataTypes.INTEGER,
      allowNull: false,
      references: {
        model: "users",
        key: "id"
      }
    }
  }, {
    sequelize,
    tableName: "repos",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "PK__repos__3213E83FA318D3D4",
        unique: true,
        fields: [
          { name: "id" },
        ]
      },
    ]
  });
};
