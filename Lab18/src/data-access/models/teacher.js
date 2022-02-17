module.exports = function(sequelize, DataTypes) {
  return sequelize.define("TEACHER", {
    TEACHER: {
      type: DataTypes.CHAR(10),
      allowNull: false,
      primaryKey: true
    },
    TEACHER_NAME: {
      type: DataTypes.STRING(100),
      allowNull: true
    },
    GENDER: {
      type: DataTypes.CHAR(1),
      allowNull: true
    },
    PULPIT: {
      type: DataTypes.CHAR(20),
      allowNull: true,
      references: {
        model: "PULPIT",
        key: "PULPIT"
      }
    }
  }, {
    sequelize,
    tableName: "TEACHER",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "TEACHER_PK",
        unique: true,
        fields: [
          { name: "TEACHER" },
        ]
      },
    ]
  });
};
