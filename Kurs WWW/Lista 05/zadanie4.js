function random(max) {
    return Math.floor(Math.random() * max);
}

function drawPixel(canvas, x, y) 
{
    canvas.fillRect(x, y, 1, 1);
}

let sideLength = 800;
let ax = 100, ay = 100;
let bx = ax + sideLength, by = 100;
let cx = ax + sideLength / 2, cy = 100 + sideLength * Math.sqrt(3) / 2;
let can = document.getElementById("canvas");
let boundingRect = can.getBoundingClientRect();
let width = boundingRect.width;
let height = boundingRect.height;
let ctx = can.getContext("2d");

ctx.fillStyle = "#fff";
ctx.fillRect(0, 0, width, height);
ctx.fillStyle = "#fff";

drawPixel(ctx, ax, ay);
drawPixel(ctx, bx, by);
drawPixel(ctx, cx, cy);

let x, y;
x = random(width);
y = random(height);
ctx.fillStyle = "#000000";

for (i = 0; i < 500000; i++) 
{
    drawPixel(ctx, x, y);

    let r = random(3);
    if (r == 0) 
    {
        x = (ax + x) / 2;
        y = (ay + y) / 2;
    } 
    else if (r == 1) 
    {
        x = (bx + x) / 2;
        y = (by + y) / 2;
    } 
    else 
    {
        x = (cx + x) / 2;
        y = (cy + y) / 2;
    }
}
