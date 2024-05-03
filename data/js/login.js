// login.js
localStorage.clear();

document.getElementById("loginForm").addEventListener("submit", function (event) {
    event.preventDefault();
    const username = document.getElementById("username").value;
    localStorage.setItem("username", username);
    
    // Send the username to the server (ESP32)
    fetch("/login", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded",
        },
        body: "username=" + encodeURIComponent(username),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error("Network response was not ok");
        }
        return response.text();
    })
    .then(() => {
        // Redirect to the control page after successful login
        window.location.href = "control.html";
    })
    .catch(error => {
        console.error("Error:", error);
        // Handle login error if needed
    });
});
