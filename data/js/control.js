// control.js
let _status = false;
let timerId = null;
let timerValue = 1;

function startTimer() {
    _status = true;
    timerId = setInterval(() => {
        if (!_status) {
            clearInterval(timerId);
            timerId = null;
        } else {
            let minutes = Math.floor((timerValue % 3600) / 60);
            let seconds = Math.floor(timerValue % 60);

            document.getElementById("timerDisplay").textContent = 
                ("0" + minutes).slice(-2) + ":" + 
                ("0" + seconds).slice(-2);
            timerValue++;
        }
    }, 1000);
}

document.getElementById("startButton").addEventListener("click", function () {
    // Send a request to the server (ESP32) indicating the start action
    if (_status) {
        document.getElementById("errorMessage").textContent = "Ya empezó";
        return;
    }
    document.getElementById("errorMessage").textContent = "";
    fetch("/start", { method: "POST" })
    .then(response => {
        if (!response.ok) {
            throw new Error("Network response was not ok");
        }
        _status = true;
        timerValue = 1;
        startTimer();
        return response.text();
    })
    .catch(error => {
        document.getElementById("errorMessage").textContent = error.message;
        console.error("Error:", error);
    });
});

document.getElementById("endButton").addEventListener("click", function () {
    // Send a request to the server (ESP32) indicating the end action
    if (!_status) {
        document.getElementById("errorMessage").textContent = "No ha empezado";
        return;
    }
    document.getElementById("errorMessage").textContent = "";
    if (timerId) {
        clearInterval(timerId);
        timerId = null;
        _status = false;
    }
    fetch("/end", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body: "timerValue=" + encodeURIComponent(timerValue),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error("Network response was not ok");
        }
        return response.text();
    })
    .then(() => {
        // Redirect to the control page after successful login
        window.location.href = "result.html";
    })
    .catch(error => {
        console.error("Error:", error);
    });
});