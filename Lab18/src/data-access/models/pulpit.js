module.exports = function(sequelize, DataTypes) {
  return sequelize.define("PULPIT", {
    PULPIT: {
      type: DataTypes.CHAR(20),
      allowNull: false,
      primaryKey: true
    },
    PULPIT_NAME: {
      type: DataTypes.STRING(100),
      allowNull: true
    },
    FACULTY: {
      type: DataTypes.CHAR(10),
      allowNull: true,
      references: {
        model: "FACULTY",
        key: "FACULTY"
      }
    }
  }, {
    sequelize,
    tableName: "PULPIT",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "PULPIT_PK",
        unique: true,
        fields: [
          { name: "PULPIT" },
        ]
      },
    ]
  });
};
