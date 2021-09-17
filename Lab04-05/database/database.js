const EventEmitter = require("events");

let id = 0;

class Database extends EventEmitter
{
    data = [
        {id: id++, name: "Oleg", bDay: new Date()},
        {id: id++, name: "Joker", bDay: new Date()},
        {id: id++, name: "Kak?", bDay: new Date()},
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
            id: id++,
            bDay: new Date(newEntity.bDay),
        };
        this.data.push(entityToAdd);
        return entityToAdd;
    }

    update(newEntity) {
        const updatedEntity = {
            ...newEntity,
            id: Number(newEntity.id),
            bDay: new Date(newEntity.bDay)
        }
        const index = this.data.findIndex(entity => entity.id === updatedEntity.id);
        if (index === -1) {
            throw new Error("Specified id is invalid");
        }
        this.data[index] = updatedEntity;
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

    commit() {
        console.log("I'm committing now...");
    }
}

module.exports = Database;