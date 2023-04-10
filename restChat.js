// Rest based chat client
// Jim Skon 2022
// Kenyon College

var baseUrl = 'http://3.143.218.37:5005';
var state="off";
var myname="";
var inthandle;

/* Start with text input and status hidden */
document.getElementById('chatinput').style.display = 'none';
document.getElementById('status').style.display = 'none';
document.getElementById('leave').style.display = 'none';




// Action if they push the join button
document.getElementById('login-btn').addEventListener("click", (e) => {
loginUser();
})


/* Set up buttons */
document.getElementById('leave-btn').addEventListener("click", leaveSession);
document.getElementById('send-btn').addEventListener("click", sendText);

// Watch for enter on message box
document.getElementById('message').addEventListener("keydown", (e)=> {
    if (e.code == "Enter") {
e.preventDefault();
sendText();
document.getElementById('message').value=""
return false;  
    }  
});

document.querySelector("#loginModal .btn-primary").addEventListener("click", registerUser);


// Call function on page exit
window.onbeforeunload = leaveSession;


function completeJoin(results) {
var status = results['status'];
if (status != "success") {
alert("Username already exists!");
leaveSession();
return;
}
var user = results['user'];
console.log("Join:"+user);
startSession(user);
}

function join() {
  myname = document.getElementById('yourname').value;
  var password = document.getElementById('yourpassword').value;
  fetch(baseUrl + '/chat/join/' + myname + '/' + password, {
    method: 'get'
  })
  .then(response => response.json())
  .then(data => {
    if (data.status === "success") {
      startSession(myname);
    } else {
      alert("Error: " + data.error);
    }
  })
  .catch(error => {
    alert("Error: Something went wrong:" + error);
  })
}

function loginUser() {
  var username = document.getElementById('orangeForm-name').value;
  var password = document.getElementById('orangeForm-pass').value;
  fetch(baseUrl + '/chat/login/' + username + '/' + password, {
    method: 'get'
  })
  .then(response => response.json())
  .then(data => {
    if (data.status === "success") {
      $("#loginModal").modal('hide');
      startSession(username);
    } else {
      alert("Error: " + data.error);
    }
  })
  .catch(error => {
    alert("Error: Something went wrong:" + error);
  })
}


function completeSend(results) {
var status = results['status'];
if (status == "success") {
console.log("Send succeeded")
} else {
alert("Error sending message!");
}
}

//function called on submit or enter on text input
function sendText() {
    var message = document.getElementById('message').value;
fetch(baseUrl+'/chat/send/'+myname+'/'+message, {
        method: 'get'
    })
    .then (response => response.json() )
    .then (data => completeSend(data))
    .catch(error => {
        {alert("Error: Something went wrong:"+error);}
    })    
document.getElementById('message').value = "";
}

function completeFetch(result) {
  // Access the inner "messages" object
  messages = result["messages"]["messages"];
  users = result["users"];
  document.getElementById('members').innerHTML = users.join(", ");
  messages.forEach(function (m, i) {
var name = m['user'];
var message = m['message'];
    document.getElementById('chatBox').innerHTML +=
      "<font color='red'>" + name + ": </font>" + message + "<br />";
  });
}

function registerUser() {
  var username = document.getElementById('orangeForm-name').value;
  var email = document.getElementById('orangeForm-email').value;
  var password = document.getElementById('orangeForm-pass').value;
  fetch(baseUrl + '/chat/register/' + username + '/' + email + '/' + password, {
    method: 'get'
  })
  .then(response => response.json())
  .then(data => {
    if (data.status === "success") {
      alert("Registration successful! Please log in.");
      $("#loginModal").modal('hide');
    } else {
      alert("Error: " + data.error);
    }
  })
  .catch(error => {
    alert("Error: Something went wrong:" + error);
  })
}


/* Check for new messaged */
/* Check for new messages */
function fetchMessage() {
  fetch(baseUrl+'/chat/fetch/'+myname, {
    method: 'get'
  })
  .then(response => response.json())
  .then(data => {
    completeFetch(data);
  })
  .catch(error => {
    {console.log("Server appears down:", error);}
  })    
}

/* Functions to set up visibility of sections of the display */
function startSession(name){
    state="on";
    myname = name;
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'none';
    document.getElementById('user').innerHTML = "User: " + name;
    document.getElementById('chatinput').style.display = 'block';
    document.getElementById('status').style.display = 'block';
    document.getElementById('leave').style.display = 'block';
    /* Check for messages every 500 ms */
    inthandle=setInterval(fetchMessage,500);
}


function leaveSession(){
    state="off";
 if (myname) {
    fetch(baseUrl + "/chat/leave/" + myname, {
      method: "get",
    }).catch((error) => {
      console.log("Error: Something went wrong:", error);
    });
  }
   
    document.getElementById('yourname').value = "";
    document.getElementById('register').style.display = 'block';
    document.getElementById('user').innerHTML = "";
    document.getElementById('chatinput').style.display = 'none';
    document.getElementById('status').style.display = 'none';
    document.getElementById('leave').style.display = 'none';
clearInterval(inthandle);
}

function toggleDarkMode() {
var element = document.body;
    element.classList.toggle("darkMode");
}
