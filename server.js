const express = require('express');
const fs = require('fs');
const path = require('path');
const app = express();
const PORT = 3000;

app.use(express.json());
app.use(express.static(path.join(__dirname))); // serve HTML and JS

// ---------- Helper to append to file ----------
function appendToFile(file, data){
    fs.appendFileSync(path.join(__dirname,'files',file), data+"\n", 'utf8');
}

// ---------- Users ----------
app.post('/users', (req, res) => {
    const {username, password} = req.body;
    const usersPath = path.join(__dirname,'files','users.txt');
    let users = [];
    if(fs.existsSync(usersPath)){
        users = fs.readFileSync(usersPath,'utf8').trim().split("\n").map(line => line.split(' '));
    }
    if(users.find(u=>u[0]===username)) return res.send("User exists!");
    appendToFile('users.txt', `${username} ${password}`);
    res.send("User registered successfully!");
});

// ---------- Events ----------
app.post('/events', (req,res)=>{
    const {id,name,date,time,capacity,price} = req.body;
    appendToFile('events.txt', `${id} ${name} ${date} ${time} ${capacity} ${price}`);
    res.send("Event saved!");
});

// ---------- Delete Event ----------
app.post('/deleteEvent', (req,res)=>{
    const {id} = req.body;
    const eventsPath = path.join(__dirname,'files','events.txt');
    if(!fs.existsSync(eventsPath)) return res.send("No events.");
    let lines = fs.readFileSync(eventsPath,'utf8').trim().split("\n");
    lines = lines.filter(line => line.split(' ')[0] != id);
    fs.writeFileSync(eventsPath, lines.join("\n"), 'utf8');
    res.send("Event deleted.");
});

// ---------- Bookings ----------
app.post('/bookings', (req,res)=>{
    const {bookingId,username,eventName,eventId,phone,status,paymentId} = req.body;
    appendToFile('bookings.txt', `${bookingId} ${username} ${eventName} ${eventId} ${phone} ${status} ${paymentId}`);
    res.send("Booking saved!");
});

// ---------- Payments ----------
app.post('/payments', (req,res)=>{
    const {paymentId,username,bookingId,mode,amount,status} = req.body;
    appendToFile('payments.txt', `${paymentId} ${username} ${bookingId} ${mode} ${amount} ${status}`);
    res.send("Payment saved!");
});

// ---------- Notifications ----------
app.post('/notifications', (req,res)=>{
    const {username,message} = req.body;
    appendToFile('notifications.txt', `${username} ${message}`);
    res.send("Notification saved!");
});

// ---------- Feedback ----------
app.post('/feedback', (req,res)=>{
    const {username,message} = req.body;
    appendToFile('feedback.txt', `${username} ${message}`);
    res.send("Feedback saved!");
});

// ---------- Waiting List ----------
app.post('/waiting', (req,res)=>{
    const {username,phone,eventId} = req.body;
    appendToFile('waiting.txt', `${username} ${phone} ${eventId}`);
    res.send("Added to waiting list!");
});

// ---------- Start server ----------
app.listen(PORT, ()=>{
    console.log(`Server running at http://localhost:${PORT}`);
});
