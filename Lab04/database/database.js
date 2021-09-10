const EventEmitter = require("events");

let id = 0;

class Database extends EventEmitter
{
    data = [
        {id: id++, name: "Oleg", bday: new Date()},
        {id: id++, name: "Joker", bday: new Date()},
        {id: id++, name: "Kak?", bday: new Date()},
    ];

    constructor(props) {
        super(props);
    }

    select() {
        return this.data;
    }

    insert(newEntity) {
        const entityToAdd = {
            ...newEntity,
            id: id++
        };
        this.data.push(entityToAdd);
        return entityToAdd;
    }

    update(newEntity) {
        const index = this.data.findIndex(entity => entity.id === newEntity.id);
        if (index === -1) {
            throw new Error("Specified id is invalid");
        }
        this.data[index] = newEntity;
    }

    delete(id) {
        const index = this.data.findIndex(entity => entity.id === id);
        if (index === -1) {
            throw new Error("Specified id is invalid");
        }
        const returnData = this.data[index];
        this.data.splice(index, 1);
        return returnData;
    }
}

module.exports = Database;