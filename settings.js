document.addEventListener("DOMContentLoaded", function(event) { 
    let sensDistSlide = document.getElementById("sensor_dist");
    let vibrateStrSlide = document.getElementById("vibrate_strength");
    let distOut = document.getElementById("sensor_dist_display");
    let strOut = document.getElementById("vibrate_strength_display");
    let str;
    let dist;
    updateValues();
    
    
    sensDistSlide.oninput = function() {
        updateValues();
    };
    
    vibrateStrSlide.oninput = function() {
        updateValues();
    };
    
    function updateValues() {
        dist=sensDistSlide.value+"m";
        str=Math.round(vibrateStrSlide.value*100)+"%";
    
        distOut.innerHTML = dist;
        strOut.innerHTML = str;
    }
    });