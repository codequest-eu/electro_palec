const express = require('express')
var app = express()
const path = require("path")
const http = require('http')

const fingerPostOptions = {
    host: '192.168.1.254',
    path: '/',
    method: 'POST',
    headers: {
        'Authorization': 'Bearer DUPA.8'
    }
};

app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname+'/index.html'))
})

app.post('/electro', function (req, res) {
  var fingerPost = http.request(fingerPostOptions, function(res) {
      res.setEncoding('utf8');
      res.on('data', function (chunk) {
          console.log('Response: ' + chunk);
      });
  });
  fingerPost.end();
  res.redirect('/');
})

app.listen(3000, function () {
  console.log('Electro palec listening on port 3000!')
})

