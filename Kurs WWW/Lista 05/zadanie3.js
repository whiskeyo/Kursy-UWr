function anim(div) {
    let dt = 0;
    let state = 0;
    let frame = setInterval(() => {
        if (state == 0) {
            dt++;
            div.style.top = 200 - dt;
            if (dt >= 100) {
                state = 1;
            }
        } else if (state == 1) {
            dt++;
            if (dt >= 150) {
                dt = 100;
                state = 2;
            }
        } else {
            dt--;
            div.style.top = 200 - dt;
            if (dt <= 0) {
                state = 0;
                clearInterval(frame);
            }

        }
    }, 1);
}

let divs = document.getElementsByName("block");

function animAll() {
    let dt = 0;
    let state = 0;
    let frame = setInterval(() => {
        if (state == 0) {
            dt++;
            if (dt >= 100) {
                state = 1;
                anim(divs[0]);
            }
        } else if (state == 1) {
            dt++;
            if (dt >= 200) {
                state = 2;
                anim(divs[1]);
            }
        } else if (state == 2) {
            dt++;
            if (dt >= 300) {
                state = 3;
                anim(divs[2]);
            }
        } else if (state == 3) {
            dt++;
            if (dt >= 400) {
                state = 0;
                anim(divs[3]);
                clearInterval(frame);
            }
        }
    }, 1);

}

setInterval(animAll, 2000);
