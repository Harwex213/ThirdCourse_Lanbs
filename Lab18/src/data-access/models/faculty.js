module.exports = function(sequelize, DataTypes) {
  return sequelize.define("FACULTY", {
    FACULTY: {
      type: DataTypes.CHAR(10),
      allowNull: false,
      primaryKey: true
    },
    FACULTY_NAME: {
      type: DataTypes.STRING(50),
      allowNull: true,
      defaultValue: "(N???"
    }
  }, {
    sequelize,
    tableName: "FACULTY",
    schema: "dbo",
    timestamps: false,
    indexes: [
      {
        name: "FACULTY_PK",
        unique: true,
        fields: [
          { name: "FACULTY" },
        ]
      },
    ],
    hooks: {
      beforeCreate(attributes, options) {
        console.log("Before create hook");
      },
      afterCreate(attributes, options) {
        console.log("After create hook");
      }
    }
  });
};
