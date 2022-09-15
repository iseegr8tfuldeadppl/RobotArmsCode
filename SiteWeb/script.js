
let updates_period = 10;
let last_update = 0;
let bottomSlider = document.getElementById("bottomSlider");
let spineSlider = document.getElementById("spineSlider");
let tiltSlider = document.getElementById("tiltSlider");
let mouthSlider = document.getElementById("mouthSlider");

let forth = document.getElementById("forth");
let left = document.getElementById("left");
let right = document.getElementById("right");
let back = document.getElementById("back");
let forthPressed = "0";
let backPressed = "0";
let leftPressed = "0";
let rightPressed = "0";

function handleForthStart(event){
    event.preventDefault();
    forthPressed = "1";
}
function handleForthEnd(event){
    event.preventDefault();
    forthPressed = "0";
}

function handleBackStart(event){
    event.preventDefault();
    backPressed = "1";
}
function handleBackEnd(event){
    event.preventDefault();
    backPressed = "0";
}

function handleLeftStart(event){
    event.preventDefault();
    leftPressed = "1";
}
function handleLeftEnd(event){
    event.preventDefault();
    leftPressed = "0";
}

function handleRightStart(event){
    event.preventDefault();
    rightPressed = "1";
}
function handleRightEnd(event){
    event.preventDefault();
    rightPressed = "0";
}


let updated = false;
function bottomSliderUpdated() {
    updated = true;
}

async function updater() {

    if((new Date()).getTime() - last_update < updates_period)
        return;

    if(updated){
        updated = false;
        const rawResponse = await fetch('http://192.168.4.1/motion/?angles=' + bottomSlider.value + ',' + spineSlider.value + ',' + tiltSlider.value + ',' + mouthSlider.value + ',69,' + forthPressed + "," + leftPressed + "," + rightPressed + "," + backPressed, {
            method: 'GET',
            mode: 'no-cors'
        });
    
        // Step 2: wait for response from backend
        console.log(rawResponse.text());
        last_update = (new Date()).getTime();
    }

};
function startup() {
    forth.addEventListener('touchstart', bottomSliderUpdated);
    forth.addEventListener('touchend', bottomSliderUpdated);
    forth.addEventListener('touchcancel', bottomSliderUpdated);
    forth.addEventListener('mousedown', bottomSliderUpdated);
    forth.addEventListener('mouseup', bottomSliderUpdated);
    
    left.addEventListener('touchstart', bottomSliderUpdated);
    left.addEventListener('touchend', bottomSliderUpdated);
    left.addEventListener('touchcancel', bottomSliderUpdated);
    left.addEventListener('mousedown', bottomSliderUpdated);
    left.addEventListener('mouseup', bottomSliderUpdated);
    
    right.addEventListener('touchstart', bottomSliderUpdated);
    right.addEventListener('touchend', bottomSliderUpdated);
    right.addEventListener('touchcancel', bottomSliderUpdated);
    right.addEventListener('mousedown', bottomSliderUpdated);
    right.addEventListener('mouseup', bottomSliderUpdated);
    
    back.addEventListener('touchstart', bottomSliderUpdated);
    back.addEventListener('touchend', bottomSliderUpdated);
    back.addEventListener('touchcancel', bottomSliderUpdated);
    back.addEventListener('mousedown', bottomSliderUpdated);
    back.addEventListener('mouseup', bottomSliderUpdated);

    const intervalId = setInterval(updater, 100);
}
document.addEventListener("DOMContentLoaded", startup);

