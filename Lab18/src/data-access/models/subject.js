module.exports = function(sequelize, DataTypes) {
  return sequelize.define("SUBJECT", {
    SUBJECT: {
      type: DataTypes.CHAR(10),
      allowNull: false,
      primaryKey: true
    },
    SUBJECT_NAME: {
      type: DataTypes.STRING(100),
      allowNull: true,
      unique: "UQ__SUBJECT__29B3CFA7E3D8022C"
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
    tableName: "SUBJECT",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "SUBJECT_PK",
        unique: true,
        fields: [
          { name: "SUBJECT" },
        ]
      },
      {
        name: "UQ__SUBJECT__29B3CFA7E3D8022C",
        unique: true,
        fields: [
          { name: "SUBJECT_NAME" },
        ]
      },
    ]
  });
};
