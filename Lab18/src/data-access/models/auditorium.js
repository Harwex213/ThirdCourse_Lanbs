const { Op } = require("sequelize");
module.exports = function(sequelize, DataTypes) {
  return sequelize.define("AUDITORIUM", {
    AUDITORIUM: {
      type: DataTypes.CHAR(20),
      allowNull: false,
      primaryKey: true
    },
    AUDITORIUM_TYPE: {
      type: DataTypes.CHAR(10),
      allowNull: true,
      references: {
        model: "AUDITORIUM_TYPE",
        key: "AUDITORIUM_TYPE"
      }
    },
    AUDITORIUM_CAPACITY: {
      type: DataTypes.INTEGER,
      allowNull: true,
      defaultValue: 1
    },
    AUDITORIUM_NAME: {
      type: DataTypes.STRING(50),
      allowNull: true
    }
  }, {
    sequelize,
    tableName: "AUDITORIUM",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "AUDITORIUM_PK",
        unique: true,
        fields: [
          { name: "AUDITORIUM" },
        ]
      },
    ],
    scopes: {
      sgt60: {
        where: {
          AUDITORIUM_CAPACITY: {
            [Op.gte]: 60
          }
        }
      }
    }
  });
};
