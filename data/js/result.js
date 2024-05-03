// result.js

fetch("/result")
.then(response => {
    if (!response.ok) {
        throw new Error("Network response was not ok");
    }
    return response.text();
})
.then(data => {
    window.onload=function() {
        const svg = document.getElementById("svg").contentDocument;
        const values = data.split(',').map(Number);
        const colorScale = chroma.scale(['#00ff00', '#ff0000']).domain([Math.min(...values), Math.max(...values)]);
        values.forEach((value, index) => {
            document.getElementById(`resultData${index}`).innerText = value;
            const svgElement = svg.getElementById(`zona${index}`);
            console.log(colorScale(value).hex());
            console.log(svgElement);
            if (svgElement) {
                svgElement.setAttribute("fill", colorScale(value).hex());
            }
        });
    }
})
.catch(error => {
    console.error("Error:", error);
});