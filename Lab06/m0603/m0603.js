const nodemailer = require("nodemailer");

const fromMail = "brionna.gleichner@ethereal.email";
const toMail = "brionna.gleichner@ethereal.email";

exports.send = async (subject, message) => {
    const transporter = nodemailer.createTransport({
        host: "smtp.ethereal.email",
        port: 587,
        auth: {
            user: "brionna.gleichner@ethereal.email",
            pass: "qcbDS6GnC1HSytjKjh"
        }
    });

    await transporter.sendMail({
        from: fromMail, // sender address
        to: toMail, // list of receivers
        subject: subject, // Subject line
        text: message, // plain text body
    });
}